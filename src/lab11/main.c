#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/led.h"
#include "../utils/key.h"

extern void key_isr(void);

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

void act1(int num) {
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

void key_handle(void) {
  /* 获取键值 */
  /*volatile unsigned char key_code = EXT_INT_0_PEND & 0x8;  */
  volatile unsigned char key_code = VIC0IRQSTATUS & 0x8;

  /* 清中断向量寄存器 */
  VIC0ADDRESS = 0;

  /* 清中断挂起寄存器 */
  EXT_INT_0_PEND |= 0x8;

  static int step = 0;

  if (step == 0) {
    step = key_code == 0x4 ? step + 1 : 0;
  } else if (step == 1) {
    step = key_code == 0x4 ? step + 1 : 0;
  } else if (step == 2) {
    step = key_code == 0x8 ? step + 1 : 0;
  }

  if (step == 3) {
    act1(10);
    step = 0;
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
