#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// memory leaks? never heard

int calc_basin(int **array, int **visited, int x, int y) {
    if (visited[y][x] || array[y][x] == 9)
        return 0;
    visited[y][x] = 1;
    return calc_basin(array, visited, x + 1, y) + calc_basin(array, visited, x, y + 1) + calc_basin(array, visited, x - 1, y) +
           calc_basin(array, visited, x, y - 1) + 1;
}



int main(int argc, char **argv) {
    size_t file_size, line_count;
    char *input = read_input("input9", &file_size);
    char **split = split_input(input, &line_count, '\n');

    line_count += 2;
    int **heights = calloc(line_count, sizeof(int *));
    int **p1 = calloc(line_count, sizeof(int *));

    size_t len = strlen(split[0]);
    len += 2;
    for (int i = 0; i < line_count; i++) {
        heights[i] = calloc(len, sizeof(int));
        p1[i] = calloc(len, sizeof(int));
    }
    for (int i = 0; i < line_count - 2; i++) {
        for (int j = 0; j < len - 2; j++) {
            heights[i + 1][j + 1] = split[i][j] - 48;
        }
    }

    for (int i = 0; i < len; i++) {
        heights[i][0] = 9;
        heights[i][len - 1] = 9;
        heights[0][i] = 9;
        heights[len - 1][i] = 9;
    }

    for (int i = 1; i < line_count - 1; i++) {
        for (int j = 1; j < line_count - 1; j++) {
            int h = heights[i][j];
            int up = 0, down = 0, left = 0, right = 0;

            up = heights[i - 1][j];
            left = heights[i][j - 1];
            down = heights[i + 1][j];
            right = heights[i][j + 1];

            if (h < up && h < down && h < left && h < right) {
                p1[i][j] = h + 1;
            }
        }
    }

    size_t basin_area = 0;

    int **visited = calloc(line_count, sizeof(int *));
    for (int i = 0; i < line_count; i++) {
        visited[i] = calloc(len, sizeof(int));
    }

    size_t res = 0;
    size_t basin_count = 0;
    for (int i = 0; i < line_count; i++) {
        for (int j = 0; j < line_count; j++) {
            if (p1[i][j]) {
                res += p1[i][j];
                basin_count++;
            }
        }
    }

    int *basin_sizes = calloc(basin_count, sizeof(int));

    int c = 0;
    for (int i = 0; i < line_count; i++) {
        for (int j = 0; j < line_count; j++) {
            if(p1[i][j]){
                basin_sizes[c++] = calc_basin(heights,visited,j,i);
            }
        }
    }

    qsort(basin_sizes,basin_count,sizeof(int),compare_int);
    basin_area = basin_sizes[0] * basin_sizes[1] * basin_sizes[2];

    printf("%u\n", res);
    printf("%u\n", basin_area);
    return 0;
}
