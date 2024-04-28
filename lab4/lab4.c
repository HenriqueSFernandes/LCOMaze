// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"

extern uint8_t byte_index;
extern struct packet mouse_packet;
extern int timerCounter;
// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  int receiver;
  uint16_t irq_set;
  message msg;
  if (mouse_subscribe_int(&irq_set)) {
    printf("Error subscribing to mouse!\n");
    return 1;
  }
  if (mouse_send_command(0xF4)) {
    printf("Error enabling data reporting!");
    return 1;
  }
  while (cnt) {
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
            sync_bytes();
            if (byte_index == 3) {
              create_packet();
              byte_index = 0;
              cnt--;
              mouse_print_packet(&mouse_packet);
            }
          }
          break;
        default:
          break;
      }
    }
  }
  if (mouse_send_command(0xF5)) {
    printf("Error disabling data reporting!");
    return 1;
  }
  if (mouse_unsubscribe_int()) {
    printf("Error unsubscribing to mouse!");
    return 1;
  }
  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  int receiver;
  uint16_t irq_set_mouse;
  uint8_t irq_set_timer;
  int time = 0;
  message msg;
  if (mouse_subscribe_int(&irq_set_mouse)) {
    printf("Error subscribing to mouse!\n");
    return 1;
  }
  if (timer_subscribe_int(&irq_set_timer)) {
    printf("Error subscribing to mouse!\n");
    return 1;
  }
  if (mouse_send_command(0xF4)) {
    printf("Error enabling data reporting!");
    return 1;
  }

  while (time < idle_time) {
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
              mouse_print_packet(&mouse_packet);
            }
            time = 0;
            timerCounter = 0;
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            // If a second has passed, increase the timer.
            if (timerCounter % 60 == 0) {
              timer_print_elapsed_time();
              time++;
            }
          }
          break;
        default:
          break;
      }
    }
  }
  if (mouse_send_command(0xF5)) {
    printf("Error disabling data reporting!");
    return 1;
  }
  if (mouse_unsubscribe_int()) {
    printf("Error unsubscribing to mouse!");
    return 1;
  }
  if (timer_unsubscribe_int()) {
    printf("Error unsubscribing to mouse!");
    return 1;
  }
  return 0;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  /* To be completed */
  printf("%s: under construction\n", __func__);
  return 1;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
