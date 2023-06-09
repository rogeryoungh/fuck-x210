#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/led.h"
#include "../utils/key.h"
#include "../utils/uart.h"

extern void key_isr(void);

int global_cnt = 0;

void pwm_init() {
  // buzzer

  // /=65
  TCFG0 &= ~(0xff << 8);
  TCFG0 |= (65 << 8);
  TCFG0 &= ~(0xff << 0);
  TCFG0 |= (65 << 0);

 // /= 16
  TCFG1 &= ~(0xf << 4);
  TCFG1 |= (4 << 4);
  TCFG1 &= ~(0xf << 8);
  TCFG1 |= (4 << 8);
  TCFG1 &= ~(0xf << 12);
  TCFG1 |= (4 << 12);

  // TCFG1 &= ~(0xf << 0);
  // TCFG1 |= (4 << 0);
}

void pwm_init_timer1() {
  // timer1
  GPD0CON &= ~(0xf << 8);
  GPD0CON |= (2 << 8);

  TCON |= (B1000 << 8); // 自动加载
  TCNTB1 = 62500 / 2;
  TCMPB1 = 31250 / 2;
  TCON |= (B0010 << 8);  // 手动更新
  TCON &= ~(B0010 << 8); // 清手动更新
  TCON |= (B0001 << 8);  // 启动 timer1
}

void pwm_init_timer2(int freq) {
  TCON |= (B1000 << 12); // �?动加�?
  TCNTB2 = freq >> 0;
  TCMPB2 = freq >> 1;

  TCON |= (B0010 << 12);  // 手动更新
  TCON &= ~(B0010 << 12); // 清手动更新
  TCON |= (B0001 << 12);  // 启动 timer1
}

void pwm_init_timer3() {
  // timer3
  TCON |= (B1000 << 16); // 自动加载
  TCNTB3 = 62500 / 2;
  TCMPB3 = 31250 / 2;
  TCON |= (B0010 << 16);  // 手动更新
  TCON &= ~(B0010 << 16); // 清手动更新
  TCON |= (B0001 << 16);  // 启动 timer1
}

void init_irq_timer2() {
  TINT_CSTAT |= (1 << 2);
}
void clear_irq_timer2() {
  TINT_CSTAT |= (1 << 7);
}

void init_irq_timer1() {
  TINT_CSTAT |= (1 << 1);
}
void clear_irq_timer1() {
  TINT_CSTAT |= (1 << 6);
}

void init_irq_timer3() {
  TINT_CSTAT |= (1 << 3);
}
void clear_irq_timer3() {
  TINT_CSTAT |= (1 << 8);
}

void int_init(void) {
  VIC0INTSELECT &= ~(0x1C | (B1100 << 21));
  VIC0INTENABLE |= (0x1C | (B1100 << 21));
  VIC0ADDRESS = 0;
  VIC0VECTADDR2 = (int)key_isr;
  VIC0VECTADDR3 = (int)key_isr;
  VIC0VECTADDR4 = (int)key_isr;
  VIC0VECTADDR21 = (int)key_isr;
  VIC0VECTADDR22 = (int)key_isr;
  VIC0VECTADDR23 = (int)key_isr;
  VIC0VECTADDR24 = (int)key_isr;
}
void pwm_clear_timer1() {
  uart0_send_string("clear sw4\r\n");
  // TCON |= (B0010 << 8); // 手动更新
  // TCON &= ~(B0010 << 8); // 清手动更新
  TCON &= ~(B0001 << 8); // 停止 timer1
}

void pwm_clear_timer3() {
  uart0_send_string("clear sw4\r\n");
  // TCON |= (B0010 << 8); // 手动更新
  // TCON &= ~(B0010 << 8); // 清手动更新
  TCON &= ~(B0001 << 16); // 停止 timer1
}

void key_handle() {
  volatile unsigned int key_code = VIC0IRQSTATUS;
  VIC0ADDRESS = 0;
  EXT_INT_0_PEND |= B16(B0000, B1011, B0001, B1100);
  clear_irq_timer2();
  clear_irq_timer3();

  if (key_code & (1 << 2)) { // sw4
    GPD0CON &= ~(0xf << 8);
    GPD0CON |= (0 << 8);
    uart0_send_string("\r\nMarquee\r\n");
    // pwm_init_timer1();
    pwm_init_timer3();
  } else if (key_code & (1 << 3)) { // sw5
    pwm_clear_timer1();
    uart0_send_string("\r\nSW5\r\n");
    pwm_init_timer2(62500 / 1433);
  } else if (key_code & (1 << 22)) {
    uart0_send_string("\r\ntimer1\r\n");
  } else if (key_code & (1 << 23)) {
    // uart0_send_string("\r\ntimer2\r\n");
  } else if (key_code & (1 << 24)) {
    uart0_send_string("\r\ntimer3\r\n");
  }
}

#define STR_T_00   "T+00"
#define STR_T_99   "T+99"
#define STR_T_PLAY "T+play"

int main() {
  // buzzer_init();
  led_init();
  key_init();
  int_init();
  pwm_init();

  // init_irq_timer1();
  init_irq_timer2();
  init_irq_timer3();
  uart0_init();
  uart0_send_string("uart0 init success!");

  int p1 = 0, p2 = 0;
  while (1) {
    char x = uart0_recv_byte();
    uart0_send_byte(x);
    // T+play
    int past_t1 = p1;
    int past_t2 = p2;
    int fail = 0;

    int in1 = STR_T_00[p1] <= x && x <= STR_T_99[p1];
    p1 = (in1) ? p1 + 1 : 0;
    p2 = (x == STR_T_PLAY[p2]) ? p2 + 1 : 0;
    if (p1 == sizeof(STR_T_00) - 1) {
      uart0_send_string("\r\nIt is me!\r\n");
      p1 = p2 = 0;
    } else if (p2 == sizeof(STR_T_PLAY) - 1) {
      uart0_send_string("\r\nT+PLAY\r\n");
      p1 = p2 = 0;
    } else {
      fail |= p1 == 0 && p2 == 0;
      fail |= past_t1 > 2 && p1 == 0;
      fail |= past_t2 > 2 && p2 == 0;
      if (fail)
        uart0_send_string("error\r\n");
    }
  }
  return 0;
}
