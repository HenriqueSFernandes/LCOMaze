#include "timer.h"
#include <lcom/lcf.h>

#include <stdint.h>

int timerHookId = 0;
int timerCounter = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
    // Validate input.
    if (timer < 0 || timer > 2)
        return 1;

    if (freq < 19 || freq > 1193181)
        return 1;

    // Read current config.
    uint8_t currentConfig;
    if (timer_get_conf(timer, &currentConfig))
        return 1;

    // Calculate the initial value for the timer.
    uint16_t initialValue = 1193181 / freq;

    // Split the initial value into LSB and MSB.
    uint8_t initialValueMSB, initialValueLSB;
    if (util_get_MSB(initialValue, &initialValueMSB))
        return 1;
    if (util_get_LSB(initialValue, &initialValueLSB))
        return 1;

    // Create the control word.
    uint8_t controlWord = 0;

    // Start by choosing the timer (bits 7 and 6).
    switch (timer) {
        case 1:
            controlWord |= 0x40; // 01xxxxxx
            break;
        case 2:
            controlWord |= 0x80; // 10xxxxxx
            break;
        default:
            break;
    }

    // Set the initialization mode to LSB followed by MSB (bits 5 and 4).
    controlWord |= 0x30; // xx11xxxx

    // The remaining bits should be that same as the current config.
    controlWord |= currentConfig & 0x0F;

    // Write the control word to the control register (0x43).
    if (sys_outb(0x43, controlWord))
        return 1;

    // Write the LSB and the MSB to the timer.
    if (sys_outb(0x40 + timer, initialValueLSB))
        return 1;
    if (sys_outb(0x40 + timer, initialValueMSB))
        return 1;

    return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
    // Validate the input.
    if (bit_no == NULL)
        return 1;

    // bit_no should indicate the position of the hook id bit.
    *bit_no = BIT(timerHookId);
    return sys_irqsetpolicy(0, IRQ_REENABLE, &timerHookId);
}

int(timer_unsubscribe_int)() {
    return sys_irqrmpolicy(&timerHookId);
}

void(timer_int_handler)() {
    timerCounter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
    // Check if the timer is invalid.
    if (timer < 0 || timer > 2)
        return 1;

    // Check if the pointer is null.
    if (st == NULL)
        return 1;

    // Create the control word: bits 7 and 6 for readback, bit 5 for disabling the counter read and bit(timer+1) to select the timer.
    uint8_t controlWord = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1);

    // Send the control word to the control register (port 0x43).
    if (sys_outb(0x43, controlWord))
        return 1;

    // Read the config for the given timer.
    return (util_sys_inb(0x40 + timer, st));
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

    // Value that will be sent to timer_print_config.
    union timer_status_field_val data;

    switch (field) {
        case tsf_all:
            // Send the full config.
            data.byte = st;
            break;
        case tsf_initial:
            // Send only the initialization mode.
            st = (st >> 4) & 0x03;
            if (st == 0) {
                data.in_mode = INVAL_val;
            }
            else if (st == 1) {
                data.in_mode = LSB_only;
            }
            else if (st == 2) {
                data.in_mode = MSB_only;
            }
            else if (st == 3) {
                data.in_mode = MSB_after_LSB;
            }
            break;
        case tsf_mode:
            // Send only the counting mode.
            st = (st >> 1) & 0x07;
            // For backwards compatibility, mode 6 and 7 are the same as 2 and 3.
            if (st == 6)
                data.count_mode = 2;
            else if (st == 7)
                data.count_mode = 3;
            else
                data.count_mode = st;
            break;
        case tsf_base:
            // Send only the time base.
            data.bcd = st & 0x01;
            break;
        default:
            return 1;
    }
    // Print the config.
    return timer_print_config(timer, field, data);
}
