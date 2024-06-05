/**
 * @file proj.h
 * @brief This file contains the declarations of the main functions of the project.
 */

/** @defgroup proj Proj
 *  @brief Proj module for managing devices.
 *  @{
 */

#ifndef PROJ_H
#    define PROJ_H

#    include "controllers/graphics.h"
#    include "controllers/keyboard.h"
#    include "controllers/mouse.h"
#    include "controllers/rtc.h"
#    include "controllers/serial.h"
#    include "controllers/timer.h"
#    include "game/game.h"
#    include "game/info.h"
#    include "game/maze.h"
#    include "game/menu.h"
#    include <lcom/lcf.h>
#    include <lcom/vbe.h>
#    include <math.h>
#    include <stdint.h>
#    include <stdio.h>

/**
 * @brief Enumeration representing the different states of the program.
 *
 * The `State` enum is used to define the possible states of the program.
 * It includes the following states:
 * - `Menu`: Represents the menu state.
 * - `Game`: Represents the game state.
 * - `HowTo`: Represents the how-to state.
 */
typedef enum {
    Menu,
    Game,
    HowTo
} State;

uint8_t kbd_value;                 ///< @brief Value of the keyboard
extern bool update_delta;          ///< @brief Flag to update the delta
extern uint8_t kbd_value;          ///< @brief Value of the keyboard
extern uint8_t byte_index;         ///< @brief Index of the byte
extern struct packet mouse_packet; ///< @brief Mouse packet
extern int timerCounter;           ///< @brief Timer counter
extern vbe_mode_info_t mode_info;  ///< @brief Mode info
extern bool isMultiplayer;         ///< @brief Flag to check if the game is multiplayer
double delta = 0;                  ///< @brief Delta angle
bool host = 0;                     ///< @brief Flag to check if the player is the host
State state = Menu;                ///< @brief State of the game
bool running = true;               ///< @brief Flag to check if the game is running

/**
 * @brief The main function of the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return 0 if the program executed successfully, 1 otherwise.
 */
int main(int argc, char *argv[]);

/**
 * @brief Converts a binary number to its decimal equivalent.
 *
 * @param bin_num The binary number to be converted.
 * @return The decimal equivalent of the binary number.
 */
int binary_to_decimal(int bin_num);

/**
 * @brief Initialization of the project.
 *
 * This function is responsible for the main execution of the project.
 * It initializes various components, subscribes to interrupts, and handles input events.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns 0 upon success, non-zero otherwise.
 */
int(proj_main_loop)(int argc, char *argv[]);

#endif

/**@}*/
