#include "rag.h"

#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846

static void draw_text(cairo_t *cr, double x, double y,
                      const char *text, double size, int bold)
{
    cairo_select_font_face(cr, "Sans",
                           CAIRO_FONT_SLANT_NORMAL,
                           bold ? CAIRO_FONT_WEIGHT_BOLD
                                : CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, size);
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text);
}

static void draw_arrow(cairo_t *cr,
                       double x1, double y1,
                       double x2, double y2,
                       double red, double green, double blue)
{
    double angle = atan2(y2 - y1, x2 - x1);
    const double head = 8.0;

    cairo_set_source_rgb(cr, red, green, blue);
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);

    cairo_move_to(cr, x2, y2);
    cairo_line_to(cr,
                  x2 - head * cos(angle - PI / 6.0),
                  y2 - head * sin(angle - PI / 6.0));
    cairo_move_to(cr, x2, y2);
    cairo_line_to(cr,
                  x2 - head * cos(angle + PI / 6.0),
                  y2 - head * sin(angle + PI / 6.0));
    cairo_stroke(cr);
}

static double node_y(int index, int count, double top, double bottom,
                     double center)
{
    if (count <= 1)
        return center;
    return top + index * (bottom - top) / (count - 1);
}

static void draw_process(cairo_t *cr, const SystemState *s,
                         int i, double x, double y, double radius)
{
    if (s->status[i] == PROCESS_DEADLOCKED)
        cairo_set_source_rgb(cr, 1.0, 0.72, 0.72);
    else if (s->status[i] == PROCESS_FINISHED)
        cairo_set_source_rgb(cr, 0.78, 0.90, 0.78);
    else if (s->status[i] == PROCESS_ABORTED)
        cairo_set_source_rgb(cr, 0.84, 0.84, 0.84);
    else
        cairo_set_source_rgb(cr, 0.82, 0.90, 1.0);

    cairo_arc(cr, x, y, radius, 0, 2 * PI);
    cairo_fill_preserve(cr);

    cairo_set_line_width(cr, 1.5);
    cairo_set_source_rgb(cr, 0.12, 0.32, 0.56);
    cairo_stroke(cr);

    char label[32];
    snprintf(label, sizeof(label), "P%d", i);
    cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
    draw_text(cr, x - 10, y + 4, label, 11, 1);

    const char *status = "";
    if (s->status[i] == PROCESS_DEADLOCKED) status = "DEADLOCKED";
    else if (s->status[i] == PROCESS_FINISHED) status = "FINISHED";
    else if (s->status[i] == PROCESS_ABORTED) status = "ABORTED";

    if (status[0] != '\0') {
        cairo_text_extents_t ext;
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 9);
        cairo_text_extents(cr, status, &ext);
        cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
        draw_text(cr, x - ext.width / 2.0, y + radius + 13,
                  status, 9, 0);
    }
}

static void draw_resource(cairo_t *cr, const SystemState *s,
                          int j, double x, double y, double half)
{
    int allocated = 0;
    for (int i = 0; i < s->process_count; ++i)
        allocated += s->allocation[i][j];

    int total = s->total_resources[j];
    int available = total - allocated;
    if (available < 0)
        available = 0;

    cairo_set_source_rgb(cr, 0.96, 0.88, 0.70);
    cairo_rectangle(cr, x - half, y - half, half * 2, half * 2);
    cairo_fill_preserve(cr);

    cairo_set_line_width(cr, 1.5);
    cairo_set_source_rgb(cr, 0.56, 0.38, 0.10);
    cairo_stroke(cr);

    char label[32];
    snprintf(label, sizeof(label), "R%d", j);
    cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
    draw_text(cr, x - 10, y + 4, label, 11, 1);

    char details[64];
    snprintf(details, sizeof(details), "T:%d A:%d Av:%d",
             total, allocated, available);
    draw_text(cr, x + half + 7, y + 4, details, 9, 0);
}

void rag_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    SystemState *s = (SystemState *)user_data;
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.98, 0.99, 1.0);
    cairo_paint(cr);

    if (width <= 0 || height <= 0) {
        cairo_restore(cr);
        return;
    }

    if (s == NULL || s->process_count <= 0 || s->resource_count <= 0) {
        cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
        draw_text(cr, 20, 35,
                  "Create a system to display the Resource Allocation Graph.",
                  13, 0);
        cairo_restore(cr);
        return;
    }

    int np = s->process_count;
    int nr = s->resource_count;

    if (np > MAX_PROCESSES || nr > MAX_RESOURCES) {
        cairo_restore(cr);
        return;
    }

    const double left_margin = 32.0;
    const double right_margin = 32.0;
    const double process_radius = 19.0;
    const double resource_half = 18.0;
    double process_x = fmax(left_margin + process_radius, width * 0.22);
    double resource_x = fmin(width - right_margin - resource_half - 90.0,
                             width * 0.68);
    if (resource_x < process_x + 100.0)
        resource_x = process_x + 100.0;

    double top = 48.0;
    double bottom = height - 65.0;
    if (bottom < top)
        bottom = top;

    double process_y[MAX_PROCESSES];
    double resource_y[MAX_RESOURCES];

    for (int i = 0; i < np; ++i)
        process_y[i] = node_y(i, np, top, bottom, height / 2.0);
    for (int j = 0; j < nr; ++j)
        resource_y[j] = node_y(j, nr, top, bottom, height / 2.0);

    /* Draw only valid edges from current allocation/request state. */
    for (int i = 0; i < np; ++i) {
        if (s->status[i] == PROCESS_ABORTED ||
            s->status[i] == PROCESS_FINISHED)
            continue;

        for (int j = 0; j < nr; ++j) {
            if (s->allocation[i][j] > 0) {
                char label[24];
                snprintf(label, sizeof(label), "%d", s->allocation[i][j]);
                draw_arrow(cr,
                           resource_x - resource_half, resource_y[j],
                           process_x + process_radius, process_y[i],
                           0.12, 0.42, 0.72);

                cairo_set_source_rgb(cr, 0.05, 0.25, 0.55);
                draw_text(cr,
                          (process_x + resource_x) / 2.0 - 15,
                          (process_y[i] + resource_y[j]) / 2.0 - 5,
                          label, 11, 1);
            }

            if (s->request[i][j] > 0) {
                char label[24];
                snprintf(label, sizeof(label), "%d", s->request[i][j]);
                draw_arrow(cr,
                           process_x + process_radius, process_y[i],
                           resource_x - resource_half, resource_y[j],
                           0.80, 0.18, 0.16);

                cairo_set_source_rgb(cr, 0.65, 0.10, 0.10);
                draw_text(cr,
                          (process_x + resource_x) / 2.0 + 5,
                          (process_y[i] + resource_y[j]) / 2.0 + 14,
                          label, 11, 1);
            }
        }
    }

    for (int i = 0; i < np; ++i)
        draw_process(cr, s, i, process_x, process_y[i], process_radius);

    for (int j = 0; j < nr; ++j)
        draw_resource(cr, s, j, resource_x, resource_y[j], resource_half);

    /* Legend */
    cairo_set_source_rgb(cr, 0.12, 0.42, 0.72);
    draw_text(cr, 10, height - 20, "Blue: Allocation (R -> P)", 10, 0);
    cairo_set_source_rgb(cr, 0.80, 0.18, 0.16);
    draw_text(cr, fmax(220.0, width * 0.48), height - 20,
              "Red: Request (P -> R)", 10, 0);

    cairo_restore(cr);
}


