#include "../utils/defs.h"
#include "../utils/buzzer.h"
#include "../utils/delay.h"
#include "../utils/led.h"

extern void key_isr(void);

void pwm_init() { // ��Ӷ�ʱ�� 2 ��ʼ�����룬ʹ�� PWM ����Ϊ 10s��ռ�ձ�Ϊ 50 %
  //
  // TCNTB2 = 631620;
  // TCMPB2 = 315810;
  // TCFG0 |= (65 << 8);
  // TCFG1 = (0x04 << 8);
  // TCON |= (1 << 13);
  // TCON = (1 << 15);
  GPD0CON &= ~(0xf << 8);
  GPD0CON |= (2 << 8); // ���ö�ʱ��2���
  TCFG0 &= ~(0xff << 8);
  TCFG0 |= (65 << 8); // Ԥ��ƵֵΪ65
  TCFG1 &= ~(0xf << 8);
  TCFG1 |= (4 << 8); // ������Ϊ��ʱ��2����Ԥ��Ƶֵ
  TCON |= (1 << 15); // �Զ�����
  TCNTB2 = 62500;
  TCMPB2 = 31250;

  TCON |= (1 << 13);  // �ֶ�����TCNTB2��TCMPB2
  TCON &= ~(1 << 13); // ���ֶ�����λ
  TCON |= (1 << 12);  // ������ʱ��2
}

void init_irq() {
  TINT_CSTAT |= (1 << 2);
}
void clear_irq() {
  TINT_CSTAT |= (1 << 7);
}
void key_init(void) {
  // ����2��4��8Ϊ�ⲿ�жϣ��ֱ�ΪSW467
  GPH0CON |= 0xFFF00;
  /* �����Ӧλ*/
  EXT_INT_0_CON &= ~0xFFF00;
  /* ����EXT_INT[2,3,4,8]Ϊ�½��ش��� 0b0 010 0 010 */
  EXT_INT_0_CON |= 0x22200;

  /* ȡ�������ⲿ�ж�EXT_INT[3] */
  EXT_INT_0_MASK &= ~0x1C;
}

void int_init(void) {
  VIC0INTSELECT &= ~(0x1C | (1 << 23));
  VIC0INTENABLE |= (0x1C | (1 << 23));
  VIC0ADDRESS = 0;
  VIC0VECTADDR2 = (int)key_isr;
  VIC0VECTADDR3 = (int)key_isr;
  VIC0VECTADDR4 = (int)key_isr;
  VIC0VECTADDR23 = (int)key_isr;
}

int sw4_1 = -1, sw5_1 = -1;
int flag = 0, x2 = 0;

void key_handle() {
  volatile unsigned int key_code = VIC0IRQSTATUS;
  VIC0ADDRESS = 0;
  EXT_INT_0_PEND |= 0x1C;
  clear_irq();
  if (key_code & 0x4) { // sw4
    sw4_1 = 0;
  } else if (key_code & 0x8) { // sw5
    sw5_1 = 0;
  } else if (key_code & 0x10) {
    flag ^= 1;
  } else if (key_code & (1 << 23)) {
    int exec = 0;
    if (flag == 0) {
      exec = 1;
    } else {
      x2 = 1 - x2;
      exec = x2;
    }

    if (exec) {
      if (sw4_1 >= 0) {
        if (sw4_1 % 2 == 0) {
          buzzer_on();
        } else {
          buzzer_off();
        }
        sw4_1 += 1;
        if (sw4_1 >= 10) {
          sw4_1 = -1;
        }
      }
      if (sw5_1 >= 0) {
        GPJ2DAT ^= 1 << (sw5_1 & 0x3);
        if (sw5_1 >= 16) {
          sw5_1 = -1;
        }
      }
    }
  }
}

int main() {
  sw4_1 = -1, sw5_1 = -1;
  flag = 0, x2 = 0;
  buzzer_init();
  led_init();
  key_init();
  int_init();
  init_irq();
  while (1)
    ;
  return 0;
}
