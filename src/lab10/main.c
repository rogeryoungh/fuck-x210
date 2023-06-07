#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/buzzer.h"
#include "../utils/uart.h"
#include "../utils/led.h"

extern void key_isr(void);

void key_handle() {
  volatile unsigned char key_code = EXT_INT_0_PEND & 0xC;
  VIC0ADDRESS = 0;

  EXT_INT_0_PEND |= 0xC;

  if (key_code == 0x04) /* SW4 */
  {
    int num = 10;
    uart0_send_string("\r\nSW4 PRESSED\r\n");
    while (num--) {
      GPJ2DAT ^= 1 << 0;
      delay(100);
      GPJ2DAT ^= 1 << 1;
      delay(100);
      GPJ2DAT ^= 1 << 2;
      delay(100);
      GPJ2DAT ^= 1 << 3;
      delay(100);
    }

  }

  /* toggle LED1 */
  else if (key_code == 0x08) /* SW5 */
  {
    int num = 10;
    uart0_send_string("\r\nSW5 PRESSED\r\n");
    while (num--) {
      GPJ2DAT ^= 1 << 3;
      delay(100);
      GPJ2DAT ^= 1 << 2;
      delay(100);
      GPJ2DAT ^= 1 << 1;
      delay(100);
      GPJ2DAT ^= 1 << 0;
      delay(100);
    }
  }
  /* toggle LED2 */
}

void key_init() {
  GPH0CON |= 0xFF00;
  EXT_INT_0_CON &= ~0xFF00;
  EXT_INT_0_CON |= 0x2200;
  EXT_INT_0_MASK &= ~0xC;
}

void int_init() {
  VIC0INTSELECT &= ~0xC;
  VIC0INTENABLE |= 0xC;
  VIC0VECTADDR2 = (int)key_isr;
  VIC0VECTADDR3 = (int)key_isr;
  VIC0ADDRESS = 0X0;
}

int main() {
  uart0_init();
  led_init();
  key_init();
  int_init();
  uart0_send_string("\r\nUART Test in S5PV210\r\n");

  while (1)
    ;

  return 0;
}
