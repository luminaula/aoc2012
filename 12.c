#include "utils.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int accum = 0;

typedef struct cave_edge_t {
    char *first, *second;
} cave_edge_t;

typedef struct cave_graph_t {
    list_t *edges;
    list_t *caves;
} cave_graph_t;

int cave_is_big(char *cave) { return isupper(cave[0]); }

list_t *cave_connections_get(cave_graph_t graph, char *cave) {

    if (list_find_str(graph.caves, cave, 0)) {
        list_t *connections = list_create();
        for (int i = 0; i < graph.edges->size; i++) {
            cave_edge_t *edge = list_at(graph.edges, i);

            if (!edge) {
                continue;
            }
            if (strcmp(cave, edge->first) == 0) {
                char *con = calloc(strlen(edge->second) + 1, sizeof(char));
                strcpy(con, edge->second);
                list_push_back(connections, con);
            } else if (strcmp(cave, edge->second) == 0) {
                char *con = calloc(strlen(edge->first) + 1, sizeof(char));
                list_push_back(connections, con);
                strcpy(con, edge->first);
            }
        }
        return connections;
    }
    return NULL;
}

cave_graph_t init_caves(const char *input_file_name) {
    size_t file_size, line_count;
    char *input = read_input(input_file_name, &file_size);
    char **lines = split_input(input, &line_count, '\n');

    cave_graph_t graph = {};

    graph.caves = list_create();
    graph.edges = list_create();

    for (int i = 0; i < line_count; i++) {
        size_t edges_count;
        char **edges = split_input(lines[i], &edges_count, '-');
        for (int j = 0; j < edges_count; j++) {
            if (!list_find_str(graph.caves, edges[j], 0)) {
                char *cave = calloc(strlen(edges[j]) + 1, sizeof(char));
                strcpy(cave, edges[j]);
                list_push_back(graph.caves, cave);
            }
        }
        cave_edge_t *edge = malloc(sizeof(cave_edge_t));
        edge->first = edges[0];
        edge->second = edges[1];
        list_push_back(graph.edges, edge);
    }

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
    free(input);
    return graph;
}

void deinit_caves(cave_graph_t graph) {
    while (graph.edges->size) {
        cave_edge_t *edge = list_pop_back(graph.edges);
        if (!edge) {
            break;
        }
        free(edge->first);
        free(edge->second);
        free(edge);
    }

    list_free(graph.caves);
    list_free(graph.edges);
    printf_set_color(WHITE);
}

int graph_traverse(cave_graph_t graph, list_t *visited, char *current, char *prev, int twice) {
    if (list_find_str(visited, current, 0) && !cave_is_big(current)) {
        if(strcmp("start",current) == 0){
            return 0;
        }
        if(!twice){
            twice++;
        }
        else{
            return 0;
        }
    }
    if (strcmp(current, "end") == 0) {
        return 1;
    }
    list_push_back(visited, current);
    list_t *cons = cave_connections_get(graph, current);
    if (cons) {
        for (int i = 0; i < cons->size; i++) {
			char *next = list_at(cons,i);

            list_t *copy = list_copy(visited);
            if (graph_traverse(graph, copy, next,current,twice)) {
                list_free(copy);
                accum++;
            }
        }
        list_free(cons);
    }
	return 0;
}

//
int main(int argc, char **argv) {
    cave_graph_t graph = init_caves("input12");
    printf_set_color(BLUE);

    list_t *visited = list_create();

    graph_traverse(graph, visited, "start", NULL, 0);

    printf("%d\n", accum);
    deinit_caves(graph);
    printf_set_color(WHITE);
    return 0;
}