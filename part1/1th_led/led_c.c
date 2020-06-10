#define	CPC0CON	(*(volatile unsigned long *)0xE0200060)
#define	CPC0DAT	(*(volatile unsigned long *)0xE0200064)

void delay()
{
	unsigned long j;
	for(j=0;j<300000;j++){}
}
int main()
{
	int i;
	CPC0CON = 0x00011000;//设置CPC0_4,CPC0_3为输出
	while(1)
	{
		for(i=0;i<4;i++)
		{
			CPC0DAT = ((i/2)<<4)|((i%2)<<3);//2进制变化
			delay();
		}
	}
	return 0;
}
