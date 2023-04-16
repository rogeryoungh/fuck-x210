#include "../utils/gpio.h"
#include "../utils/uart.h"

extern void key_isr(void);
extern void uart_send_byte(unsigned char byte);
extern unsigned char uart_recv_byte();
extern void uart_send_string(char *str);
void delay(volatile unsigned int t) {
  volatile unsigned int t2 = 0xFFFF;
  while (t--)
    for (; t2; t2--)
      ;
}

void buzzer_init(void) {
  GPD0CON |= 1 << 8;
}

void buzzer_on(void) {
  GPD0DAT |= 1 << 2;
}

void buzzer_off(void) {
  GPD0DAT &= ~(1 << 2);
}

void key_handle() {
  volatile unsigned char key_code = EXT_INT_0_PEND & 0xC;
  VIC0ADDRESS = 0;

  EXT_INT_0_PEND |= 0xC;

  if (key_code == 0x04) /* SW4 */
  {
    int num = 10;
    uart_send_string("\r\nSW4 PRESSED\r\n");
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
    uart_send_string("\r\nSW5 PRESSED\r\n");
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

int main() {
  char byte;

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
  while (1)
    ;

  return 0;
}
