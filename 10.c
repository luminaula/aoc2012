#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *lefts = "[{(<";

const char *rights = "]})>";

int main(int argc, char **argv) {
    size_t file_size, line_count;
    char *input = read_input("input10", &file_size);
    char **lines = split_input(input, &line_count, '\n');
    printf_set_color(WHITE);
    size_t result = 0;
    list_t *result2_list =list_create();

    for (int i = 0; i < line_count; i++) {
        char *it = lines[i];
        list_t *list = list_create();
        while (*it) {

            char *data = calloc(1, sizeof(char));
            *data = *it;
            list_push_back(list, data);
            *it++;
        }
        for (int j = 0; j < list->size; j++) {
            //
            char c_right = *(char *)list_at(list, j);
            char *idx_right = strchr(rights, c_right);
            int norm_idx_right = idx_right - rights;
            if (norm_idx_right < 4 && norm_idx_right >= 0) {
                int k = j - 1;
                char c_left = *(char *)list_at(list, k);
                char *idx_left = strchr(lefts, c_left);
                int norm_idx_left = idx_left - lefts;
                if (norm_idx_left == norm_idx_right) {
                    list_erase(list, k);
                    list_erase(list, k);
                    j = 0;
                }
            }
        }
        int corrupted = 0;
        for (int j = 0; j < list->size; j++) {
            char c = *(char *)list_at(list, j);
            char *idx = strchr(rights, c);
            if (idx) {
                corrupted = 1;
                switch (*idx) {
                case ']':
                    result += 57;
                    break;
                case '}':
                    result += 1197;
                    break;
                case ')':
                    result += 3;
                    break;
                case '>':
                    result += 25137;
                    break;
                }
                break;
            }
        }
        if (!corrupted) {
			size_t *result2 = calloc(1,sizeof(size_t));
            for (int j = list->size - 1; j >= 0; j--) {
				*result2 = *result2 * 5;
                char c = *(char *)list_at(list, j);
                switch (c) {
                case '[':
                    *result2 = *result2 + 2;
                    break;
                case '{':
                    *result2 = *result2 + 3;
                    break;
                case '(':
                    *result2 = *result2 + 1;
                    break;
                case '<':
                    *result2 = *result2 + 4;
                    break;
                }
            }
			list_push_back(result2_list,result2);
			// printf("%u\n",*result2);
        }
        list_free(list);
    }

	size_t *r2 = calloc(result2_list->size,sizeof(size_t));

	for(int i=0; i < result2_list->size; i++){
		r2[i] = *(size_t*)list_at(result2_list,i);
	}

	qsort(r2,result2_list->size,sizeof(size_t),compare_size_t);

    printf_set_color(RED);
    printf("%u %u\n", result,r2[result2_list->size /2]);
    printf_set_color(WHITE);
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
    free(input);

    return 0;
}
