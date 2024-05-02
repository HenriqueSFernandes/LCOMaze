#ifndef GAME_H
#define GAME_H

#include "../controllers/graphics.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

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
