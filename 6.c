#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

char *read_input(char *filename, size_t *file_size) {
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

int _atoi(const char *str, size_t len) {
    int val = 0;
    for (int i = 0; i < len; i++)
        val = val * 10 + (*str++ - '0');
    return val;
}

int *parse_ints(char *input, size_t *count) {
    size_t capacity = 1;
    *count = 0;
    int *data = malloc(1 * sizeof(int));
    while (*input) {
        while ((*input < '0' || *input > '9') && *input)
            input++;
        char *begin = input;
        while ((*input >= '0' && *input <= '9') && *input)
            input++;
        if (*count >= capacity) {
            capacity *= 2;
            data = realloc(data, capacity * sizeof(int));
        }
        data[*count] = _atoi(begin, input - begin);
        (*count)++;
        input++;
    }
    return data;
}

int main(int argc, char **argv) {
    size_t file_size, input_count;
    char *input_data = read_input("input6", &file_size);

    int *fishes = parse_ints(input_data, &input_count);

    uint64_t num_fishes = 0;


    uint64_t fish_days[9] = {};

    for (int i = 0; i < input_count; i++) {
        fish_days[fishes[i]]++;
    }
    for (int i = 0; i < 256; i++) {
        uint64_t tmp = fish_days[0];
        fish_days[0] = fish_days[1];
        fish_days[1] = fish_days[2];
        fish_days[2] = fish_days[3];
        fish_days[3] = fish_days[4];
        fish_days[4] = fish_days[5];
        fish_days[5] = fish_days[6];
        fish_days[6] = tmp;
        fish_days[6] += fish_days[7];
        fish_days[7] = fish_days[8];
        fish_days[8] = tmp;



    }

    num_fishes = 0;
    for (int i = 0; i < 9; i++) {
        num_fishes += fish_days[i];
    }
    printf("%" PRIu64 "\n", num_fishes);

    return 0;
}