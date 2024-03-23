#include <lcom/lcf.h>

#include <stdint.h>

#define LSB_FILTER 0XFF // SAME AS 0X00...FF

#ifdef LAB3
uint32_t counter = 0;
#endif

/**
 * @brief This function gets the least significant byte of a 16bit number.
 * @param val A 16-bit number.
 * @param lsb pointer to a 8bit number that will hold the final result.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  if (lsb == NULL) {
    return 1;
  }
  *lsb = val & LSB_FILTER;
  return 0;
}

/**
 * @brief This function gets the most significant byte of a 16bit number.
 * @param val A 16-bit number.
 * @param lsb A pointer to a 8bit number that will hold the final result.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) {
    return 1;
  }
  *msb = val >> 8;
  return 0;
}

/**
 * @brief This function sends an 8bit value to a port.
 * @param port The communication port.
 * @param lsb A pointer to an 8bit number
 */
int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t dummy_value;
  if (sys_inb(port, &dummy_value))
    return 1;
  *value = dummy_value & LSB_FILTER;
#ifdef LAB3
  counter++;
#endif
  return 0;
}
