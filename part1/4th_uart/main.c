#define	CPC0CON	(*(volatile unsigned long *)0xE0200060)
#define	CPC0DAT	(*(volatile unsigned long *)0xE0200064)
#include "uart.h"
void delay()
{
	unsigned long j;
	for(j=0;j<300000;j++){}
}

int main()
{
	char flag;
	CPC0CON = 0x00011000;//ÉèÖÃCPC0_4,CPC0_3ÎªÊä³ö
	uart_init();
	while(1)
	{
		put_string("\r\n/****welcome to zbf_bootloader****/\r\n");
		put_string("\rEnter 0 or 1 to test led:");
		flag = getc();
		putc(flag);putc('\n');
		if(flag == '0')
			CPC0DAT = 0x10;
		if(flag == '1')
			CPC0DAT = 0x08;
	}
	return 0;
}
