#include <string.h>
/*
 * Copyright (c) 2021 "Lumi"
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

list_t *list_create() {
    list_t *list = calloc(1, sizeof(list_t));
    return list;
}

list_t *list_copy(list_t *list) {
    list_t *copy = list_create();
    for (int i = 0; i < list->size; i++) {
        char *c = calloc(strlen(list_at(list, i)) + 1, sizeof(char));
        strcpy(c,list_at(list, i));
        list_push_back(copy, c);
    }
    return copy;
}

void list_free(list_t *list) {
    while (list->size) {
        free(list_pop_back(list));
    }
    free(list);
}

void list_push_back(list_t *list, void *data) {
    list_node_t *node = calloc(1, sizeof(list_node_t));
    node->data = data;
    list->size++;
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
}

void list_push_front(list_t *list, void *data) {
    list_node_t *node = calloc(1, sizeof(list_node_t));
    node->data = data;
    list->size++;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        return;
    }
    list->head->prev = node;
    node->next = list->head;
    list->head = node;
}

void *list_pop_front(list_t *list) {
    list_node_t *head = list->head;
    void *data = head->data;
    list->head = list->head->next;
    free(head);
    list->size--;
    if (!list->size) {
        list->head = NULL;
        list->tail = NULL;
        return NULL;
    }
    return data;
}

void *list_pop_back(list_t *list) {
    list_node_t *tail = list->tail;
    void *data = tail->data;
    list->tail = list->tail->prev;
    free(tail);
    list->size--;
    if (!list->size) {
        list->head = NULL;
        list->tail = NULL;
        return NULL;
    }
    return data;
}

void *list_front(list_t *list) {
    if (list->head == NULL) {
        return NULL;
    }
    return list->head->data;
}

void *list_back(list_t *list) {
    if (list->tail == NULL) {
        return NULL;
    }
    return list->tail->data;
}

void *list_at(list_t *list, int index) {
    list_node_t *node = list->head;
    while (index--) {
        node = node->next;
    }
    return node->data;
}

void list_erase(list_t *list, int index) {
    list_node_t *node = list->head;
    while (index--) {
        if (node == list->tail)
            break;
        node = node->next;
    }
    if (node == list->head) {
        list->head = list->head->next;
        list->head->prev = NULL;
    } else if (node == list->tail) {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    list->size--;
    free(node->data);
    free(node);
}

size_t list_size(list_t *list) { return list->size; }

int list_find(list_t *list, void *entry, size_t size, size_t offset) {
    for (int i = 0; i < list->size; i++) {
        char *d = list_at(list, i);
        d += offset;
        if (memcmp(d, entry, size) == 0) {
            return 1;
        }
    }
    return 0;
}

int list_find_str(list_t *list, const char *entry, size_t offset) {
    if (!list->size)
        return 0;
    for (int i = 0; i < list->size; i++) {
        char *d = list_at(list, i);
        if (strcmp(d, entry) == 0) {
            return 1;
        }
    }
    return 0;
}

void list_print_str(list_t *list) {
    for (int i = 0; i < list->size; i++) {
        printf("%s ", list_at(list, i));
    }
    printf("\n");
}

stack_t *stack_create() {
    stack_t *stack = calloc(1, sizeof(stack_t));
    return stack;
}

void stack_free(stack_t *stack) {
    while (stack->size) {
        free(stack_pop(stack));
    }
    free(stack);
}

void stack_push(stack_t *stack, void *data) {
    stack_node_t *node = calloc(1, sizeof(node));
    node->data = data;
    stack->size++;
    node->prev = stack->top;
    stack->top = node;
}

void *stack_pop(stack_t *stack) {
    if (stack->top == NULL) {
        return NULL;
    }
    stack_node_t *top = stack->top;
    stack->size--;
    void *data = top->data;
    stack->top = top->prev;
    free(top);
    return data;
}

void *stack_top(stack_t *stack) {
    if (stack->top == NULL) {
        return NULL;
    }
    return stack->top->data;
}

void stack_print(stack_t *stack) {
    stack_node_t *top = stack_top(stack);
    while (top) {
        printf("%s", (char *)top->data);
        top = top->prev;
    }
}

char *read_input(const char *filename, size_t *file_size) {
    *file_size = 0;
    FILE *input_file = fopen(filename, "r");
    fseek(input_file, 0, SEEK_END);
    *file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    char *input_data = calloc(*file_size + 1, sizeof(char));
    size_t bytes_read = fread(input_data, *file_size, 1, input_file);
    fclose(input_file);
    return input_data;
}

char **split_input(char *input, size_t *count, char delimiter) {
    char *begin = input;
    int capacity = 512;
    char **lines = malloc(capacity * sizeof(char **));
    *count = 0;

    while (*input++) {
        if (*input == delimiter || *input == '\0') {
            if (capacity == *count) {
                capacity *= 2;
                lines = realloc(lines, sizeof(char *) * capacity);
            }
            size_t len = input - begin;
            lines[*count] = calloc(len + 1, sizeof(char));
            memcpy(lines[*count], begin, len * sizeof(char));
            *count += 1;
            *input++;
            begin = input;
        }
    }
    return lines;
}

int compare_int(const void *a, const void *b) {
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

int compare_size_t(const void *a, const void *b) {
    size_t _a = *(const size_t *)a;
    size_t _b = *(const size_t *)b;

    if (_a < _b)
        return 1;
    else if (_a > _b)
        return -1;
    else {
        return 0;
    }
}

void printf_set_color(enum printf_colors color) {
    switch (color) {
    case BLACK:
        printf("\033[0;30m");
        break;
    case RED:
        printf("\033[0;31m");
        break;
    case GREEN:
        printf("\033[0;32m");
        break;
    case YELLOW:
        printf("\033[0;33m");
        break;
    case BLUE:
        printf("\033[0;34m");
        break;
    case PURPLE:
        printf("\033[0;35m");
        break;
    case CYAN:
        printf("\033[0;36m");
        break;
    case WHITE:
        printf("\033[0;37m");
        break;
    }
}