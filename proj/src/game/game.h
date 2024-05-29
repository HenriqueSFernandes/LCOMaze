#ifndef GAME_H
#define GAME_H

#include "../controllers/graphics.h"
#include "../images/player.xpm"
#include <lcom/lcf.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
xpm_image_t img; // pixmap and metadata
double delta;
uint8_t kbd_value;
bool update_delta;
double x;
double y;
double x_mouse;
double y_mouse;
extern struct packet mouse_packet;
struct point {
  int x;
  int y;
  int z;
  int projected_x;
  int projected_y;
};
void game_keyboard_handler();
void game_mouse_handler();
void game_update_delta();
void game_main_loop();
#endif
