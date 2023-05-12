#include "../utils/defs.h"
#include "../utils/buzzer.h"

extern void key_isr(void);

void pwm_init() { // 添加定时器 2 初始化代码，使得 PWM 周期为 10s，占空比为 50 %
  //
  // TCNTB2 = 631620;
  // TCMPB2 = 315810;
  // TCFG0 |= (65 << 8);
  // TCFG1 = (0x04 << 8);
  // TCON |= (1 << 13);
  // TCON = (1 << 15);
  GPD0CON &= ~(0xf << 8);
  GPD0CON |= (2 << 8); // 设置定时器2输出
  TCFG0 &= ~(0xff << 8);
  TCFG0 |= (65 << 8); // 预分频值为65
  TCFG1 &= ~(0xf << 8);
  TCFG1 |= (4 << 8); // 设置是为定时器2设置预分频值
  TCON |= (1 << 15); // 自动加载
  TCNTB2 = 62500;
  TCMPB2 = 31250;

  TCON |= (1 << 13);  // 手动更新TCNTB2和TCMPB2
  TCON &= ~(1 << 13); // 清手动更新位
  TCON |= (1 << 12);  // 启动定时器2
}

void init_irq() {
  TINT_CSTAT |= (1 << 2);
}
void clear_irq() {
  TINT_CSTAT |= (1 << 7);
}

void int_init() {
  VIC0INTSELECT |= ~(1 << 23);
  VIC0ADDRESS = 0;
  VIC0VECTADDR23 = (int)key_isr;
  VIC0INTENABLE |= (1 << 23);
}
static int flag = 0;

void key_handle(void) {
  volatile unsigned char status = (VIC0IRQSTATUS >> 23) & 0x1;
  VIC0ADDRESS = 0;
  clear_irq();
  if (status == 1) {
    if (flag == 0) {
      buzzer_on();
    } else {
      buzzer_off();
    }
    flag ^= 1;
  }
}

int main() {
  pwm_init();
  int_init();
  init_irq();
  buzzer_init();
  while (1) {}
  return 0;
}