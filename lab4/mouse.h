#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <minix/sysutil.h>

int(mouse_subscribe_int)(uint16_t *irq_set);

int(mouse_unsubscribe_int)();

void(mouse_ih)();

void (sync_bytes)();

void (create_packet)();

int (kbc_send_command)(uint8_t port, uint8_t command);

int (mouse_send_command)(uint8_t command);


#endif
