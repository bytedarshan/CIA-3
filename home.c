#include <stdio.h>
#include <stdlib.h>

// Forward declarations
void entry();
void bill();

int main() {
    int choice, again;

    do {
        system("cls"); // clear screen on Windows
        printf("                      |----------------------------------------------------------------------|\n");
        printf("                      |                       Darshan's Billing Software                     |\n");
        printf("                      |----------------------------------------------------------------------|\n");
        printf("                      |                  Press 1 for DATA ENTRY                              |\n");
        printf("                      |                  Press 2 for BILLING                                 |\n");
        printf("                      |                  Press 3 to EXIT                                     |\n");
        printf("                      |----------------------------------------------------------------------|\n");

        printf("                                         Enter here : ");
        scanf("%d", &choice);

        printf("                      |----------------------------------------------------------------------|\n");
        if (choice == 1) {
            entry();
        } else if (choice == 2) {
            bill();
        } else if (choice == 3) {
            printf("Exiting program...\n");
            break;
        } else {
            printf("                      |                  PLEASE ENTER A VALID ENTRY                          |\n");
            printf("                      |----------------------------------------------------------------------|\n");
        }

        // After finishing an operation
        printf("\nDo you want to perform another operation?\n");
        printf("1 - Go back to Home Menu\n");
        printf("2 - Exit\n");
        printf("Enter choice: ");
        scanf("%d", &again);

    } while (again == 1);

    return 0;
}

