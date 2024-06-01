/**
 * @file game.h
 *
 * @brief This file contains the declarations of the functions and variables used in the game module.
 * The game module is responsible for managing the game logic and handling user input.
 */

#ifndef GAME_H
#define GAME_H

#define FOV_RADIUS 200.0 ///< The distance the player can see

#include "../controllers/graphics.h"
#include "../images/player.xpm"
#include "../images/player3.xpm"
#include "../images/player4.xpm"
#include "../images/player5.xpm"
#include "maze.h"
#include <lcom/lcf.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

xpm_image_t img;                   ///< 
double delta;                      ///< Angle for the player's movement direction
uint8_t kbd_value;                 ///< Value of the keyboard input
bool update_delta;                 ///< Flag indicating if the delta value needs to be updated
double x;                          ///< X-coordinate of the player's position
double y;                          ///< Y-coordinate of the player's position
double x_mouse;                    ///< X-coordinate of the mouse position
double y_mouse;                    ///< Y-coordinate of the mouse position
extern struct packet mouse_packet; ///< Mouse packet containing mouse movement information
double direction;                  ///< Direction of the player's movement
bool is_moving;                    ///< Flag indicating if the player is currently moving
double frame_counter;              ///< Counter for tracking the frames
int current_frame;                 ///< Current frame of the game animation

/**
 * @brief Initializes the game module.
 */
void init_game();

/**
 * @brief Handles keyboard input for the game.
 */
void game_keyboard_handler();

/**
 * @brief Handles mouse input for the game.
 */
void game_mouse_handler();

/**
 * @brief Updates the delta value for the game.
 */
void game_update_delta();

/**
 * @brief Checks if the mouse cursor is within the screen boundaries and adjusts its position if necessary.
 */
void game_check_bound();

/**
 * @brief Main game loop.
 */
void game_main_loop();

/**
 * @brief Draws the cursor on the screen.
 *
 * The cursor is represented by a small rectangle.
 * The color of the rectangle depends on whether the cursor has moved or not.
 * If the cursor has not moved, the rectangle is drawn with the sky color.
 * If the cursor has moved, the rectangle is drawn with the color red.
 */
void game_draw_cursor();

/**
 * @brief Checks for collision at the given position and size.
 *
 * @param x The x-coordinate of the position to check.
 * @param y The y-coordinate of the position to check.
 * @param size The size of the object to check.
 * @return True if there is a collision, false otherwise.
 */
bool check_collision(int x, int y, int size);

/**
 * @brief Draws the hero character on the screen.
 *
 * This function is responsible for drawing the hero character on the screen.
 * It uses different frames of the character sprite to create an animation effect
 * when the character is moving.
 */
void game_draw_hero();

/**
 * @brief Checks if the game has ended.
 *
 * @return True if the game has ended, false otherwise.
 */
bool check_game_end();

/**
 * @brief Draws the field of view (FOV) circle on the screen.
 *
 * The FOV circle is centered at the coordinates (x, y) and has a radius of FOV_RADIUS.
 * Only pixels within the FOV circle are copied from the maze buffer to the back buffer.
 *
 */
void game_draw_fov_circle();

#endif
