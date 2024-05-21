#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

int kbd_hook_id = 1;
uint8_t kbd_value;
extern uint32_t sysinb_calls;
extern int timerCounter;

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

int(kbd_subscribe_int)(uint8_t *irq_set) {
  // Validate input.
  if (irq_set == NULL)
    return 1;

  // The irq_line should be the position of the hook id.
  *irq_set = BIT(kbd_hook_id);

  // In addition to IRQ_REENABLE, the policy also needs to be exclusive, because MINIX has a builtin interrupt handler for the keyboard.
  return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int(kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&kbd_hook_id);
}

int(kbd_ih)() {
  // Read the status from the KBC.
  uint8_t status;
  if (util_sys_inb(0x64, &status))
    return 1;

  // Check for error in the status.
  if (status & BIT(7)) {
    printf("Parity error!\n");
    return 1;
  }
  if (status & BIT(6)) {
    printf("Timeout error!\n");
    return 1;
  }
  if (status & BIT(1)) {
    printf("The input buffer is full!\n");
    return 1;
  }

  // If the output buffer is full, data is available for reading.
  if ((status & BIT(0)) != 0) {
    return util_sys_inb(0x60, &kbd_value);
  }
  return 1;
}

int(kbd_test_scan)() {
  kbd_value = 0x00;
  int ipc_status;  // Indicates if a message has been received.
  int receiver;    // Indicates if there was an error receiving the message.
  uint8_t irq_set; // 8-bit value that indicates which interrupts the program should care about.
  message msg;     // The message itself.
  uint8_t keys[2] = {0x00, 0x00};

  // Subscribe to the interruptions.
  if (kbd_subscribe_int(&irq_set) != 0)
    return 1;

  // Loop until the key is ESC.
  while (kbd_value != 0x81) {
    // Check if there is a message available
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // If the interrupt is for the keyboard, call the interrupt handler.
          if (msg.m_notify.interrupts & irq_set) {
            // Call the interrupt handler.
            if (kbd_ih())
              return 1;
            // If the previous key was 0xE0, then it means the makecode and breakcode for that key has size 2.
            if (keys[0] == 0xE0) {
              keys[1] = kbd_value;
            }
            else {
              keys[0] = kbd_value;
            }
            if (kbd_value != 0xE0) {
              kbd_print_scancode((kbd_value & BIT(7)) == 0, keys[0] == 0xE0 ? 2 : 1, keys);
              keys[0] = 0x00;
            }
          }
          break;
        default:
          break;
      }
    }
  }
  if (kbd_print_no_sysinb(sysinb_calls))
    return 1;
  return kbd_unsubscribe_int();
}

int(kbd_test_poll)() {
  uint8_t keys[2] = {0x00, 0x00};
  kbd_value = 0x00;

  // Loop until the key is ESC.
  while (kbd_value != 0x81) {
    // Read the status from the KBC.
    uint8_t status;
    if (util_sys_inb(0x64, &status))
      return 1;

    // Check for error in the status.
    if (status & BIT(7)) {
      printf("Parity error!\n");
      return 1;
    }
    if (status & BIT(6)) {
      printf("Timeout error!\n");
      return 1;
    }
    if (status & BIT(1)) {
      printf("The input buffer is full!\n");
      return 1;
    }

    // If the output buffer is full, data is available for reading.
    if ((status & BIT(0)) != 0) {
      if (util_sys_inb(0x60, &kbd_value))
        return 1;

      // If the previous key was 0xE0, then it means the makecode and breakcode for that key has size 2.
      if (keys[0] == 0xE0) {
        keys[1] = kbd_value;
      }
      else {
        keys[0] = kbd_value;
      }
      if (kbd_value != 0xE0) {
        kbd_print_scancode((kbd_value & BIT(7)) == 0, keys[0] == 0xE0 ? 2 : 1, keys);
        keys[0] = 0x00;
      }
    }
  }
  // Restore the keyboard controller by reenabling the interrupts.
  // Send command 0x20 to the keyboard controller, telling it we want to read the command byte.
  if (sys_outb(0x64, 0x20))
    return 1;

  // Read the command byte.
  uint8_t command_byte;
  if (util_sys_inb(0x60, &command_byte))
    return 1;

  // Enable the bit 0, which is the bit that enables / disables the interrupts.
  command_byte |= BIT(0);

  // Send command 0x60 to the keyboard controller, telling it we want to write a new command byte.
  if (sys_outb(0x64, 0x60))
    return 1;

  // Write the new command byte.
  if (sys_outb(0x60, command_byte))
    return 1;

  return kbd_print_no_sysinb(sysinb_calls);
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t irq_set_timer, irq_set_kbd; // 8-bit value that indicates which interrupts the program should care about.
  int ipc_status;                     // Indicates if a message has been received.
  int receiver;                       // Indicates if there was an error receiving the message.
  message msg;                        // The message itself.

  int time = 0;
  kbd_value = 0x00;
  uint8_t keys[2] = {0x00, 0x00};

  // Subscribe to the interrupts
  if (timer_subscribe_int(&irq_set_timer))
    return 1;
  if (kbd_subscribe_int(&irq_set_kbd))
    return 1;

  // Loop until the key is ESC or the time has passed.
  while (kbd_value != 0x81 && time < n) {
    // Check if there is a message available
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // If the interrupt is for the keyboard, call the interrupt handler.
          if (msg.m_notify.interrupts & irq_set_kbd) {
            // Call the interrupt handler.
            if (kbd_ih())
              return 1;
            // If the previous key was 0xE0, then it means the makecode and breakcode for that key has size 2.
            if (keys[0] == 0xE0) {
              keys[1] = kbd_value;
            }
            else {
              keys[0] = kbd_value;
            }
            if (kbd_value != 0xE0) {
              kbd_print_scancode((kbd_value & BIT(7)) == 0, keys[0] == 0xE0 ? 2 : 1, keys);
              keys[0] = 0x00;
            }
            // Reset the counters.
            time = 0;
            timerCounter = 0;
          }
          // If the interrupt is for the timer, call the interrupt handler.
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
  // Unsubscribe from the interrupts.
  if (kbd_unsubscribe_int())
    return 1;
  return timer_unsubscribe_int();
}