#define FOV_ANGLE 200.0
#include "game.h"

extern vbe_mode_info_t mode_info;
struct Maze maze;
struct LinkedList *maze_solution;
bool won=0;
GameState gameState=Waiting;
 time_el time_initial;
 time_el time_final;
double FOV_V;
double FOV_H;
bool initialTimeSet = 0;
bool finalTimeSet = 0;

void init_game() {
    x = 50;
    y = 50;
    maze = generate_maze();
    generate_maze_buffer(&maze);
    maze_solution = get_solution(&maze);
    draw_solution(&maze, maze_solution);
    draw_maze(&maze);
}

void game_keyboard_handler() {
    double x_changer = 0;
    double y_changer = 0;
    is_moving = (kbd_value == 0x11 || kbd_value == 0x1f || kbd_value == 0x1e || kbd_value == 0x20);
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

void game_draw_cursor() {
    draw_xpm_at_pos((xpm_map_t) cursor_xpm, (int) x_mouse, (int) y_mouse);
}

void game_draw_hero() {
    if (is_moving) {
        frame_counter++;
        if (frame_counter > 16) {
            frame_counter = 0;
        }
    }
    else {
        current_frame = 0;
    }
    if (frame_counter <= 4) {
        draw_xpm_at_pos_at_delta((xpm_map_t) player, (int) x, (int) y, delta);
    }
    else if (frame_counter <= 8) {
        draw_xpm_at_pos_at_delta((xpm_map_t) player3, (int) x, (int) y, delta);
    }
    else if (frame_counter <= 12) {
        draw_xpm_at_pos_at_delta((xpm_map_t) player4, (int) x, (int) y, delta);
    }
    else if (frame_counter <= 16) {
        draw_xpm_at_pos_at_delta((xpm_map_t) player5, (int) x, (int) y, delta);
    }
    else if (frame_counter == 0) {
        draw_xpm_at_pos_at_delta((xpm_map_t) player, (int) x, (int) y, delta);
    }
}

bool check_collision(int x, int y, int size) {
    // check for collisions against walls.
    int left_x = x - size;
    int right_x = x + size;
    int top_y = y - size;
    int bottom_y = y + size;
    struct Cell *left_cell = get_cell(&maze, left_x, y);
    struct Cell *right_cell = get_cell(&maze, right_x, y);
    struct Cell *top_cell = get_cell(&maze, x, top_y);
    struct Cell *bottom_cell = get_cell(&maze, x, bottom_y);
    struct Cell *center_cell = get_cell(&maze, x, y);

    if (left_cell == NULL || right_cell == NULL || top_cell == NULL || bottom_cell == NULL) {
        return true;
    }

    if (left_cell != center_cell && left_cell->right_wall && center_cell->left_wall) {
        return true;
    }
    if (right_cell != center_cell && right_cell->left_wall && center_cell->right_wall) {
        return true;
    }
    if (top_cell != center_cell && top_cell->bottom_wall && center_cell->top_wall) {
        return true;
    }
    if (bottom_cell != center_cell && bottom_cell->top_wall && center_cell->bottom_wall) {
        return true;
    }
    return false;
}

bool check_game_end() {
    return x >= (maze.width - 1) * maze.cell_size - 25 && y >= (maze.height - 1) * maze.cell_size - 25;
}

void game_draw_fov_circle() {

    double fov_radius = FOV_RADIUS;

    uint32_t bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    uint32_t frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

    for (int y_pixel = 0; y_pixel < mode_info.YResolution; y_pixel++) {
        for (int x_pixel = 0; x_pixel < mode_info.XResolution; x_pixel++) {
            if (x_pixel == x && y_pixel == y) {
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

void game_activate_multiplayer() {
    gameState = Running;
}
void game_reset() {
   isMultiplayer=0;
   won=0;
    gameState=Waiting;
    initialTimeSet=0;
    finalTimeSet=0;
     x=50;
     y=50;
}
void check_time() {
    if (!initialTimeSet) {
        initialTimeSet = 1;
        memcpy(&time_initial, &time_stamp, sizeof(time_el));
        printf("Time: %x:%x:%x\n", time_initial.hours, time_initial.minutes, time_initial.seconds);
    }
    if (!finalTimeSet && gameState == Finish) {
        finalTimeSet = 1;
        memcpy(&time_final, &time_stamp, sizeof(time_el));
        printf("Time: %x:%x:%x\n", time_final.hours, time_final.minutes, time_final.seconds);
    }
}
int calculate_time() {
    int time = 0;
    printf("Time: %x:%x:%x\n", time_final.hours, time_final.minutes, time_final.seconds);
    printf("Time: %x:%x:%x\n", time_initial.hours, time_initial.minutes, time_initial.seconds);
    
     int length = snprintf( NULL, 0, "%x",  time_final.hours);
     char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%x",  time_final.hours );
    printf("str: %s\n", str);
    int hours_final = atoi(str);
    free(str);
    length = snprintf( NULL, 0, "%x",  time_final.minutes);
    str = malloc( length + 1 );
    snprintf( str, length + 1, "%x",  time_final.minutes );
    int minutes_final = atoi(str);
    free(str);
    length = snprintf( NULL, 0, "%x",  time_final.seconds);
    str = malloc( length + 1 );
    snprintf( str, length + 1, "%x",  time_final.seconds );
    int seconds_final = atoi(str);
     length = snprintf( NULL, 0, "%x",  time_initial.hours);
     str = malloc( length + 1 );
    snprintf( str, length + 1, "%x", time_initial.hours );
    int hours_initial = atoi(str);
    free(str);
    length = snprintf( NULL, 0, "%x", time_initial.minutes);
    str = malloc( length + 1 );
    snprintf( str, length + 1, "%x", time_initial.minutes );
    int minutes_initial = atoi(str);
    free(str);
    length = snprintf( NULL, 0, "%x",  time_initial.seconds);
    str = malloc( length + 1 );
    snprintf( str, length + 1, "%x",  time_initial.seconds );
    int seconds_initial = atoi(str);
    printf("final: %d:%d:%d\n", hours_final, minutes_final, seconds_final);
    printf("begin: %d:%d:%d\n", hours_initial, minutes_initial, seconds_initial);

    time+= (hours_final - hours_initial) * 3600;
    time+= (minutes_final - minutes_initial) * 60;
    time+= seconds_final - seconds_initial;
    return time;
}
void game_lose(){
    gameState=Finish;
    won=0;
    printf("You lost\n");
}
void game_main_loop() {

    check_time();
    if (gameState == Waiting) {
        clear(back_buffer);
        draw_text("WAITING FOR SOUTO", mode_info.XResolution / 2 - 200, 500);
          sp_send_int(0x3f8, 6, 2, 0x3, 115200, "M", 1);
        swap();
    }
    else if (gameState == Running) {
        game_check_bound();
        clear(back_buffer);
        game_draw_fov_circle();
        game_draw_hero();
        game_draw_cursor();
        if (check_game_end()) {
            gameState = Finish;
            won=1;
            if(isMultiplayer)
             sp_send_int(0x3f8, 6, 2, 0x3, 115200, "L", 1);
        }
        swap();
    }
    else {
        clear(back_buffer);
        if(won){
              draw_text("YOU WON", mode_info.XResolution / 2 - 200, 200);
              draw_text("Time", mode_info.XResolution / 2 - 200, 400);
              int sec=calculate_time();
              printf("Time: %x\n", sec);    
              int length = snprintf( NULL, 0, "%d", sec );
                char* str = malloc( length + 1 );
                snprintf( str, length + 1, "%d", sec );
              draw_text(str, mode_info.XResolution / 2 - 200, 500);
              draw_text("seconds", mode_info.XResolution / 2 - 200, 600);
              draw_text("Press ESC to exit", mode_info.XResolution / 2 - 200, 700);
              free(str );
        }
          
        else{draw_text("YOU LOST", 500, 500);}
            
       
        swap();
    }
}
