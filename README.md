# Arena-allocated doubly-linked list (C Implementation)

This implementation is backed by an arena allocation technique that optimized cache behavior for nodes handling. You can find in my repos a more traditional implementation of the linked list without this optimization.

## Usage
Just include the **list.h** and the **arena.h** headers in your project.

### List Allocation and Deallocation
```
#define MAX_NODES 10000
list_t* a_list = list_alloc(sizeof(int), MAX_NODES);
list_free(a_list);
```

### List node insertion
```
#define MAX_NODES 10000
list_t* a_list = list_alloc(sizeof(int), MAX_NODES);
int* first = list_insert_front(a_list); // you can also use insert_back or insert_at
*first = 1;
list_free(a_list);
```

### List node deletion
```
#define MAX_NODES 10000
list_t* a_list = list_alloc(sizeof(int), MAX_NODES);
int* first = list_insert_front(a_list); // you can also use insert_back or insert_at
*first = 1;
int removed = 0;
list_delete_at(a_list, 0, &removed); // you can also use delete_back or delete_front
list_free(a_list);
```

## Testing

Run tests:

```
gcc test.c -o test && ./test
```

## Benchmark

Benchmark utilities are included (bench.h and bench.c)

Run benchmark:
```
gcc bench.c -o bench && ./bench
```

### Results

Measured over 1e6 iterations with 100 warmup cycles:

| Operation                 |    p50   |     p90    |    p99    |
| -----------------------   | -------- | ---------- | --------- | 
| Alloc + Free              | 38.05 ns | 39.94 ns   | 178.53 ns |
| Front insert (10000)      | 40.04 µs | 43.38 µs   | 173.23 µs |
| Back insert  (10000)      | 46.26 µs | 51.62 µs   | 175.33 µs |
| Traversal    (10000)      | 58.51 µs | 72.76 µs   | 199.90 µs |

For comparison, my benchmarks show that inserting one element in this list is **~2.6** times slower than in my dynamic array.

## Improvements of the arena allocation
Insertion is **~8-9x** faster on the arena, and traversal is **~7x** faster: arena enables contiguous nodes, while traditional nodes may be scattered across the heap. That makes pointers chasing easier for the CPU.

The major benefit is that this comes from a ~50 line allocator with zero changes to the list logic itself.



