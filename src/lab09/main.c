#include "../utils/defs.h"
#include "../utils/uart.h"
#include "../utils/delay.h"
#include "../utils/led.h"

int main() {
  uart_init();
  led_init();
  uart_send_string("\r\nUART Test in S5PV210\r\n");

  while (1) {
    uart_send_string("\r\n1.LED1 Toggle\r\n");
    uart_send_string("\r\n2.LED2 Toggle\r\n");
    uart_send_string("\r\nPlease select 1 or 2 to Toggle the LED\r\n");
    char byte = uart_recv_byte();
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
    }

    delay(1000);
  }
  return 0;
}
