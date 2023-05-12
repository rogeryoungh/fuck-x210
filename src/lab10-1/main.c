#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/uart.h"
#include "../utils/buzzer.h"
#include "../utils/led.h"

extern void key_isr(void);

void act1(int num) {
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

void act2(int num) {
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

void key_handle() {
  volatile unsigned char key_code = EXT_INT_0_PEND & 0xC;
  VIC0ADDRESS = 0;

  EXT_INT_0_PEND |= 0xC;

  if (key_code == 0x04) { /* SW4 */
    uart_send_string("\r\nSW4 PRESSED\r\n");
    act1(10);
  } else if (key_code == 0x08) { /* SW5 */
    uart_send_string("\r\nSW5 PRESSED\r\n");
    act2(10);
  }

  /* toggle LED1 */
  else if (key_code == 0x08) {
    int num = 10;
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
  uart_init();
  led_init();
  key_init();
  int_init();
  uart_send_string("\r\nUART Test in S5PV210\r\n");

#define STR1 "SW4+5T"
#define STR2 "SW4+10T"
  int p1 = 0, p2 = 0;
  while (1) {
    char byte = uart_recv_byte();
    p1 = (byte == STR1[p1]) ? p1 + 1 : 0;
    p2 = (byte == STR2[p2]) ? p2 + 1 : 0;
    if (p1 == sizeof(STR1) - 1) {
      act1(5);
      p1 = 0;
    } else if (p2 == sizeof(STR2) - 1) {
      act2(10);
      p2 = 0;
    }
  }

  return 0;
}
