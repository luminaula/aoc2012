/**
 * Copyright (c) 2021 "Lumi"
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <string.h>
#include "utils.h"
#include <stdio.h>

struct Node;

typedef struct Node{
	char value;
	struct Node *next;
	struct Node *prev;
} Node;

struct List{
	Node *head;
	size_t size;
};



int main(int argc, char **argv) {
	
    size_t file_size, line_count;
    char *input = read_input("input8", &file_size);
    char **lines = split_input(input, &line_count, '\n');
	size_t count = 0;

	for(int i = 0; i<line_count; i++){
		char segments;
		size_t token_count = 0;
    	char **tokens = split_input(lines[i],&token_count, ' ');
		int done = 0;
		for(int j = 10; j< token_count;j++){
			size_t len = strlen(tokens[j]);
			if(len == 2 || len == 4 || len == 3 || len == 7){

				count++;
			}
		}
		for(int j=0;j<token_count;j++){
			char *token = tokens[j];
			while(*token++){

			}
		}
		printf("asd\n");
		int k = 0;
		while(!done){
			char *token = tokens[k++ % token_count];
			size_t len = strlen(token);
			switch(len){
				case 2:
				
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					break;
				case 7:
					break;
				default:
					break;
			}
		}
	}

	printf("%u\n",count);


    return 0;
}