#include "info.h"

void info_main_loop() {
    clear(back_buffer);
    draw_title("How to Play", mode_info.XResolution / 2 - 330, mode_info.YResolution / 10);
    draw_text("W - Move Forward", mode_info.XResolution / 2 - 240, 250, 0xFFFFFFFF);
    draw_text("A - Move Left", mode_info.XResolution / 2 - 195, 315, 0xFFFFFFFF);
    draw_text("S - Move Back", mode_info.XResolution / 2 - 195, 380, 0xFFFFFFFF);
    draw_text("D - Move Right", mode_info.XResolution / 2 - 210, 445, 0xFFFFFFFF);
    draw_text("Mouse Left - Aim Player",  mode_info.XResolution / 2 - 345, 510, 0xFFFFFFFF);
    draw_text("ESC - Go back to the menu", mode_info.XResolution / 2 - 375, 575, 0xFFFFFFFF);
    swap();
}
