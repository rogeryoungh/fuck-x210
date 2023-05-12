#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/led.h"

extern void key_isr(void);

void key_init(void) {
  GPH0CON |= 0xFF00;
  /* ����GPH0_0��GPH0_1Ϊ�ⲿ�жϣ�key1��key2 */
  EXT_INT_0_CON &= ~0xFF00;
  /* ��յͰ�λ*/
  EXT_INT_0_CON |= 0x220;
  /* ����EXT_INT[0]��EXT_INT[1]Ϊ�½��ش��� 0b010 0 010 */
  EXT_INT_0_MASK &= ~0xC;
  /* ȡ�������ⲿ�ж�EXT_INT[0]��EXT_INT[1] */
}

void int_init(void) {
  VIC0INTSELECT &= ~0xC;
  /* ѡ���ⲿ�ж�EXT_INT[0]���ⲿ�ж�EXT_INT[1]ΪIRQ���͵��ж� */
  VIC0INTENABLE |= 0xC;
  /* ʹ���ⲿ�ж�EXT_INT[0]��EXT_INT[1] */
  VIC0VECTADDR2 = (int)key_isr;
  /* ��EXT_INT[0]�����жϣ����û�����key1ʱ��
   * CPU�ͻ��Զ��Ľ�VIC0VECTADDR0��ֵ����VIC0ADDRESS����ת�������ַȥִ ��*/
  VIC0VECTADDR3 = (int)key_isr;
  VIC0ADDRESS = 0;
}

void key_handle() {
  volatile unsigned char key_code = VIC0IRQSTATUS & 0xC;
  VIC0ADDRESS = 0;
  /* ���ж������Ĵ��� */
  EXT_INT_0_PEND |= 0x3;
  /* ���жϹ���Ĵ��� */

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
