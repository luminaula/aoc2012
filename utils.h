// Copyright (c) 2021 "Lumi"
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <stdlib.h>


enum printf_colors { BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, PURPLE = 5, CYAN = 6, WHITE = 7 };

struct list_node_t;
struct stack_node_t;

typedef struct list_node_t {
    struct list_node_t *prev, *next;
    void *data;
} list_node_t;

typedef struct list_t {
    struct list_node_t *head, *tail;
    size_t size;
} list_t;

typedef struct stack_node_t {
    struct stack_node_t *prev;
    void *data;
} stack_node_t;

typedef struct stack_t {
    struct stack_node_t *top;
    size_t size;
} stack_t;

// classes in C...?

list_t *list_create();
list_t *list_copy(list_t *list);
void list_free(list_t *list);
void list_push_front(list_t *list, void *data);
void list_push_back(list_t *list, void *data);
void *list_front(list_t *list);
void *list_back(list_t *list);
void *list_at(list_t *list, int index);
void list_erase(list_t *list, int index);
void *list_pop_front(list_t *list);
void *list_pop_back(list_t *list);
int list_find(list_t *list, void *entry, size_t size, size_t offset);
int list_find_str(list_t *list, const char *entry, size_t offset);
int list_find_cave(list_t *list, const char *entry);
size_t list_size(list_t *list);
void list_print_str(list_t *list);

stack_t *stack_create();
void stack_free(stack_t *stack);
void stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
void *stack_top(stack_t *stack);
void stack_print(stack_t *stack);

char *read_input(const char *filename, size_t *file_size);
char **split_input(char *input, size_t *count, char delimiter);

int compare_int(const void *a, const void *b);
int compare_size_t(const void *a, const void *b);

void printf_set_color(enum printf_colors color);