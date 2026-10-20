#include <gtk/gtk.h>
#include "../Include/common.h"
#include "../Sourabh/gui.h"

int main(int argc, char *argv[])
{
    SystemState system = {0};

    return gui_run(&system, &argc, &argv);
}
