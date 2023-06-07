#include "./defs.h"

void key_init() {
  // ���� GPIO Ϊ�ⲿ�ж� EXT_INT
  GPH0CON |=  0xFFF00;
  GPH1CON |=   0xF0FF;
  // �����Ӧλ
  EXT_INT_0_CON &= ~0xFFF00;
  EXT_INT_1_CON &=  ~0xF0FF;
  // �����ⲿ�ж�λ�½��ش���
  EXT_INT_0_CON |=  0x22200;
  EXT_INT_1_CON |=   0x2022;
  // ȡ�������ⲿ�ж�
//   EXT_INT_0_MASK &= 0b1011'00011100;
  EXT_INT_0_MASK &= ~B16(B0000, B1011, B0001, B1100);
}
