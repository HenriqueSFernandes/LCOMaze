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

/**
 * @struct Cell
 * @brief Represents a cell in a maze.
 *
 * This struct contains information about a cell in a maze, including its coordinates,
 * whether it has been visited, and the presence of walls on its top, bottom, left, and right sides.
 */
struct Cell {
    int x;            /**< The x-coordinate of the cell. */
    int y;            /**< The y-coordinate of the cell. */
    bool visited;     /**< Flag indicating whether the cell has been visited. */
    bool top_wall;    /**< Flag indicating the presence of a wall on the top side of the cell. */
    bool bottom_wall; /**< Flag indicating the presence of a wall on the bottom side of the cell. */
    bool left_wall;   /**< Flag indicating the presence of a wall on the left side of the cell. */
    bool right_wall;  /**< Flag indicating the presence of a wall on the right side of the cell. */
};

/**
 * @struct Maze
 * @brief Represents a maze with a specified width and height.
 *
 * The `Maze` struct contains information about the dimensions of the maze
 * and a 2D array of `Cell` pointers representing the maze cells.
 */
struct Maze {
    int width;            /**< The width of the maze. */
    int height;           /**< The height of the maze. */
    struct Cell ***cells; /**< A 2D array of `Cell` pointers representing the maze cells. */
};

/**
 * Generates a maze using a randomized iterative depth-first search algorithm.
 * The algorithm is based on the one described in https://en.wikipedia.org/wiki/Maze_generation_algorithm
 *
 * @return The generated maze as a struct Maze.
 */
struct Maze generate_maze();

/**
 * Generates a buffer for the maze.
 *
 * This function checks if the maze buffer is NULL and allocates memory for it if necessary.
 * The buffer is then used to optimize the drawing of the maze by drawing the entire maze to 
 * the buffer first and then copying only the visible portion to the screen.
 *
 * @param maze A pointer to the Maze structure.
 */
void generate_maze_buffer(struct Maze *maze);

/**
 * Draws the maze on the screen.
 * 
 * @param maze The maze structure containing the maze cells and walls.
 */
void draw_maze(struct Maze *maze);

#endif
