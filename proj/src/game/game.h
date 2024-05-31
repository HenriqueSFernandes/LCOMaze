#ifndef GAME_H
#define GAME_H
#define FOV_ANGLE 200.0

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

xpm_image_t img; // pixmap and metadata
double delta;
uint8_t kbd_value;
bool update_delta;
double x;
double y;
double x_mouse;
double y_mouse;
extern struct packet mouse_packet;
double lanterna_x;
double lanterna_y;
double direction;
bool is_moving;
double frame_counter;
double frame_delay = 4;
int current_frame;

void init_game();
void game_keyboard_handler();
void game_mouse_handler();
void game_update_delta();
void game_main_loop();
bool check_collision(int x, int y, int size);
#endif
