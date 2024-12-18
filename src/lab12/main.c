#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/led.h"
#include "../utils/key.h"

extern void key_isr(void);

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
