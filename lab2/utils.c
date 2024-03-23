#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  // Check if the pointer is null.
  if (lsb == NULL)
    return 1;

  // Extract the LSB from val.
  *lsb = val & 0xFF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  // Check if the pointer is null.
  if (msb == NULL)
    return 1;

  // Extract the MSB from val.
  *msb = (val >> 8) & 0xFF;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  // Check if the pointer is null.
  if (value == NULL)
    return 1;

  // Create a 32 bit variable to hold the result of sys_inb.
  uint32_t newValue;
  int ret = sys_inb(port, &newValue);
  // Extract the LSB.
  *value = 0xFF & newValue;
  return ret;
}
