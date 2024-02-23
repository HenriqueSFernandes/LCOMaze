#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  if (lsb == NULL) {
    return 1;
  }
  *lsb = val & 0xFF;
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
  *msb = (val >> 8) & 0xFF;
  return 0;
}

/**
 * @brief This function sends an 8bit value to a port.
 * @param port The communication port.
 * @param lsb A pointer to an 8bit number
 */
int(util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) {
    return 1;
  }
  // The value needs to be casted to 32 bit because sys_inb needs a 32 bit number.
  return (sys_inb(port, (uint32_t *) value));
}
