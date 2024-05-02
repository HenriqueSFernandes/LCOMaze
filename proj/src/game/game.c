#include "game.h"
#include <math.h>

extern vbe_mode_info_t mode_info;

double FOV_V;
double FOV_H;

void init_game() {
  FOV_V = M_PI_4;
  FOV_H = FOV_V * mode_info.XResolution / mode_info.YResolution;
  // struct point points[3] = {{1,1,1}, {4,2,0}, {1,0,3}};
}

void main_loop() {
  uint32_t sky_color;
  uint32_t ground_color;

  normalizeColor(0x87CEEB, &sky_color);
  normalizeColor(0x4A4A4F, &ground_color);
  vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution / 2, sky_color);
  // vg_draw_rectangle(0, mode_info.YResolution / 2, 900, 900, ground_color);
}
