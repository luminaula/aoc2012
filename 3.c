#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tree;

struct Tree {
    struct Tree *l, *r;
    long value;
};

char *read_input(char *filename, size_t *file_size) {
    FILE *input_file = fopen(filename, "r");
    fseek(input_file, 0, SEEK_END);
    *file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    char *input_data = aligned_alloc(64, *file_size + 1 + 64);
    size_t bytes_read = fread(input_data, *file_size, 1, input_file);
    input_data[*file_size] = 0;
    fclose(input_file);
    return input_data;
}

int main(int argc, char **argv) {
    size_t file_size;
    size_t line_len;
    char *file_data = read_input("input3", &file_size);
    char *_file_data = file_data;
    for (line_len = 0; file_data[line_len] && file_data[line_len] != '\n'; line_len++)
        ;
    size_t num_lines = file_size / (line_len + 1);

    __m256i ascii_ones = _mm256_set1_epi8('1');
    __m256i one = _mm256_set1_epi8(1);
    char *buffer = aligned_alloc(32, 64);
    __m256i results[4];
    __m256i ones[2];
    struct Tree *tree = calloc(1, sizeof(struct Tree));
    for (int i = 0; i < num_lines; i++) {
        memset(buffer, 0, 64);
        memcpy(buffer, file_data, line_len);
        //printf("%d\n", i);

        for (int j = 0; j < 2; j++) {
            ones[j] = _mm256_loadu_si256((__m256i *)&buffer[j * 32]);
            ones[j] = _mm256_cmpeq_epi8(ones[j], ascii_ones);
			ones[j] = _mm256_and_si256(ones[j],one);


            __m256i ones16[2];
            __m256i ones32[4];

            ones16[0] = _mm256_cvtepi8_epi16(_mm256_extractf128_si256(ones[j], 0));
            ones16[1] = _mm256_cvtepi8_epi16(_mm256_extractf128_si256(ones[j], 1));

            ones32[0] = _mm256_cvtepu16_epi32(_mm256_extractf128_si256(ones16[0], 0));
            ones32[1] = _mm256_cvtepu16_epi32(_mm256_extractf128_si256(ones16[0], 1));
            ones32[2] = _mm256_cvtepu16_epi32(_mm256_extractf128_si256(ones16[1], 0));
            ones32[3] = _mm256_cvtepu16_epi32(_mm256_extractf128_si256(ones16[1], 1));

            for (int k = 0; k < 4; k++) {
                results[k] = _mm256_add_epi32(results[k], ones32[k]);
            }
        }

        struct Tree *it = tree;
        char *lows = (char*)&ones[0];
        for (int j = 0; j < line_len; j++) {
            if (lows[j]) {
                if (it->l == NULL) {
                    it->l = calloc(1, sizeof(struct Tree));
                }
                it->value++;
                it = it->l;
            } else {
                if (it->r == NULL) {
                    it->r = calloc(1, sizeof(struct Tree));
                }
                it->value--;
                it = it->r;
            }
        }

        file_data += line_len + 1;
    }
    int *data = &results[0];
    long gamma = 0;
    long epsilon = 0;
    for (int i = 0; i < line_len; i++) {
        gamma |= (data[i] > num_lines ? 0l : 1l) << (line_len - i - 1);
        epsilon |= (data[i] < num_lines ? 0l : 1l) << (line_len - i - 1);
    }
    printf("%ld\n", gamma * epsilon);
    struct Tree *it = tree;
    long ox = 0;
    for (int i = 0; i < line_len; i++) {
			if (it->value >= 0 && it->l) {
				ox |= 1 << (line_len - i);
				it = it->l;
			} else if(it->r){
				it = it->r;
			}
		
    }
    it = tree;
    long co = 0;
    for (int i = 0; i < line_len; i++) {
		printf("%ld ",it->value);

		if (it->value < 0 && it->l) {
			co |= 1l << (line_len - i);
			it = it->l;
		} else{
			if(it->r){
				it = it->r;
			}
			else{
				it = it->l;
			}
		}
    }
    printf("%ld\n", ox * co);
    free(_file_data);
    return 0;
}
