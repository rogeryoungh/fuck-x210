#include "./defs.h"

void led_init() {
  GPJ2CON &= ~0xFFFF;
  GPJ2CON |= 0x1111;
  GPJ2DAT |= B1111;
}
