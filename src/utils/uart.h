#include "./type.h"

#define UART0_BASE 0xE2900000
#define UART1_BASE 0xE2900400

#define ULCON0    DEF(u32, UART0_BASE + 0x00)
#define UCON0     DEF(u32, UART0_BASE + 0x04)
#define UFCON0    DEF(u32, UART0_BASE + 0x08)
#define UTRSTAT0  DEF(u32, UART0_BASE + 0x10)
#define UTXH0     DEF(u32, UART0_BASE + 0x20)
#define URXH0     DEF(u32, UART0_BASE + 0x24)
#define UBRDIV0   DEF(u32, UART0_BASE + 0x28)
#define UDIVSLOT0 DEF(u32, UART0_BASE + 0x2C)

extern void uart_send_byte(unsigned char byte);
extern unsigned char uart_recv_byte();
extern void uart_send_string(char *str);
