#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7

int print_menu(int *choice);

int main() {
    // Use a while loop to print the menu for the user and call the appropriate functions.
    // The definitions above are provided to assist with this.

    SubsystemCollection collection;
    Subsystem sub;
    subsys_collection_init(&collection);
    int choice;
    int result;
    int index;
    char name[MAX_STR];
    char filter_str[9];
    unsigned char status_id, value;
    unsigned int data;

    while (1) {
        print_menu(&choice);
        switch (choice) {
            case MENU_ADD:
                printf("Enter subsystem name (no spaces): ");
                scanf("%31s", name);
                subsys_init(&sub, name, 0);
                result = subsys_append(&collection, &sub);
                if (result != ERR_SUCCESS) {
                    printf("Error adding subsystem.\n");
                } 
                else {
                    printf("Subsystem '%s' added successfully.\n", name);
                }
                break;
            case MENU_PRINT:
                printf("Enter subsystem name to print: ");
                scanf("%31s", name);
                index = subsys_find(&collection, name);
                if (index == ERR_NULL_POINTER) {
                    printf("Null pointer error.\n");
                } 
                else if (index == ERR_SYS_NOT_FOUND){
                    printf("No subsystem found at this location.\n");
                }
                else {
                    result = subsys_print(&collection.subsystems[index]);
                    
                    if (result != ERR_SUCCESS){
                        printf("Error printing subsystem.");
                    }
                }
                break;
            case MENU_PRINTALL:
                printf("\n");
                result = subsys_collection_print(&collection);
                if (result != ERR_SUCCESS) {
                    printf("No subsystems to display.\n");
                }
                break;
            case MENU_STATUS:
                printf("Enter: <Subsystem Name> <Status ID (0, 2, 4, 5, 6, 7)> <New Value (0, 1, 2, 3)> ");
                scanf("%31s %hhu %hhu", name, &status_id, &value);
                index = subsys_find(&collection, name);
                if (index == ERR_NULL_POINTER) {
                    printf("Null pointer error.\n");
                } 
                else if (index == ERR_SYS_NOT_FOUND){
                    printf("No subsystem found at this location.\n");
                } 
                else {
                    result = subsys_status_set(&collection.subsystems[index], status_id, value);
                    if (result != ERR_SUCCESS) {
                        printf("Invalid status update or new value.\n");
                    }
                }
                break;
            case MENU_REMOVE:
                printf("Enter subsystem name (no spaces): ");
                scanf("%31s", name);
                index = subsys_find(&collection, name);
                if (index == ERR_NULL_POINTER) {
                    printf("Null pointer error.\n");
                } 
                else if (index == ERR_SYS_NOT_FOUND){
                    printf("No subsystem found with this name.\n");
                } 
                else {
                    subsys_remove(&collection, index);
                    printf("Subsystem '%s' removed successfully.", name);
                }
                break;
            case MENU_FILTER:
                printf("Enter filter (8 chars 0/1/*): ");
                scanf("%8s", filter_str);
                SubsystemCollection filtered;
                result = subsys_filter(&collection, &filtered, (const unsigned char*)filter_str);
                if (result == ERR_SUCCESS) {
                    subsys_collection_print(&filtered);
                } else {
                    printf("Filter error.\n");
                }
                break;
            case MENU_DATA:
                printf("Enter name and data (HEX in all CAPS without 0x): ");
                scanf("%31s %X", name, &data);
                index = subsys_find(&collection, name);
                if (index == ERR_NULL_POINTER) {
                    printf("Null pointer error.\n");
                } 
                else if (index == ERR_SYS_NOT_FOUND){
                    printf("No subsystem found at this location.\n");
                } 
                else {
                    unsigned int old_data;
                    result = subsys_data_set(&collection.subsystems[index], data, &old_data);
                    if (result != ERR_SUCCESS) {
                        printf("Data set error.\n");
                    }
                }    
                break;
            case MENU_EXIT:
                return 0;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
    return 0;
}


/* 
    Prints a menu to the user and prompts them until they enter a valid menu option.

    out: *choice, an integer corresponding to a MENU_ definition defining the choice made by the user.

    Returns: 
        - ERR_SUCCESS if there is no error
        - ERR_NULL_POINTER if `choice` is NULL

*/
int print_menu(int *choice) {
    if (choice == NULL) {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if (*choice < MENU_MIN || *choice > MENU_MAX) {
            printf("Error: Invalid choice. Please try again.\n");
        }
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}