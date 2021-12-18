#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct point_t {
    int x, y;
} point_t;

int compare_point_t(const void *_a, const void *_b) {
    point_t a = *(point_t *)_a;
    point_t b = *(point_t *)_b;

    if (a.x < b.x)
        return 1;
    else if (a.x > a.y)
        return -1;
    return 0;
}

void origami(const char *file_name) {
    size_t file_size, line_count;
    char *input = read_input(file_name, &file_size);
    char **lines = split_input(input, &line_count, '\n');
    int i;

    point_t *point_array = malloc(sizeof(point_t));
    size_t point_count = 0;
    size_t point_capacity = 1;

    for (i = 0; i < line_count; i++) {
        if (lines[i][0] == 'f') {
            break;
        }
        size_t token_count;
        char **tokens = split_input(lines[i], &token_count, ',');

        if (point_count > point_capacity) {
            point_capacity <<= 1;
            point_array = realloc(point_array, point_capacity * sizeof(point_t));
        }

        point_t *point = &point_array[point_count++];
        point->x = atoi(tokens[0]);
        point->y = atoi(tokens[1]);
        for (int j = 0; j < token_count; j++) {
            free(tokens[j]);
        }
        free(tokens);
    }
    point_array = realloc(point_array, sizeof(point_t) * point_count);

    int p1 = 1;
    for (; i < line_count; i++) {
        char *it = lines[i];
        while (*it != 'x' && *it != 'y') {
            it++;
        }
        char **tokens = split_input(it, NULL, '=');
        char axis = tokens[0][0];
        int idx = atoi(tokens[1]);

        for (int j = 0; j < point_count; j++) {
            switch (axis) {
            case 'x':
                if (point_array[j].x > idx) {
                    point_array[j].x = idx - (point_array[j].x - idx);
                }
                break;
            case 'y':
                if (point_array[j].y > idx) {
                    point_array[j].y = idx - (point_array[j].y - idx);
                }
                break;
            }
        }
        for (int j = 0; j < point_count; j++) {
            point_t a = point_array[j];
            for (int k = j + 1; k < point_count; k++) {
                point_t b = point_array[k];
                if (a.x == b.x && a.y == b.y) {
                    point_array[k].x = -1;
                    point_array[k].y = -1;
                }
            }
        }
        if (p1) {
            p1 = 0;
            int count = 0;
            for (int j = 0; j < point_count; j++) {
                if (point_array[j].x != -1) {
                    count++;
                }
            }
            printf("%d\n", count);
        }
    }
    qsort(point_array, point_count, sizeof(point_t), compare_point_t);
    point_count = 0;
    while (1) {
        if (point_array[point_count].x == -1) {
            break;
        }
        point_count++;
    }
    point_array = realloc(point_array, sizeof(point_t) * point_count +1);

    int width = 0, height = 0;

    for (int i = 0; i < point_count; i++) {
        width = MAX(width, point_array[i].x);
        height = MAX(height, point_array[i].y);
    }
    width++;
    height++;

    char *paper = calloc(width * height, sizeof(char));
    memset(paper, ' ', width * height);

    for (int i = 0; i < point_count; i++) {
        point_t p = point_array[i];
        paper[p.y * width + p.x] = 'X';
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", paper[i * width + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    origami("input13");
    return 0;
}