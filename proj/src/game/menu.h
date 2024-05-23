#ifndef MENU_H
#define MENU_H

#include "../controllers/graphics.h"
#include <math.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

extern vbe_mode_info_t mode_info;

xpm_image_t img; // pixmap and metadata

extern struct packet mouse_packet;

typedef struct {
    int x, y;
    int width, height;
    char* label;
    void (*onClick)();
} Button;

// Maximum number of buttons
#define MAX_BUTTONS 10

// Button array and button count
Button buttons[MAX_BUTTONS];
int button_count;

void menu_keyboard_handler();
void menu_mouse_handler();
void menu_main_loop();

void create_button(int x, int y, int width, int height, char* label, void (*onClick)());
void draw_buttons();
int is_button_pressed(Button* button, int mouse_x, int mouse_y);
void handle_mouse_click(int mouse_x, int mouse_y);

#endif // MENU_H
