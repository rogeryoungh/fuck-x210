#include "./defs.h"

void led_init() {
  GPJ2CON &= ~0xFF;
  GPJ2CON |= 0x1111;
  GPJ2DAT |= 0xFF;
}
