#include "keyboard.h"

int kbd_hook_id = 7;
uint8_t kbd_value;

int kbd_subscribe_int(uint16_t *irq_set) {
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

void(kbd_ih)() {
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

  // If the output buffer is full, data is available for reading.
  if ((status & BIT(0)) != 0 && ((status & BIT(5)) == 0)) {
    util_sys_inb(0x60, &kbd_value);
  }
}
