#include "game.h"

extern vbe_mode_info_t mode_info;
struct Maze maze;
struct LinkedList *maze_solution;

double FOV_V;
double FOV_H;

void init_game() {
    x = 50;
    y = 50;
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
    bool collision = check_collision(x + x_changer * 10, y - y_changer * 10, 25);
    printf("collision: %d\n", collision);
    if (!collision) {
        x += x_changer * 10;
        y -= y_changer * 10;
    }
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

bool check_collision(int x, int y, int size) {
    // check for collisiong agains walls.
    int left_x = x - size;
    int right_x = x + size;
    int top_y = y - size;
    int bottom_y = y + size;
    struct Cell *left_cell = get_cell(&maze, left_x, y);
    struct Cell *right_cell = get_cell(&maze, right_x, y);
    struct Cell *top_cell = get_cell(&maze, x, top_y);
    struct Cell *bottom_cell = get_cell(&maze, x, bottom_y);
    struct Cell *center_cell = get_cell(&maze, x, y);
    // printf("center: ");
    // print_cell(center_cell);
    // printf("left: ");
    // print_cell(left_cell);
    // printf("right: ");
    // print_cell(right_cell);
    // printf("top: ");
    // print_cell(top_cell);
    // printf("bottom: ");
    // print_cell(bottom_cell);

    if (left_cell != NULL && left_cell != center_cell && left_cell->right_wall && center_cell->left_wall) {
        printf("left wall\n");
        return true;
    }
    if (right_cell != NULL && right_cell != center_cell && right_cell->left_wall && center_cell->right_wall) {
        printf("right wall\n");
        return true;
    }
    if (top_cell != NULL && top_cell != center_cell && top_cell->bottom_wall && center_cell->top_wall) {
        printf("top wall\n");
        return true;
    }
    if (bottom_cell != NULL && bottom_cell != center_cell && bottom_cell->top_wall && center_cell->bottom_wall) {
        printf("bottom wall\n");
        return true;
    }
    return false;
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
