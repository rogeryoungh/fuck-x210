#include "./defs.h"

void key_init() {
  // 配置 GPIO 为外部中断 EXT_INT
  GPH0CON |=  0xFFF00;
  GPH1CON |=   0xF0FF;
  // 清空相应位
  EXT_INT_0_CON &= ~0xFFF00;
  EXT_INT_1_CON &=  ~0xF0FF;
  // 配置外部中断位下降沿触发
  EXT_INT_0_CON |=  0x22200;
  EXT_INT_1_CON |=   0x2022;
  // 取消屏蔽外部中断
//   EXT_INT_0_MASK &= 0b1011'00011100;
  EXT_INT_0_MASK &= ~B16(B0000, B1011, B0001, B1100);
}
