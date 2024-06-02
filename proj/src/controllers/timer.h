/**
 * @file timer.h
 * @brief Function to handle the Timer.
 */

/**
 * \addtogroup devices
 * @{
 */

/** @defgroup timer Timer
 * @brief Module for handling the Timer.
 * @{
 */

#ifndef TIMER_H
#    define TIMER_H

#    include <lcom/lcf.h>

/**
 * @brief Sets the frequency of a timer.
 *
 * This function sets the frequency of the specified timer by writing the control word and the initial value to the timer's control and data registers.
 *
 * @param timer The timer to set the frequency for (0, 1, or 2).
 * @param freq The desired frequency in Hz.
 *
 * @return Returns 0 upon success, non-zero otherwise.
 */
int(timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes and enables interrupts from the timer.
 *
 * This function subscribes to the timer interrupts by setting the policy for the timer's IRQ line.
 * It also enables the interrupts by setting the corresponding bit in the bit_no variable.
 *
 * @param bit_no Pointer to a variable that will store the position of the hook id bit.
 * @return Returns 0 upon success, 1 if bit_no is NULL.
 */
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the timer interrupts.
 *
 * This function unsubscribes the timer interrupts by calling sys_irqrmpolicy() with the timerHookId.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int(timer_unsubscribe_int)();

/**
 * @brief Interrupt handler for the timer.
 *
 * This function is called whenever a timer interrupt occurs. It increments the timerCounter variable.
 */
void(timer_int_handler)();

/**
 * @brief Retrieves the configuration of a timer.
 *
 * This function retrieves the configuration of a timer specified by the parameter `timer`.
 * The configuration is stored in the variable pointed to by `st`.
 *
 * @param timer The timer number (0, 1, or 2).
 * @param st Pointer to a variable where the configuration will be stored.
 * @return Returns 0 upon success, non-zero otherwise.
 */
int(timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the configuration of a timer.
 *
 * This function takes a timer number, a status byte, and a field to specify which part of the configuration to display.
 * It extracts the relevant information from the status byte based on the specified field and prints the configuration.
 *
 * @param timer The timer number.
 * @param st The status byte containing the timer configuration.
 * @param field The field specifying which part of the configuration to display.
 *
 * @return 0 on success, 1 if an invalid field is specified.
 */
int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field);

#endif

/**@}*/

/**@}*/
