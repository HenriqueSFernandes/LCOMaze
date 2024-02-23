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

/**
 * @brief This sends a control word to port 43 (timer config port) and gets the config for the specified timer. Refer to 3.3 for more information
 * @param timer 8 bit number representing which timer to use (0, 1 or 2).
 * @param st Pointer to store the status code (the resulting config).
 */
int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  // This sends a control word to port 43 (timer config port) and gets the config for the specified timer. Refer to 3.3 for more information.

  // Validate input
  if (st == NULL || timer < 0 || timer > 2) {
    return 1;
  }

  uint8_t cw = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1); // cw = 1110xxx0, x represents the timer.

  // Sends the control word to the timer controller.
  if (sys_outb(TIMER_CTRL, cw)) {
    return 1;
  }
  // Reads the config for the specified timer (the config is located at the timer that was specified in the control word).
  return (util_sys_inb(0x40 + timer, st));
}

/**
 * @brief Displays the config for the timer.
 * @param timer The timer to display the config.
 * @param st The status of the timer.
 * @param field The field that should be returned.
 */
int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val data;
  switch (field) {
    case tsf_all:
      // Full status byte.
      data.byte = st;
      break;

    case tsf_initial:
      // Initialization mode.
      st = st & (BIT(5) | BIT(4) >> 4);
      if (st == 1)
        data.in_mode = LSB_only;
      else if (st == 2)
        data.in_mode = MSB_only;
      else if (st == 3)
        data.in_mode = MSB_after_LSB;
      else
        data.in_mode = INVAL_val;
      break;

    case tsf_mode:
      // Counting mode.

      // Extracts bit 3, 2 and 1
      st = st & (BIT(3) | BIT(2) | BIT(1) >> 1);

      // on mode 2 and 3 the 3rd bit doesn't matter, so we should also consider when it is 1.
      if (st == 6) {
        data.count_mode = 2;
      }
      else if (st == 7) {
        data.count_mode = 3;
      }
      else {
        data.count_mode = st;
      }
      break;

    case tsf_base:
      // BCD

      data.bcd = st & 0x01;
      break;

    default:
      return 1;
  }

  return timer_print_config(timer, field, data);

  return 1;
}
