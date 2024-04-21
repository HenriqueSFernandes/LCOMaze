// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

int mouse_hook_id = 12;
struct packet mouse_packet;
uint8_t byte_counter = 0;
uint8_t packet_bytes[3];
uint8_t current_byte;
uint32_t counter = 0;

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

int(mouse_subscribe_int)(uint8_t *irq_set) {
  // Validate input.
  if (irq_set == NULL)
    return 1;

  // The irq_line should be the position of the hook id.
  *irq_set = BIT(mouse_hook_id);

  // In addition to IRQ_REENABLE, the policy also needs to be exclusive, because MINIX has a builtin interrupt handler for the keyboard.
  return sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&mouse_hook_id);
}

void(mouse_ih)() {
  // Read the status from the KBC.
  uint8_t status;
  if (util_sys_inb(0x64, &status))
    return;
  // Check for error in the status.
  if (status & BIT(7)) {
    printf("Parity error!\n");
    return;
  }
  if (status & BIT(6)) {
    printf("Timeout error!\n");
    return;
  }
  if (status & BIT(1)) {
    printf("The input buffer is full!\n");
    return;
  }
  if (((status & BIT(5)) == 1) && ((status & BIT(0)) != 0)) {
    // If the data is ready
    if (util_sys_inb(0x60, &current_byte))
      return;
    // Sync the mouse with the driver
    if (byte_counter == 0 && (current_byte & BIT(3))) {
      packet_bytes[byte_counter] = current_byte;
      byte_counter++;
    }
  }
  else if (byte_counter > 0) {
    packet_bytes[byte_counter] = current_byte;
    byte_counter++;
  }
}

int(mouse_write_command)(uint8_t command) {
  int tries = 5;
  uint8_t response;

  do {
    tries--;
    if (sys_outb(0x64, 0xD4))
      return 1;
    if (sys_outb(0x60, command))
      return 1;
    if (util_sys_inb(0x60, &response))
      return 1;
    if (response == 0xFA)
      return 0;
  } while (response != 0xFA && tries > 0);
  return 1;
}

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  int receiver;
  uint8_t irq_set;
  message msg;

  if (mouse_enable_data_reporting())
    return 1;
    
  // Subscribe to the interruptions.
  if (mouse_subscribe_int(&irq_set) != 0)
    return 1;

  while (counter < cnt) {
    // Check if there is a message available
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // If the interrupt is for the mouse, call the interrupt handler.
          if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
          }
          break;
        default:
          break;
      }
    }
  }
  if (mouse_write_command(0xF5))
    return 1;

  return mouse_unsubscribe_int();
}

int(mouse_test_async)(uint8_t idle_time) {
  /* To be completed */
  printf("%s(%u): under construction\n", __func__, idle_time);
  return 1;
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
