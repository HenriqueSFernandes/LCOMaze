#include "serial.h"
#include <stdio.h>

int sp_hook_id;
static queue_t *transmit_queue;
static queue_t *receive_queue;

int sp_enable_int(unsigned short base_addr, uint8_t mask) {
    uint8_t reg_ier;
    if (util_sys_inb(base_addr + IER, &reg_ier)) {
        printf("Failed to read IER\n");
        return 1;
    }
    reg_ier &= 0xF8; // Ensure bits 4-7 remain unchanged
    if (sys_outb(base_addr + IER, reg_ier | mask)) {
        printf("Failed to write IER\n");
        return 1;
    }
    return 0;
}

int clear_transmit_queue() {
    while (!empty(transmit_queue)) {
        pop(transmit_queue);
    }
    return 0;
}

int clear_receive_queue() {
    while (!empty(receive_queue)) {
        pop(receive_queue);
    }
    return 0;
}

int sp_subscribe_int(unsigned int base, uint32_t *bit_no) {
    if (base != 0x3f8 && base != 0x2f8) {
        printf("Invalid serial port\n");
        return 1;
    }

    if (bit_no == NULL) {
        printf("Error: Null pointer bit_no\n");
        return 1;
    }

    int irq;
    if (base == 0x3f8) {
        irq = 4;
        sp_hook_id = 9;
        *bit_no = BIT(sp_hook_id);
    }
    else {
        irq = 3;
        sp_hook_id = irq;
        *bit_no = BIT(sp_hook_id);
    }
    if (sys_irqsetpolicy(irq, IRQ_REENABLE | IRQ_EXCLUSIVE, &sp_hook_id)) {
        printf("Error: Failed to set policy\n");
        return 1;
    }

    transmit_queue = new_queue();
    if (!transmit_queue) {
        printf("Error: Failed to initialize transmit queue\n");
        return 1;
    }

    receive_queue = new_queue();
    if (!receive_queue) {
        printf("Error: Failed to initialize receive queue\n");
        delete_queue(transmit_queue);
        return 1;
    }

    return 0;
}

int sp_unsubscribe() {
    if (sys_irqdisable(&sp_hook_id))
        return 1;
    if (sys_irqrmpolicy(&sp_hook_id))
        return 1;
    delete_queue(transmit_queue);
    delete_queue(receive_queue);
    return 0;
}

int ser_ih(unsigned short base_addr) {
    uint8_t iir;
    if (util_sys_inb(base_addr + IIR, &iir)) {
        return 1;
    }

    if (!(iir & BIT(0))) { // if it is pending
        uint32_t lsr;
        if (sys_inb(base_addr + LSR, &lsr) != OK)
            return 1;

        if (iir & BIT(1)) { // transmitter full
            while (lsr & BIT(5)) {
                if (empty(transmit_queue)) {
                    break; // No more data to send
                }
                char c = top(transmit_queue);
                if (sys_outb(base_addr, c) != OK)
                    return 1;
                pop(transmit_queue);
                if (sys_inb(base_addr + LSR, &lsr) != OK)
                    return 1;
            }
        }
        else if (iir & BIT(2)) { // receiver available
            while (lsr & BIT(0)) {
                uint32_t c;
                if (sys_inb(base_addr, &c) != OK)
                    return 1;
                push(receive_queue, (char) c);
                if (sys_inb(base_addr + LSR, &lsr) != OK)
                    return 1;
            }
        }
    }
    return 0;
}

int sp_get_status(uint8_t *status) {
    return util_sys_inb(0x3f8 + LSR, status);
}

void clean_queue() {
    delete_queue(transmit_queue);
    delete_queue(receive_queue);
}

int receive(char *c) {
    ser_ih(0x3f8);
    if (!empty(receive_queue)) {
        *c = top(receive_queue);
        pop(receive_queue);
    }

    return 0;
}

int sp_send_int(unsigned short base_addr, unsigned long bits,
                unsigned long stop, long parity, unsigned long rate,
                char string[], int stringc) {
    transmit_queue = new_queue();

    for (int i = 0; i < stringc; ++i) {
        push(transmit_queue, string[i]);
    }

    while (!empty(transmit_queue)) {

        uint8_t iir;
        uint32_t lsr;

        if (util_sys_inb(base_addr + IIR, &iir))
            return 1;
        if (sys_inb(base_addr + LSR, &lsr) != OK)
            return 1;
        if (lsr & BIT(5)) {
            if (empty(transmit_queue)) {
                break; // No more data to send
            }
            char c = top(transmit_queue);
            uint8_t status, attempts = 10;
            while (attempts--) {
                if (sp_get_status(&status))
                    return 1;
                if (status & BIT(5)) {
                    sys_outb(0X3F8 + THR, (uint8_t) c);
                    break;
                }
            }
            pop(transmit_queue);
        }
    }

    return 0;
}
