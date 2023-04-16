#define GPJ2CON (*(volatile unsigned long *)0xE0200280)
#define GPJ2DAT (*(volatile unsigned long *)0xE0200284)
#define GPH0CON *((volatile unsigned long *)0xE0200C00)
#define GPH0DAT *((volatile unsigned long *)0xE0200C04)
// 外中断0-7的中断控制寄存器
#define EXT_INT_0_CON *((volatile unsigned int *)0xE0200E00)
// 外中断0-7的中断屏蔽寄存器
#define EXT_INT_0_MASK *((volatile unsigned int *)0xE0200F00)
// 第一组矢量中断选择寄存器
#define VIC0INTSELECT *((volatile unsigned int *)0xF200000C)
// 第一组矢量中断使能寄存器
#define VIC0IRQSTATUS *((volatile unsigned int *)0xF2000000)
#define VIC0INTENABLE *((volatile unsigned int *)0xF2000010)
#define VIC0VECTADDR2 *((volatile unsigned int *)0xF2000108)
#define VIC0VECTADDR3 *((volatile unsigned int *)0xF200010C)
#define VIC0ADDRESS   *((volatile unsigned int *)0xF2000F00)
// 外部中断0-7的中断挂起寄存器，记录是否有中断产生
#define EXT_INT_0_PEND *((volatile unsigned int *)0xE0200F40)

extern void key_isr(void);

void delay(volatile unsigned int t) {
  volatile unsigned int t2 = 0xFFFF;
  while (t--)
    for (; t2; t2--)
      ;
}

void led_init(void) {
  GPJ2CON &= ~(0xFF << 0);
  GPJ2CON |= ((0x01 << 0) | (0x01 << 4) | (0x01 << 8) | (0x01 << 12));
  GPJ2DAT |= (0xFF << 0);
}

void key_init(void) {
  GPH0CON |= 0xFF << 8;
  /* 配置GPH0_0和GPH0_1为外部中断：key1和key2 */
  EXT_INT_0_CON &= ~(0xFF << 8);
  /* 清空低八位*/
  EXT_INT_0_CON |= (2 << 4) | (2 << 8);
  /* 配置EXT_INT[0]和EXT_INT[1]为下降沿触发 0b010 0 010 */
  EXT_INT_0_MASK &= ~0xC;
  /* 取消屏蔽外部中断EXT_INT[0]和EXT_INT[1] */
}

void int_init(void) {
  VIC0INTSELECT &= ~0xC;
  /* 选择外部中断EXT_INT[0]和外部中断EXT_INT[1]为IRQ类型的中断 */
  VIC0INTENABLE |= 0xC;
  /* 使能外部中断EXT_INT[0]和EXT_INT[1] */
  VIC0VECTADDR2 = (int)key_isr;
  /* 当EXT_INT[0]触发中断，即用户按下key1时，
   * CPU就会自动的将VIC0VECTADDR0的值赋给VIC0ADDRESS并跳转到这个地址去执 行*/
  VIC0VECTADDR3 = (int)key_isr;
  VIC0ADDRESS = 0;
}

void key_handle() {
  volatile unsigned char key_code = VIC0IRQSTATUS & 0xC;
  VIC0ADDRESS = 0;
  /* 清中断向量寄存器 */
  EXT_INT_0_PEND |= 0x3;
  /* 清中断挂起寄存器 */

  if (key_code == 0x04) /* SW4 */
  {
    int num = 10;
    while (num--) {
      GPJ2DAT ^= 1 << 0;
      delay(100);
      GPJ2DAT ^= 1 << 1;
      delay(100);
      GPJ2DAT ^= 1 << 2;
      delay(100);
      GPJ2DAT ^= 1 << 3;
      delay(100);
    }
  }
  /* toggle LED1 */

  else if (key_code == 0x08) /* SW5 */
  {
    int num = 10;
    while (num--) {
      GPJ2DAT ^= 1 << 3;
      delay(100);
      GPJ2DAT ^= 1 << 2;
      delay(100);
      GPJ2DAT ^= 1 << 1;
      delay(100);
      GPJ2DAT ^= 1 << 0;
      delay(100);
    }
  }
  /* toggle LED2 */
}

int main() {
  led_init();
  key_init();
  int_init();

  while (1)
    ;
  return 0;
}
