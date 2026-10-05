#include <stdio.h>
#include <string.h>

#define MAX_INPUT 100

/* =========================================================
   INPUT FUNCTIONS
   ========================================================= */

// Read an integer safely
int get_integer(const char *message)
{
    int value;

    printf("%s", message);

    while (scanf("%d", &value) != 1)
    {
        printf("Invalid input. Please enter a number: ");

        while (getchar() != '\n')
            ;
    }

    // Clear remaining input
    while (getchar() != '\n')
        ;

    return value;
}

// Read a string safely
void get_string(const char *message, char *buffer, int size)
{
    printf("%s", message);

    fgets(buffer, size, stdin);

    // Remove newline
    buffer[strcspn(buffer, "\n")] = '\0';
}

// Get process information from user
void get_process_input(int *process_id, int *burst_time, int *priority)
{
    printf("\n========================================\n");
    printf("          PROCESS INPUT\n");
    printf("========================================\n");

    *process_id = get_integer("Enter Process ID: ");
    *burst_time = get_integer("Enter Burst Time: ");
    *priority = get_integer("Enter Priority: ");

    printf("\nProcess information captured successfully.\n");
}


/* =========================================================
   OUTPUT / DISPLAY FUNCTIONS
   ========================================================= */

// Display main menu
void display_menu(void)
{
    printf("\n");
    printf("========================================\n");
    printf("       MULTI-PROCESS SIMULATOR\n");
    printf("========================================\n");
    printf("1. Create Process\n");
    printf("2. Execute Process\n");
    printf("3. View Process Status\n");
    printf("4. View Execution Result\n");
    printf("5. Exit\n");
    printf("========================================\n");
}

// Display process information
void display_process(int process_id, int burst_time, int priority)
{
    printf("\n========================================\n");
    printf("          PROCESS INFORMATION\n");
    printf("========================================\n");
    printf("Process ID  : %d\n", process_id);
    printf("Burst Time  : %d\n", burst_time);
    printf("Priority    : %d\n", priority);
    printf("========================================\n");
}

// Display process status
void display_status(int process_id, const char *status)
{
    printf("\n========================================\n");
    printf("          PROCESS STATUS\n");
    printf("========================================\n");
    printf("Process ID : %d\n", process_id);
    printf("Status     : %s\n", status);
    printf("========================================\n");
}

// Display execution result
void display_result(int process_id, const char *result)
{
    printf("\n========================================\n");
    printf("          EXECUTION RESULT\n");
    printf("========================================\n");
    printf("Process ID : %d\n", process_id);
    printf("Result     : %s\n", result);
    printf("========================================\n");
}

// Display error message
void display_error(const char *message)
{
    printf("\n[ERROR] %s\n", message);
}

// Display success message
void display_success(const char *message)
{
    printf("\n[SUCCESS] %s\n", message);
}


/* =========================================================
   MAIN FUNCTION
   ========================================================= */

int main()
{
    int choice;
    int process_id = 0;
    int burst_time = 0;
    int priority = 0;

    printf("\n========================================\n");
    printf("     MULTI-PROCESS SIMULATOR UI\n");
    printf("          STUDENT 1 - UI\n");
    printf("========================================\n");

    while (1)
    {
        display_menu();

        choice = get_integer("Enter your choice: ");

        switch (choice)
        {
            case 1:
                // Get process information
                get_process_input(
                    &process_id,
                    &burst_time,
                    &priority
                );

                // Display entered information
                display_process(
                    process_id,
                    burst_time,
                    priority
                );

                display_success(
                    "Process input is ready to be sent to Core."
                );
                break;


            case 2:
                // Dummy response for now
                if (process_id == 0)
                {
                    display_error(
                        "No process has been created yet."
                    );
                }
                else
                {
                    printf("\nSending execution request to Core...\n");

                    // Mock Core response
                    display_status(
                        process_id,
                        "RUNNING"
                    );

                    printf("\nCore response received.\n");

                    display_status(
                        process_id,
                        "COMPLETED"
                    );
                }
                break;


            case 3:
                // Dummy process status
                if (process_id == 0)
                {
                    display_error(
                        "No process available."
                    );
                }
                else
                {
                    display_status(
                        process_id,
                        "READY"
                    );
                }
                break;


            case 4:
                // Dummy execution result
                if (process_id == 0)
                {
                    display_error(
                        "No process has been created."
                    );
                }
                else
                {
                    display_result(
                        process_id,
                        "Process executed successfully."
                    );
                }
                break;


            case 5:
                printf("\n========================================\n");
                printf("Exiting Multi-Process Simulator...\n");
                printf("Thank you!\n");
                printf("========================================\n");

                return 0;


            default:
                display_error(
                    "Invalid choice. Please select 1-5."
                );
        }
    }

    return 0;
}
