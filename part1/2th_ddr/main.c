#define	CPC0CON	(*(volatile unsigned long *)0xE0200060)
#define	CPC0DAT	(*(volatile unsigned long *)0xE0200064)

void delay()
{
	unsigned long j;
	for(j=0;j<300000;j++){}
}
int main()
{
	CPC0CON = 0x00011000;//设置CPC0_4,CPC0_3为输出
	while(1)
	{
		CPC0DAT = 0x10;//流水灯
		delay();
		CPC0DAT = 0x08;
		delay();
	}
	return 0;
}
