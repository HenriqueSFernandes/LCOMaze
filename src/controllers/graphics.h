/**
 * @file graphics.h
 * @brief Functions to handle graphics.
 */

/**
 * \addtogroup devices
 * @{
 */

/** @defgroup graphics Video Card
 * @brief Module for handling graphics.
 * @{
 */
#ifndef GRAPHICS_H
#    define GRAPHICS_H

#    include "../images/font/cursor.xpm"
#    include "../images/font/letter_a.xpm"
#    include "../images/font/letter_b.xpm"
#    include "../images/font/letter_c.xpm"
#    include "../images/font/letter_d.xpm"
#    include "../images/font/letter_e.xpm"
#    include "../images/font/letter_f.xpm"
#    include "../images/font/letter_g.xpm"
#    include "../images/font/letter_h.xpm"
#    include "../images/font/letter_i.xpm"
#    include "../images/font/letter_j.xpm"
#    include "../images/font/letter_k.xpm"
#    include "../images/font/letter_l.xpm"
#    include "../images/font/letter_m.xpm"
#    include "../images/font/letter_n.xpm"
#    include "../images/font/letter_o.xpm"
#    include "../images/font/letter_p.xpm"
#    include "../images/font/letter_q.xpm"
#    include "../images/font/letter_r.xpm"
#    include "../images/font/letter_s.xpm"
#    include "../images/font/letter_t.xpm"
#    include "../images/font/letter_u.xpm"
#    include "../images/font/letter_v.xpm"
#    include "../images/font/letter_w.xpm"
#    include "../images/font/letter_x.xpm"
#    include "../images/font/letter_y.xpm"
#    include "../images/font/letter_z.xpm"
#    include "../images/font/number_eight.xpm"
#    include "../images/font/number_five.xpm"
#    include "../images/font/number_four.xpm"
#    include "../images/font/number_nine.xpm"
#    include "../images/font/number_one.xpm"
#    include "../images/font/number_seven.xpm"
#    include "../images/font/number_six.xpm"
#    include "../images/font/number_three.xpm"
#    include "../images/font/number_two.xpm"
#    include "../images/font/number_zero.xpm"
#    include "../images/font/right_arrow.xpm"
#    include <lcom/lcf.h>
#    include <lcom/vbe.h>
#    include <stdint.h>
#    include <stdio.h>

vbe_mode_info_t mode_info; ///< Variable to store the current graphics mode information
xpm_image_t img;           ///< Variable to store the current image information
uint8_t *frame_buffer;     ///< Variable to store the current frame buffer
uint8_t *back_buffer;      ///< Variable to store the back buffer

/**
 * @brief Sets the frame buffer for the specified video mode.
 *
 * @param mode The video mode to set the frame buffer for.
 * @return 0 if successful, 1 otherwise.
 */
int setFrameBuffer(uint16_t mode);

/**
 * @brief Sets the graphics mode for the video controller.
 *
 * @param mode The graphics mode to set.
 * @return 0 on success, 1 on failure.
 */
int setGraphicsMode(uint16_t mode);

/**
 * @brief Sets the text mode for the video controller.
 *
 * @return 0 on success, 1 on failure.
 */
int vg_exit_to_text();

/**
 * @brief Normalize the color based on the number of bits per pixel.
 *
 * This function takes a color value and normalizes it based on the number of bits per pixel.
 * If each pixel is 32 bits, the given color is used as is. Otherwise, the unnecessary bits are set to 0.
 *
 * @param color The original color value.
 * @param newColor Pointer to store the normalized color value.
 * @return 0 on success, non-zero otherwise.
 */
int(normalizeColor(uint32_t color, uint32_t *newColor));

/**
 * @brief Draws a pixel of a specified color at the given coordinates on the screen.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @param buffer The buffer representing the screen.
 * @return 1 if the pixel is outside the screen boundaries, 0 otherwise.
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *buffer);

/**
 * @brief Draws a rectangle on a buffer with the specified position, size, and color.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @param buffer The buffer to draw the rectangle on.
 * @return 1 if an error occurred, 0 otherwise.
 */
int(vg_draw_rectangle_to_buffer)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *buffer);

/**
 * @brief Fills the given buffer with the specified color.
 *
 * @param color The color to fill the buffer with.
 * @param buffer The buffer to be filled.
 * @return 0 on success, non-zero otherwise.
 */
int(fill_color)(uint32_t color, uint8_t *buffer);

/**
 * @brief Swaps the contents of the frame buffer and the back buffer.
 *
 * @return 0 if the swap was successful, non-zero otherwise.
 */
int(swap)();

/**
 * @brief Clears the given buffer by setting all its elements to zero.
 *
 * @param buffer The buffer to be cleared.
 * @return 1 if the buffer was successfully cleared, 0 otherwise.
 */
int(clear)(uint8_t *buffer);

/**
 * @brief Draws an XPM image at a specified position on the screen.
 *
 * This function loads the XPM image specified by the xpm parameter and draws it on the screen
 * at the position (x, y). The image is loaded as a 32-bit color map and each pixel of the image
 * is drawn using the vg_draw_pixel function.
 *
 * @param xpm The XPM image to be drawn.
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @return 0 on success, non-zero otherwise.
 */
int(draw_xpm_at_pos)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Calculates the bounds of a rotated rectangle.
 *
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param theta The rotation angle in radians.
 * @param out_width Pointer to store the calculated width of the rotated rectangle.
 * @param out_height Pointer to store the calculated height of the rotated rectangle.
 */
void calculate_rotated_bounds(double width, double height, double theta, double *out_width, double *out_height);

/**
 * @brief Draws an XPM image at a specified position with a given rotation angle.
 *
 * @param xpm The XPM image to be drawn.
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @param theta The rotation angle in radians.
 * @return 0 on success, -1 if the XPM image could not be loaded.
 */
int draw_xpm_at_pos_at_delta(xpm_map_t xpm, uint16_t x, uint16_t y, double theta);

/**
 * @brief Retrieves the XPM map corresponding to a given letter.
 *
 * @param letter The letter for which to retrieve the XPM map.
 * @return The XPM map corresponding to the given letter, or NULL if the letter is not supported.
 */
xpm_map_t get_xpm(char letter);

/**
 * @brief Draws an XPM image on the screen, scaling it by a given factor.
 *
 * @param xpm The XPM image to be drawn.
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @param times The scaling factor for the image.
 * @param color The color of the image, 0xFFFFFFFF means the default color.
 * @return 0 on success, non-zero otherwise.
 */
int draw_xpm_x_times_bigger(xpm_map_t xpm, uint16_t x, uint16_t y, uint16_t times, uint32_t color);

/**
 * Draws a title on the screen.
 *
 * @param text The text to be displayed as the title.
 * @param x The x-coordinate of the starting position of the title.
 * @param y The y-coordinate of the starting position of the title.
 * @return 0 if successful.
 */
int draw_title(char *text, uint16_t x, uint16_t y);

/**
 * @brief Draws text on the screen at the specified coordinates.
 *
 * @param text The text to be drawn.
 * @param x The x-coordinate of the starting position.
 * @param y The y-coordinate of the starting position.
 * @param color The color of the text, 0xFFFFFFFF for default.
 * @return 0 if successful, -1 otherwise.
 */
int draw_text(char *text, uint16_t x, uint16_t y, uint32_t color);
#endif

/**@}*/

/**@}*/
