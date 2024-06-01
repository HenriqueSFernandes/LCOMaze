/**
 * @file maze.h
 * @brief This file contains the declarations of the `Maze` and `Cell` structs,
 * as well as the function prototypes for maze generation, solution calculation,
 * and drawing functions.
 */

/** @defgroup maze Maze
 *  @brief Maze module creating the maze.
 *  @{
 */

#ifndef MAZE_H
#    define MAZE_H

#    include "../controllers/graphics.h"
#    include "../data_structures/linked_list.h"
#    include "../data_structures/stack.h"
#    include <lcom/vbe.h>
#    include <limits.h>
#    include <stdbool.h>
#    include <stdint.h>
#    include <stdio.h>
#    include <stdlib.h>

extern vbe_mode_info_t mode_info; ///< Information about the graphics mode.
uint8_t *maze_buffer;             ///< Buffer for the maze image.

/**
 * @struct Cell
 * @brief Represents a cell in a maze.
 *
 * The Cell struct stores information about a cell in a maze, including its coordinates,
 * whether it has been visited, and the presence of walls on its sides.
 * It also includes additional fields for distance and previous cell information used for solving the maze.
 */
struct Cell {
    int x;             ///< The x-coordinate of the cell.
    int y;             ///< The y-coordinate of the cell.
    bool visited;      ///< Flag indicating whether the cell has been visited.
    bool top_wall;     ///< Flag indicating the presence of a wall on the top side of the cell.
    bool bottom_wall;  ///< Flag indicating the presence of a wall on the bottom side of the cell.
    bool left_wall;    ///< Flag indicating the presence of a wall on the left side of the cell.
    bool right_wall;   ///< Flag indicating the presence of a wall on the right side of the cell.
    int dist;          ///< The distance of the cell from a starting point.
    struct Cell *prev; ///< A pointer to the previous cell in the path.
};

/**
 * @struct Maze
 * @brief Represents a maze with a specified width and height.
 *
 * The `Maze` struct contains information about the dimensions of the maze
 * and a 2D array of `Cell` pointers representing the maze cells.
 */
struct Maze {
    int width;            ///< The width of the maze.
    int height;           ///< The height of the maze.
    int cell_size;        ///< The size of each cell in pixels.
    struct Cell ***cells; ///< A 2D array of `Cell` pointers representing the maze cells.
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
 * Calculates the solution path for a given maze.
 *
 * @param maze The maze structure.
 * @return A linked list representing the solution path, or NULL if no solution is found.
 */
struct LinkedList *get_solution(struct Maze *maze);

/**
 * Draws the solution path on the maze.
 *
 * @param maze The maze structure.
 * @param solution The linked list containing the solution path.
 */
void draw_solution(struct Maze *maze, struct LinkedList *solution);

/**
 * Draws the maze on the screen.
 *
 * @param maze The maze structure containing the maze cells and walls.
 */
void draw_maze(struct Maze *maze);

/**
 * Retrieves the cell at the specified coordinates in the maze.
 *
 * @param maze The maze structure.
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
 * @return A pointer to the cell at the specified coordinates, or NULL if the coordinates are out of bounds.
 */
struct Cell *get_cell(struct Maze *maze, int x, int y);

/**
 * Prints the details of a cell.
 *
 * @param cell The cell to be printed.
 */
void print_cell(struct Cell *cell);

#endif

/** @} */
