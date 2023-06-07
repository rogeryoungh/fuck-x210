#include "./defs.h"

void led_init() {
  // 清空低 4 位
  GPJ2CON &= ~0xFFFF;
  // 设置低 4 位为 Output
  GPJ2CON |=  0x1111;
  // 亮灯
  GPJ2DAT |=  B1111;
}
