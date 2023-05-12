#include "../utils/defs.h"
#include "../utils/pwn.h"
#include "../utils/buzzer.h"

extern void key_isr(void);

void init_irq() {
  TINT_CSTAT |= (1 << 2);
}
void clear_irq() {
  TINT_CSTAT |= (1 << 7);
}

void int_init() {
  VIC0INTSELECT |= ~(1 << 23);
  VIC0ADDRESS = 0;
  VIC0VECTADDR23 = (int)key_isr;
  VIC0INTENABLE |= (1 << 23);
}
static int flag = 0;

void key_handle(void) {
  volatile unsigned char status = ((VIC0IRQSTATUS & (1 << 23)) >> 23) & 0x1;
  VIC0ADDRESS = 0;
  clear_irq();
  if (status == 1) {
    if (flag == 0) {
      buzzer_on();
    } else {
      buzzer_off();
    }
    flag ^= 1;
  }
}


int main() {
  pwm_init();
  int_init();
  init_irq();
  buzzer_init();
  while (1) {}
  return 0;
}