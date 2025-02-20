#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

int init_subsystem(Subsystem *subsystem, const char *name, char status) {
    if (subsystem == NULL || name == NULL || status == NULL) {
        return ERR_NULL_POINTER;
    }

    strncpy(subsystem->name, name, MAX_STR - 1);
    subsystem->name[MAX_STR - 1] = '\0';
    subsystem->status = status;
    subsystem->data = 0;

    return ERR_SUCCESS;
}


