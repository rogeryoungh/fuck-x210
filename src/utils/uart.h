#include "./type.h"

#define UART0_BASE(x) 0xE29000##x
#define UART1_BASE(x) 0xE29004##x

#define UART(x) UART0_BASE(x)

#define ULCON0    DEF(u32, UART(00))
#define UCON0     DEF(u32, UART(04))
#define UFCON0    DEF(u32, UART(08))
#define UTRSTAT0  DEF(u32, UART(10))
#define UTXH0     DEF(u32, UART(20))
#define URXH0     DEF(u32, UART(24))
#define UBRDIV0   DEF(u32, UART(28))
#define UDIVSLOT0 DEF(u32, UART(2C))

extern void uart_send_byte(unsigned char byte);
extern unsigned char uart_recv_byte();
extern void uart_send_string(char *str);
