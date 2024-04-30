// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
// Any header files included below this line should have been created by you

extern uint8_t kbd_value;
extern uint8_t byte_index;
extern struct packet mouse_packet;
extern int timerCounter;
extern vbe_mode_info_t mode_info;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  // lcf_trace_calls("/home/lcom/labs/g1/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/g1/proj/src/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  int ipc_status;
  int receiver;
  // kbd_value = 0x00;
  // uint8_t keys[2] = {0x00, 0x00};
  // uint16_t irq_set_keyboard;
  uint16_t irq_set_mouse;
  uint8_t irq_set_timer;
  message msg;
  int16_t x = 0;
  int16_t y = 0;
  if (mouse_subscribe_int(&irq_set_mouse)) {
    printf("Error subscribing to mouse!\n");
    return 1;
  }
  if (mouse_send_command(0xF4)) {
    printf("Error enabling data reporting!\n");
    return 1;
  }
  if (setFrameBuffer(0x11A)) {
    printf("Error setting frame buffer!\n");
    return 1;
  }
  if (setGraphicsMode(0x119)) {
    printf("Error setting graphics mode!\n");
    return 1;
  }
  if (timer_set_frequency(0, 240)) {
    printf("Error setting the frequency!\n");
    return 1;
  }
  if (timer_subscribe_int(&irq_set_timer)) {
    printf("Error subscribing to timer!\n");
    return 1;
  }

  // if (kbd_subscribe_int(&irq_set_keyboard)) {
  //   printf("Error subscribing to keyboard!\n");
  //   return 1;
  // }
  uint32_t color;
  normalizeColor(0xFFFFFF, &color);
  while (!mouse_packet.rb) {
    // Check if there is a message available
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_mouse) {
            mouse_ih();
            sync_bytes();
            if (byte_index == 3) {
              create_packet();
              byte_index = 0;
              x += mouse_packet.delta_x;
              y -= mouse_packet.delta_y;
              if (x < 0)
                x = 0;
              if (y < 0)
                y = 0;
              if (x > mode_info.XResolution - 5)
                x = mode_info.XResolution - 5;
              if (y > mode_info.YResolution - 5)
                y = mode_info.YResolution - 5;
              // printf("x: %d, y: %d\n", x, y);
            }
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            vg_draw_rectangle(x, y, 5, 5, color);
          }
          break;
        default:
          break;
      }
    }
  }
  // if (kbd_unsubscribe_int()) {
  //   printf("Error unsubscribing to keyboard!\n");
  //   return 1;
  // }
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

  return vg_exit();
}
