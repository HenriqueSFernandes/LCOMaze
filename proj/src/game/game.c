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
  if(rotation_state == STOPPED){
              printf("STOPPED\n");
             }
              else if(rotation_state == CLOCKWISE){
                printf("CLOCKWISE\n");
              }else{
                printf("COUNTER_CLOCKWISE\n");
              }
  if(rotation_state == CLOCKWISE) {
    delta -= 0.05;
  } else if(rotation_state == COUNTER_CLOCKWISE) {
    delta += 0.05;
  }
  if(delta >2*M_PI){
    delta = 0;
  }
  if(delta<0){
    delta=2*M_PI-0.01;
  } 

  delta=atan2(-(y+(img.height/2)-y_mouse),x+(img.width/2)-x_mouse)+M_PI;
  printf("Angle\n");
  print_double(delta);
  
 
  vg_draw_rectangle(0,0, mode_info.XResolution, mode_info.YResolution, 0x000000);
 

  draw_xpm_at_pos_at_delta((xpm_map_t)player, (int)x, (int)y, delta);
    vg_draw_rectangle((int)x_mouse, (int)y_mouse,3, 3 , sky_color);
  
  swap();
  // vg_draw_rectangle(0, mode_info.YResolution / 2, 900, 900, ground_color);
}
