#include "heap.h"
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

heap_t *heap_create(int (*comparator)(const void *, const void *), size_t elem_size) {
    heap_t *heap = malloc(sizeof(heap_t));
    heap->elem_size = elem_size;
    heap->comparator = comparator;
    heap->size = 0;
    heap->capacity = 1;
    heap->data = calloc(heap->capacity, elem_size);
    heap->head = heap->data;
    return heap;
}

void _heap_swap(heap_t *heap, void *a, void *b) {
    char buffer[heap->elem_size];
    memcpy(buffer, a, heap->elem_size);
    memcpy(a, b, heap->elem_size);
    memcpy(b, buffer, heap->elem_size);
}

size_t _heap_get_elem_level(heap_t *heap, void *elem) {
    ptrdiff_t diff = (uintptr_t)elem - (uintptr_t)heap->data;
    size_t idx = diff / heap->elem_size;
    return 64 - _lzcnt_u64(idx);
}

void *_heap_get_level_ptr(heap_t *heap, size_t level){
    if(!level){
        return heap->data;
    }
    return &((char *)heap->data)[(1 << (level - 1)) * heap->elem_size];
}

void _heap_get_elem_level_index(heap_t *heap, void *elem, size_t *level, size_t *index){
    *level = _heap_get_elem_level(heap,elem);
    void *cur_level = _heap_get_level_ptr(heap,*level);
    ptrdiff_t diff = (uintptr_t)elem - (uintptr_t)cur_level;
    *index = diff / heap->elem_size;
}

void *_heap_get_loc_down(heap_t *heap, void *cur) {
    size_t level,index;
    _heap_get_elem_level_index(heap,cur,&level,&index);
    void *level_down = _heap_get_level_ptr(heap, level + 1);
    void *next = &((char*)level_down)[index * heap->elem_size * 2];
    if(next >= heap->head){
        return NULL;
    }
    return next;
}


void *_heap_get_loc_up(heap_t *heap, void *cur){
    size_t level,index;
    _heap_get_elem_level_index(heap,cur,&level,&index);
    if(!level){
        return NULL;
    }
    void *level_up = _heap_get_level_ptr(heap,level-1);
    void *next = &((char*)level_up)[(index/2) * heap->elem_size];
    return next;
}

void _heap_heapify_up(heap_t *heap, void *cur) {
    void *up = _heap_get_loc_up(heap,cur);
    if(!up){
        return;
    }
    int cmp = heap->comparator(cur,up);
    if(cmp > 0){
        _heap_swap(heap,cur,up);
        _heap_heapify_up(heap,up);
    }
}

void _heap_heapify_down(heap_t *heap, void *cur){
    void *down_left = _heap_get_loc_down(heap,cur);
    void *down_right = &((char*)down_left)[heap->elem_size];
    if(!down_left){
        return;
    }
    void *next = cur;
    int cmp = heap->comparator(down_left,next);
    if(cmp > 0){
        next = down_left;
    }
    if(down_right <= heap->head){
        cmp = heap->comparator(down_right,next);
        if(cmp > 0){
            next = down_right;
        }
    }
    if(next != cur){
        _heap_swap(heap,next,cur);
        _heap_heapify_down(heap,next);
    }
}

void heap_insert(heap_t *heap, void *data) {
    if (heap->size == heap->capacity) {
        heap->capacity <<= 1;
        heap->data = realloc(heap->data, heap->elem_size * heap->capacity);
        heap->head = &((char *)heap->data)[heap->elem_size * heap->size];
    }

    memcpy(heap->head, data, heap->elem_size);
    _heap_heapify_up(heap,heap->head);
    heap->head = &((char *)heap->head)[heap->elem_size];
    heap->size++;
}

void *heap_extract(heap_t *heap) {
    if (!heap->size) {
        return NULL;
    }
    heap->head = &((char *)heap->head)[-heap->elem_size];

    void *data = malloc(heap->elem_size);
    memcpy(data, heap->data, heap->elem_size);
    memcpy(heap->data, heap->head, heap->elem_size);

    --heap->size;
    _heap_heapify_down(heap,heap->data);
    return data;
}

int compare_int_higher(const void *a, const void *b) {
    int _a = *(const int *)a;
    int _b = *(const int *)b;

    if (_a < _b)
        return 1;
    else if (_a > _b)
        return -1;
    else {
        return 0;
    }
}


int compare_int_lower(const void *a, const void *b) {
    int _a = *(const int *)a;
    int _b = *(const int *)b;

    if (_a > _b)
        return 1;
    else if (_a < _b)
        return -1;
    else {
        return 0;
    }
}

int main(int argc, char **argv) {
    heap_t *heap = heap_create(compare_int_higher, sizeof(int));
    int a = 1;
    for (size_t i = 0; i < 64; i++) {
        heap_insert(heap, &i);
    }
    printf("\n");
    for (size_t i = 0; i < 70; i++) {
        int *b = heap_extract(heap);
        if (b) {
            printf("%d\n", *b);
            free(b);
        }
    }
    return EXIT_SUCCESS;
}