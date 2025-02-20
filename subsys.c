#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

/* 
Initializes a Subsystem structure with a name and initial status.

in/out subsystem: Subsystem structure to initialize
in name: Name of the subsystem (max 31 characters)
in status: Initial status value for the subsystem

Returns:
    - ERR_SUCCESS on success
    - ERR_NULL_POINTER if subsystem/name is NULL
*/
int subsys_init(Subsystem *subsystem, const char *name, char status) {
    if (subsystem == NULL || name == NULL) {                                            // Anushka - check if status is NULL?
        return ERR_NULL_POINTER;
    }

    strncpy(subsystem->name, name, MAX_STR - 1);
    subsystem->name[MAX_STR - 1] = '\0';
    subsystem->status = status;
    subsystem->data = 0;

    return ERR_SUCCESS;
}

/* 
Prints subsystem details including status flags and queued data.

in/out subsystem: Subsystem to print

Returns:
    - ERR_SUCCESS on success
    - ERR_NULL_POINTER if subsystem is NULL
*/
int subsys_print(Subsystem *subsystem) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // same format as seen in project overview however, name is printed from the left side
    printf("Name: %-16s | ", subsystem->name);
    printf("Status: %d | \n", subsystem->status);
    //subsys_status_print(subsystem);

    return ERR_SUCCESS;
}   

