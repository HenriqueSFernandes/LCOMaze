#include "maze.h"

void dump_memory(void *p, size_t n) {
    unsigned char *ptr = (unsigned char *) p;
    for (size_t i = 0; i < n; i++) {
        printf("%02x ", ptr[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
}

struct Maze generate_maze() {
    const int maze_width = 46;
    const int maze_height = 34;

    struct Cell ***cells = malloc(maze_height * sizeof(struct Cell *));

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
    }

    struct Cell *current_cell = cells[0][0];
    struct Stack stack = {NULL, 0};
    current_cell->visited = 1;
    stack_push(&stack, current_cell);
    struct Cell *top_cell = stack_top(&stack);
    printf("top cell: %x\n", top_cell);
    while (stack.size > 0) {
        current_cell = stack_pop(&stack);
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
            stack_push(&stack, current_cell);
            struct Cell *neighbour = neighbours[rand() % neighbours_count];
            // remove the wall between the current cell and the neighbour.
            if (neighbour->x == x - 1) {
                current_cell->left_wall = 0;
                neighbour->right_wall = 0;
            }
            else if (neighbour->x == x + 1) {
                current_cell->right_wall = 0;
                neighbour->left_wall = 0;
            }
            else if (neighbour->y == y - 1) {
                current_cell->top_wall = 0;
                neighbour->bottom_wall = 0;
            }
            else if (neighbour->y == y + 1) {
                current_cell->bottom_wall = 0;
                neighbour->top_wall = 0;
            }
            // mark the neighbour as visited and push it to the stack.
            neighbour->visited = 1;
            stack_push(&stack, neighbour);
        }
        // ***** ENABLE THIS TO SEE THE MAZE BEING GENERATED *****
        // clear();
        // struct Maze maze;
        // maze.width = maze_width;
        // maze.height = maze_height;
        // maze.cells = cells;
        // draw_maze(&maze);
        // swap();
        // *******************************************************
    }
    struct Maze maze;
    maze.width = maze_width;
    maze.height = maze_height;
    maze.cells = cells;
    return maze;
}

void generate_maze_buffer(struct Maze *maze) {
    if (maze_buffer == NULL) {
        uint32_t bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
        uint32_t frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
        maze_buffer = (uint8_t *) malloc(frameSize);
    }
}

void draw_solution(struct Maze *maze) {
    int dist[maze->height * maze->width];
    int prev[maze->height * maze->width];
    int nodes[maze->height * maze->width];
    int nodes_size = 0;
    for (int i = 0; i < maze->height * maze->width; i++) {
        dist[i] = -1;
        prev[i] = -1;
        nodes[i] = i;
        nodes_size++;
    }
    dist[0] = 0;
    while (nodes_size > 0) {
    }
}

void draw_maze(struct Maze *maze) {
    // TODO change this to draw to the maze buffer
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
