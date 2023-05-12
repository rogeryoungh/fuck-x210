#define GPD0CON        (*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT        (*(volatile unsigned long *)0xE02000A4)
#define GPJ2CON        *((volatile unsigned int *)0xE0200280)
#define GPJ2DAT        *((volatile unsigned int *)0xE0200284)
#define GPH0CON        *((volatile unsigned int *)0xE0200C00)
#define GPH0DAT        *((volatile unsigned int *)0xE0200C04)
#define EXT_INT_0_CON  *((volatile unsigned int *)0xE0200E00)
#define EXT_INT_0_MASK *((volatile unsigned int *)0xE0200F00)
#define VIC0IRQSTATUS  *((volatile unsigned int *)0xF0000000)
#define VIC0INTSELECT  *((volatile unsigned int *)0xF200000C)
#define VIC0INTENABLE  *((volatile unsigned int *)0xF2000010)
#define VIC0VECTADDR2  *((volatile unsigned int *)0xF2000108)
#define VIC0VECTADDR4  *((volatile unsigned int *)0xF2000110)
#define VIC0ADDRESS    *((volatile unsigned int *)0xF2000F00)
#define EXT_INT_0_PEND *((volatile unsigned int *)0xE0200F40)

// extern void buzzer_on(void);
// extern void buzzer_init(void);
// extern void buzzer_off(void);

extern void key_isr(void);

void buzzer_init(void) {
  GPD0CON |= 1 << 8;
}

void buzzer_on(void) {
  GPD0DAT |= 1 << 2;
}

void buzzer_off(void) {
  GPD0DAT &= ~(1 << 2);
}

void delay(unsigned long times) {
  unsigned long i = times;
  while (i--)
    ;
}

void led_init(void) {
  GPJ2CON &= ~(0xFFFF << 0);
  GPJ2CON |= ((0x1 << 0) | (0x1 << 4) | (0x1 << 8) | (0x1 << 12));
  GPJ2DAT |= 0xF << 0;
}

void key_init(void) {
  GPH0CON |= 0xF0F << 8;
  EXT_INT_0_CON &= ~(0xF0F << 8);
  EXT_INT_0_CON |= (2 << 8) | (2 << 16);
  EXT_INT_0_MASK &= ~0x14;
}

void int_init(void) {
  VIC0INTSELECT &= ~0x14;
  VIC0INTENABLE |= 0x14;
  VIC0VECTADDR2 = (int)key_isr;
  VIC0VECTADDR4 = (int)key_isr;
}

void key_handle() {
  volatile unsigned char key_code = EXT_INT_0_PEND & 0x14;
  VIC0ADDRESS = 0;
  EXT_INT_0_PEND |= 0x14;

  if (key_code == 0x4) /* sw4 */
  {
    int num = 10;
    while (num--) {

      buzzer_on();
      delay(0x50000);
      buzzer_off();
      delay(0x50000);
    }
  } else if (key_code == 0x10) /* SW6 */
  {
    int num = 10;
    while (num--) {
      GPJ2DAT ^= 1 << 0;
      delay(0x50000);
      GPJ2DAT ^= 1 << 1;
      delay(0x50000);
      GPJ2DAT ^= 1 << 2;
      delay(0x50000);
      GPJ2DAT ^= 1 << 3;
      delay(0x50000);
    }
  }
}

int main() {
  buzzer_init();
  led_init();
  key_init();
  int_init();

  while (1)
    ;
  return 0;
}
