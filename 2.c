/**
 * Copyright (c) 2021 "Lumi"
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

char *get_line(char **stream){
	char *begin = *stream;
	size_t len = 0;
	char *line = NULL;
	while(**stream){
		if(**stream == '\n'){
			**stream = '\0';
			break;
		}
		*(*stream)++;
	}

	len = *stream - begin;
	*(*stream)++;
	if(!len){
		return NULL;
	}
	line = calloc(1,len + 1);
	memcpy(line,begin,len);
	return line;
}



int main(int argc, char **argv) {
	char *file_data = read_input("input");
	char *line;
	char direction[32];
	int value = 0;
	int posx = 0,posy = 0;
	int aim = 0;

	while(line = get_line(&file_data)){
		sscanf(line,"%s%d",direction,&value);
		if(strcmp(direction,"forward") == 0){
			posx += value;
			posy += aim * value;
		}
		else if(strcmp(direction,"up") == 0){
			aim -= value;
		}
		else if(strcmp(direction,"down") == 0){
			aim += value;
		}
		free(line);
	}
	printf("%d\n",posx *posy);
	return 0;
}