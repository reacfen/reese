#include "reese_core.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Array of pointers of captured variables
static void **cap_pointers_chain = NULL;
static size_t cap_pointers_chain_length = 0;

// Array of pointers of return values of member functions invoked using the captured variable
static void **cap_results_chain = NULL;
static size_t cap_results_chain_length = 0;

// Checks if the pointer to the return value of the currently invoked function is NULL
static bool is_null = false;
// Checks if the pointer to the return value was recently added to the chain
static bool is_recently_added = false;

REESE_API void *reese_allocate(size_t const byte_size, ...) {
    // Allocate sufficient memory
    void *memory = malloc(byte_size);
    // Fill the memory with the required value
    memcpy(memory, (void *)((uintptr_t)&byte_size + sizeof(size_t)), byte_size);
    // Return the newly created memory
    return memory;
}

REESE_API void *reese_set_capture(void *capture_var) {
    // If the captured results is more than one, then flush it
    if (cap_results_chain_length > 0)
        reese_finish_capture();
    // Allocate/reallocate the chain of pointers of variables
    cap_pointers_chain = realloc(cap_pointers_chain, ++cap_pointers_chain_length * sizeof(void *));
    // Add the captured variable to the chain and return it
    return cap_pointers_chain[cap_pointers_chain_length - 1] = capture_var;
}

REESE_API void *reese_get_capture(void) {
    // Return the most recently captured pointer to variable
    return cap_pointers_chain[cap_pointers_chain_length - 1];
}

REESE_API void reese_set_capture_result(void *capture_result) {
    // Check if the captured pointer is not NULL
    if (!(is_null = capture_result == NULL)) {
        // Notify the other functions that the variable was recently captured
        is_recently_added = true;
        // Allocate/reallocate the chain of return values
        cap_results_chain = realloc(cap_results_chain, ++cap_results_chain_length * sizeof(void *));
        // Add the captured variable to the chain and return it
        cap_results_chain[cap_results_chain_length - 1] = capture_result;
    }
}

REESE_API const void *reese_get_capture_result(void) {
    // Check if the most recent capture result is NULL, if yes, return NULL
    if (is_null) return NULL;
    // Free the last pointer to the return value if it was not recently added
    if (is_recently_added)
        is_recently_added = false;
    else
        free(cap_results_chain[cap_results_chain_length]);
    // Decrease the length of the pointers to variables
    --cap_pointers_chain_length;
    // Decrease the length of the pointers to return values
    --cap_results_chain_length;
    // Returns the last pointer to return value in the chain
    return cap_results_chain[cap_results_chain_length];
}

REESE_API void reese_finish_capture(void) {
    // Free the captured pointers chain if it is not NULL
    if (cap_pointers_chain != NULL)
        free(cap_pointers_chain);
    // Check if the chain of pointers to return values of all invoked member functions is not NULL
    if (cap_results_chain != NULL) {
        // Free all the pointers to return values inside the chain
        for (size_t i = 0; i < cap_results_chain_length; i++)
            free(cap_results_chain[i]);
        // Free the chain of pointers to return values
        free(cap_results_chain);
    }
    // Set the chain of captured pointers and the chain of pointers to return values to NULL
    cap_pointers_chain = cap_results_chain = NULL;
    // Set the length of both chains to zero
    cap_pointers_chain_length = cap_results_chain_length = 0;
}
