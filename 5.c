/**
 * Copyright (c) 2021 "Lumi"
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#define ARENA_DIM 20000l

typedef struct Line {
    int x0, y0, x1, y1;
} Line;

int _atoi(const char *str) {
    int val = 0;
    while (*str) {
        val = val * 10 + (*str++ - '0');
    }
    return val;
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

char *get_line(char **stream) {
    size_t len = 0;
    char *line;
    char *begin = *stream;
    while (*(*stream)++) {
        len++;
        if (**stream == '\n') {
            line = calloc(len + 1, sizeof(char));
            memcpy(line, begin, len);
            //**stream = 0;
            (*stream)++;
            return line;
        }
    }
    return NULL;
}

 int vert(Line line) {
    if (line.x0 == line.x1) {
        return 1;
    }
    return 0;
}

 int hor(Line line) {
    if (line.y0 == line.y1) {
        return 1;
    }
    return 0;
}

void p2(char *_input) {
    char *input = (char *)_input;
    char *input_line;
    unsigned int *arena = calloc(ARENA_DIM * ARENA_DIM, sizeof(unsigned int));

    int count = 0;

    while (input_line = get_line(&_input)) {
        Line line;
        char *begin = input_line;
        char *_input_line = input_line;

        // sscanf(input_line, "%d,%d -> %d,%d", &line.x0, &line.y0, &line.x1, &line.y1);
        int *line_data = (int *)&line;
        int read_index = 0;
        for (int i = 0; i < 4; i++) {

            while ((*input_line < '0' || *input_line > '9') && *input_line++)
                ;
            char *begin = input_line;
            while (*input_line >= '0' && *input_line <= '9' && *input_line++)
                ;
            *input_line++ = 0;
            line_data[i] = _atoi(begin);
        }

        if (hor(line)) {
            for (int i = MIN(line.x0, line.x1); i <= MAX(line.x0, line.x1); i++) {
                if (arena[line.y0 * ARENA_DIM + i]++ == 1)
                    count++;
            }
        } else if (vert(line)) {
            for (int i = MIN(line.y0, line.y1); i <= MAX(line.y0, line.y1); i++) {
                if (arena[i * ARENA_DIM + line.x0]++ == 1)
                    count++;
            }
        } else {
            if (line.x0 > line.x1) {
                int tmp = line.x0;
                line.x0 = line.x1;
                line.x1 = tmp;

                tmp = line.y0;
                line.y0 = line.y1;
                line.y1 = tmp;
            }

            if (line.y0 > line.y1) {
                for (int i = 0; i <= line.x1 - line.x0; i++) {
                    if (arena[(line.y0 - i) * ARENA_DIM + i + line.x0]++ == 1)
                        count++;
                }
            } else {
                for (int i = 0; i <= line.x1 - line.x0; i++) {
                    if (arena[(line.y0 + i) * ARENA_DIM + i + line.x0]++ == 1)
                        count++;
                }
            }
        }

        free(_input_line);
    }

    printf("%d\n", count);
}

void *p1(void *_input) {
    char *input = (char *)_input;
    char *input_line;
    unsigned int *arena = calloc(ARENA_DIM * ARENA_DIM, sizeof(unsigned int));

    int count = 0;

    while (input_line = get_line(&input)) {
        Line line;
        char *begin = input_line;
        char *_input_line = input_line;

        // sscanf(input_line, "%d,%d -> %d,%d", &line.x0, &line.y0, &line.x1, &line.y1);
        int *line_data = (int *)&line;
        int read_index = 0;
        for (int i = 0; i < 4; i++) {
            while ((*input_line < '0' || *input_line > '9') && *input_line++)
                ;
            char *begin = input_line;
            while (*input_line >= '0' && *input_line <= '9' && *input_line++)
                ;
            *input_line++ = 0;
            line_data[i] = _atoi(begin);
        }

        if (hor(line)) {
            for (int i = MIN(line.x0, line.x1); i <= MAX(line.x0, line.x1); i++) {
                if (arena[line.y0 * ARENA_DIM + i]++ == 1)
                    count++;
            }
        } else if (vert(line)) {
            for (int i = MIN(line.y0, line.y1); i <= MAX(line.y0, line.y1); i++) {
                if (arena[i * ARENA_DIM + line.x0]++ == 1)
                    count++;
            }
        }

        free(_input_line);
    }
    printf("%d\n", count);
}

int main(int argc, char **argv) {
    size_t file_size = 0, line_count = 0;
    char *input = read_input("5-50000-10000.in", &file_size);
    p1(input);
    p2(input);

    free(input);

    return 0;
}