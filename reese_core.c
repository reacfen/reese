#include "reese_core.h"
#include <stdlib.h>
#include <string.h>

static void **cap_pointers_chain = NULL;
static size_t cap_pointers_chain_length = 0;

static void **cap_results_chain = NULL;
static size_t cap_results_chain_length = 0;

static _Bool is_null = 0;
static _Bool has_recently_added = 0;

void *reese_allocate(size_t const byte_size, ...) {
    void *memory = malloc(byte_size);
    memcpy(memory, (void *) ((uintptr_t) &byte_size + sizeof(size_t)), byte_size);
    return memory;
}

void *reese_set_capture(void *capture_var) {
    if (cap_results_chain_length > 0)
        reese_finish_capture();
    cap_pointers_chain = realloc(cap_pointers_chain, ++cap_pointers_chain_length * sizeof(void *));
    return cap_pointers_chain[cap_pointers_chain_length - 1] = capture_var;
}

void *reese_get_capture(void) {
    return cap_pointers_chain[cap_pointers_chain_length - 1];
}

void reese_set_capture_result(void *capture_result) {
    is_null = capture_result == NULL;
    if (!is_null) {
        has_recently_added = 1;
        cap_results_chain = realloc(cap_results_chain, ++cap_results_chain_length * sizeof(void *));
        cap_results_chain[cap_results_chain_length - 1] = capture_result;
    }
}

const void *reese_get_capture_result(void) {
    if (is_null) return NULL;
    if (!has_recently_added)
        free(cap_results_chain[cap_results_chain_length]);
    else
        has_recently_added = 0;
    --cap_pointers_chain_length;
    --cap_results_chain_length;
    return cap_results_chain[cap_results_chain_length];
}

void reese_finish_capture(void) {
    if (cap_pointers_chain != NULL)
        free(cap_pointers_chain);
    if (cap_results_chain != NULL) {
        for (size_t i = 0; i < cap_results_chain_length; i++)
            free(cap_results_chain[i]);
        free(cap_results_chain);
    }
    cap_pointers_chain = cap_results_chain = NULL;
    cap_pointers_chain_length = cap_results_chain_length = 0;
}
