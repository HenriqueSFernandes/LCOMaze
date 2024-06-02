/**
 * @file serial.h
 * @brief Function to handle the Serial Port.
 */

/**
 * \addtogroup devices
 * @{
 */

/** @defgroup serial_port Serial Port
 * @brief Module for handling the Serial Port.
 * @{
 */

#ifndef SERIAL_H
#    define SERIAL_H

#    include "../data_structures/queue.h"
#    include "UART.h"
#    include <lcom/lcf.h>
#    include <stdbool.h>
#    include <stdint.h>

extern int sp_hook_id; ///< Hook ID for the serial port

/**
 * @brief Enables the specified interrupt(s) for a serial port.
 *
 * This function enables the specified interrupt(s) for a serial port by modifying the Interrupt Enable Register (IER).
 *
 * @param base_addr The base address of the serial port.
 * @param mask The bitmask representing the interrupt(s) to enable.
 * @return Returns 0 on success, 1 if there was an error.
 */
int sp_enable_int(unsigned short base_addr, uint8_t mask);

/**
 * @brief Subscribes to the serial port interrupts.
 *
 * This function subscribes to the interrupts of the serial port specified by the base address.
 * It also initializes the transmit and receive queues.
 *
 * @param base The base address of the serial port (0x3f8 or 0x2f8).
 * @param bit_no Pointer to store the bit number associated with the interrupt.
 * @return Returns 0 upon success, 1 otherwise.
 */
int sp_subscribe_int(unsigned int base, uint32_t *bit_no);

/**
 * @brief Unsubscribes the serial port interrupts and deletes the transmit and receive queues.
 *
 * This function disables the serial port interrupts by calling sys_irqdisable() with the sp_hook_id,
 * and then removes the interrupt policy by calling sys_irqrmpolicy() with the sp_hook_id.
 * It also deletes the transmit and receive queues by calling delete_queue() on each of them.
 *
 * @return 0 upon success, 1 otherwise.
 */
int sp_unsubscribe();

/**
 * @brief Interrupt handler for the serial port.
 *
 * This function handles interrupts from the serial port specified by the base address.
 * It checks the Interrupt Identification Register (IIR) to determine the type of interrupt.
 * If the interrupt is pending, it checks the Line Status Register (LSR) to determine the reason for the interrupt.
 * If the interrupt is due to the transmitter being full, it sends data from the transmit queue to the serial port until the transmitter is no longer full.
 * If the interrupt is due to data being available in the receiver, it reads the data from the serial port and adds it to the receive queue.
 *
 * @param base_addr The base address of the serial port.
 * @return Returns 0 on success, 1 on failure.
 */
int ser_ih(unsigned short base_addr);

/**
 * @brief Sends a string of characters over a serial port.
 *
 * @param base_addr The base address of the serial port.
 * @param bits The number of bits per character.
 * @param stop The number of stop bits.
 * @param parity The parity bit.
 * @param rate The baud rate.
 * @param string The string of characters to be sent.
 * @param stringc The length of the string.
 *
 * @return Returns 0 on success, 1 if there was an error.
 */
int sp_send_int(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate, char string[], int stringc);

/**
 * @brief Reads the status register of the serial port.
 *
 * This function reads the status register of the serial port specified by the base address 0x3f8 + LSR.
 *
 * @param status Pointer to a variable where the status register value will be stored.
 * @return 0 upon success, non-zero otherwise.
 */
int sp_get_status(uint8_t *status);

/**
 * @brief Receives a character from the serial port.
 *
 * This function reads a character from the receive queue of the serial port.
 * If the receive queue is not empty, the function retrieves the top character
 * and stores it in the provided pointer 'c'.
 *
 * @param c Pointer to a character variable where the received character will be stored.
 * @return 0 on success, indicating that a character was received and stored in 'c'.
 */
int receive(char *c);

/**
 * @brief Cleans the transmit and receive queues.
 *
 * This function deletes all the elements in the transmit_queue and receive_queue,
 * effectively cleaning the queues.
 */
void clean_queue();

/**
 * Clears the receive queue by popping all elements from it.
 *
 * @return 0 if the receive queue was successfully cleared, otherwise an error code.
 */
int clear_receive_queue();

/**
 * Clears the transmit queue by popping all elements until it is empty.
 *
 * @return 0 if the transmit queue is successfully cleared, otherwise an error code.
 */
int clear_transmit_queue();

#endif
/**@}*/

/**@}*/
