#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count_lines(char *input) {
    size_t lines = 0;
    while (*input++) {
        if (*input == '\n') {
            lines++;
        }
    }
    return lines;
}

char *read_input(char *filename) {
    FILE *input_file;
    char *input_data;
    size_t file_size;
    input_file = fopen(filename, "r");

    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    input_data = malloc(file_size + 1);
    size_t bytes_read = fread(input_data, file_size, 1, input_file);
    input_data[file_size] = 0;
    fclose(input_file);
    return input_data;
}

int main(int argc, char **argv) {
    char *raw_input = read_input("input");
    char *_raw_input = raw_input;
    int higher2 = 0;
    int higher1 = 0;
    int i = 0;
    size_t line_count = count_lines(raw_input);
    int *buffer = aligned_alloc(64, line_count * sizeof(int) + 64);
    int *buffer2 = aligned_alloc(64, line_count * sizeof(int) + 64);
    for (size_t i = 0; i < line_count; i++) {
        buffer[i] = strtol(_raw_input, &_raw_input, 10);
        buffer2[i + 3] = buffer[i];
    }

    int count = -3;
    for (size_t i = 0; i < line_count; i += 8 * 7) {
        __m256i a1 = _mm256_load_si256((__m256i *)&buffer[i]);
        __m256i a2 = _mm256_load_si256((__m256i *)&buffer[i + 8]);
        __m256i a3 = _mm256_load_si256((__m256i *)&buffer[i + 8 * 2]);
        __m256i a4 = _mm256_load_si256((__m256i *)&buffer[i + 8 * 3]);
        __m256i a5 = _mm256_load_si256((__m256i *)&buffer[i + 8 * 4]);
        __m256i a6 = _mm256_load_si256((__m256i *)&buffer[i + 8 * 5]);
        __m256i a7 = _mm256_load_si256((__m256i *)&buffer[i + 8 * 6]);

        __m256i b1 = _mm256_load_si256((__m256i *)&buffer2[i]);
        __m256i b2 = _mm256_load_si256((__m256i *)&buffer2[i + 8]);
        __m256i b3 = _mm256_load_si256((__m256i *)&buffer2[i + 8 * 2]);
        __m256i b4 = _mm256_load_si256((__m256i *)&buffer2[i + 8 * 3]);
        __m256i b5 = _mm256_load_si256((__m256i *)&buffer2[i + 8 * 4]);
        __m256i b6 = _mm256_load_si256((__m256i *)&buffer2[i + 8 * 5]);
        __m256i b7 = _mm256_load_si256((__m256i *)&buffer2[i + 8 * 6]);

        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpgt_epi32(a1, b1))) / 4;
        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpgt_epi32(a2, b2))) / 4;
        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpgt_epi32(a3, b3))) / 4;
        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpgt_epi32(a4, b4))) / 4;
        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpgt_epi32(a5, b5))) / 4;
        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpgt_epi32(a6, b6))) / 4;
        count += _mm_popcnt_u32(_mm256_movemask_epi8(_mm256_cmpgt_epi32(a7, b7))) / 4;
    }
    printf("%d\n", count);
    free(raw_input);

    return 0;
}
