#include "../utils/gpio.h"
#include "../utils/uart.h"

/*
** UART0��ʼ��
*/
void uart_init() {
  /*
  ** ����GPA0_0ΪUART_0_RXD
  ** ����GPA0_1ΪUART_0_TXD
  */
  GPA0CON &= ~0xFF;
  GPA0CON |= 0x22; // 0b 00100010

  /* 8-bits/One stop bit/No parity/Normal mode operation */
  /*ÿ��8λ��1��ֹͣλ������ż��֤����������ģʽ���Ǻ��⣩*/
  ULCON0 = 0x3 | (0 << 2) | (0 << 3) | (0 << 6); // 0b 0 0xx 0 11

  /* Interrupt request or polling mode/Normal transmit/Normal operation/PCLK/*/
  /*���ͺͽ������Ų����жϺ���ѯ��ѯģʽ���������ͣ����������ʱ��ѡ��ΪPCLK*/
  UCON0 = 1 | (1 << 2) | (0 << 10);

  /* ��ֹFIFO */
  UFCON0 = 0;

  /*
  ** �����ʼ��㣺115200bps
  ** PCLK = 66MHz
  ** DIV_VAL = (66000000/(115200 x 16))-1 = 35.8 - 1 = 34.8
  ** UBRDIV0 = 34 (DIV_VAL����������)
  ** (num of 1's in UDIVSLOTn)/16 = 0.8 (DIV_VAL��С������)
  ** (num of 1's in UDIVSLOTn) = 12
  ** UDIVSLOT0 = 0xDDDD (�������ֲ���880ҳ���)
  */
  UBRDIV0 = 34; // �����ʷֶ�ֵ
  UDIVSLOT0 = 0xDDDD;
}

void uart_send_byte(unsigned char byte) {
  while (!(UTRSTAT0 & (1 << 2)))
    ;           /* �ȴ����ͻ�����Ϊ�� */
  UTXH0 = byte; /* ����һ�ֽ����� */
}

unsigned char uart_recv_byte() {
  while (!(UTRSTAT0 & 1))
    ;           /* �ȴ����ջ����������ݿɶ� */
  return URXH0; /* ����һ�ֽ����� */
}

void uart_send_string(char *str) {
  char *p = str;
  while (*p)
    uart_send_byte(*p++);
}