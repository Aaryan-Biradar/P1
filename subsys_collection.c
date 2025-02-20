#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

int subsys_collection_init(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    subsystems->size = 0;

    return ERR_SUCCESS;
}


int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem) {
    if (subsystems == NULL || subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    if (subsystems->size >= MAX_ARR) {
        return ERR_MAX_CAPACITY;
    }

    subsystems->subsystems[subsystems->size] = *subsystem;
    subsystems->size++;

}

int subsys_find(const SubsystemCollection *subsystems, const char *name) {
    if (subsystems == NULL || name == NULL) {
        return ERR_NULL_POINTER;
    }

    for (int i = 0; i < subsystems->size; i++) {
        if (strcmp(subsystems->subsystems[i].name, name) == 0) {
            return i;
        }
    }

    return ERR_SYS_NOT_FOUND;
}

int subsys_collection_print(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    for (int i = 0; i < subsystems->size; i++) {
        subsys_print(&subsystems->subsystems[i]);                                               //prints adress not value
    }

    return ERR_SUCCESS;
}   

