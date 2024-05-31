#include "controllers/graphics.h"
#include "controllers/keyboard.h"
#include "controllers/mouse.h"
#include "controllers/timer.h"
#include "controllers/rtc.h"
#include "game/game.h"
#include "game/maze.h"
#include "game/menu.h"
#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <stdint.h>
#include <stdio.h>

uint8_t kbd_value;
extern bool update_delta;
// Any header files included below this line should have been created by you
extern uint8_t kbd_value;
extern uint8_t byte_index;
extern struct packet mouse_packet;
extern int timerCounter;
extern vbe_mode_info_t mode_info;
double delta = 0;
typedef enum {
  Menu,
  Game,
  Pause
} State;
State state = Menu;

void print_double(double n) {

  char str[100];
  sprintf(str, "%f", n);
  printf("%s", str);
}

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  // lcf_trace_calls("/home/lcom/labs/g1/proj/src/log/trace.txt");
  // lcf_log_output("/home/lcom/labs/g1/proj/src/log/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  int ipc_status;
  int receiver;
  uint16_t irq_set_mouse;
  uint8_t irq_set_timer;
  uint16_t irq_set_kbd;
  uint16_t irq_set_rtc;
  message msg;

  if (mouse_subscribe_int(&irq_set_mouse)) {
    printf("Error subscribing to mouse!\n");
    return 1;
  }
  if (kbd_subscribe_int(&irq_set_kbd))
    return 1;

  if (mouse_send_command(0xF4)) {
    printf("Error enabling data reporting!\n");
    return 1;
  }

  if (setFrameBuffer(0x14C)) {
    printf("Error setting frame buffer!\n");
    return 1;
  }

  if (setGraphicsMode(0x14C)) {
    printf("Error setting graphics mode!\n");
    return 1;
  }

  if (timer_set_frequency(0, 20)) {
    printf("Error setting the frequency!\n");
    return 1;
  }

  if (timer_subscribe_int(&irq_set_timer)) {
    printf("Error subscribing to timer!\n");
    return 1;
  }
  if(activate_interrupt_mode()){
    printf("Error activating interrupt mode!\n");
    return 1;
  }
  if(subscribe_rtc(&irq_set_rtc)){
    printf("Error subscribing to rtc!\n");
    return 1;
  }

  init_game();

  while (!mouse_packet.rb) {
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbd_ih();
            if (state == Game) {
              game_keyboard_handler();
            }
          }
          if (msg.m_notify.interrupts & irq_set_mouse) {
            mouse_ih();
            sync_bytes();
            if (byte_index == 3) {
              byte_index = 0;
              create_packet();
            }
            if (state == Game) {
              game_mouse_handler();
            }
            else if (state == Menu) {
              menu_mouse_handler();
            }
          }
          if (msg.m_notify.interrupts & irq_set_timer) {

            if (state == Game) {
              game_main_loop();
            }
            else if (state == Menu) {
              menu_main_loop();
            }
          }
          if(msg.m_notify.interrupts & irq_set_rtc){
            update();
          }
          break;
        default:
          break;
      }
    }
  }
  if (kbd_unsubscribe_int()) {
    printf("Error subscribing to keyboard!\n");
    return 1;
  }
  if (mouse_send_command(0xF5)) {
    printf("Error disabling data reporting!\n");
    return 1;
  }

  if (mouse_unsubscribe_int()) {
    printf("Error unsubscribing to mouse!\n");
    return 1;
  }

  if (timer_unsubscribe_int()) {
    printf("Error unsubscribing to timer!\n");
    return 1;
  }
  if (unsubscribe_rtc()) {
    printf("Error unsubscribing to rtc!\n");
    return 1;
  }
  if (deactivate_interrupt_mode()) {
    printf("Error deactivating interrupt mode!\n");
    return 1;
  }

  return vg_exit();
}
