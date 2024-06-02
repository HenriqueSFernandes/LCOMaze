/**
 * @file rtc.h
 * @brief Functions to handle the RTC.
 */

/**
 * \addtogroup devices
 * @{
 */

/** @defgroup rtc RTC
 * @brief Module for handling the RTC.
 * @{
 */

#ifndef RTC_H
#    define RTC_H

#    include <lcom/lcf.h>
#    include <math.h>
#    include <minix/sysutil.h>

/**
 * @brief Structure representing a time element.
 *
 * This structure holds the values for seconds, minutes, and hours of a time element.
 */
typedef struct timeEl {
    uint32_t seconds; /**< The seconds value of the time element. */
    uint32_t minutes; /**< The minutes value of the time element. */
    uint32_t hours;   /**< The hours value of the time element. */
} time_el;

#    define RTC_IRQ 8
#    define RTC_ADDR_REG 0x70
#    define RTC_DATA_REG 0x71
#    define RTC_REG_A 10
#    define RTC_REG_B 11
#    define RTC_REG_C 12
#    define RTC_REG_D 13
#    define RTC_REG_SECONDS 0
#    define RTC_REG_MINUTES 2
#    define RTC_REG_HOURS 4
#    define UE BIT(4)
#    define UIE BIT(4)
#    define VRT BIT(7)


/**
 * @brief Activates the interrupt mode for the Real-Time Clock (RTC).
 *
 * This function sets the interrupt enable bit in the RTC's Register B, allowing
 * the RTC to generate interrupts. The interrupt is triggered when the RTC's
 * internal clock reaches the value set in the alarm registers.
 *
 * @return 0 on success, non-zero otherwise.
 */
int set_update();

/**
 * @brief Deactivates the interrupt mode of the Real-Time Clock (RTC).
 *
 * This function disables the interrupt mode of the RTC by clearing the 4th bit of the RTC register B.
 *
 * @return 0 on success, non-zero otherwise.
 */
int clean_update();

/**
 * @brief Subscribes to RTC interrupts.
 *
 * This function subscribes to RTC interrupts by setting the policy for the RTC IRQ line.
 * It also retrieves the bit number associated with the RTC hook ID.
 *
 * @param bit_no Pointer to the variable where the bit number will be stored.
 * @return Returns 0 upon success, 1 otherwise.
 */
int subscribe_rtc(uint16_t *bit_no);

/**
 * @brief Unsubscribes the RTC interrupts.
 *
 * This function unsubscribes the RTC interrupts by calling sys_irqrmpolicy() with the RTC's hook_id.
 * If an error occurs during the unsubscription process, an error message is printed and the function returns 1.
 *
 * @return 0 upon success, 1 upon failure.
 */
int unsubscribe_rtc();

/**
 * @brief Retrieves the current time from the RTC (Real-Time Clock) chip.
 *
 * This function reads the hours, minutes, and seconds from the RTC chip and stores them in the `time_stamp` structure.
 *
 * @return 0 on success, indicating that the time was successfully retrieved.
 */
int get_time();

/**
 * @brief Updates the Real-Time Clock (RTC) and retrieves the current time.
 *
 * This function updates the RTC by reading the cause register and then retrieves the current time
 * from the RTC. The time is stored in the `time_stamp` structure.
 *
 * @return 0 on success.
 */
int update();


/**
 * @brief Retrieves the current time from the RTC (Real-Time Clock) chip.
 *
 * This function updates the RTC by reading the cause register and then retrieves the current time
 * from the RTC. The time is stored in 3 variables: seconds, minutes, and hours.
 *
 * @return 0 on success.
 */
int get_time_direct(uint32_t *seconds, uint32_t *minutes, uint32_t *hours);
#endif

/**@}*/

/**@}*/
