#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/led.h"
#include "../utils/key.h"

extern void key_isr(void);

void int_init(void) {
  /* ѡ���ⲿ�ж�EXT_INT[3]ΪIRQ���͵��ж� */
  VIC0INTSELECT &= ~0x8;
  /* ʹ���ⲿ�ж�EXT_INT[3] */
  VIC0INTENABLE |= 0x8;
  /* ��VIC0ADDRESS */
  VIC0ADDRESS = 0X0;
  /* ��EXT_INT[0]�����жϣ����û�����key1ʱ��CPU�ͻ��Զ��Ľ�VIC0VECTADDR0��ֵ����VIC0ADDRESS����ת�������ַȥִ��*/
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
  /* ��ȡ��ֵ */
  /*volatile unsigned char key_code = EXT_INT_0_PEND & 0x8;  */
  volatile unsigned char key_code = VIC0IRQSTATUS & 0x8;

  /* ���ж������Ĵ��� */
  VIC0ADDRESS = 0;

  /* ���жϹ���Ĵ��� */
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
