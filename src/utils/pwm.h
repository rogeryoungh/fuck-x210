#include "./defs.h"

void pwm_init() {
  // buzzer

  // /=65
  TCFG0 &= ~(0xff << 8);
  TCFG0 |= (65 << 8);
  TCFG0 &= ~(0xff << 0);
  TCFG0 |= (65 << 0);

  // /= 1
  TCFG1 &= ~(0xf << 4);
  TCFG1 |= (1 << 4);
  TCFG1 &= ~(0xf << 8);
  TCFG1 |= (1 << 8);
  TCFG1 &= ~(0xf << 12);
  TCFG1 |= (1 << 12);
  TCFG1 &= ~(0xf << 0);
  TCFG1 |= (1 << 0);
}

void pwm_tie_buzzer() {
  GPD0CON &= ~(0xf << 8);
  GPD0CON |= (2 << 8);
}

void pwm_untie_buzzer() {
  GPD0CON &= ~(0xf << 8);
  GPD0CON |= (1 << 8);
}

void pwm_init_timer1(int freq, int b) {
  TCON |= (B1000 << 8); // 自动加载
  TCNTB1 = freq >> 0;
  TCMPB1 = freq >> b;
  TCON |= (B0010 << 8);  // 手动更新
  TCON &= ~(B0010 << 8); // 清手动更新
  TCON |= (B0001 << 8);  // 启动 timer1
}

void pwm_stop_timer1() {
  TCON &= ~(B0001 << 8); // 停止 timer1
}

void pwm_init_timer2(int freq, int b) {
  TCON |= (B1000 << 12); // 自动加载
  TCNTB2 = freq >> 0;

  int freq32 = freq >> 5;

  TCMPB2 = freq32 * b;

  TCON |= (B0010 << 12);  // 手动更新
  TCON &= ~(B0010 << 12); // 清手动更新
  TCON |= (B0001 << 12);  // 启动 timer2
}

void pwm_stop_timer2() {
  TCON &= ~(B0001 << 12); // 停止 timer1
}

void pwm_init_timer3(int freq, int b) {
  // timer3
  TCON |= (B1000 << 16); // 自动加载
  TCNTB3 = freq >> 0;
  TCMPB3 = freq >> b;
  TCON |= (B0010 << 16);  // 手动更新
  TCON &= ~(B0010 << 16); // 清手动更新
  TCON |= (B0001 << 16);  // 启动 timer1
}

void pwm_stop_timer3() {
  TCON &= ~(B0001 << 16); // 停止 timer3
}

void init_irq_timer1() {
  TINT_CSTAT |= (1 << 1);
}
void init_irq_timer2() {
  TINT_CSTAT |= (1 << 2);
}
void init_irq_timer3() {
  TINT_CSTAT |= (1 << 3);
}

void clear_irq_timer1() {
  TINT_CSTAT |= (1 << 6);
}
void clear_irq_timer2() {
  TINT_CSTAT |= (1 << 7);
}
void clear_irq_timer3() {
  TINT_CSTAT |= (1 << 8);
}
