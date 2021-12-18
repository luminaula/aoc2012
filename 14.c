#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct polymer_pair_t {
    char a[3];
    char b;
    long count;
	long tmp;
} polymer_pair_t;

int sort_pairs(const void *a, const void *b) {
    polymer_pair_t _a = *(polymer_pair_t *)a;
    polymer_pair_t _b = *(polymer_pair_t *)b;
    return strcmp(_a.a, _b.a);
}

polymer_pair_t *get_pairs(char **lines, size_t line_count) {
    polymer_pair_t *pairs = malloc(sizeof(polymer_pair_t) * line_count);

    for (int i = 0; i < line_count; i++) {
        char **tokens = split_input(lines[i], NULL, ' ');
        if (strlen(tokens[0]) == 2) {
            strcpy(pairs[i].a, tokens[0]);
        }
        pairs[i].b = tokens[2][0];
        pairs[i].count = 0;
    }

    return pairs;
}

void populate_pairs(const char *str, polymer_pair_t *pairs, size_t pair_count) {
    while (*str) {
        if (!*(str + 1)) {
            break;
        }
        for (int i = 0; i < pair_count; i++) {
            char buffer[3];
            buffer[0] = str[0];
            buffer[1] = str[1];
            buffer[2] = 0;

            if (strcmp(buffer, pairs[i].a) == 0) {
                pairs[i].count++;
            }
        }
        str++;
    }
}

polymer_pair_t *search_pair(polymer_pair_t *pairs, size_t count, char *name) { // hash table next years
    for (int i = 0; i < count; i++) {
        if (strcmp(pairs[i].a, name) == 0) {
            return &pairs[i];
        }
    }
	return NULL;
}

int main(int argc, char **argv) {
    size_t file_size, line_count, pair_count;
    char *input = read_input("input14", &file_size);
    char **input_lines = split_input(input, &line_count, '\n');

    pair_count = line_count - 1;
    polymer_pair_t *pairs = get_pairs(input_lines + 1, pair_count);

    qsort(pairs, pair_count, sizeof(polymer_pair_t), sort_pairs);
    populate_pairs(input_lines[0], pairs, pair_count);

    char buffer[3] = {0};
	long results[0xff] = {0};

    for (int k = 0; k < 40; k++) {
        for (int i = 0; i < pair_count; i++) {
            buffer[0] = pairs[i].a[0];
            buffer[1] = pairs[i].b;
			polymer_pair_t *pair =  search_pair(pairs, pair_count, buffer);
			pair->tmp += pairs[i].count;
			buffer[0] = pairs[i].b;
			buffer[1] = pairs[i].a[1];
			pair =  search_pair(pairs, pair_count, buffer);
			pair->tmp += pairs[i].count;
			pairs[i].count = 0;
        }
		for (int i = 0; i < pair_count; i++){
			pairs[i].count += pairs[i].tmp;
			pairs[i].tmp = 0;
		}
    }
	// count first and last
	results[input_lines[0][0]]++;
	results[input_lines[0][strlen(input_lines[0])-1]]++;

    for (int i = 0; i < pair_count; i++) {
		 results[pairs[i].a[0]] += pairs[i].count;
		 results[pairs[i].a[1]] += pairs[i].count;
    }
	size_t highest = 0,lowest=-1;
	for(int i='A'; i<'Z'; i++){
		highest = MAX(highest,results[i] / 2);
		if(results[i])
			lowest = MIN(lowest,results[i] / 2);
	}
	printf("%lu\n",highest - lowest);


    return 0;
}