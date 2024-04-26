#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

int(kbd_subscribe_int)(uint8_t *irq_set);

int(kbd_unsubscribe_int)();

int(kbd_ih)();

#endif
