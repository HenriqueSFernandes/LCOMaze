#include "info.h"

void info_main_loop() {
    clear(back_buffer);
    draw_text("W - Move Forward", 0, 0, 0xFFFFFFFF);
    draw_text("A - Move Left", 0, 100, 0xFFFFFFFF);
    draw_text("S - Move Back", 0, 200, 0xFFFFFFFF);
    draw_text("D - Move Right", 0, 300, 0xFFFFFFFF);
    draw_text("MOUSE LEFT - Select Direction", 0, 400, 0xFFFFFFFF);
    draw_text("MOUSE RIGHT - Exit Game", 0, 500, 0xFFFFFFFF);
    draw_text("ESC - Go back to the menu", 0, 600, 0xFFFFFFFF);
    swap();
}
