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
  /* To be implemented by the students */
  // This sends a control word to port 43 (timer config port) and gets the
  uint8_t cw = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1);
  if (sys_outb(TIMER_CTRL, cw)) {
    return 1;
  }
  if (util_sys_inb(0x40 + timer, st)) {
    return 1;
  }
  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  // union timer_status_field_val data;
  // switch (field){
  //   case tsf_all:
  //     data.byte = st;
  //   break;
  //   case tsf_initial:
  //     data.in_mode = st &
  //   case tsf_mode:
  //   case tsf_base:
  //   default:
  //     return 1;
  // }

  return 1;
}
