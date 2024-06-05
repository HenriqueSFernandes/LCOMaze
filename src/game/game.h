/**
 * @file game.h
 *
 * @brief This file contains the declarations of the functions and variables used in the game module.
 * The game module is responsible for managing the game logic and handling user input.
 */

/** @defgroup game Game
 *  @brief Game module for managing game logic and user input.
 *  @{
 */

#ifndef GAME_H
#    define GAME_H

#    define FOV_RADIUS 200.0 ///< The distance the player can see
#    include "../controllers/graphics.h"
#    include "../controllers/rtc.h"
#    include "../images/player.xpm"
#    include "../images/player3.xpm"
#    include "../images/player4.xpm"
#    include "../images/player5.xpm"
#    include "maze.h"
#    include <lcom/lcf.h>
#    include <math.h>
#    include <stdint.h>
#    include <stdio.h>

#include "controllers/serial.h"
xpm_image_t img; // pixmap and metadata
double delta;
uint8_t kbd_value;
bool update_delta;
extern bool host;
bool can_lose();
bool can_enter_multiplayer();
double x;
double y;
double x_mouse;
double y_mouse;
bool initialTimeSet;
bool finalTimeSet;
extern struct time_el time_stamp;
extern struct packet mouse_packet;
double lanterna_x;
double lanterna_y;
double direction;
bool is_moving;
double frame_counter;
int current_frame;
extern bool isMultiplayer;
extern struct packet mouse_packet;
typedef enum {
    Waiting,
    Running,
    Finish,
} GameState;
void game_reset();
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

/**
 * @brief Activates the multiplayer mode of the game.
 * Sets the game state to "Running".
 */
void game_activate_multiplayer();
/**
 * @brief Update game state upon losing.
 *
 */
void game_lose();
#endif

/** @} */
