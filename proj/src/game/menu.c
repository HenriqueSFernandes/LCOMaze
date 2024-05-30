#include "menu.h"

extern int state;

Button buttons[MAX_BUTTONS];
int button_count = 0;

void menu_keyboard_handler() {
  // Handle keyboard input for the menu
}
void menu_mouse_handler() {
  // Handle mouse input for the menu
  x_mouse += mouse_packet.delta_x * 0.5;
  y_mouse -= mouse_packet.delta_y * 0.5;
  if (x_mouse > mode_info.XResolution)
    x_mouse = mode_info.XResolution;
  if (y_mouse > mode_info.YResolution)
    y_mouse = mode_info.YResolution;
  if (x_mouse < 0)
    x_mouse = 0;
  if (y_mouse < 0)
    y_mouse = 0;
}
void create_button(int x, int y, int width, int height, char *label, void (*onClick)()) {
  if (button_count < MAX_BUTTONS) {
    buttons[button_count].x = x;
    buttons[button_count].y = y;
    buttons[button_count].width = width;
    buttons[button_count].height = height;
    buttons[button_count].label = label;
    buttons[button_count].onClick = onClick;
    button_count++;
  }
  else {
    printf("Maximum number of buttons reached.\n");
  }
}
void draw_button(Button button) {
  // Draw the button background
  uint32_t button_color = 0x000000; // Default button color
  if (is_button_pressed(&button, x_mouse, y_mouse)) {
    button_color = 0xFF0000; // Change color when button is pressed
  }
  vg_draw_rectangle(button.x, button.y, button.width, button.height, button_color);

  // Add more padding around the text
  int padding = 20;                           // Increase padding as needed
  int text_width = strlen(button.label) * 30; // Approximate text width
  int text_height = 20;                       // Approximate text height

  // Ensure the text is centered with the added padding
  int text_x = button.x + padding + (button.width - 2 * padding) / 2 - text_width / 2;
  int text_y = button.y + padding + (button.height - 2 * padding) / 2 - text_height / 2;

  // Draw the text
  draw_text(button.label, text_x, text_y);
}

void draw_buttons() {
  for (int i = 0; i < button_count; i++) {
    draw_button(buttons[i]);
  }
}
void menu_draw_cursor() {
  uint32_t sky_color;
  uint32_t ground_color;
  normalizeColor(0x87CEEB, &sky_color);
  normalizeColor(0x4A4A4F, &ground_color);

  vg_draw_rectangle((int) x_mouse, (int) y_mouse, 3, 3, 0xff0000);
}

int is_button_pressed(Button *button, int mouse_x, int mouse_y) {
  return mouse_x >= button->x && mouse_x <= button->x + button->width &&
         mouse_y >= button->y && mouse_y <= button->y + button->height;
}

void handle_mouse_click(int mouse_x, int mouse_y) {
  for (int i = 0; i < button_count; i++) {
    if (is_button_pressed(&buttons[i], mouse_x, mouse_y)) {
      if (mouse_packet.lb) {
        buttons[i].onClick();
      }
    }
  }
}

// Example of a button click handler
void example_button_click() {
  printf("Button clicked!\n");
  state = 1;
}

void menu_main_loop() {
  if (button_count < 1) {
    create_button(mode_info.XResolution / 2 - 200, mode_info.YResolution * 1 / 5, 400, 50, "Single", example_button_click);
    create_button(mode_info.XResolution / 2 - 200, mode_info.YResolution * 2 / 5, 400, 50, "Multiplayer", example_button_click);
    create_button(mode_info.XResolution / 2 - 200, mode_info.YResolution * 3 / 5, 400, 50, "Settings", example_button_click);
  }
  clear();
  handle_mouse_click(x_mouse, y_mouse);
  draw_buttons();
  menu_draw_cursor();
  swap();
}
