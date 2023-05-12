#define TCFG0   (*(volatile unsigned long *)0xE2500000)
#define TCFG1   (*(volatile unsigned long *)0xE2500004)
#define TCNTB2  (*(volatile unsigned long *)0xE2500024)
#define TCMPB2  (*(volatile unsigned long *)0xE2500028)
#define TCON    (*(volatile unsigned long *)0xE2500008)
#define GPD0CON (*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT (*(volatile unsigned long *)0xE02000A4)

void pwm_init() {
  GPD0CON &= ~(0xf << 8);
  GPD0CON |= (2 << 8);
  TCFG0 &= ~(0xff << 8);
  TCFG0 |= (65 << 8);
  TCFG1 &= ~(0xf << 8);
  TCFG1 |= (4 << 8);
  TCON |= (1 << 15);
  TCNTB2 = 62500;
  TCMPB2 = 31250;

  TCON |= (1 << 13);
  TCON &= ~(1 << 13);
  TCON |= (1 << 12);
}

int main() {
  pwm_init();

  while (1)
    ;

  return 0;
}
