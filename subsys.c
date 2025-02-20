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
    printf("Name: %-16s ", subsystem->name);
    subsys_status_print(subsystem);
    unsigned int data;
    int result = subsys_data_get(subsystem, &data);
    if (result == ERR_SUCCESS) {
        printf("| Data: %08X", data);
    }
    printf(")\n");
    return ERR_SUCCESS;
} 

int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value) {
    if (subsystem == NULL) {                                                                // Anushka - error checking  for value?
        return ERR_NULL_POINTER;
    }

    unsigned char mask;
    switch (status) {
        case STATUS_POWER:
        case STATUS_DATA:
        case STATUS_ACTIVITY:
        case STATUS_ERROR:
            if (value > 1) {
                return ERR_INVALID_STATUS;
            }
            mask = 1 << status;
            // (subsystem->status & ~mask) clears the bit at the status position, ((value << status) & mask) sets the bit at the status position
            subsystem->status = (subsystem->status & ~mask) | ((value << status) & mask);
            break;
        case STATUS_PERFORMANCE:
            if (value > 3) {
                return ERR_INVALID_STATUS;
            }
            // 0x0C is 00001100
            mask = 0x0C;
            // (subsystem->status & ~mask) clears the bits at the performance position, ((value << 2) & mask) sets the bits at the performance position
            subsystem->status = (subsystem->status & ~mask) | ((value << 2) & mask);
            break;
        case STATUS_RESOURCE:
            if (value > 3) {
                return ERR_INVALID_STATUS;
            }
            // 0x03 is 00000011
            mask = 0x03;
            // (subsystem->status & ~mask) clears the bits at the resource position, (value & mask) sets the bits at the resource position
            subsystem->status = (subsystem->status & ~mask) | (value & mask);
            break;
        default:
            return ERR_INVALID_STATUS;
    }
    return ERR_SUCCESS;
}

int subsys_status_print(const Subsystem *subsystem) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }
    unsigned char status = subsystem->status;
    printf("PWR: %d | DATA: %d | ACT: %d | ERR: %d | PERF: %d | RES: %d", // Anushka - change brackets once we get to the data part
           (status >> STATUS_POWER) & 1,
           (status >> STATUS_DATA) & 1,
           (status >> STATUS_ACTIVITY) & 1,
           (status >> STATUS_ERROR) & 1,
           (status >> 2) & 0x03,
           status & 0x03);
    return ERR_SUCCESS;
}

int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }
    // stores old data in old_data if it is not first time setting data
    if (old_data != NULL) {
        *old_data = subsystem->data;
    }
    subsystem->data = new_data;
    return subsys_status_set(subsystem, STATUS_DATA, 1);
}

int subsys_data_get(Subsystem *subsystem, unsigned int *data) {
    if (subsystem == NULL || data == NULL) {
        return ERR_NULL_POINTER;
    }
    // true if there is no data to get
    if (!((subsystem->status >> STATUS_DATA) & 1)) {
        *data = 0;
        return ERR_NO_DATA;
    }

    // there is data to get
    *data = subsystem->data;
    subsystem->data = 0;
    // clear the data status bit
    return subsys_status_set(subsystem, STATUS_DATA, 0);
}
