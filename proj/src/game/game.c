#include "game.h"

extern vbe_mode_info_t mode_info;
void print_double(double n) {

  char str[100];
  sprintf(str, "%f", n);
  printf("%s", str);
}

double FOV_V;
double FOV_H;

void init_game() {

}

void main_loop() {
  uint32_t sky_color;
  uint32_t ground_color;
  normalizeColor(0x87CEEB, &sky_color);
  normalizeColor(0x4A4A4F, &ground_color);
  if(x_mouse > mode_info.XResolution) x_mouse = mode_info.XResolution;
  if(y_mouse > mode_info.YResolution) y_mouse = mode_info.YResolution;
  if(x_mouse < 0) x_mouse = 0;
  if(y_mouse < 0) y_mouse = 0;
  delta=atan2(-(y+(img.height/2)-y_mouse),x+(img.width/2)-x_mouse)+M_PI; 
  vg_draw_rectangle(0,0, mode_info.XResolution, mode_info.YResolution, 0x000000);
  draw_xpm_at_pos_at_delta((xpm_map_t)player, (int)x, (int)y, delta);
  vg_draw_rectangle((int)x_mouse, (int)y_mouse,3, 3 , sky_color);
  swap();
  // vg_draw_rectangle(0, mode_info.YResolution / 2, 900, 900, ground_color);
}
