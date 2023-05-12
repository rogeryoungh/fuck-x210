#include "../utils/defs.h"
#include "../utils/delay.h"

extern void key_isr(void);

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

  static int led = 0;

  led ^= key_code >> 2;

  GPJ2DAT ^= led;

  delay(100);

  GPJ2DAT ^= led;
}

int main() {
  led_init();
  key_init();
  int_init();

  while (1)
    ;
  return 0;
}
