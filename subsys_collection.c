#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

/* 
Initializes an empty SubsystemCollection by setting its size to 0.

Parameters:
    out subsystems: Pointer to the SubsystemCollection to initialize.

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystems` is NULL.
*/
int subsys_collection_init(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    subsystems->size = 0;

    return ERR_SUCCESS;
}

/* 
Adds a copy of a subsystem to the collection. Prints a success message on addition.

Parameters:
    in/out subsystems: Pointer to the SubsystemCollection to modify.
    in subsystem: Pointer to the Subsystem to copy into the collection.

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystems` or `subsystem` is NULL.
    - ERR_MAX_CAPACITY if the collection is full (MAX_ARR reached).
*/
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem) {
    if (subsystems == NULL || subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    if (subsystems->size >= MAX_ARR) {
        return ERR_MAX_CAPACITY;
    }

    subsystems->subsystems[subsystems->size] = *subsystem;
    subsystems->size++;
    // "Subsystem '%s' added successfully.\n" as seen in project overview
    printf("Subsystem '%s' added successfully.\n", subsystem->name);

    return ERR_SUCCESS;

}


/* 
Finds the first subsystem in the collection with a matching name.

Parameters:
    in subsystems: Pointer to the SubsystemCollection to search.
    in name: Name of the subsystem to find.

Returns:
    - Index of the subsystem if found.
    - ERR_SYS_NOT_FOUND if no match is found.
    - ERR_NULL_POINTER if `subsystems` or `name` is NULL.
*/
int subsys_find(const SubsystemCollection *subsystems, const char *name) {
    if (subsystems == NULL || name == NULL) {
        return ERR_NULL_POINTER;
    }

    for (int i = 0; i < subsystems->size; i++) {
        // strcmp returns 0 if strings are equal
        if (strcmp(subsystems->subsystems[i].name, name) == 0) {
            return i;
        }
    }

    return ERR_SYS_NOT_FOUND;
}


/* 
Prints all subsystems in the collection using `subsys_print()`.

Parameters:
    in subsystems: Pointer to the SubsystemCollection to print.

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystems` is NULL.
*/
int subsys_collection_print(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    for (int i = 0; i < subsystems->size; i++) {
        // returns address of subsystems[i] to subsys_print
        subsys_print(&subsystems->subsystems[i]);
    }

    return ERR_SUCCESS;
}   

/* 
Removes a subsystem from the collection by index and shifts remaining elements left.

Parameters:
    in/out subsystems: Pointer to the SubsystemCollection to modify.
    in index: Index of the subsystem to remove.

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `subsystems` is NULL.
    - ERR_INVALID_INDEX if `index` is out of bounds.
*/
int subsys_remove(SubsystemCollection *subsystems, int index){
    if (subsystems == NULL){
        return ERR_NULL_POINTER;
    }
    if (index < 0 || index >= subsystems->size){
        return ERR_INVALID_INDEX;
    }

    //shift values right of index left by 1
    for (int i = index; i < subsystems->size - 1; i++){
        subsystems->subsystems[i] = subsystems->subsystems[i + 1];
    }

    subsystems->size--;
    return ERR_SUCCESS;
}

/* 
Filters subsystems into a destination collection based on a status bitmask filter.

Parameters:
    in src: Pointer to the source SubsystemCollection.
    out dest: Pointer to the destination SubsystemCollection for filtered results.
    in filter: 8-character filter string

Returns:
    - ERR_SUCCESS on success.
    - ERR_NULL_POINTER if `src`, `dest`, or `filter` is NULL.
    - ERR_INVALID_STATUS if `filter` contains invalid characters.
    - ERR_MAX_CAPACITY if `dest` is full during appending.
*/
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter) {
    if (src == NULL || dest == NULL || filter == NULL) {
        return ERR_NULL_POINTER;
    }

    unsigned char filter_mask = 0;
    unsigned char wildcard_mask = 0;

    for (int i = 0; i < 8; i++) {
        if (filter[i] == '1') {
            filter_mask |= 1 << (7 - i); // Set the bit for '1'
        } else if (filter[i] == '*') {
            wildcard_mask |= 1 << (7 - i); // Set the bit for '*'
        } else if (filter[i] != '0') {
            return ERR_INVALID_STATUS; // Handle invalid characters
        }
    }

    filter_mask = ~filter_mask;
    subsys_collection_init(dest);

    for (int i = 0; i < src->size; i++) {
        Subsystem sub = src->subsystems[i];
        // unsigned char result = ((filter_mask ^ sub.status) | wildcard_mask);1
        if (((filter_mask ^ sub.status) | wildcard_mask) == 0b11111111) {
            int append_result = subsys_append(dest, &sub);
            if (append_result != ERR_SUCCESS) {
                return append_result;
            }
        }
    }
    return ERR_SUCCESS;
}