#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    void* data;
    size_t offset;
    size_t total;
} arena_t;

// Allocates memory for the whole arena with given total space
arena_t* arena_alloc(size_t total);

// Returns pointer to the current offset of the arena. Returns NULL if failed.
void* arena_borrow(arena_t* arena, size_t size);

// Frees arena memory.
void arena_free(arena_t* arena);

#ifdef ARENA_IMPLEMENTATION

arena_t* arena_alloc(size_t total) {
    arena_t* arena = malloc(sizeof(arena_t));
    if (!arena) return NULL;
    void* data = malloc(total);
    if (!data) return NULL;
    arena->data = data;
    arena->offset = 0;
    arena->total = total;
    return arena;
}

void* arena_borrow(arena_t* arena, size_t size) {
    if (!arena) return NULL;
    if (arena->offset + size > arena->total) return NULL;
    uint8_t* current = (uint8_t*)arena->data + arena->offset;
    if (!current) return NULL;
    arena->offset += size;
    return current;
}

void arena_free(arena_t* arena) {
    free(arena->data);
    free(arena);
}

#endif // ARENA_IMPLEMENTATION
