#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "i8042.h"
#include <lcom/lcf.h>

int(kbd_subscribe_int)(uint8_t *bit_no);
int(kbd_unsubscribe_int)();
void(kbc_ih)();
int(restoreKeyboard)();
#endif
