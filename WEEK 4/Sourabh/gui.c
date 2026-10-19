#include "gui.h"
#include "rag.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    SystemState *s;
    GtkWidget *window;
    GtkWidget *process_spin;
    GtkWidget *resource_spin;
    GtkWidget *quantum_spin;
    GtkWidget *matrices_view;
    GtkWidget *status_label;
    GtkWidget *rag_area;
    GtkWidget *request_process;
    GtkWidget *request_resource;
    GtkWidget *request_amount;
} Gui;


static void apply_basic_theme(void)
{
    const char *css =
        "window { background-color: #f4f7fb; color: #203047; }"
        "label { color: #203047; }"
        "button { background-image: none; background-color: #ffffff;"
        " color: #24466b; border: 1px solid #cbd7e6; border-radius: 6px;"
        " padding: 7px 11px; }"
        "button:hover { background-color: #e8f0fb; }"
        "button:active { background-color: #d9e7fa; }"
        "entry, spinbutton { background-color: #ffffff; color: #203047;"
        " border: 1px solid #cbd7e6; border-radius: 5px; padding: 4px; }"
        "textview { background-color: #ffffff; color: #203047; }"
        "textview text { background-color: #ffffff; color: #203047; }"
        "paned > separator { background-color: #dce5ef; min-width: 3px; }";

    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;

    if (gtk_css_provider_load_from_data(provider, css, -1, &error)) {
        GdkScreen *screen = gdk_screen_get_default();
        if (screen != NULL) {
            gtk_style_context_add_provider_for_screen(
                screen, GTK_STYLE_PROVIDER(provider),
                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        }
    } else {
        g_warning("Could not load UI theme: %s", error->message);
        g_clear_error(&error);
    }

    g_object_unref(provider);
}



static void recalculate(SystemState *s)
{
    for (int j = 0; j < s->resource_count; ++j) {
        int used = 0;
        for (int i = 0; i < s->process_count; ++i)
            used += s->allocation[i][j];
        s->available[j] = s->total_resources[j] - used;
        if (s->available[j] < 0)
            s->available[j] = 0;
    }
    for (int i = 0; i < s->process_count; ++i) {
        for (int j = 0; j < s->resource_count; ++j) {
            s->need[i][j] = s->max[i][j] - s->allocation[i][j];
            if (s->need[i][j] < 0)
                s->need[i][j] = 0;
        }
    }
}

static void set_status(Gui *g, const char *message)
{
    char full_message[600];
    snprintf(full_message, sizeof(full_message), "System Status: %s",
             message ? message : "Ready.");
    gtk_label_set_text(GTK_LABEL(g->status_label), full_message);
}

static void append_vector(GString *out, const char *name, const int *values, int n)
{
    g_string_append_printf(out, "%-12s", name);
    for (int j = 0; j < n; ++j)
        g_string_append_printf(out, "%5d", values[j]);
    g_string_append_c(out, '\n');
}

static void refresh(Gui *g)
{
    SystemState *s = g->s;
    recalculate(s);
    GString *out = g_string_new(NULL);
    g_string_append_printf(out, "Processes: %d    Resources: %d\n\n",
                           s->process_count, s->resource_count);
    g_string_append(out, "AVAILABLE\n");
    append_vector(out, "Available", s->available, s->resource_count);
    g_string_append(out, "\nRESOURCE TOTALS\n");
    append_vector(out, "Total", s->total_resources, s->resource_count);

    for (int i = 0; i < s->process_count; ++i) {
        char name[32];
        g_string_append_printf(out, "\nP%d\n", i);
        snprintf(name, sizeof(name), "Allocation");
        append_vector(out, name, s->allocation[i], s->resource_count);
        append_vector(out, "Maximum", s->max[i], s->resource_count);
        append_vector(out, "Need", s->need[i], s->resource_count);
        append_vector(out, "Request", s->request[i], s->resource_count);
        const char *status = "ACTIVE";
        if (s->status[i] == PROCESS_FINISHED) status = "FINISHED";
        else if (s->status[i] == PROCESS_DEADLOCKED) status = "DEADLOCKED";
        else if (s->status[i] == PROCESS_ABORTED) status = "ABORTED";
        g_string_append_printf(out, "Status: %s\n", status);
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g->matrices_view));
    gtk_text_buffer_set_text(buffer, out->str, -1);
    g_string_free(out, TRUE);
    gtk_widget_queue_draw(g->rag_area);
}

static gboolean safe_sequence(SystemState *s, int *sequence)
{
    int work[MAX_RESOURCES] = {0};
    int finish[MAX_PROCESSES] = {0};
    int count = 0;

    for (int j = 0; j < s->resource_count; ++j)
        work[j] = s->available[j];

    for (int i = 0; i < s->process_count; ++i) {
        if (s->status[i] == PROCESS_FINISHED ||
            s->status[i] == PROCESS_ABORTED)
            finish[i] = 1;
    }

    while (count < s->process_count) {
        int progress = 0;
        for (int i = 0; i < s->process_count; ++i) {
            if (finish[i])
                continue;

            int can_finish = 1;
            for (int j = 0; j < s->resource_count; ++j) {
                if (s->need[i][j] > work[j]) {
                    can_finish = 0;
                    break;
                }
            }

            if (can_finish) {
                for (int j = 0; j < s->resource_count; ++j)
                    work[j] += s->allocation[i][j];
                finish[i] = 1;
                sequence[count++] = i;
                progress = 1;
            }
        }
        if (!progress)
            break;
    }

    for (int i = 0; i < s->process_count; ++i)
        if (!finish[i])
            return FALSE;

    return TRUE;
}

static void on_safety(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    int seq[MAX_PROCESSES];

    if (!safe_sequence(g->s, seq)) {
        set_status(g, "UNSAFE STATE: no safe sequence exists for the current allocation and totals.");
        return;
    }

    char msg[256] = "SAFE STATE. Safe sequence: ";
    for (int k = 0; k < g->s->process_count; ++k) {
        char part[24];
        snprintf(part, sizeof(part), "%sP%d", k == 0 ? "" : " -> ", seq[k]);
        strncat(msg, part, sizeof(msg) - strlen(msg) - 1);
    }
    set_status(g, msg);
}

static void on_detect(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    SystemState *s = g->s;
    int work[MAX_RESOURCES], finish[MAX_PROCESSES] = {0};
    for (int j = 0; j < s->resource_count; ++j) work[j] = s->available[j];
    for (int i = 0; i < s->process_count; ++i) {
        int has_allocation = 0;
        for (int j = 0; j < s->resource_count; ++j)
            if (s->allocation[i][j] > 0) has_allocation = 1;
        finish[i] = !has_allocation || s->status[i] == PROCESS_FINISHED ||
                    s->status[i] == PROCESS_ABORTED;
    }
    int progress;
    do {
        progress = 0;
        for (int i = 0; i < s->process_count; ++i) {
            if (finish[i]) continue;
            int can_finish = 1;
            for (int j = 0; j < s->resource_count; ++j)
                if (s->request[i][j] > work[j]) { can_finish = 0; break; }
            if (can_finish) {
                for (int j = 0; j < s->resource_count; ++j)
                    work[j] += s->allocation[i][j];
                finish[i] = 1;
                progress = 1;
            }
        }
    } while (progress);

    char msg[256] = "No deadlocked processes detected.";
    int found = 0;
    for (int i = 0; i < s->process_count; ++i) {
        if (!finish[i]) {
            if (!found) snprintf(msg, sizeof(msg), "Potential deadlock: ");
            char part[20];
            snprintf(part, sizeof(part), "P%d ", i);
            strncat(msg, part, sizeof(msg) - strlen(msg) - 1);
            s->status[i] = PROCESS_DEADLOCKED;
            found = 1;
        } else if (s->status[i] == PROCESS_DEADLOCKED) {
            s->status[i] = PROCESS_ACTIVE;
        }
    }
    set_status(g, msg);
    refresh(g);
}

static void on_recover(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    SystemState *s = g->s;
    int victim = -1, largest = -1;
    for (int i = 0; i < s->process_count; ++i) {
        if (s->status[i] == PROCESS_DEADLOCKED) {
            int held = 0;
            for (int j = 0; j < s->resource_count; ++j) held += s->allocation[i][j];
            if (held > largest) { largest = held; victim = i; }
        }
    }
    if (victim < 0) {
        set_status(g, "No marked deadlocked process. Run Detect Deadlock first.");
        return;
    }
    for (int j = 0; j < s->resource_count; ++j) {
        s->allocation[victim][j] = 0;
        s->request[victim][j] = 0;
    }
    s->status[victim] = PROCESS_ABORTED;
    recalculate(s);

    on_detect(NULL, g);

    char msg[160];
    snprintf(msg, sizeof(msg),
             "Recovery complete: aborted P%d and released its resources.",
             victim);
    set_status(g, msg);
    refresh(g);
}

static void on_set_totals(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    SystemState *s = g->s;
    if (s->resource_count <= 0) {
        set_status(g, "Create a system first.");
        return;
    }

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Set Total Resource Instances", GTK_WINDOW(g->window),
        GTK_DIALOG_MODAL, "_Cancel", GTK_RESPONSE_CANCEL,
        "_Apply", GTK_RESPONSE_ACCEPT, NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    GtkWidget *entries[MAX_RESOURCES] = {0};

    for (int j = 0; j < s->resource_count; ++j) {
        char label[32];
        snprintf(label, sizeof(label), "Resource R%d total:", j);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new(label), 0, j, 1, 1);
        entries[j] = gtk_spin_button_new_with_range(0, 100000, 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(entries[j]), s->total_resources[j]);
        gtk_grid_attach(GTK_GRID(grid), entries[j], 1, j, 1, 1);
    }

    gtk_box_pack_start(GTK_BOX(content), grid, TRUE, TRUE, 4);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        int totals[MAX_RESOURCES] = {0};
        int valid = 1;
        for (int j = 0; j < s->resource_count; ++j) {
            totals[j] = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(entries[j]));
            int allocated = 0;
            for (int i = 0; i < s->process_count; ++i)
                allocated += s->allocation[i][j];
            if (totals[j] < allocated)
                valid = 0;
        }
        if (!valid) {
            set_status(g, "Invalid totals: each total must be at least the amount currently allocated.");
        } else {
            for (int j = 0; j < s->resource_count; ++j)
                s->total_resources[j] = totals[j];
            recalculate(s);
            set_status(g, "Resource totals updated successfully.");
            refresh(g);
        }
    }
    gtk_widget_destroy(dialog);
}

static void on_create(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    int np = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(g->process_spin));
    int nr = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(g->resource_spin));
    SystemState *s = g->s;
    memset(s, 0, sizeof(*s));
    s->process_count = np;
    s->resource_count = nr;
    for (int j = 0; j < nr; ++j) s->total_resources[j] = 3;
    for (int i = 0; i < np; ++i) s->status[i] = PROCESS_ACTIVE;
    recalculate(s);
    set_status(g, "Created system. Use Edit Matrices to enter allocation and maximum values.");
    refresh(g);
}

static gboolean parse_matrix_row(const char *line, int *values, int n)
{
    char *copy = g_strdup(line), *save = NULL;
    int count = 0;
    for (char *tok = strtok_r(copy, " ,\t", &save); tok;
         tok = strtok_r(NULL, " ,\t", &save)) {
        char *end = NULL;
        long value = strtol(tok, &end, 10);
        if (*end != '\0' || value < 0 || value > 100000 || count >= n) {
            g_free(copy); return FALSE;
        }
        values[count++] = (int)value;
    }
    g_free(copy);
    return count == n;
}

static void on_edit_matrices(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    SystemState *s = g->s;
    if (s->process_count <= 0 || s->resource_count <= 0) {
        set_status(g, "Create a system first.");
        return;
    }
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Edit Allocation and Maximum matrices", GTK_WINDOW(g->window),
        GTK_DIALOG_MODAL, "_Cancel", GTK_RESPONSE_CANCEL,
        "_Apply", GTK_RESPONSE_ACCEPT, NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *instructions = gtk_label_new(
        "Enter one process per line, with resource values separated by spaces or commas.\n"
        "Allocation rows first, then a blank line, then Maximum rows.\n"
        "Example for 2 processes and 3 resources:\n0 1 0\n2 0 0\n\n2 1 1\n3 2 1");
    gtk_label_set_xalign(GTK_LABEL(instructions), 0);
    gtk_box_pack_start(GTK_BOX(content), instructions, FALSE, FALSE, 6);
    GtkWidget *view = gtk_text_view_new();
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(view), TRUE);
    gtk_widget_set_size_request(view, 440, 260);
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    GString *initial = g_string_new(NULL);
    for (int i = 0; i < s->process_count; ++i) {
        for (int j = 0; j < s->resource_count; ++j)
            g_string_append_printf(initial, "%d%s", s->allocation[i][j],
                                   j + 1 == s->resource_count ? "" : " ");
        g_string_append_c(initial, '\n');
    }
    g_string_append_c(initial, '\n');
    for (int i = 0; i < s->process_count; ++i) {
        for (int j = 0; j < s->resource_count; ++j)
            g_string_append_printf(initial, "%d%s", s->max[i][j],
                                   j + 1 == s->resource_count ? "" : " ");
        g_string_append_c(initial, '\n');
    }
    gtk_text_buffer_set_text(buf, initial->str, -1);
    g_string_free(initial, TRUE);
    gtk_box_pack_start(GTK_BOX(content), view, TRUE, TRUE, 6);
    gtk_widget_show_all(dialog);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buf, &start, &end);
        char *text = gtk_text_buffer_get_text(buf, &start, &end, FALSE);
        char **lines = g_strsplit(text, "\n", -1);
        int rows = 0, blank_seen = 0, ok = 1;
        int alloc[MAX_PROCESSES][MAX_RESOURCES] = {{0}};
        int maxm[MAX_PROCESSES][MAX_RESOURCES] = {{0}};
        for (int k = 0; lines[k] && rows < 2 * s->process_count; ++k) {
            if (lines[k][0] == '\0' || strspn(lines[k], " \t\r") == strlen(lines[k])) {
                if (rows == s->process_count) blank_seen = 1;
                continue;
            }
            if (rows == s->process_count && !blank_seen) { ok = 0; break; }
            int row = rows < s->process_count ? rows : rows - s->process_count;
            int *target = rows < s->process_count ? alloc[row] : maxm[row];
            if (!parse_matrix_row(lines[k], target, s->resource_count)) { ok = 0; break; }
            ++rows;
        }
        if (rows != 2 * s->process_count || !blank_seen) ok = 0;
        if (ok) {
            for (int i = 0; i < s->process_count && ok; ++i)
                for (int j = 0; j < s->resource_count; ++j)
                    if (alloc[i][j] > maxm[i][j]) ok = 0;
            for (int j = 0; j < s->resource_count && ok; ++j) {
                int sum = 0;
                for (int i = 0; i < s->process_count; ++i) sum += alloc[i][j];
                if (sum > s->total_resources[j]) ok = 0;
            }
        }
        if (ok) {
            for (int i = 0; i < s->process_count; ++i) {
                for (int j = 0; j < s->resource_count; ++j) {
                    s->allocation[i][j] = alloc[i][j];
                    s->max[i][j] = maxm[i][j];
                }
            }
            recalculate(s);
            set_status(g, "Matrices updated successfully.");
        } else {
            set_status(g, "Invalid matrix input. Check rows, values, Allocation <= Maximum, and resource totals.");
        }
        g_strfreev(lines);
        g_free(text);
        refresh(g);
    }
    gtk_widget_destroy(dialog);
}

static void on_request(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    SystemState *s = g->s;
    int p = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(g->request_process));
    int r = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(g->request_resource));
    int amount = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(g->request_amount));
    if (p < 0 || p >= s->process_count || r < 0 || r >= s->resource_count) {
        set_status(g, "Select a valid process and resource after creating the system.");
        return;
    }
    if (s->status[p] != PROCESS_ACTIVE) {
        set_status(g, "Requests can only be made by active processes.");
        return;
    }
    if (amount > s->need[p][r]) {
        set_status(g, "Request exceeds the process's remaining Need.");
        return;
    }
    if (amount > s->available[r]) {
        s->request[p][r] = amount;
        set_status(g, "Request recorded as pending: insufficient Available resources.");
        refresh(g);
        return;
    }

    s->available[r] -= amount;
    s->allocation[p][r] += amount;
    s->request[p][r] = 0;
    recalculate(s);
    int seq[MAX_PROCESSES];
    if (!safe_sequence(s, seq)) {
        s->allocation[p][r] -= amount;
        recalculate(s);
        set_status(g, "Request denied: it would leave the system in an unsafe state.");
    } else {
        set_status(g, "Request granted: system remains in a safe state.");
    }
    refresh(g);
}

static GtkWidget *spin(int min, int max, int value)
{
    GtkWidget *w = gtk_spin_button_new_with_range(min, max, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(w), value);
    return w;
}

static void add_labeled(GtkWidget *box, const char *label, GtkWidget *widget)
{
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(row), gtk_label_new(label), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row), widget, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), row, FALSE, FALSE, 3);
}

static void on_exit(GtkButton *button, gpointer data)
{
    (void)button;
    Gui *g = data;
    if (g && g->window)
        gtk_widget_destroy(g->window);
}

int gui_run(SystemState *system, int *argc, char ***argv)
{
    if (!system || !argc || !argv) return 1;
    gtk_init(argc, argv);
    apply_basic_theme();
    Gui *g = g_new0(Gui, 1);
    g->s = system;

    g->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(g->window), "Deadlock Simulation System");
    gtk_window_set_default_size(GTK_WINDOW(g->window), 1100, 720);
    g_signal_connect(g->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(root), 10);
    gtk_container_add(GTK_CONTAINER(g->window), root);

    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large' weight='bold'>Deadlock Simulation System</span>");
    gtk_box_pack_start(GTK_BOX(root), title, FALSE, FALSE, 2);

    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(root), controls, FALSE, FALSE, 2);
    g->process_spin = spin(1, MAX_PROCESSES, 5);
    g->resource_spin = spin(1, MAX_RESOURCES, 3);
    add_labeled(controls, "Processes:", g->process_spin);
    add_labeled(controls, "Resources:", g->resource_spin);
    GtkWidget *create = gtk_button_new_with_label("Create / Reset System");
    g_signal_connect(create, "clicked", G_CALLBACK(on_create), g);
    gtk_box_pack_start(GTK_BOX(controls), create, FALSE, FALSE, 0);
    GtkWidget *edit = gtk_button_new_with_label("Edit Matrices");
    g_signal_connect(edit, "clicked", G_CALLBACK(on_edit_matrices), g);
    gtk_box_pack_start(GTK_BOX(controls), edit, FALSE, FALSE, 0);
    GtkWidget *totals = gtk_button_new_with_label("Set Resource Totals");
    g_signal_connect(totals, "clicked", G_CALLBACK(on_set_totals), g);
    gtk_box_pack_start(GTK_BOX(controls), totals, FALSE, FALSE, 0);

    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(root), paned, TRUE, TRUE, 0);

    g->matrices_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(g->matrices_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(g->matrices_view), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(g->matrices_view), TRUE);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), g->matrices_view);
    gtk_paned_pack1(GTK_PANED(paned), scroll, TRUE, FALSE);

    g->rag_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(g->rag_area, 500, 450);
    g_signal_connect(g->rag_area, "draw", G_CALLBACK(rag_draw), g->s);
    gtk_paned_pack2(GTK_PANED(paned), g->rag_area, TRUE, FALSE);

    GtkWidget *actions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(root), actions, FALSE, FALSE, 2);
    GtkWidget *safe = gtk_button_new_with_label("Banker's Safety Check");
    g_signal_connect(safe, "clicked", G_CALLBACK(on_safety), g);
    gtk_box_pack_start(GTK_BOX(actions), safe, FALSE, FALSE, 0);
    GtkWidget *detect = gtk_button_new_with_label("Detect Deadlock");
    g_signal_connect(detect, "clicked", G_CALLBACK(on_detect), g);
    gtk_box_pack_start(GTK_BOX(actions), detect, FALSE, FALSE, 0);
    GtkWidget *recover = gtk_button_new_with_label("Recover / Abort Victim");
    g_signal_connect(recover, "clicked", G_CALLBACK(on_recover), g);
    gtk_box_pack_start(GTK_BOX(actions), recover, FALSE, FALSE, 0);

    GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit), g);
    gtk_box_pack_end(GTK_BOX(actions), exit_button, FALSE, FALSE, 0);

    GtkWidget *requests = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(root), requests, FALSE, FALSE, 2);
    g->request_process = spin(0, MAX_PROCESSES - 1, 0);
    g->request_resource = spin(0, MAX_RESOURCES - 1, 0);
    g->request_amount = spin(0, 100000, 1);
    add_labeled(requests, "Process ID:", g->request_process);
    add_labeled(requests, "Resource ID:", g->request_resource);
    add_labeled(requests, "Request amount:", g->request_amount);
    GtkWidget *request = gtk_button_new_with_label("Submit Request");
    g_signal_connect(request, "clicked", G_CALLBACK(on_request), g);
    gtk_box_pack_start(GTK_BOX(requests), request, FALSE, FALSE, 0);

    g->status_label = gtk_label_new("System Status: Create a system to begin.");
    gtk_label_set_xalign(GTK_LABEL(g->status_label), 0);
    gtk_label_set_line_wrap(GTK_LABEL(g->status_label), TRUE);
    gtk_box_pack_start(GTK_BOX(root), g->status_label, FALSE, FALSE, 2);

    refresh(g);
    gtk_widget_show_all(g->window);
    gtk_main();
    g_free(g);
    return 0;
}
