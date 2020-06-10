@for zbf
.global _start
_start:
	b       reset	
	ldr	pc, _undefined_instruction	
	ldr	pc, _software_interrupt		
	ldr	pc, _prefetch_abort			
	ldr	pc, _data_abort				
	ldr	pc, _not_used				
	ldr	pc, _irq					
	ldr	pc, _fiq
	
_undefined_instruction: .word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq					

undefined_instruction:
		nop

software_interrupt:
		nop

prefetch_abort:
		nop

data_abort:
		nop

not_used:
		nop

irq:
		nop

fiq:
		nop

reset:
	bl	set_svc
	bl 	disable_watch_dog
	bl      disable_interrupt
	bl	disable_mmu
	bl 	clock_init
	
	ldr	sp, =0xd0030000
	bl	ddr_init
	bl	copy_code_to_dram	
	
	bl	init_stack
	bl 	clean_bss
	ldr     pc, =main

	
@设置工作模式，svc模式
set_svc:
	mrs r0, cpsr     @取出cpsr寄存器
	bic r0,r0,#0x1f @把cpsr寄存器的后5位置0
	orr r0,r0,#0xd3 @赋值0xd3给cpsr
	msr cpsr, r0     @赋值r0到cpsr寄存器中
	mov pc, lr      @返回
	
@关看门狗	
disable_watch_dog:
	ldr r0, =0xE2700000  
	mov r1, #0x0
	str r1, [r0]	
	mov pc, lr
	
@关中断
disable_interrupt:
	mvn r1,#0x0
	ldr r0,=0xf2000014
	str r1,[r0]

	ldr r0,=0xf2100014
	str r1,[r0]

	ldr r0,=0xf2200014
	str r1,[r0]

	ldr r0,=0xf2300014
	str r1,[r0]
	mov pc, lr
	
@关闭mmu
disable_mmu:	
	mcr p15,0,r0,c7,c5,0
	mcr p15,0,r0,c7,c6,1
	mrc p15,0,r0,c1,c0,0
	bic r0, r0, #0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc, lr
	
@时钟初始化
@ARMCLK = 1000MHZ HCLK_MSYS = 200MHZ PCLK_MSYS = 100MHZ HCLK_PSYS = 133MHZ PCLK_PSYS = 66MHZ
	
#define CLK_DIV0  0xE0100300
#define CLK_SRC0  0xE0100200
#define APLL_CON0 0xE0100100
#define MPLL_CON  0xE0100108

#define DIV0_VAL       ((0x0<<0)|(4<<4)|(4<<8)|(1<<12)|(3<<16)|(1<<20)|(4<<24)|(1<<28))
#define APLL_CON0_VAL (3<<8)|(125<<16)|(1<<0)|(1<<31) @1000MHZ
#define MPLL_CON_VAL  (12<<8)|(667<<16)|(1<<0)|(1<<31)@667MHZ
clock_init:
	
	ldr r0, =APLL_CON0  @APLL_CON0
	ldr r1, =APLL_CON0_VAL
	str r1, [r0]
	
	ldr r0, =MPLL_CON  @MPLL_CON
	ldr r1, =MPLL_CON_VAL
	str r1, [r0]
	
	ldr r0, =CLK_DIV0  @CLK_DIV0
	ldr r1, =DIV0_VAL
	str r1, [r0]

	ldr r0, =CLK_SRC0  @CLK_SRC0
	ldr r1, =0x1111
	str r1, [r0]
	
	mov pc, lr
init_stack:
	ldr sp, =0x24000000
	mov pc ,lr	
@清楚bss段
clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	cmp r0, r1
	moveq pc, lr

clean_loop:
	mov r2, #0
	str r2, [r0], #4
	cmp r0, r1
	bne clean_loop
	mov pc, lr
	

	

 
