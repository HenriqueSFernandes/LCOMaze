#define FOV_ANGLE 200.0
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
        direction = delta;
    }
    else if (kbd_value == 0x1f) {
        x_changer = -cos(delta);
        y_changer = -sin(delta);
        direction = delta + M_PI;
         
    }
    else if (kbd_value == 0x1e) {
        x_changer = -sin(delta);
        y_changer = cos(delta);
        direction = delta - M_PI / 2;
    }
    else if (kbd_value == 0x20) {
        x_changer = sin(delta);
        y_changer = -cos(delta);
        direction = delta + M_PI / 2;
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

void game_draw_fov_cone() {
    
    double fov_radius = FOV_ANGLE;

    uint32_t bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    uint32_t frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

    
    for (double angle = 0; angle < 2 * M_PI; angle += 0.01) {
        
        int pixel_x = x + fov_radius * cos(angle);
        int pixel_y = y + fov_radius * sin(angle);

        //Desenha a circunferencia mas n é preciso que o faça
        if (pixel_x >= 0 && pixel_x < mode_info.XResolution && pixel_y >= 0 && pixel_y < mode_info.YResolution) {
            vg_draw_pixel(pixel_x, pixel_y, 0xFFFFFF);
        }
    }

    
    for (int y_pixel = 0; y_pixel < mode_info.YResolution; y_pixel++) {
        for (int x_pixel = 0; x_pixel < mode_info.XResolution; x_pixel++) {
            if(x_pixel == x && y_pixel == y){
              continue;
            }
            double dx = x_pixel - x;
            double dy = y_pixel - y;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance <= fov_radius) {
                uint32_t index = (mode_info.XResolution * y_pixel + x_pixel) * bytesPerPixel;
                if (index < frameSize) {
                    memcpy(&back_buffer[index], &maze_buffer[index], bytesPerPixel);
                  
                    
                }
            }
        }
    }
}








void game_main_loop() {
    game_check_bound();
    clear();
    draw_maze(&maze);
    game_draw_fov_cone();
    game_draw_hero();
    game_draw_cursor();
    swap();
}
