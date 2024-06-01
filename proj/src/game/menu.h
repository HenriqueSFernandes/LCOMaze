/**
 * @file menu.h
 * @brief This file contains the declarations of functions and structures related to the menu module.
 * The menu module provides functionality for creating buttons, handling keyboard and mouse events,
 * and drawing buttons on the screen.
 */

/** @defgroup menu Menu
 *  @brief Menu module for creating and handling menu buttons.
 *  @{
 */

#ifndef MENU_H
#    define MENU_H

#    include "../controllers/graphics.h"
#    include <lcom/lcf.h>
#    include <math.h>
#    include <stdint.h>
#    include <stdio.h>

extern vbe_mode_info_t mode_info;  ///< Information about the graphics mode.
xpm_image_t img;                   ///< Pixmap and metadata for the game image.
extern struct packet mouse_packet; ///< Mouse packet containing mouse movement information.
double x_mouse;                    ///< X-coordinate of the mouse position.
double y_mouse;                    ///< Y-coordinate of the mouse position.

/**
 * @struct Button
 * @brief Structure representing a button.
 *
 * The Button structure contains information about the position, size, label, and onClick function of a button.
 */
typedef struct {
    int x, y;          ///< The x and y coordinates of the button.
    int width, height; ///< The width and height of the button.
    char *label;       ///< The label text of the button.
    void (*onClick)(); ///< The function to be called when the button is clicked.
} Button;

#    define MAX_BUTTONS 10   ///< Maximum number of buttons.
Button buttons[MAX_BUTTONS]; ///< The array of buttons.
int button_count;            ///< The number of buttons.

/**
 * @brief Mouse event handler for the menu module.
 */
void menu_mouse_handler();

/**
 * @brief Main loop of the menu module.
 */
void menu_main_loop();

/**
 * @brief Creates a button with the specified parameters and adds it to the button array.
 *
 * @param x The x coordinate of the button.
 * @param y The y coordinate of the button.
 * @param width The width of the button.
 * @param height The height of the button.
 * @param label The label text of the button.
 * @param onClick The function to be called when the button is clicked.
 */
void create_button(int x, int y, int width, int height, char *label, void (*onClick)());

/**
 * @brief Draws all the buttons on the screen.
 */
void draw_buttons();

/**
 * Draws a button on the screen.
 *
 * @param button The button to be drawn.
 */
void draw_button(Button button);

/**
 * @brief Checks if a button is pressed based on the mouse coordinates.
 *
 * @param button The button to check.
 * @param mouse_x The x coordinate of the mouse.
 * @param mouse_y The y coordinate of the mouse.
 * @return 1 if the button is pressed, 0 otherwise.
 */
int is_button_pressed(Button *button, int mouse_x, int mouse_y);

/**
 * @brief Draws the cursor on the menu screen.
 *
 * This function draws a cursor on the menu screen using the specified colors and position.
 * The cursor is represented by a small rectangle.
 */
void menu_draw_cursor();

/**
 * @brief Handles a mouse click event.
 *
 * @param mouse_x The x coordinate of the mouse.
 * @param mouse_y The y coordinate of the mouse.
 */
void handle_mouse_click(int mouse_x, int mouse_y);

/**
 * @brief Handles the click event of the example button.
 *
 * This function is called when the example button is clicked. It prints a message
 * indicating that the button has been clicked and updates the state variable to 1.
 */
void example_button_click();

#endif // MENU_H

/** @} */
