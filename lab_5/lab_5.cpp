#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#define MAX_SIZE 100

unsigned int read() {
    unsigned int value;
    char input[20];

    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%u", &value) == 1) {
            return value;
        }
    }
    printf("\x1b[установлено значение по умолчанию!!!\x1b\n");
    return 10; // значение по умолчанию
}

void create_graph(int size, int graph[MAX_SIZE][MAX_SIZE], int* graph_size) {
    srand(time(NULL));
    *graph_size = 0;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            graph[i][j] = rand() % 2;
            graph[j][i] = graph[i][j];
            if (graph[i][j] == 1) {
                (*graph_size)++;
            }
        }
    }
}

void create_incidence_matrix(int size, int graph[MAX_SIZE][MAX_SIZE], int incidence_matrix[MAX_SIZE][MAX_SIZE], int* edges) {
    int edge_index = 0;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (graph[i][j] == 1) {
                incidence_matrix[i][edge_index] = 1;
                incidence_matrix[j][edge_index] = 1;
                edge_index++;
            }
        }
    }
    *edges = edge_index;
    printf("Количество рёбер (по матрице инцидентности) == %d\n", *edges);
}

int determine_graph_size(int size, int incidence_matrix[MAX_SIZE][MAX_SIZE], int edges) {
    int graph_size = 0;
    for (int i = 0; i < edges; i++) {
        int vertex_count = 0;
        for (int j = 0; j < size; j++) {
            if (incidence_matrix[j][i] == 1) {
                vertex_count++;
            }
        }
        if (vertex_count == 2) {
            graph_size++;
        }
    }
    return graph_size;
}

void find_isolated_terminal_dominant_vertices_from_adjacency(int size, int graph[MAX_SIZE][MAX_SIZE], int* isolated, int* terminal, int* dominant) {
    *isolated = *terminal = *dominant = 0;
    int isolated_vertices[MAX_SIZE], terminal_vertices[MAX_SIZE], dominant_vertices[MAX_SIZE];

    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = 0; j < size; j++) {
            if (graph[i][j] == 1) {
                count++;
            }
        }
        if (count == 0) {
            isolated_vertices[(*isolated)++] = i;
        }
        else if (count == 1) {
            terminal_vertices[(*terminal)++] = i;
        }
        else if (count == size - 1) {
            dominant_vertices[(*dominant)++] = i;
        }
    }

    printf("Изолированные вершины (по матрице смежности): ");
    for (int i = 0; i < *isolated; i++) {
        printf("%d ", isolated_vertices[i] + 1);
    }
    printf("\nКонцевые вершины (по матрице смежности): ");
    for (int i = 0; i < *terminal; i++) {
        printf("%d ", terminal_vertices[i] + 1);
    }
    printf("\nДоминирующие вершины (по матрице смежности): ");
    for (int i = 0; i < *dominant; i++) {
        printf("%d ", dominant_vertices[i] + 1);
    }
    printf("\n");
}

void find_from_incidence(int size, int incidence_matrix[MAX_SIZE][MAX_SIZE], int edges, int* isolated, int* terminal, int* dominant) {
    *isolated = *terminal = *dominant = 0;
    int isolated_vertices[MAX_SIZE], terminal_vertices[MAX_SIZE], dominant_vertices[MAX_SIZE];
    
    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = 0; j < edges; j++) {
            if (incidence_matrix[i][j] == 1) {
                count++;
            }
        }
        if (count == 0) {
            isolated_vertices[(*isolated)++] = i;
        }
        else if (count == 1) {
            terminal_vertices[(*terminal)++] = i;
        }
        else if (count == size - 1) {
            dominant_vertices[(*dominant)++] = i;
        }
    }

    printf("Изолированные вершины (по матрице инцидентности): ");
    for (int i = 0; i < *isolated; i++) {
        printf("%d ", isolated_vertices[i] + 1);
    }
    printf("\nКонцевые вершины (по матрице инцидентности): ");
    for (int i = 0; i < *terminal; i++) {
        printf("%d ", terminal_vertices[i] + 1);
    }
    printf("\nДоминирующие вершины (по матрице инцидентности): ");
    for (int i = 0; i < *dominant; i++) {
        printf("%d ", dominant_vertices[i] + 1);
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "RUS");
    int size, graph[MAX_SIZE][MAX_SIZE] = { 0 }, incidence_matrix[MAX_SIZE][MAX_SIZE] = { 0 };
    int graph_size, edges;

    printf("Введите кол-во вершин графа > ");
    size = read();

    create_graph(size, graph, &graph_size);

    printf("Граф (матрица смежности):\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }

    create_incidence_matrix(size, graph, incidence_matrix, &edges);

    printf("Матрица инцидентности:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < edges; j++) {
            printf("%d ", incidence_matrix[i][j]);
        }
        printf("\n");
    }

    int determined_size = determine_graph_size(size, incidence_matrix, edges);
    printf("Размер графа (по матрице инцидентности) == %d\n", determined_size);

    int isolated, terminal, dominant;
    find_isolated_terminal_dominant_vertices_from_adjacency(size, graph, &isolated, &terminal, &dominant);
    find_from_incidence(size, incidence_matrix, edges, &isolated, &terminal, &dominant);

    return 0;
}
