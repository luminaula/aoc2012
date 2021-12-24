#ifndef HEAP_H
#define HEAP_H
#include <stddef.h>



typedef struct heap_t {
    int (*comparator)(const void *, const void *);
    void *data;
	void *head;
	size_t size;
	size_t capacity;
	size_t layers;
	size_t elem_size;
} heap_t;

heap_t *heap_create(int (*comparator)(const void *, const void *),size_t elem_size);
void heap_insert(heap_t *heap, void *data);
void *heap_extract(heap_t *heap);


#endif //HEAP_H