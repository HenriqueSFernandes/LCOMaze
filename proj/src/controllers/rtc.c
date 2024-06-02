#include "rtc.h"
#include <lcom/lcf.h>
#include <stdint.h>

time_el time_stamp;
int hook_id_rtc = 8;

int set_update() {
    uint32_t inf;
    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_inb(RTC_DATA_REG, &inf);
    inf |= UIE;
    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_outb(RTC_DATA_REG, inf);
    return 0;
}

int clean_update() {
    uint32_t inf;
    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_inb(RTC_DATA_REG, &inf);
    inf &= 0xFF ^ UIE;
    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_outb(RTC_DATA_REG, inf);
    return 0;
}

int subscribe_rtc(uint16_t *bit_no) {
    *bit_no = BIT(hook_id_rtc);
    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id_rtc) != OK) {
        printf("Error in sys_irqsetpolicy()\n");
        return 1;
    }
    return 0;
}

int unsubscribe_rtc() {
    if (sys_irqrmpolicy(&hook_id_rtc) != OK) {
        printf("Error in sys_irqrmpolicy()\n");
        return 1;
    }
    return 0;
}
int update(){
    uint32_t cause;
    sys_outb(RTC_ADDR_REG, RTC_REG_C);
    sys_inb(RTC_DATA_REG, &cause);
    if(cause & UE){
        get_time();
    }
    return 0;
}

int get_time() {
    uint32_t seconds = 0;
    uint32_t minutes = 0;
    uint32_t hours = 0;
    memset(&time_stamp, 0, sizeof(time_stamp));
    // Read hours
    sys_outb(RTC_ADDR_REG, RTC_REG_HOURS);
    sys_inb(RTC_DATA_REG, &hours);
    time_stamp.hours = hours;

    // Read minutes
    sys_outb(RTC_ADDR_REG, RTC_REG_MINUTES);
    sys_inb(RTC_DATA_REG, &minutes);
    time_stamp.minutes = minutes;

    // Read seconds
    sys_outb(RTC_ADDR_REG, RTC_REG_SECONDS);
    sys_inb(RTC_DATA_REG, &seconds);
    time_stamp.seconds = seconds;
    return 0;
}

int get_time_direct(uint32_t *seconds, uint32_t *minutes, uint32_t *hours) {
    // Read hours
    sys_outb(RTC_ADDR_REG, RTC_REG_HOURS);
    sys_inb(RTC_DATA_REG, hours);

    // Read minutes
    sys_outb(RTC_ADDR_REG, RTC_REG_MINUTES);
    sys_inb(RTC_DATA_REG, minutes);

    // Read seconds
    sys_outb(RTC_ADDR_REG, RTC_REG_SECONDS);
    sys_inb(RTC_DATA_REG, seconds);
    return 0;
}
