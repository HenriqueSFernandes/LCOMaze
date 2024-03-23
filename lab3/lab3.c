#include "keyboard.h"
#include "timer.c"
#include <i8042.h>
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

extern uint8_t valueKBD;
extern uint32_t counter;
extern int counterTimer;
extern uint8_t r;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t res[2];
  int ipc_status;
  bool waiting = false;
  message msg;
  uint8_t irq_set;
  if (kbd_subscribe_int(&irq_set))
    return 1;
  int error;
  while (valueKBD != 0x81) {
    if ((error = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            if (waiting) {
              res[1] = valueKBD;
            }
            else {
              res[0] = valueKBD;
            }
            if (valueKBD == 0xE0) {
              waiting = true;
              break;
            }
            if (valueKBD != 0xE0) {
              uint8_t x = waiting ? 2 : 1;
              kbd_print_scancodde(!(valueKBD & MAKE_CODE), x, res);
            }
            waiting = false;
          }
          break;
        default:
          break;
      }
    }
  }
  kbd_print_no_sysinb(counter);
  return kbd_unsubscribe_int();
}

int(kbd_test_poll)() {
  uint8_t res[2];
  bool waiting = false;
  while (valueKBD != 0x81) {
    uint8_t status;
    if (util_sys_inb(STATUS_REG, &status)) {
      return 1;
    };
    if ((status & (BIT(6) | BIT(7))) != 0) {
      return 1;
    }
    if (status & BIT(0)) {
      if (util_sys_inb(0x60, &valueKBD)) {
        return 1;
      };
      if (waiting) {
        res[1] = valueKBD;
      }
      else {
        res[0] = valueKBD;
      }
      if (valueKBD == 0xE0) {
        waiting = true;
      }
      if (valueKBD != 0xE0 && valueKBD) {
        uint8_t x = waiting ? 2 : 1;
        kbd_print_scancode(!(valueKBD & MAKE_CODE), x, res);
      }
      waiting = false;
    }
  }

  kbd_print_no_sysinb(counter);
  return restoreKeyboard();
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t res[2];
  bool waiting = false;
  int receiver;
  int ipc_status;
  uint8_t irq_set_timer, irq_set_kbd;
  message msg;
  uint8_t time = 0;

  if (timer_subscribe_int(&irq_set_timer) != 0)
    return 1;
  if (kbd_subscribe_int(&irq_set_kbd) != 0)
    return 1;

  while (valueKBD != 0x81 && time < n) {
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (counterTimer % 60 == 0)
              time++;
          }
          if (msg.m_notify.interrupts & irq_set_kbd) {
            time = 0;
            kbc_ih();
            if (waiting) {
              res[1] = valueKBD;
            }
            else {
              res[0] = valueKBD;
            }
            if (valueKBD == 0xE0) {
              waiting = true;
              break;
            }
            if (valueKBD != 0xE0) {
              uint8_t x = waiting ? 2 : 1;
              kbd_print_scancode(!(valueKBD & MAKE_CODE), x, res);
            }
            waiting = false;
          }
          break;
        default:
          break;
      }
    }
  }
  if (kbd_unsubscribe_int())
    return 1;
  if (timer_unsubscribe_int())
    return 1;
  return 0;
}
