#define GPJ2CON (*(volatile unsigned long *)0xE0200280)
#define GPJ2DAT (*(volatile unsigned long *)0xE0200284)
#define GPH0CON (*(volatile unsigned long *)0xE0200C00)
#define GPH0DAT (*(volatile unsigned long *)0xE0200C04)
// 外中断0-7的中断控制寄存器
#define EXT_INT_0_CON *((volatile unsigned int *)0xE0200E00)
// 外中断0-7的中断屏蔽寄存器
#define EXT_INT_0_MASK *((volatile unsigned int *)0xE0200F00)
// 外部中断0-7的中断挂起寄存器，记录是否有中断产生
#define EXT_INT_0_PEND *((volatile unsigned int *)0xE0200F40)

// 第一组矢量中断选择寄存器
#define VIC0INTSELECT *((volatile unsigned int *)0xF200000C)
// 第一组矢量中断使能寄存器
#define VIC0IRQSTATUS *((volatile unsigned int *)0xF2000000)
#define VIC0INTENABLE *((volatile unsigned int *)0xF2000010)
#define VIC0VECTADDR2 *((volatile unsigned int *)0xF2000108)
#define VIC0VECTADDR3 *((volatile unsigned int *)0xF200010C)
#define VIC0ADDRESS   *((volatile unsigned int *)0xF2000F00)

extern void key_isr(void);

void led_init(void) {
  /* 配置GPC0_1~GPC0_4为输出：LED1~LED4 */
  GPJ2CON &= ~(0xFF << 0);
  GPJ2CON |= ((0x01 << 0) | (0x01 << 4) | (0x01 << 8) | (0x01 << 12));
  GPJ2DAT |= (0xFF << 0);
}

void key_init(void) {
  /* 配置GPH0_3为外部中断：SW5 */
  GPH0CON |= 0xF << 4 * 3;
  /* 清空相应位*/
  EXT_INT_0_CON &= ~(0xF << 4 * 3);
  /* 配置EXT_INT[0]和EXT_INT[1]为下降沿触发 0b0 010 0 010 */
  EXT_INT_0_CON |= (2 << 4 * 3);
  /* 取消屏蔽外部中断EXT_INT[3] */
  EXT_INT_0_MASK &= ~0x8;
}

void int_init(void) {
  /* 选择外部中断EXT_INT[3]为IRQ类型的中断 */
  VIC0INTSELECT &= ~0x8;
  /* 使能外部中断EXT_INT[3] */
  VIC0INTENABLE |= 0x8;
  /* 清VIC0ADDRESS */
  VIC0ADDRESS = 0X0;
  /* 当EXT_INT[0]触发中断，即用户按下key1时，CPU就会自动的将VIC0VECTADDR0的值赋给VIC0ADDRESS并跳转到这个地址去执行*/
  VIC0VECTADDR3 = (int)key_isr;
}

void key_handle(void) {
  /* 获取键值 */
  /*volatile unsigned char key_code = EXT_INT_0_PEND & 0x8;  */
  volatile unsigned char key_code = VIC0IRQSTATUS & 0x8;

  /* 清中断向量寄存器 */
  VIC0ADDRESS = 0;

  /* 清中断挂起寄存器 */
  EXT_INT_0_PEND |= 0x8;

  if (key_code == 0x8) /* SW5 */
  {
    GPJ2DAT ^= 0xF;
  }
}

int main() {
  key_init();
  led_init();
  int_init();
  while (1)
    ;
  return 0;
}
