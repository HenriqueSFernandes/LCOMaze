#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void(timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
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
