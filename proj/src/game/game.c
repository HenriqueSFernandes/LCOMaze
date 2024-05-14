#include "game.h"

extern vbe_mode_info_t mode_info;

double FOV_V;
double FOV_H;

void init_game() {

}

void main_loop() {
  uint32_t sky_color;
  uint32_t ground_color;

  normalizeColor(0x87CEEB, &sky_color);
  normalizeColor(0x4A4A4F, &ground_color);
  draw_xpm_at_pos_at_delta((xpm_map_t)player, 0, 0, 20);
  swap();
  // vg_draw_rectangle(0, mode_info.YResolution / 2, 900, 900, ground_color);
}
