#define GPJ2CON (*(volatile unsigned long *)0xE0200280)
#define GPJ2DAT (*(volatile unsigned long *)0xE0200284)
#define GPH0CON *((volatile unsigned long *)0xE0200C00)
#define GPH0DAT *((volatile unsigned long *)0xE0200C04)
// ���ж�0-7���жϿ��ƼĴ���
#define EXT_INT_0_CON *((volatile unsigned int *)0xE0200E00)
// ���ж�0-7���ж����μĴ���
#define EXT_INT_0_MASK *((volatile unsigned int *)0xE0200F00)
// ��һ��ʸ���ж�ѡ��Ĵ���
#define VIC0INTSELECT *((volatile unsigned int *)0xF200000C)
// ��һ��ʸ���ж�ʹ�ܼĴ���
#define VIC0IRQSTATUS *((volatile unsigned int *)0xF2000000)
#define VIC0INTENABLE *((volatile unsigned int *)0xF2000010)
#define VIC0VECTADDR2 *((volatile unsigned int *)0xF2000108)
#define VIC0VECTADDR3 *((volatile unsigned int *)0xF200010C)
#define VIC0ADDRESS   *((volatile unsigned int *)0xF2000F00)
// �ⲿ�ж�0-7���жϹ���Ĵ�������¼�Ƿ����жϲ���
#define EXT_INT_0_PEND *((volatile unsigned int *)0xE0200F40)

extern void key_isr(void);

void delay(volatile unsigned int t) {
  volatile unsigned int t2 = 0xFFFF;
  while (t--)
    for (; t2; t2--)
      ;
}

void led_init(void) {
  GPJ2CON &= ~(0xFF << 0);
  GPJ2CON |= ((0x01 << 0) | (0x01 << 4) | (0x01 << 8) | (0x01 << 12));
  GPJ2DAT |= (0xFF << 0);
}

void key_init(void) {
  GPH0CON |= 0xFF << 8;
  /* ����GPH0_0��GPH0_1Ϊ�ⲿ�жϣ�key1��key2 */
  EXT_INT_0_CON &= ~(0xFF << 8);
  /* ��յͰ�λ*/
  EXT_INT_0_CON |= (2 << 4) | (2 << 8);
  /* ����EXT_INT[0]��EXT_INT[1]Ϊ�½��ش��� 0b010 0 010 */
  EXT_INT_0_MASK &= ~0xC;
  /* ȡ�������ⲿ�ж�EXT_INT[0]��EXT_INT[1] */
}

void int_init(void) {
  VIC0INTSELECT &= ~0xC;
  /* ѡ���ⲿ�ж�EXT_INT[0]���ⲿ�ж�EXT_INT[1]ΪIRQ���͵��ж� */
  VIC0INTENABLE |= 0xC;
  /* ʹ���ⲿ�ж�EXT_INT[0]��EXT_INT[1] */
  VIC0VECTADDR2 = (int)key_isr;
  /* ��EXT_INT[0]�����жϣ����û�����key1ʱ��
   * CPU�ͻ��Զ��Ľ�VIC0VECTADDR0��ֵ����VIC0ADDRESS����ת�������ַȥִ ��*/
  VIC0VECTADDR3 = (int)key_isr;
  VIC0ADDRESS = 0;
}

void key_handle() {
  volatile unsigned char key_code = VIC0IRQSTATUS & 0xC;
  VIC0ADDRESS = 0;
  /* ���ж������Ĵ��� */
  EXT_INT_0_PEND |= 0x3;
  /* ���жϹ���Ĵ��� */

  if (key_code == 0x04) /* SW4 */
  {
    int num = 10;
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
  led_init();
  key_init();
  int_init();

  while (1)
    ;
  return 0;
}
