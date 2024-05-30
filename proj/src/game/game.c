#include "game.h"

extern vbe_mode_info_t mode_info;
struct Maze maze;
struct LinkedList *maze_solution;

double FOV_V;
double FOV_H;

void init_game() {
    maze = generate_maze();
    generate_maze_buffer(&maze);
    maze_solution = get_solution(&maze);
}

void game_keyboard_handler() {
    double x_changer = 0;
    double y_changer = 0;
    if (kbd_value == 0x11) {
        x_changer = cos(delta);
        y_changer = sin(delta);
    }
    else if (kbd_value == 0x1f) {
        x_changer = -cos(delta);
        y_changer = -sin(delta);
    }
    else if (kbd_value == 0x1e) {
        x_changer = -sin(delta);
        y_changer = cos(delta);
    }
    else if (kbd_value == 0x20) {
        x_changer = sin(delta);
        y_changer = -cos(delta);
    }
    x += x_changer * 10;
    y -= y_changer * 10;
    if (update_delta) {
        game_update_delta();
    }
}

void game_update_delta() {
    delta = atan2(-(y + (img.height / 2) - y_mouse), x + (img.width / 2) - x_mouse) + M_PI;
}
void game_mouse_handler() {
    x_mouse += mouse_packet.delta_x * 0.5;
    y_mouse -= mouse_packet.delta_y * 0.5;
    update_delta = mouse_packet.lb;
    if (update_delta) {
        game_update_delta();
    }
}

void game_check_bound() {
    if (x_mouse + 4 > mode_info.XResolution)
        x_mouse = mode_info.XResolution - 3;
    if (y_mouse + 4 > mode_info.YResolution)
        y_mouse = mode_info.YResolution - 3;
    if (x_mouse < 0)
        x_mouse = 0;
    if (y_mouse < 0)
        y_mouse = 0;
}
void game_draw() {
}
void game_draw_cursor() {
    uint32_t sky_color;
    uint32_t ground_color;
    normalizeColor(0x87CEEB, &sky_color);
    normalizeColor(0x4A4A4F, &ground_color);
    if (!update_delta) {
        vg_draw_rectangle((int) x_mouse, (int) y_mouse, 3, 3, sky_color);
    }
    else {
        vg_draw_rectangle((int) x_mouse, (int) y_mouse, 3, 3, 0xff0000);
    }
}
void game_draw_hero() {
    draw_xpm_at_pos_at_delta((xpm_map_t) player, (int) x, (int) y, delta);
}

void game_main_loop() {
    game_check_bound();
    clear();
    draw_maze(&maze);
    draw_solution(&maze, maze_solution);
    game_draw_hero();
    game_draw_cursor();
    swap();
}
