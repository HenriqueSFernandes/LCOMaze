#include "keyboard.h"
uint8_t valueKBD;
uint8_t r;
int hook_id_kbd = 1;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(hook_id_kbd);
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != 0)
    return 1;

  return 0;
}

int(kbd_unsubscribe_int)() {
  return (sys_irqrmpolicy(&hook_id_kbd));
}

int(restoreKeyboard)() {
  uint8_t currentKBDConfig;
  if (sys_outb(0x64, 0x20))
    return 1;
  if (util_sys_inb(0x60, &currentKBDConfig))
    return 1;
  uint8_t newKBDConfig = currentKBDConfig | BIT(0);
  printf("Current keyboard config: %x\n", currentKBDConfig);
  printf("New keyboard config: %x\n", newKBDConfig);
  if (sys_outb(0x64, 0x60))
    return 1;
  return (sys_outb(0x60, newKBDConfig));
}

void(kbc_ih)() {
  // 1- ler estado do kbd
  // 2- verificar se out buf esta cheio e se n existe erro
  // 3- ler o valor do out buf
  // 4- return
  r = 0;
  uint8_t status;
  if (util_sys_inb(STATUS_REG, &status)) {
    r = 1;
    return;
  };
  if ((status & (BIT(6) | BIT(7))) != 0) {
    r = 1;
    return;
  }
  if (status & BIT(0)) {
    if (util_sys_inb(0x60, &valueKBD)) {
      r = 1;
      return;
    };
  }
}
