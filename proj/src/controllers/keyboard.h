/**
 * @file keyboard.h
 * @brief Functions to handle the keyboard.
 */

/**
 * \addtogroup devices
 * @{
 */

/** @defgroup keyboard Keyboard
 * @brief Module for handling the keyboard.
 * @{
 */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param irq_set Pointer to a variable that will store the IRQ set.
 * @return 0 upon success, 1 if the input is invalid.
 */
int(kbd_subscribe_int)(uint16_t *irq_set);

/**
 * @brief Unsubscribes the keyboard interrupt.
 *
 * @return Returns 0 upon success, -1 otherwise.
 */
int(kbd_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler.
 * 
 * This function is the interrupt handler for the keyboard. It reads the status from the KBC
 * and checks for any errors. If the output buffer is full, it reads the data from the keyboard.
 */
void(kbd_ih)();

#endif

/**@}*/

/**@}*/

