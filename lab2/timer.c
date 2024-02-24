#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ)
    return 1;
  uint16_t count = TIMER_FREQ / freq; // It is a 16 bit counter
  // lets calculate the lsb and msb of the counter that we want to set in the timer
  uint8_t count_lsb;
  uint8_t count_msb;
  if (util_get_LSB(count, &count_lsb)) {
    return 1;
  }
  if (util_get_MSB(count, &count_msb))
    return 1;
  // lets get the current config as there is stuff we dont want to change aka 4 last bits configuration mode and bcd
  uint8_t current_config;
  if (timer_get_conf(timer, &current_config))
    ;
  uint8_t warning = (timer << 6) | TIMER_LSB_MSB | (current_config & 0x0F);
  // Lets warn the control that we want to set the counter
  if (sys_outb(TIMER_CTRL, warning))
    return 1;
  // now lets load the lsb and then the msb into the counter
  if (sys_outb(TIMER_0 + timer, count_lsb))
    return 1;
  if (sys_outb(TIMER_0 + timer, count_msb))
    return 1;
  return 0;
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

  uint8_t cw = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); // cw = 1110xxx0, x represents the timer.

  // Sends the control word to the timer controller.
  if (sys_outb(TIMER_CTRL, cw)) {
    return 1;
  }
  // Reads the config for the specified timer (the config is located at the timer that was specified in the control word).
  if (util_sys_inb(TIMER_0 + timer, st)) {
    return 1;
  }
  return 0;
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
      st = (st & TIMER_LSB_MSB) >> 4;
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
      st = (st & (BIT(3) | BIT(2) | BIT(1)) >> 1);
      // on mode 2 and 3, the 3rd bit doesn't matter, so we should also consider when it is 1.
      if (st > 5) {
        st = st & (BIT(0) | BIT(1));
      }
      data.count_mode = st;
      break;

    case tsf_base:
      // BCD

      data.bcd = st & TIMER_BCD;
      break;

    default:
      return 1;
  }

  if (timer_print_config(timer, field, data)) {
    return 1;
  }
  return 0;
}
