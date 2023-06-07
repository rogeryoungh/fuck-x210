#include "./defs.h"

void uart0_init() {
  // 启用 UART0 的 RXD 和 TXD
  GPA0CON &= ~0xFF;
  GPA0CON |= 0x22;

  // 8 位数据，1 位停止，无校验，正常模式
  ULCON0 = (B0011 << 0) | (0 << 2) | (0 << 3) | (0 << 6);

  // 中断或轮询方式、时钟源 PCLK
  // 课本上还打开了错误中断使能
  UCON0 = (B0001 << 0) | (B0001 << 2) | (0 << 10);

  // 不使用 FIFO
  UFCON0 = 0;

  // 波特率计算 115200bps，PCLK = 66MHz
  // 芯片手册 P880 查表
  UBRDIV0 = 34;
  UDIVSLOT0 = 0xDDDD;
}

void uart1_init() {
  // 启用 UART0 的 RXD 和 TXD
  GPA0CON &= ~0xFF0000;
  GPA0CON |= 0x220000;

  // 8 位数据，1 位停止，无校验，正常模式
  ULCON1 = (B0011 << 0) | (0 << 2) | (0 << 3) | (0 << 6);

  // 中断或轮询方式、时钟源 PCLK
  // 课本上还打开了错误中断使能
  UCON1 = (B0001 << 0) | (B0001 << 2) | (0 << 10);

  // 不使用 FIFO
  UFCON1 = 0;

  // 波特率计算 115200bps，PCLK = 66MHz
  // 芯片手册 P880 查表
  UBRDIV1 = 34;
  UDIVSLOT1 = 0xDDDD;
}

void uart0_send_byte(unsigned char byte) {
  // 等待发送缓冲区为空
  while (!(UTRSTAT0 & B0100))
    ;
  // 发送一字节数据
  UTXH0 = byte;
}

void uart1_send_byte(unsigned char byte) {
  // 等待发送缓冲区为空
  while (!(UTRSTAT1 & B0100))
    ;
  // 发送一字节数据
  UTXH1 = byte;
}

unsigned char uart0_recv_byte() {
  // 等待接收缓冲区有数据可读
  while (!(UTRSTAT0 & B0001))
    ;
  // 接收一字节数据
  return URXH0;
}

unsigned char uart1_recv_byte() {
  // 等待接收缓冲区有数据可读
  while (!(UTRSTAT1 & B0001))
    ;
  // 接收一字节数据
  return URXH1;
}

void uart0_send_string(char *str) {
  char *p = str;
  while (*p)
    uart0_send_byte(*p++);
}

void uart1_send_string(char *str) {
  char *p = str;
  while (*p)
    uart1_send_byte(*p++);
}
