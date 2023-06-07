#include "./defs.h"

void uart0_init() {
  // ���� UART0 �� RXD �� TXD
  GPA0CON &= ~0xFF;
  GPA0CON |= 0x22;

  // 8 λ���ݣ�1 λֹͣ����У�飬����ģʽ
  ULCON0 = (B0011 << 0) | (0 << 2) | (0 << 3) | (0 << 6);

  // �жϻ���ѯ��ʽ��ʱ��Դ PCLK
  // �α��ϻ����˴����ж�ʹ��
  UCON0 = (B0001 << 0) | (B0001 << 2) | (0 << 10);

  // ��ʹ�� FIFO
  UFCON0 = 0;

  // �����ʼ��� 115200bps��PCLK = 66MHz
  // оƬ�ֲ� P880 ���
  UBRDIV0 = 34;
  UDIVSLOT0 = 0xDDDD;
}

void uart1_init() {
  // ���� UART0 �� RXD �� TXD
  GPA0CON &= ~0xFF0000;
  GPA0CON |= 0x220000;

  // 8 λ���ݣ�1 λֹͣ����У�飬����ģʽ
  ULCON1 = (B0011 << 0) | (0 << 2) | (0 << 3) | (0 << 6);

  // �жϻ���ѯ��ʽ��ʱ��Դ PCLK
  // �α��ϻ����˴����ж�ʹ��
  UCON1 = (B0001 << 0) | (B0001 << 2) | (0 << 10);

  // ��ʹ�� FIFO
  UFCON1 = 0;

  // �����ʼ��� 115200bps��PCLK = 66MHz
  // оƬ�ֲ� P880 ���
  UBRDIV1 = 34;
  UDIVSLOT1 = 0xDDDD;
}

void uart0_send_byte(unsigned char byte) {
  // �ȴ����ͻ�����Ϊ��
  while (!(UTRSTAT0 & B0100))
    ;
  // ����һ�ֽ�����
  UTXH0 = byte;
}

void uart1_send_byte(unsigned char byte) {
  // �ȴ����ͻ�����Ϊ��
  while (!(UTRSTAT1 & B0100))
    ;
  // ����һ�ֽ�����
  UTXH1 = byte;
}

unsigned char uart0_recv_byte() {
  // �ȴ����ջ����������ݿɶ�
  while (!(UTRSTAT0 & B0001))
    ;
  // ����һ�ֽ�����
  return URXH0;
}

unsigned char uart1_recv_byte() {
  // �ȴ����ջ����������ݿɶ�
  while (!(UTRSTAT1 & B0001))
    ;
  // ����һ�ֽ�����
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
