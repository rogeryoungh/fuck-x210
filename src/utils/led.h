#include "./defs.h"

void led_init() {
  // ��յ� 4 λ
  GPJ2CON &= ~0xFFFF;
  // ���õ� 4 λΪ Output
  GPJ2CON |=  0x1111;
  // ����
  GPJ2DAT |=  B1111;
}
