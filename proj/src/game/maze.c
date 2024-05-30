#include "maze.h"

void dump_memory(void *p, size_t n) {
    unsigned char *ptr = (unsigned char *) p;
    for (size_t i = 0; i < n; i++) {
        printf("%02x ", ptr[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

struct Maze generate_maze() {
    const int maze_width = 46;
    const int maze_height = 34;
    // const int maze_width = 5;
    // const int maze_height = 5;

    struct Cell ***cells = malloc(maze_height * sizeof(struct Cell *));
    printf("first cell: %x\n", &cells[0]);
    printf("last cell: %x\n\n", &cells[maze_height - 1]);
    printf("size of cell: %d\n", sizeof(struct Cell));

    // Initialize the maze to have 4 walls on every cell
    for (int i = 0; i < maze_height; i++) {
        cells[i] = malloc(maze_width * sizeof(struct Cell *));
        for (int j = 0; j < maze_width; j++) {
            cells[i][j] = malloc(sizeof(struct Cell));
            cells[i][j]->x = j;
            cells[i][j]->y = i;
            cells[i][j]->visited = 0;
            cells[i][j]->top_wall = 1;
            cells[i][j]->bottom_wall = 1;
            cells[i][j]->left_wall = 1;
            cells[i][j]->right_wall = 1;
        }
        for (int j = 0; j < maze_width; j++) {
            printf("cell[%d][%d] stored at %x and with value %x\n", i, j, &cells[i][j], cells[i][j]);
            // dump_memory(cells[i][j], sizeof(struct Cell));
        }
    }

    // Generate the maze using the a randomized iterative depth-first search algorithm.
    // The algorithm is based on the one described in https://en.wikipedia.org/wiki/Maze_generation_algorithm
    // printf("cells pointers:\n");
    // for (int i = 0; i < maze_height; i++) {
    //     for (int j = 0; j < maze_width; j++) {
    //         printf("cell: %x\n", &cells[i][j]);
    //     }
    // }
    struct Cell *current_cell = cells[0][0];
    printf("current cell: %x\n", current_cell);
    // printf("initial cell: %x\n", current_cell);
    struct Stack stack = {NULL, 0};
    current_cell->visited = 1;
    push(&stack, current_cell);
    struct Cell *top_cell = top(&stack);
    printf("top cell: %x\n", top_cell);
    while (stack.size > 0) {
        current_cell = pop(&stack);
        int x = current_cell->x;
        int y = current_cell->y;
        // Get all the neighbours that haven't been visited
        struct Cell *neighbours[4] = {NULL, NULL, NULL, NULL};
        int neighbours_count = 0;

        if (x > 0 && !cells[y][x - 1]->visited) {
            // Left neighbour
            neighbours[neighbours_count++] = cells[y][x - 1];
        }
        if (x < maze_width - 1 && !cells[y][x + 1]->visited) {
            // Right neighbour
            neighbours[neighbours_count++] = cells[y][x + 1];
        }
        if (y > 0 && !cells[y - 1][x]->visited) {
            // Top neighbour.
            neighbours[neighbours_count++] = cells[y - 1][x];
        }
        if (y < maze_height - 1 && !cells[y + 1][x]->visited) {
            // Bottom neighbour.
            neighbours[neighbours_count++] = cells[y + 1][x];
        }
        // if there are unvisited neighbours, push the current cell to the stack and choose a random neighbour.
        if (neighbours_count > 0) {
            push(&stack, current_cell);
            struct Cell *neighbour = neighbours[rand() % neighbours_count];
            // remove the wall between the current cell and the neighbour.
            if (neighbour->x == x - 1) {
                current_cell->left_wall = 0;
                neighbour->right_wall = 0;
                // printf("disabling left wall of cell %d %d\n", x, y);
                // printf("disabling right wall of cell %d %d\n\n", neighbour->x, neighbour->y);
            }
            else if (neighbour->x == x + 1) {
                current_cell->right_wall = 0;
                neighbour->left_wall = 0;
                // printf("current right: %d\n", current_cell->right_wall);
                // printf("neighbour left: %d\n", neighbour->left_wall);
                // printf("disabling right wall of cell %d %d\n", x, y);
                // printf("disabling left wall of cell %d %d\n\n", neighbour->x, neighbour->y);
            }
            else if (neighbour->y == y - 1) {
                current_cell->top_wall = 0;
                neighbour->bottom_wall = 0;
                // printf("disabling top wall of cell %d %d\n", x, y);
                // printf("disabling bottom wall of cell %d %d\n\n", neighbour->x, neighbour->y);
            }
            else if (neighbour->y == y + 1) {
                current_cell->bottom_wall = 0;
                neighbour->top_wall = 0;
                // printf("disabling bottom wall of cell %d %d\n", x, y);
                // printf("disabling top wall of cell %d %d\n\n", neighbour->x, neighbour->y);
            }
            // mark the neighbour as visited and push it to the stack.
            neighbour->visited = 1;
            // printf("current pointer: %x, with x: %d and y: %d\n", current_cell, current_cell->x, current_cell->y);
            // printf("neighbour pointer: %x, with x: %d and y: %d\n", neighbour, neighbour->x, neighbour->y);
            push(&stack, neighbour);
        }
        clear();
        struct Maze maze;
        maze.width = maze_width;
        maze.height = maze_height;
        maze.cells = cells;
        draw_maze(&maze);
        swap();
    }
    struct Maze maze;
    maze.width = maze_width;
    maze.height = maze_height;
    maze.cells = cells;
    // for (int i = 0; i < maze.height; i++) {
    //     for (int j = 0; j < maze.width; j++) {
    //         printf("top: %d bottom: %d left: %d right: %d x: %d y: %d\n", maze.cells[i][j]->top_wall, maze.cells[i][j]->bottom_wall, maze.cells[i][j]->left_wall, maze.cells[i][j]->right_wall, maze.cells[i][j]->x, maze.cells[i][j]->y);
    //     }
    // }
    return maze;
}

void generate_maze_buffer(struct Maze *maze) {
    if (maze_buffer == NULL) {
        uint32_t bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
        uint32_t frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
        maze_buffer = (uint8_t *) malloc(frameSize);
    }
}

void draw_maze(struct Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (maze->cells[i][j]->top_wall) {
                vg_draw_rectangle(j * 25, i * 25, 25, 1, 0xFFFFFF);
            }
            if (maze->cells[i][j]->bottom_wall) {
                vg_draw_rectangle(j * 25, i * 25 + 25, 25, 1, 0xFFFFFF);
            }
            if (maze->cells[i][j]->left_wall) {
                vg_draw_rectangle(j * 25, i * 25, 1, 25, 0xFFFFFF);
            }
            if (maze->cells[i][j]->right_wall) {
                vg_draw_rectangle(j * 25 + 25, i * 25, 1, 25, 0xFFFFFF);
            }
        }
    }
}
