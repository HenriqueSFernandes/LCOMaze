#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

int(kbd_subscribe_int)(uint16_t *irq_set);

int(kbd_unsubscribe_int)();

void(kbd_ih)();

#endif
