
#include "gui.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../ananya/fifo.h"
#include "../Ranvil/lru.h"
#include "../Sourabh/optimal.h"

#define MAX_TEXT 12000

static GtkWidget *pages_entry;
static GtkWidget *frames_spin;
static GtkWidget *algorithm_combo;
static GtkWidget *results_view;
static GtkWidget *status_label;

static void show_message(GtkWindow *parent, GtkMessageType type,
                         const char *message)
{
    GtkWidget *dialog = gtk_message_dialog_new(
        parent, GTK_DIALOG_MODAL, type, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static int parse_pages(const char *text, int pages[], int *count)
{
    const char *p = text;
    int n = 0;

    while (*p) {
        char *end;
        long value;

        while (isspace((unsigned char)*p) || *p == ',')
            ++p;

        if (*p == '\0')
            break;

        if (*p == '-' || *p == '+') {
            if (*p == '-') return -1;
        }

        value = strtol(p, &end, 10);
        if (end == p || value < 0 || value > 2147483647L)
            return -1;

        if (n >= MAX_PAGES)
            return -1;

        pages[n++] = (int)value;
        p = end;

        if (*p && !isspace((unsigned char)*p) && *p != ',')
            return -1;
    }

    if (n == 0)
        return -1;

    *count = n;
    return 0;
}

static void append_result(GString *out, const char *name,
                          const SimulationResult *r, int frame_count)
{
    int i, j;

    g_string_append_printf(out, "\n%s\n", name);
    g_string_append(out, "Ref\tPage\t");

    for (j = 0; j < frame_count; ++j)
        g_string_append_printf(out, "F%d\t", j + 1);

    g_string_append(out, "Status\n");

    for (i = 0; i < r->total_references; ++i) {
        g_string_append_printf(out, "%d\t%d\t",
                               i + 1, r->steps[i].page);

        for (j = 0; j < frame_count; ++j) {
            if (r->steps[i].frames[j] == -1)
                g_string_append(out, "-\t");
            else
                g_string_append_printf(out, "%d\t",
                                        r->steps[i].frames[j]);
        }

        g_string_append_printf(out, "%s\n",
                               r->steps[i].hit ? "Hit" : "Fault");
    }

    g_string_append_printf(
        out,
        "References: %d | Hits: %d | Faults: %d\n"
        "Hit ratio: %.2f%% | Fault ratio: %.2f%%\n",
        r->total_references, r->total_hits, r->total_faults,
        r->hit_ratio, r->fault_ratio);
}

static void on_run_clicked(GtkButton *button, gpointer data)
{
    GtkWindow *window = GTK_WINDOW(data);
    const char *input = gtk_entry_get_text(GTK_ENTRY(pages_entry));
    int pages[MAX_PAGES];
    int n, frame_count;
    const char *selected;
    SimulationResult fifo = {0}, lru = {0}, optimal = {0};
    GString *out;

    (void)button;

    if (parse_pages(input, pages, &n) != 0) {
        show_message(window, GTK_MESSAGE_ERROR,
                     "Enter non-negative page numbers separated by spaces or commas.");
        return;
    }

    frame_count = gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(frames_spin));

    if (frame_count < 1 || frame_count > MAX_FRAMES) {
        show_message(window, GTK_MESSAGE_ERROR,
                     "Frame count must be between 1 and 100.");
        return;
    }

    selected = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo));

    if (!selected)
        return;

    out = g_string_new("");

    if (strcmp(selected, "FIFO") == 0 ||
        strcmp(selected, "Compare All") == 0) {
        if (simulate_fifo(pages, n, frame_count, &fifo) != 0) {
            show_message(window, GTK_MESSAGE_ERROR, "FIFO simulation failed.");
            g_string_free(out, TRUE);
            g_free((gpointer)selected);
            return;
        }

        /* FIFO returns ratios as fractions; convert to percentages. */
        fifo.hit_ratio *= 100.0;
        fifo.fault_ratio *= 100.0;
        append_result(out, "FIFO", &fifo, frame_count);
    }

    if (strcmp(selected, "LRU") == 0 ||
        strcmp(selected, "Compare All") == 0) {
        if (simulate_lru(pages, n, frame_count, &lru) != 0) {
            show_message(window, GTK_MESSAGE_ERROR, "LRU simulation failed.");
            g_string_free(out, TRUE);
            g_free((gpointer)selected);
            return;
        }
        append_result(out, "LRU", &lru, frame_count);
    }

    if (strcmp(selected, "Optimal") == 0 ||
        strcmp(selected, "Compare All") == 0) {
        if (simulate_optimal(pages, n, frame_count, &optimal) != 0) {
            show_message(window, GTK_MESSAGE_ERROR, "Optimal simulation failed.");
            g_string_free(out, TRUE);
            g_free((gpointer)selected);
            return;
        }
        append_result(out, "Optimal", &optimal, frame_count);
    }

    if (strcmp(selected, "Compare All") == 0) {
        g_string_append(out, "\nCOMPARISON SUMMARY\n");
        g_string_append(out,
            "Algorithm\tHits\tFaults\tHit ratio\tFault ratio\n");

        g_string_append_printf(out, "FIFO\t%d\t%d\t%.2f%%\t%.2f%%\n",
            fifo.total_hits, fifo.total_faults,
            fifo.hit_ratio, fifo.fault_ratio);

        g_string_append_printf(out, "LRU\t%d\t%d\t%.2f%%\t%.2f%%\n",
            lru.total_hits, lru.total_faults,
            lru.hit_ratio, lru.fault_ratio);

        g_string_append_printf(out, "Optimal\t%d\t%d\t%.2f%%\t%.2f%%\n",
            optimal.total_hits, optimal.total_faults,
            optimal.hit_ratio, optimal.fault_ratio);
    }

    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(results_view)),
        out->str, -1);

    gtk_label_set_text(GTK_LABEL(status_label), "Simulation completed.");
    g_string_free(out, TRUE);
    g_free((gpointer)selected);
}

static void on_reset_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    gtk_entry_set_text(GTK_ENTRY(pages_entry), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(frames_spin), 3);
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithm_combo), 0);
    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(results_view)), "", -1);
    gtk_label_set_text(GTK_LABEL(status_label), "Ready.");
}

void create_main_window(GtkApplication *app)
{
    GtkWidget *window, *root, *grid, *label;
    GtkWidget *buttons, *run_button, *reset_button;
    GtkWidget *scroll;
    int row = 0;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Page Replacement Algorithm Simulator");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 650);

    root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(root), 14);
    gtk_container_add(GTK_CONTAINER(window), root);

    label = gtk_label_new(
        "Simulation and Comparative Analysis of Page Replacement Algorithms");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(root), label, FALSE, FALSE, 0);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_box_pack_start(GTK_BOX(root), grid, FALSE, FALSE, 0);

    label = gtk_label_new("Page references:");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);

    pages_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(pages_entry),
                                   "Example: 7 0 1 2 0 3 0 4 2 3");
    gtk_widget_set_hexpand(pages_entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), pages_entry, 1, row++, 2, 1);

    label = gtk_label_new("Number of frames:");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);

    frames_spin = gtk_spin_button_new_with_range(1, MAX_FRAMES, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(frames_spin), 3);
    gtk_grid_attach(GTK_GRID(grid), frames_spin, 1, row++, 1, 1);

    label = gtk_label_new("Algorithm:");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);

    algorithm_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "FIFO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LRU");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Optimal");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Compare All");
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithm_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), algorithm_combo, 1, row++, 1, 1);

    buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(root), buttons, FALSE, FALSE, 0);

    run_button = gtk_button_new_with_label("Run Simulation");
    reset_button = gtk_button_new_with_label("Reset");
    gtk_box_pack_start(GTK_BOX(buttons), run_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(buttons), reset_button, FALSE, FALSE, 0);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(root), scroll, TRUE, TRUE, 0);

    results_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(results_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(results_view), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(results_view), TRUE);
    gtk_container_add(GTK_CONTAINER(scroll), results_view);

    status_label = gtk_label_new("Ready.");
    gtk_widget_set_halign(status_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(root), status_label, FALSE, FALSE, 0);

    g_signal_connect(run_button, "clicked",
                     G_CALLBACK(on_run_clicked), window);
    g_signal_connect(reset_button, "clicked",
                     G_CALLBACK(on_reset_clicked), window);

    gtk_widget_show_all(window);
}
