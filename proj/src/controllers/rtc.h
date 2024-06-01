#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <math.h>

struct time_el {
    uint32_t  seconds;
    uint32_t minutes;
   uint32_t hours;
};
#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
int activate_interrupt_mode();
int deactivate_interrupt_mode();
int subscribe_rtc(uint16_t *bit_no);
int unsubscribe_rtc();
int get_time();
int update();
#endif

