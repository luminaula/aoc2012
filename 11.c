#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// today a bit nicer looking code

typedef struct octopus_t {
    int value;
    int flashed;
} octopus_t;

typedef struct octopus_array_t {
    octopus_t **data;
    size_t width, height;
} octopus_array_t;

void end_phase(octopus_array_t *oct) {
    for (int i = 0; i < oct->height; i++) {
        for (int j = 0; j < oct->width; j++) {
            oct->data[i][j].flashed = 0;
            if (oct->data[i][j].value > 9) {
                oct->data[i][j].value = 0;
            }
        }
    }
}

void flash(octopus_array_t *oct, int x, int y) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int idx = x + i;
            int idy = y + j;
            if (idx < 0 || idx + 1 > oct->width || idy < 0 || idy + 1 > oct->height) {
                continue;
            }
            oct->data[idy][idx].value++;
        }
    }
}

size_t flash_phase(octopus_array_t *oct) {
    int flashing = 0;
    size_t flashes = 0;
    do {
        flashing = 0;
        for (int i = 0; i < oct->height; i++) {
            for (int j = 0; j < oct->width; j++) {
                if (oct->data[i][j].value > 9 && !oct->data[i][j].flashed) {
                    flash(oct, j, i);
                    flashes++;
                    oct->data[i][j].flashed = 1;
                    flashing = 1;
                }
            }
        }
    } while (flashing);
    return flashes;
}

void energy_increase_phase(octopus_array_t *oct) {
    for (int i = 0; i < oct->height; i++) {
        for (int j = 0; j < oct->width; j++) {
            oct->data[i][j].value++;
        }
    }
}

size_t step(octopus_array_t *oct) {
    size_t flashes = 0;
    energy_increase_phase(oct);
    flashes = flash_phase(oct);
    end_phase(oct);
    return flashes;
}

void print_octopi(octopus_array_t *oct){
	for (int i = 0; i < oct->height; i++) {
        for (int j = 0; j < oct->width; j++) {
			if(oct->data[i][j].value == 0){
				printf_set_color(RED);
			}
			else{
				printf_set_color(WHITE);
			}
            printf("%d",oct->data[i][j].value);
        }
		printf("\n");
    }
	printf("\n");
	printf_set_color(WHITE);
}

octopus_array_t *init_octopuses(const char *input_file_name) {
    size_t file_size;
    octopus_array_t *octopi = malloc(sizeof(octopus_array_t));
    char *input_data = read_input(input_file_name, &file_size);
    char **input_lines = split_input(input_data, &octopi->height, '\n');
    octopi->width = strlen(*input_lines);
    octopi->data = calloc(octopi->height, sizeof(octopus_t *));

    for (int i = 0; i < octopi->height; i++) {
        octopi->data[i] = calloc(octopi->width, sizeof(octopus_t));
        for (int j = 0; j < octopi->width; j++) {
            octopi->data[i][j].value = input_lines[i][j] - 0x30;
            // printf("%d", octopi->data[i][j].value);
        }
        // printf("\n");
        free(input_lines[i]);
    }
    free(input_lines);
    free(input_data);
    return octopi;
}

void deinit_octopuses(octopus_array_t *octopi) {
    for (int i = 0; i < octopi->height; i++) {
        free(octopi->data[i]);
    }
    free(octopi->data);
    free(octopi);
}

int main(int argc, char **argv) {
    size_t flashes = 0;
    octopus_array_t *oct = init_octopuses("input11");
	// print_octopi(oct);
	int i=0;

    while(1){
		i++;
		size_t flashed = step(oct);
        flashes += flashed;
		// print_octopi(oct);
		if(i == 100){
    		printf("%u\n", flashes);
		}
		if(flashed == oct->height * oct->width){
			printf("%d\n",i);
			break;
		}
    }
    deinit_octopuses(oct);

    return 0;
}