#include "menu.h"

Button buttons[MAX_BUTTONS];
int button_count = 0;

void create_button(int x, int y, int width, int height, char* label, void (*onClick)()) {
    if (button_count < MAX_BUTTONS) {
        buttons[button_count].x = x;
        buttons[button_count].y = y;
        buttons[button_count].width = width;
        buttons[button_count].height = height;
        buttons[button_count].label = label;
        buttons[button_count].onClick = onClick;
        button_count++;
    }
}

void draw_buttons() {
    for (int i = 0; i < button_count; i++) {
        Button* button = &buttons[i];
        draw_rectangle(button->x, button->y, button->width, button->height, 0xFFFFFF);
        draw_text(button->x + 10, button->y + 10, button->label, 0x000000);
    }
}

int is_button_pressed(Button* button, int mouse_x, int mouse_y) {
    return mouse_x >= button->x && mouse_x <= button->x + button->width &&
           mouse_y >= button->y && mouse_y <= button->y + button->height;
}

void handle_mouse_click(int mouse_x, int mouse_y) {
    for (int i = 0; i < button_count; i++) {
        if (is_button_pressed(&buttons[i], mouse_x, mouse_y)) {
            if (buttons[i].onClick) {
                buttons[i].onClick();
            }
        }
    }
}

// Example onClick handler function
void example_button_click() {
    printf("Button was clicked!\n");
}

void menu_keyboard_handler() {
    // Handle keyboard input for the menu
}

void menu_mouse_handler() {
    // Example: get mouse coordinates from the packet and handle the click
    int mouse_x = mouse_packet.bytes[1];
    int mouse_y = mouse_packet.bytes[2];
    handle_mouse_click(mouse_x, mouse_y);
}

void menu_main_loop() {
    // Example menu setup
    create_button(50, 50, 100, 40, "Start", example_button_click);
    create_button(50, 100, 100, 40, "Exit", example_button_click);

    while (1) { // Replace with actual condition for your menu loop
        fill_color(0xff0000); // Red background
        draw_buttons();
        swap();
    }
}
