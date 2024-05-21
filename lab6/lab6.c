#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define bit(n) ( 1 << n )

int main(int argc, char **argv) {
	printf("start \n");
	//sys_outb(0x70,10);
	uint32_t res=0;
	//sys_inb(0x71,&res);
	printf("REGISTER IN A IS %x",res);
	printf("hello \n");
	return 0;
}

