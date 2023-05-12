#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/uart.h"
#include "../utils/buzzer.h"

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

int main() {

  GPJ2CON &= ~(0xFF << 0);
  GPJ2CON |= ((0x01 << 0) | (0x01 << 4) | (0x01 << 8) |
              (0x01 << 12)); //((0x01 << 0) | (0x01 << 4));
  GPJ2DAT |= (0xFF << 0);
  GPH0CON |= 0xFF << 8;
  EXT_INT_0_CON &= ~(0xFF << 8);
  EXT_INT_0_CON |= (2 << 8) | (2 << 12);
  EXT_INT_0_MASK &= ~0xC;
  uart_send_string("\r\nUART Test in S5PV210\r\n");

  VIC0INTSELECT &= ~0xC;
  VIC0INTENABLE |= 0xC;
  VIC0VECTADDR2 = (int)key_isr;
  VIC0VECTADDR3 = (int)key_isr;


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
