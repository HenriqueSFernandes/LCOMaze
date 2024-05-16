#ifndef GAME_H
#define GAME_H

#include "../controllers/graphics.h"
#include <math.h>
#include "../images/player.xpm"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
extern xpm_image_t img; // pixmap and metadata
extern double delta;
typedef enum {
    CLOCKWISE,
    COUNTER_CLOCKWISE,
    STOPPED
} RotationState;
RotationState rotation_state;
uint8_t cur;

double x ;
double y ;
double x_mouse;
double y_mouse;
struct point {
  int x;
  int y;
  int z;
  int projected_x;
  int projected_y;
};

void main_loop();

void init_game();

#endif
