#include "graphics.h"
#include "keyboard.h"
#include <ctype.h>
#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

extern int kbd_hook_id;
extern uint8_t kbd_value;
uint32_t bytesPerPixel;

int setFrameBuffer(uint16_t mode) {
    // Set mode_info to 0
    memset(&mode_info, 0, sizeof(mode_info));
    // Get the info
    if (vbe_get_mode_info(mode, &mode_info))
        return 1;

    // Calculating the bytes per pixel, by rounding up the bits per pixel and dividing it by 8.
    bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    // Calculate the frame size (width * height * bytes per pixel).
    uint32_t frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

    // Allocate physical addresses
    struct minix_mem_range physicAddresses;
    // The start of the physical address.
    physicAddresses.mr_base = mode_info.PhysBasePtr;
    // The end of the physical address, which is the start + size.
    physicAddresses.mr_limit = physicAddresses.mr_base + frameSize;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physicAddresses))
        return 1;

    // Allocate virtual addresses.
    frame_buffer = vm_map_phys(SELF, (void *) physicAddresses.mr_base, frameSize);
    back_buffer = (uint8_t *) malloc(frameSize);

    return frame_buffer == NULL;
}

int vg_exit_to_text() {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;        
    reg86.ah = 0x00;           
    reg86.al = 0x03;           
    reg86.bx = 0x0000;
    if (sys_int86(&reg86) != 0)
        return 1;
    return 0;
}

int setGraphicsMode(uint16_t mode) {
    // Initialize the struct.
    reg86_t reg86;
    // Set the struct to 0.
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;        // BIOS video service.
    reg86.ah = 0x4F;           // ax higher bits.
    reg86.al = 0x02;           // ax lower bits.
    reg86.bx = mode | BIT(14); // Set the mode with bit 14 (linear memory mapping).
    // Send the command.
    if (sys_int86(&reg86) != 0)
        return 1;
    return 0;
}

int(normalizeColor(uint32_t color, uint32_t *newColor)) {
    // If each pixel is 32 bits, then just use the given color.
    if (mode_info.BitsPerPixel == 32) {
        *newColor = color;
    }
    // Otherwise, set the unnecessary bits to 0.
    else {
        *newColor = color & (BIT(mode_info.BitsPerPixel) - 1); // Imagine we want 5 bits, the mask should be 0...11111. BIT(5) = 0...100000, BIT(5) - 1 = 0...11111.
    }
    return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *buffer) {
    if (x >= mode_info.XResolution || y >= mode_info.YResolution)
        return 1;

    uint32_t bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    uint32_t index = (mode_info.XResolution * y + x) * bytesPerPixel;
    return memcpy(&buffer[index], &color, bytesPerPixel) == NULL;
}

int vg_draw_rectangle_to_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *buffer) {
    for (uint16_t i = x; i < width + x; i++) {
        for (uint16_t j = y; j < height + y; j++) {
            if (vg_draw_pixel(i, j, color, buffer)) {
                return 1;
            }
        }
    }
    return 0;
}

int draw_xpm_at_pos(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t img;
    uint32_t *map;
    enum xpm_image_type image_type = XPM_8_8_8_8;
    map = (uint32_t *) xpm_load(xpm, image_type, &img);
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            if (map[i * img.width + j] != 0x00ff00) {
                vg_draw_pixel(x + j, y + i, map[i * img.width + j], back_buffer);
            }
        }
    }
    return 0;
}

void calculate_rotated_bounds(double width, double height, double theta, double *out_width, double *out_height) {
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    double corners_x[] = {-width / 2, width / 2, width / 2, -width / 2};
    double corners_y[] = {-height / 2, -height / 2, height / 2, height / 2};

    double min_x = 1e9, max_x = -1e9, min_y = 1e9, max_y = -1e9;

    for (int i = 0; i < 4; i++) {
        double x = corners_x[i] * cos_theta - corners_y[i] * sin_theta;
        double y = corners_x[i] * sin_theta + corners_y[i] * cos_theta;

        if (x < min_x)
            min_x = x;
        if (x > max_x)
            max_x = x;
        if (y < min_y)
            min_y = y;
        if (y > max_y)
            max_y = y;
    }

    *out_width = max_x - min_x;
    *out_height = max_y - min_y;
}

int draw_xpm_at_pos_at_delta(xpm_map_t xpm, uint16_t x, uint16_t y, double theta) {
    xpm_image_t img;
    uint32_t *map;
    enum xpm_image_type image_type = XPM_8_8_8_8;
    map = (uint32_t *) xpm_load(xpm, image_type, &img);

    if (!map) {
        return -1;
    }

    double rotated_width, rotated_height;
    calculate_rotated_bounds(img.width, img.height, theta, &rotated_width, &rotated_height);

    double center_x = img.width / 2.0;
    double center_y = img.height / 2.0;

    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    for (int i = 0; i < (int) rotated_height; i++) {
        for (int j = 0; j < (int) rotated_width; j++) {
            double translated_x = j - rotated_width / 2.0;
            double translated_y = i - rotated_height / 2.0;

            double source_x = cos_theta * translated_x - sin_theta * translated_y + center_x;
            double source_y = sin_theta * translated_x + cos_theta * translated_y + center_y;

            if (source_x >= 0 && source_x < img.width && source_y >= 0 && source_y < img.height) {
                int src_x = (int) source_x;
                int src_y = (int) source_y;

                uint32_t color = map[src_y * img.width + src_x];

                if (color != 0x00ff00) {
                    vg_draw_pixel(x + j - rotated_width / 2.0, y + i - rotated_height / 2.0, color, back_buffer);
                }
            }
        }
    }
    return 0;
}

int(fill_color)(uint32_t color, uint8_t *buffer) {
    return vg_draw_rectangle_to_buffer(0, 0, mode_info.XResolution, mode_info.YResolution, color, buffer);
}

int swap() {
    return memcpy(frame_buffer, back_buffer, mode_info.XResolution * mode_info.YResolution * bytesPerPixel) == NULL;
}

int(clear)(uint8_t *buffer) {
    return memset(buffer, 0, mode_info.XResolution * mode_info.YResolution * bytesPerPixel) == NULL;
}

xpm_map_t get_xpm(char letter) {
    switch (letter) {
        case 'a':
            return (xpm_map_t) a_xpm;
        case 'b':
            return (xpm_map_t) b_xpm;
        case 'c':
            return (xpm_map_t) c_xpm;
        case 'd':
            return (xpm_map_t) d_xpm;
        case 'e':
            return (xpm_map_t) e_xpm;
        case 'f':
            return (xpm_map_t) f_xpm;
        case 'g':
            return (xpm_map_t) g_xpm;
        case 'h':
            return (xpm_map_t) h_xpm;
        case 'i':
            return (xpm_map_t) i_xpm;
        case 'j':
            return (xpm_map_t) j_xpm;
        case 'k':
            return (xpm_map_t) k_xpm;
        case 'l':
            return (xpm_map_t) l_xpm;
        case 'm':
            return (xpm_map_t) m_xpm;
        case 'n':
            return (xpm_map_t) n_xpm;
        case 'o':
            return (xpm_map_t) o_xpm;
        case 'p':
            return (xpm_map_t) p_xpm;
        case 'q':
            return (xpm_map_t) q_xpm;
        case 'r':
            return (xpm_map_t) r_xpm;
        case 's':
            return (xpm_map_t) s_xpm;
        case 't':
            return (xpm_map_t) t_xpm;
        case 'u':
            return (xpm_map_t) u_xpm;
        case 'v':
            return (xpm_map_t) v_xpm;
        case 'w':
            return (xpm_map_t) w_xpm;
        case 'x':
            return (xpm_map_t) x_xpm;
        case 'y':
            return (xpm_map_t) y_xpm;
        case 'z':
            return (xpm_map_t) z_xpm;
        case '0':
            return (xpm_map_t) number_zero_xpm;
        case '1':
            return (xpm_map_t) number_one_xpm;
        case '2':
            return (xpm_map_t) number_two_xpm;
        case '3':
            return (xpm_map_t) number_three_xpm;
        case '4':
            return (xpm_map_t) number_four_xpm;
        case '5':
            return (xpm_map_t) number_five_xpm;
        case '6':
            return (xpm_map_t) number_six_xpm;
        case '7':
            return (xpm_map_t) number_seven_xpm;
        case '8':
            return (xpm_map_t) number_eight_xpm;
        case '9':
            return (xpm_map_t) number_nine_xpm;
        case '-':
            return (xpm_map_t) right_arrow_xpm;
        default:
            return NULL;
    }
}

int draw_xpm_x_times_bigger(xpm_map_t xpm, uint16_t x, uint16_t y, uint16_t times, uint32_t color) {
    xpm_image_t img; // pixmap and metadata
    uint32_t *map;   // pixmap itself
    enum xpm_image_type image_type = XPM_8_8_8_8;
    map = (uint32_t *) xpm_load(xpm, image_type, &img);

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            for (int k = 0; k < times; k++) {
                for (int l = 0; l < times; l++) {
                    if (color == 0xFFFFFFFF) {
                        vg_draw_pixel(x + j * times + k, y + i * times + l, map[i * img.width + j], back_buffer);
                    }
                    else {
                        uint32_t new_color;
                        if (map[i * img.width + j] == 0xFF0000) {
                            new_color = color;
                        }
                        else {
                            new_color = 0xFF0000;
                        }
                        vg_draw_pixel(x + j * times + k, y + i * times + l, new_color, back_buffer);
                    }
                }
            }
        }
    }
    return 0;
}

int draw_title(char *text, uint16_t x, uint16_t y) {
    int i = 0;
    while (text[i] != '\0') {
        char lowercase_letter = tolower((unsigned char) text[i]); // Convert to lowercase
        if (lowercase_letter == ' ') {
            x += 20; // Increase x by 10 for space
        }
        else {
            draw_xpm_x_times_bigger(get_xpm(lowercase_letter), x, y, 4, 0xFFFFFFFF);
            x += 20; // Increase x by 10 for the letter width
        }
        x += 40; // Add space between letters
        i++;
    }
    return 0;
}

int draw_text(char *text, uint16_t x, uint16_t y, uint32_t color) {
    int i = 0;
    while (text[i] != '\0') {
        char lowercase_letter = tolower((unsigned char) text[i]); // Convert to lowercase
        if (lowercase_letter == ' ') {
            x += 10; // Increase x by 10 for space
        }
        else {
            draw_xpm_x_times_bigger(get_xpm(lowercase_letter), x, y, 2, color);
            x += 10; // Increase x by 10 for the letter width
        }
        x += 20; // Add space between letters
        i++;
    }
    return 0;
}
