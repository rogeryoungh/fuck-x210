#include "./defs.h"

void buzzer_init(void) {
  GPD0CON |= 1 << 8;
}

void buzzer_on(void) {
  GPD0DAT |= 1 << 2;
}

void buzzer_off(void) {
  GPD0DAT &= ~(1 << 2);
}
