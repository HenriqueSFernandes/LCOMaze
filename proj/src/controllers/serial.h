#ifndef SERIAL_H
#define SERIAL_H

#include "UART.h"
#include "../data_structures/queue.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

// Global variable declaration
extern int sp_hook_id;  

// Function prototypes
int sp_enable_int(unsigned short base_addr, uint8_t mask);
int sp_subscribe_int(unsigned int base, uint32_t *bit_no) ;
int sp_unsubscribe();
int ser_ih(unsigned short base_addr);
int sp_send_int(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate, char string[],int stringc);
int receive(char * c);
#endif /* SERIAL_H */
