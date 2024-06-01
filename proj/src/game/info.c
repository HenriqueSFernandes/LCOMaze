#include "info.h"

void info_main_loop() {
    clear(back_buffer);
    draw_text("W - Move Forward", 0, 0);
    draw_text("A - Move Forward", 0, 100);
    draw_text("S - Move Forward", 0, 200);
    draw_text("D - Move Forward", 0, 300);
    draw_text("MOUSE LEFT - Select Direction", 0, 400);
    draw_text("MOUSE RIGHT - Exit Game", 0, 500);
    swap();
}
