#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

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

int compare_int(const void *_a, const void *_b){
	int a = *(const int*)_a;
	int b = *(const int*)_b;

	if(a > b){
		return 1;
	}
	if(a < b){
		return -1;
	}
	return 0;
}

int main(int argc, char **argv){
	size_t input_count, file_size;
	char *input = read_input("input7",&file_size);
	int *crabs = parse_ints(input,&input_count);

	int max = 0;

	for(int i=0; i<input_count;i++){
		max = MAX(max,crabs[i]);
	}

	qsort(crabs,input_count,sizeof(int),compare_int);

	int median = crabs[input_count / 2];

	int f= 0;
	int total = 0;
	for(int i=0; i<input_count;i++){
		f += abs(crabs[i]- median);
		total += crabs[i];
	}
	printf("%d\n",f);

	int mean = 0;

	mean = total / input_count;
	int fuel = 0;
	

	for(int i=0; i<input_count; i++){
		int step = 1;
		if(mean < crabs[i]){
			for(int j=mean; j<crabs[i]; j++){
				fuel += step;
				step++;
			}
		}
		else if(mean > crabs[i]){
			for(int j=crabs[i]; j <mean; j++){
				fuel += step;
				step++;
			}
		}

	}

	printf("%d\n",fuel);

	
	return 0;
}