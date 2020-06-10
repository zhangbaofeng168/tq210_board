///**lcd初始化**////
#define  GPF0CON (*(volatile unsigned long *)0xE0200120)
#define  GPF1CON (*(volatile unsigned long *)0xE0200140)
#define  GPF2CON (*(volatile unsigned long *)0xE0200160)
#define  GPF3CON (*(volatile unsigned long *)0xE0200180)

#define  GPD0CON (*(volatile unsigned long *)0xE02000A0)
#define  GPD0DAT (*(volatile unsigned long *)0xE02000A4)

#define  DISPLAY_CONTROL  (*(volatile unsigned long *)0xE0107008)	// 00000002

#define  VIDCON0 (*(volatile unsigned long *)0xF8000000)
#define  VIDCON1 (*(volatile unsigned long *)0xF8000004)
#define  VIDCON2 (*(volatile unsigned long *)0xF8000008)
#define  VIDCON3 (*(volatile unsigned long *)0xF800000C)

#define  VIDTCON0 (*(volatile unsigned long *)0xF8000010)
#define  VIDTCON1 (*(volatile unsigned long *)0xF8000014)
#define  VIDTCON2 (*(volatile unsigned long *)0xF8000018)
#define  VIDTCON3 (*(volatile unsigned long *)0xF800001C)

#define  WINCON0  (*(volatile unsigned long *)0xF8000020)
#define  VIDOSD0A  (*(volatile unsigned long *)0xF8000040)
#define  VIDOSD0B  (*(volatile unsigned long *)0xF8000044)
#define  VIDOSD0C  (*(volatile unsigned long *)0xF8000048)

#define  VIDW00ADD0B0  (*(volatile unsigned long *)0xF80000A0)
#define  VIDW00ADD1B0 (*(volatile unsigned long *)0xF80000D0)

#define SHODOWCON   (*(volatile unsigned long *)0xF8000034)	// 00000001

#define VBPD (23) //垂直同步信号后肩Vsync back-porch
#define VFPD (22) //垂直同步信号前肩Vsync front-porch
#define VSPW (2) //垂直同步信号脉宽Vsync pulse width

#define HBPD (46) //水平同步信号后肩Hsync back-porch
#define HFPD (210) //水平同步信号前肩Hsync front-porch
#define HSPW (2) //水平同步信号脉宽Hsync pulse width

#define WIDTH 800
#define HEIGHT  480
#define BPP		24

static unsigned int bytes_per_line;
static unsigned int  frame_buffer_size;
static unsigned int  frame_buffer_addr = 0x28000000;
void init_lcd()
{
	//1.配置lcd相应的引脚
	GPF0CON = 0x22222222;
	GPF1CON = 0x22222222;
	GPF2CON = 0x22222222;
	GPF3CON = 0x2222;
	
	GPD0CON  = (GPD0CON & ~0xF) | (0x1 << 0); //设置电源控制引脚为输出
	
	DISPLAY_CONTROL = (2 << 0);
	
	bytes_per_line = WIDTH;
	if (BPP > 16)
		bytes_per_line *= 32;
	else
		bytes_per_line *= BPP;
	bytes_per_line /= 8;
	
	frame_buffer_size = bytes_per_line * HEIGHT;
	
	VIDCON0 =(5<<6)|(1<<4)|(0<<2);  //平行显示 VCLK =33.3MHZ  时钟源 HCLK bit0,1用来使能开关
	VIDCON1 =(0<<7)|(1<<6)|(1<<5)|(0 << 4);//Video data is fetched at VCLK rising edge 
	/* Specifies output data format control */
	VIDCON2  = 	(0b000 << 19) |			/* RGB interface output order(Even line, line #2,4,6,8):RGB */
				(0b000 << 16);			/* RGB interface output order(Odd line, line #1,3,5,7):RGB */
	
	VIDTCON0 =	(VBPD << 16) |			/* VBPD */
				(VFPD << 8) |			/* VFPD */
				(VSPW << 0);			/* VSPW */
	VIDTCON1 =	(HBPD << 16) |			/* HBPD */
				(HFPD << 8) |			/* HFPD */
				(HSPW << 0);			/* HSPW */
	
	VIDTCON2 =((HEIGHT - 1)<<11)|(WIDTH-1);;//800*480
	VIDTCON3 =	(0b1 << 31);			/* Enables VSYNC Signal Output */
	
	WINCON0 &= ~(0xf << 2);
	WINCON0 |=	(0 << 30) |				/* BUFSEL_H = 0 */
				(0 << 20) |				/* BUFSEL_L = 0, BUFSEL=0b00(Selects the Buffer set 0) */
				(1 << 15) |				/* the Word swap Enable */
				(0xB << 2);				/* Unpacked 24 bpp ( non-palletized R:8-G:8-B:8 ) */
	
	VIDOSD0A = 0x0;
	VIDOSD0B =((WIDTH - 1)<<11)|(HEIGHT-1);
	/* Specifies the Window Size:Height * Width (Number of Word) */
	VIDOSD0C = frame_buffer_size / 4; //800*480*24/8
	
	/* the start address for Video frame buffer */
	VIDW00ADD0B0 =  frame_buffer_addr;
	VIDW00ADD1B0 =  VIDW00ADD0B0 + frame_buffer_size;
	
	SHODOWCON = 0x1;	/* Enables Channel 0 */
	GPD0DAT |= 1;
	VIDCON0 |= 0x3;
	WINCON0 |= 1;
}

void draw_pixel(int x, int y, unsigned int color)
{
	unsigned int *p = (unsigned int *)frame_buffer_addr;
	*(p + y * bytes_per_line / 4 + x) = color;
}

void clear_screen()
{
	int x, y;
	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
			draw_pixel(x, y, 0);
	}
}

void draw_line(int x0, int y0, int x1, int y1, unsigned int color)
{
	int x, y, dx, dy, Dx, Dy, e, i;
	Dx = x1 - x0;
	Dy = y1 - y0;

	dx = x1 - x0;
	if (dx < 0)
		dx *= -1;
	dy = y1 - y0;
	if (dy < 0)
		dy *= -1;
	x = x0;
	y = y0;
	if(dy > dx)
	{
		e = -dy;
		for(i = 0; i < dy; i++)
		{
			draw_pixel(x, y, color);
			if(Dy >= 0)
				y++;  
			else
				y--;   
			e += 2 * dx;
			if(e >= 0)
			{
				if(Dx >= 0)
					x++;
				else
					x--; 
				e -= 2 * dy;
			}
		}
	}
	else
	{
		e = -dx;
		for(i = 0; i < dx; i++)
		{
			draw_pixel(x, y, color);
			if(Dx >= 0)
				x++;
			else
				x--;
			e += 2 * dy;
			if(e >= 0)
			{
				if(Dy >= 0)
					y++;
				else
					y--;
				e -= 2 * dx;
			}
		}
	}
}

void draw_circle(int x, int y, int r, unsigned int color)
{
	int a, b, num; 
	a = 0; 
	b = r; 
	while(2 * b * b >= r * r) 
	{ 
		draw_pixel(x + a, y - b,color); 
		draw_pixel(x - a, y - b,color); 
		draw_pixel(x - a, y + b,color); 
		draw_pixel(x + a, y + b,color); 
		draw_pixel(x + b, y + a,color); 
		draw_pixel(x + b, y - a,color); 
		draw_pixel(x - b, y - a,color); 
		draw_pixel(x - b, y + a,color); 
		a++; 
		num = (a * a + b * b) - r*r; 
		if(num > 0) 
		{ 
			b--; 
			a--; 
		} 
	} 
}