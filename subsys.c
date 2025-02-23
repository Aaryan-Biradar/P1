#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

/* 
Initializes a Subsystem structure with a name and initial status.

Parameters:
    in/out subsystem: Subsystem structure to initialize.
    in name: Name of the subsystem (max 31 characters, null-terminated).
    in status: Initial status byte (8-bit value, not a pointer).

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystem` or `name` is NULL.
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
Prints subsystem details including status flags and queued data (if available).

Parameters:
    in subsystem: Subsystem to print.

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystem` is NULL.

Note:
    - Clears the DATA status bit after printing if data was present.
*/
int subsys_print(Subsystem *subsystem) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }
    printf(" - [Name: %-16s, Status: ", subsystem->name);
    subsys_status_print(subsystem);

    // if data is avalible add it to the end of status
    unsigned int data;
    if (subsys_data_get(subsystem, &data) == ERR_SUCCESS) {
        printf(" | Data: %08X", data);
    }

    // "]" formating for status
    printf(")]\n");
    return ERR_SUCCESS;
} 

/* 
Sets a specific status flag in the subsystem's status byte.

Parameters:
    in/out subsystem: Subsystem to modify.
    in status: Status flag to modify (STATUS_POWER, STATUS_DATA, etc.).
    in value: Value to set:
        - 0/1 for single-bit flags (POWER/DATA/ACTIVITY/ERROR).
        - 0-3 for 2-bit flags (PERFORMANCE/RESOURCE).

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystem` is NULL.
    - ERR_INVALID_STATUS if `status` is invalid or `value` is out of range.
*/
int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value) {
    if (subsystem == NULL) {                                                                // Anushka - error checking  for value?
        return ERR_NULL_POINTER;
    }

    unsigned char mask;
    switch (status) {
        // same cases of either 0 or 1
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

            mask = 0b00001100;
            // (subsystem->status & ~mask) clears the bits at the performance position, ((value << 2) & mask) sets the bits at the performance position
            subsystem->status = (subsystem->status & ~mask) | ((value << 2) & mask);
            break;
        case STATUS_RESOURCE:
            if (value > 3) {
                return ERR_INVALID_STATUS;
            }
            
            mask = 0b00000011;
            // (subsystem->status & ~mask) clears the bits at the resource position, (value & mask) sets the bits at the resource position
            subsystem->status = (subsystem->status & ~mask) | (value & mask);
            break;
        default:
            return ERR_INVALID_STATUS;
    }
    return ERR_SUCCESS;
}


/* 
Prints the subsystem's status flags in a human-readable format.

Parameters:
    in subsystem: Subsystem to print.

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystem` is NULL.

Output Format:
    [PWR: X | DATA: X | ACT: X | ERR: X | PERF: XX | RES: XX]
*/
int subsys_status_print(const Subsystem *subsystem) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    unsigned char status = subsystem->status;
    printf("(PWR: %d | DATA: %d | ACT: %d | ERR: %d | PERF: %d | RES: %d",
           (status >> STATUS_POWER) & 1,
           (status >> STATUS_DATA) & 1,
           (status >> STATUS_ACTIVITY) & 1,
           (status >> STATUS_ERROR) & 1,
           (status >> 2) & 0b00000011,
            status & 0b00000011);

    return ERR_SUCCESS;
}


/* 
Updates the subsystem's data field and sets the DATA status flag.

Parameters:
    in/out subsystem: Subsystem to modify.
    in new_data: New data value to store.
    out old_data: Optional pointer to receive previous data (NULL allowed).

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystem` is NULL.
*/
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }
    // stores old data in old_data if it is not first time setting data (aka not NULL)
    if (old_data != NULL) {
        *old_data = subsystem->data;
    }

    // store data and change status to True
    subsystem->data = new_data;
    return subsys_status_set(subsystem, STATUS_DATA, 1);
}

/* 
Retrieves the subsystem's data and clears the DATA status flag.

Parameters:
    in/out subsystem: Subsystem to read from.
    out data: Pointer to store retrieved data.

Returns:
    - ERR_SUCCESS on success (data valid).
    - ERR_NULL_POINTER if `subsystem` or `data` is NULL.
    - ERR_NO_DATA if no data is available (DATA status bit not set).

*/
int subsys_data_get(Subsystem *subsystem, unsigned int *data) {
    if (subsystem == NULL || data == NULL) {
        return ERR_NULL_POINTER;
    }
    // true if there is no data to get
    if (!((subsystem->status >> STATUS_DATA) & 1)) {
        *data = 0;
        return ERR_NO_DATA;
    }

    *data = subsystem->data;
    subsystem->data = 0;

    // clear the data status bit
    return subsys_status_set(subsystem, STATUS_DATA, 0);
}
