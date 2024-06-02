/**
 * @file mouse.h
 * @brief Functions to handle the mouse.
 */

/**
 * \addtogroup devices
 * @{
 */

/** @defgroup mouse Mouse
 * @brief Module for handling the mouse.
 * @{
 */
#ifndef MOUSE_H
#    define MOUSE_H

#    include <lcom/lcf.h>
#    include <minix/sysutil.h>

/**
 * @brief Subscribes to mouse interrupts.
 *
 * This function subscribes to mouse interrupts by setting the IRQ line associated with the mouse
 * to the specified bit position in the irq_set variable. It also sets the policy for the IRQ line
 * to enable and exclusive mode.
 *
 * @param irq_set Pointer to the variable that will store the IRQ line bit position.
 * @return Returns 0 upon success, 1 if irq_set is NULL.
 */
int(mouse_subscribe_int)(uint16_t *irq_set);

/**
 * @brief Unsubscribes mouse interrupts.
 *
 * This function unsubscribes the mouse interrupts by calling sys_irqrmpolicy() with the mouse_hook_id.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int(mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 *
 * This function is the interrupt handler for the mouse. It reads the status from the KBC
 * and checks for errors. If the output buffer is full, it reads the data from the keyboard.
 */
void(mouse_ih)();

/**
 * @brief Stores the current byte in the mouse_bytes array if certain conditions are met.
 *
 * This function is responsible for storing the current byte in the mouse_bytes array
 * if either of the following conditions are met:
 * - The byte_index is 0 and the 3rd bit of the current_byte is set.
 * - The byte_index is greater than 0.
 */
void(sync_bytes)();

/**
 * @brief Creates a mouse packet based on the received mouse bytes.
 *
 * This function creates a mouse packet by copying the mouse bytes into the packet structure.
 */
void(create_packet)();

/**
 * @brief Sends a command to the keyboard controller (KBC) through the specified port.
 *
 * @param port The port to send the command to.
 * @param command The command to send.
 * @return 0 upon success, 1 otherwise.
 */
int(kbc_send_command)(uint8_t port, uint8_t command);

/**
 * @brief Sends a command to the mouse.
 *
 * This function sends a command to the mouse and waits for an acknowledgement response.
 * It retries sending the command up to 10 times if the response is not an acknowledgement.
 *
 * @param command The command to send to the mouse.
 * @return Returns 0 on success, 1 on failure.
 */
int(mouse_send_command)(uint8_t command);

#endif

/**@}*/

/**@}*/
