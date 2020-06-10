
#include "lib.h"


#define GPH0CON         (*(volatile unsigned long *)0xE0200C00)
#define GPH0DAT         (*(volatile unsigned long *)0xE0200C04)

#define EXT_INT_0_CON   (*(volatile unsigned long *)0xE0200E00)
#define EXT_INT_0_MASK  (*(volatile unsigned long *)0xE0200F00)

#define VIC0_INTENABLE  (*(volatile unsigned long *)0xF2000010)
#define VIC0_SOFTINT    (*(volatile unsigned long *)0xF2000018)
#define VIC0_IRQSTATUS  (*(volatile unsigned long *)0xF2000000)
#define VIC0_INTSELECT  (*(volatile unsigned long *)0xF200000C)
#define VIC0_INTENCLEAR (*(volatile unsigned long *)0xF2000014)
#define VIC0_ADDRESS  	(*(volatile unsigned int *)0xF2000F00)
#define EXT_INT_0_PEND  (*(volatile unsigned int *)0xE0200F40)

int i=0;
void do_irq(void)
{
	/* 清中断 */
	EXT_INT_0_PEND |= 0xf;
	/* 清中断向量 */
	VIC0_ADDRESS = 0;

	if(!(GPH0DAT & (1<<1)))
	{
		printf("counter(k4) : %d \n",i++);
	}
	if(!(GPH0DAT & (1<<0)))
	{
		printf("counter(K5) : %d \n",i--);
	}
}

extern void key_IRQ(void);
void init_key()
{
	//1.引脚配置
	GPH0CON |=0xffff; //EXINT[0,3]
	EXT_INT_0_CON |=(2<<0)|(2<<4)|(2<<8)|(2<<12);//下降沿触发
	EXT_INT_0_MASK &= 0xfffffff0;   //使能引脚中断
	//2.中断配置
	VIC0_INTSELECT   &= 0xfffffff0;  //irq中断
	VIC0_INTENABLE   |=0xf; //使能中断
	
	VIC0_ADDRESS      = (int)key_IRQ;
}