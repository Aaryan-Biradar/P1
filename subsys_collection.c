#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

/* 
Initializes an empty SubsystemCollection.

out subsystems: Collection to initialize

Returns:
    - ERR_SUCCESS on success
    - ERR_NULL_POINTER if subsystems is NULL
*/
int subsys_collection_init(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    subsystems->size = 0;

    return ERR_SUCCESS;
}

/* 
Adds a copy of a subsystem to the collection.

in/out subsystems: Collection to modify
in subsystem: Subsystem to copy into collection

Returns:
    - ERR_SUCCESS on success
    - ERR_NULL_POINTER if subsystems/subsystem is NULL
    - ERR_MAX_CAPACITY if collection is full
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
Finds first subsystem with matching name in collection.

in subsystems: Collection to search
in name: Name to search for

Returns:
    - Index of subsystem if found
    - ERR_SYS_NOT_FOUND if not found
    - ERR_NULL_POINTER if subsystems/name is NULL
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
Prints all subsystems in a collection.

in/out subsystems: Collection to print
Returns:
    - ERR_SUCCESS on success
    - ERR_NULL_POINTER if subsystems is NULL
    - ERR_NO_DATA if collection is empty
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

int subsys_remove(SubsystemCollection *subsystems, int index){
    if (subsystems == NULL){
        return ERR_NULL_POINTER;
    }
    if (index < 0 || index >= subsystems->size){
        return ERR_INVALID_INDEX;
    }
    for (int i = index; i < subsystems->size - 1; i++){
        subsystems->subsystems[i] = subsystems->subsystems[i + 1];
    }
    subsystems->size--;
    return ERR_SUCCESS;
}


int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter){
    if (src == NULL || dest == NULL || filter == NULL){
        return ERR_NULL_POINTER;
    }

    unsigned char filter_mask = 0;
    unsigned char wildcard_mask = 0;

    for (int i = 0; i < 8; i++){
        if (filter[i] == '1'){
            filter_mask |= (1 << (7 - i));
        }
        else if (filter[i] == '*'){
            wildcard_mask |= (1 << (7 - i));
        }
        else {
            return ERR_INVALID_STATUS;
        }

        filter_mask = ~filter_mask;

        subsys_collection_init(dest);

        for (int i = 0; i < src->size; i++){
            if (((filter_mask ^ (src->subsystems[i]).status) | wildcard_mask) == 0b11111111){
                int result = subsys_append(dest, &(src->subsystems[i]));
                if (result != ERR_SUCCESS){
                    return result;
                }
            }
        }
    }

    return ERR_SUCCESS;
}