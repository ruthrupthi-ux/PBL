
#include <gtk/gtk.h>
#include "gui.h"

static void on_activate(GtkApplication *app, gpointer user_data)
{
    (void)user_data;
    create_main_window(app);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
        "org.pbl.pagereplacement",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
