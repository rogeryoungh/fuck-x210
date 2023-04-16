#define GPJ2CON (*(volatile unsigned long *)0xE0200280)
#define GPJ2DAT (*(volatile unsigned long *)0xE0200284)
#define GPH0CON (*(volatile unsigned long *)0xE0200C00)
#define GPH0DAT (*(volatile unsigned long *)0xE0200C04)
// ���ж�0-7���жϿ��ƼĴ���
#define EXT_INT_0_CON *((volatile unsigned int *)0xE0200E00)
// ���ж�0-7���ж����μĴ���
#define EXT_INT_0_MASK *((volatile unsigned int *)0xE0200F00)
// �ⲿ�ж�0-7���жϹ���Ĵ�������¼�Ƿ����жϲ���
#define EXT_INT_0_PEND *((volatile unsigned int *)0xE0200F40)

// ��һ��ʸ���ж�ѡ��Ĵ���
#define VIC0INTSELECT *((volatile unsigned int *)0xF200000C)
// ��һ��ʸ���ж�ʹ�ܼĴ���
#define VIC0IRQSTATUS *((volatile unsigned int *)0xF2000000)
#define VIC0INTENABLE *((volatile unsigned int *)0xF2000010)
#define VIC0VECTADDR2 *((volatile unsigned int *)0xF2000108)
#define VIC0VECTADDR3 *((volatile unsigned int *)0xF200010C)
#define VIC0ADDRESS   *((volatile unsigned int *)0xF2000F00)

extern void key_isr(void);

void led_init(void) {
  /* ����GPC0_1~GPC0_4Ϊ�����LED1~LED4 */
  GPJ2CON &= ~(0xFF << 0);
  GPJ2CON |= ((0x01 << 0) | (0x01 << 4) | (0x01 << 8) | (0x01 << 12));
  GPJ2DAT |= (0xFF << 0);
}

void key_init(void) {
  /* ����GPH0_3Ϊ�ⲿ�жϣ�SW5 */
  GPH0CON |= 0xF << 4 * 3;
  /* �����Ӧλ*/
  EXT_INT_0_CON &= ~(0xF << 4 * 3);
  /* ����EXT_INT[0]��EXT_INT[1]Ϊ�½��ش��� 0b0 010 0 010 */
  EXT_INT_0_CON |= (2 << 4 * 3);
  /* ȡ�������ⲿ�ж�EXT_INT[3] */
  EXT_INT_0_MASK &= ~0x8;
}

void int_init(void) {
  /* ѡ���ⲿ�ж�EXT_INT[3]ΪIRQ���͵��ж� */
  VIC0INTSELECT &= ~0x8;
  /* ʹ���ⲿ�ж�EXT_INT[3] */
  VIC0INTENABLE |= 0x8;
  /* ��VIC0ADDRESS */
  VIC0ADDRESS = 0X0;
  /* ��EXT_INT[0]�����жϣ����û�����key1ʱ��CPU�ͻ��Զ��Ľ�VIC0VECTADDR0��ֵ����VIC0ADDRESS����ת�������ַȥִ��*/
  VIC0VECTADDR3 = (int)key_isr;
}

void key_handle(void) {
  /* ��ȡ��ֵ */
  /*volatile unsigned char key_code = EXT_INT_0_PEND & 0x8;  */
  volatile unsigned char key_code = VIC0IRQSTATUS & 0x8;

  /* ���ж������Ĵ��� */
  VIC0ADDRESS = 0;

  /* ���жϹ���Ĵ��� */
  EXT_INT_0_PEND |= 0x8;

  if (key_code == 0x8) /* SW5 */
  {
    GPJ2DAT ^= 0xF;
  }
}

int main() {
  key_init();
  led_init();
  int_init();
  while (1)
    ;
  return 0;
}
