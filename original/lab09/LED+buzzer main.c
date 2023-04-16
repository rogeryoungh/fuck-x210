#define GPJ2CON (*(volatile unsigned long *)0xE0200280)
#define GPJ2DAT (*(volatile unsigned long *)0xE0200284)
#define GPD0CON (*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT (*(volatile unsigned long *)0xE02000A4)
#define GPC1CON (*((volatile unsigned long *)0xE0200060))
#define GPC1DAT (*((volatile unsigned long *)0xE0200064))
#define GPC1PUD (*((volatile unsigned long *)0xE0200068))
#define LEDS    1

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

int main() {
  buzzer_init();
  char byte;

  GPJ2CON &= ~(0xFF << 0);
  GPJ2CON |= ((0x01 << 0) | (0x01 << 4) | (0x01 << 8) |
              (0x01 << 12)); //((0x01 << 0) | (0x01 << 4));
  GPJ2DAT |= (0xFF << 0);

  uart_send_string("\r\nUART Test in S5PV210\r\n");

  while (1) {
    uart_send_string("\r\n1.LED1 Toggle\r\n");
    uart_send_string("\r\n2.LED2 Toggle\r\n");
    uart_send_string("\r\nPlease select 1 or 2 to Toggle the LED\r\n");
    byte = uart_recv_byte();
    uart_send_byte(byte);

    if (byte == '1') {
      GPJ2DAT ^= 1 << 3;
      delay(1000);
      GPJ2DAT ^= 1 << 2;
      delay(1000);
      GPJ2DAT ^= 1 << 1;
      delay(1000);
      GPJ2DAT ^= 1 << 0;
      delay(2000);
      GPJ2DAT = 0xFF;
    }

    else if (byte == '2') {
      GPJ2DAT ^= 1 << 0;
      delay(1000);
      GPJ2DAT ^= 1 << 1;
      delay(1000);
      GPJ2DAT ^= 1 << 2;
      delay(1000);
      GPJ2DAT ^= 1 << 3;
      delay(2000);
      GPJ2DAT = 0xFF;
    } else if (byte == 'a') {
      int num = 5;
      while (num--) {
        buzzer_on();
        delay(10000);
        buzzer_off();
        delay(10000);
      }
    }
    delay(1000);
  }
  return 0;
}
