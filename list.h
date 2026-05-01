#pragma once

#define ARENA_IMPLEMENTATION
#include "arena.h"

#include <stdlib.h>
#include <string.h>

typedef struct node_t node_t;

struct node_t {
    void* data;
    node_t* next;
    node_t* prev;
};

typedef struct {
    node_t* head;
    node_t* tail;
    size_t elem_size;
    size_t count;
    arena_t* arena;
} list_t;

// Allocates memory for the node using elem_size. Returns NULL if failed.
node_t* node_alloc(arena_t* arena, size_t elem_size);

// Allocates memory for the list using elem_size. Returns NULL if failed.
list_t* list_alloc(size_t elem_size, size_t max_nodes);

// Frees list memory including nodes memory. Safe on NULL pointer.
void list_free(list_t* list);

// Inserts a new head to the list and returns pointer to its data. Returns NULL if failed.
void* list_insert_front(list_t* list);

// Inserts a new tail to the list and returns pointer to its data. Returns NULL if failed.
void* list_insert_back(list_t* list);

// Inserts a new node at specified index and returns pointer to its data. Returns NULL if failed.
void* list_insert_at(list_t* list, size_t index);

// Removes head from list and copies its data to dest.
void list_delete_front(list_t* list, void* dest);

// Removes tail from list and copies its data to dest.
void list_delete_back(list_t* list, void* dest);

// Deletes node at specified index and copies its data to dest.
void list_delete_at(list_t* list, size_t index, void* dest);

#ifdef LIST_IMPLEMENTATION

node_t* node_alloc(arena_t* arena, size_t elem_size) {
    node_t* n = arena_borrow(arena, sizeof(node_t));
    if (!n) return NULL;
    void* data = arena_borrow(arena, elem_size);
    if (!data) return NULL;
    n->data = data;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

list_t* list_alloc(size_t elem_size, size_t max_nodes) {
    size_t total_size = sizeof(list_t) + max_nodes * (sizeof(node_t) + elem_size);
    arena_t* arena = arena_alloc(total_size);
    if (!arena) return NULL;

    list_t* l = arena_borrow(arena, sizeof(list_t));
    if (!l) return NULL;

    l->head = NULL;
    l->tail = NULL;
    l->elem_size = elem_size;
    l->count = 0;
    l->arena = arena;
    return l;
}

void list_free(list_t* list) {
    if (!list) return;
    arena_free(list->arena);
}

void* list_insert_front(list_t* list) {
    if (!list) return NULL;
    
    node_t* head = node_alloc(list->arena, list->elem_size);
    if (!head) return NULL;

    if (list->head) {
        list->head->prev = head;
    }
    else { 
        list->tail = head;
    }
    head->next = list->head;
    list->head = head;
    list->count++;

    return list->head->data;
}

void* list_insert_back(list_t* list) {
    if (!list) return NULL;
    
    node_t* tail = node_alloc(list->arena, list->elem_size);
    if (!tail) return NULL;

    if (list->tail) {
        list->tail->next = tail;
    }
    else { 
        list->head = tail;
    }
    tail->prev = list->tail;
    list->tail = tail;
    list->count++;

    return list->tail->data;
}

void* list_insert_at(list_t* list, size_t index) {
    if (!list) return NULL;
    if (index >= list->count) return NULL;

    if (index == 0) return list_insert_front(list);
    if (index + 1 == list->count) return list_insert_back(list);

    node_t* it = NULL;
    if (index >= list->count / 2) {
        it = list->tail;
        size_t i = list->count - 1;
        while (it && i > index) {
            it = it->prev;
            i -= 1;
        }
    }
    else {
        it = list->head;
        size_t i = 0;
        while (it && i < index) {
            it = it->next;
            i += 1;
        }
    }

    node_t* n = node_alloc(list->arena, list->elem_size);
    if (it->prev) it->prev->next = n;
    it->prev = n;
    n->next = it;
    list->count++;
    return n->data;
}

void list_delete_front(list_t* list, void* dest) {
    if (!list) return;
    if (!dest) return;
    node_t* head = list->head;
    if (!head) return;
    node_t* next = head->next;
    if (next) next->prev = NULL;
    memcpy(dest, head->data, list->elem_size);
    list->head = next;
    list->count--;
}

void list_delete_back(list_t* list, void* dest) {
    if (!list) return;
    if (!dest) return;
    node_t* tail = list->tail;
    if (!tail) return;
    node_t* prev = tail->prev;
    if (prev) prev->next = NULL;
    memcpy(dest, tail->data, list->elem_size);
    list->tail = prev;
    list->count--;
}

void list_delete_at(list_t* list, size_t index, void* dest) {
    if (!list) return;
    if (!dest) return;
    if (index >= list->count) return;

    if (index == 0) return list_delete_front(list, dest);
    if (index + 1 == list->count) return list_delete_back(list, dest);

    node_t* it = NULL;
    if (index >= list->count / 2) {
        it = list->tail;
        size_t i = list->count - 1;
        while (it && i > index) {
            it = it->prev;
            i -= 1;
        }
    }
    else {
        it = list->head;
        size_t i = 0;
        while (it && i < index) {
            it = it->next;
            i += 1;
        }
    }
    if (it->prev) it->prev->next = it->next;
    if (it->next) it->next->prev = it->prev;
    memcpy(dest, it->data, list->elem_size);
    list->count--;
}

#endif // LIST_IMPLEMENTATION
