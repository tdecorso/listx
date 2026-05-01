#define LIST_IMPLEMENTATION
#include "list.h"
#include "bench.h"

#define ITERS 1000000
#define WARMUP 100
#define BATCH 100

#define MAX_NODES 10000

volatile int sink = 0;

int main(void) {
    BENCH("List allocation", ITERS, BATCH, WARMUP, {
        list_t* list = list_alloc(sizeof(int), MAX_NODES);
        sink += list->elem_size;
        list_free(list);
    });

    BENCH("List front insertion", ITERS, 1, WARMUP, {
        list_t* list = list_alloc(sizeof(int), MAX_NODES);
        for (int i = 0; i < MAX_NODES; ++i) {
            int* inserted = list_insert_front(list);
            *inserted = i;
            sink += *inserted;
        }
        list_free(list);
    });

    BENCH("List back insertion", ITERS, 1, WARMUP, {
        list_t* list = list_alloc(sizeof(int), MAX_NODES);
        for (int i = 0; i < MAX_NODES; ++i) {
            int* inserted = list_insert_back(list);
            *inserted = i;
            sink += *inserted;
        }
        list_free(list);
    });

    BENCH("List traversal", ITERS, 1, WARMUP, {
        list_t* list = list_alloc(sizeof(int), MAX_NODES);
        for (int i = 0; i < MAX_NODES; ++i) {
            int* d = list_insert_back(list);
            *d = i;
        }
        node_t* it = list->head;
        while (it) {
            sink += *(int*)it->data;
            it = it->next;
        }
        list_free(list);
    });

    (void)sink;

    return 0;
}
