#ifndef MAZE_H
#define MAZE_H

#include "../controllers/graphics.h"
#include "stack.h"
#include <lcom/vbe.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern vbe_mode_info_t mode_info;
uint8_t *maze_buffer;

struct Cell {
    // total of 16 bytes (3 ints + 2 booleans = 16 bytes)
    int x;
    int y;
    int visited;
    bool top_wall;
    bool bottom_wall;
    bool left_wall;
    bool right_wall;
};

struct Maze {
    int width;
    int height;
    struct Cell ***cells;
};

struct Maze generate_maze();

void generate_maze_buffer(struct Maze *maze);

void draw_maze(struct Maze *maze);

#endif
