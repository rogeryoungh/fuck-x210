#include "../utils/defs.h"
#include "../utils/delay.h"
#include "../utils/led.h"
#include "../utils/key.h"
#include "../utils/uart.h"
#include "music-8.h"

extern void key_isr(void);

int global_cnt = 0;

void int_init(void) {
  VIC0INTSELECT &= ~(0x1C | (B0110 << 21));
  VIC0INTENABLE |= (0x1C | (B0110 << 21));
  VIC0ADDRESS = 0;
  VIC0VECTADDR2 = (int)key_isr;
  VIC0VECTADDR3 = (int)key_isr;
  VIC0VECTADDR4 = (int)key_isr;
  VIC0VECTADDR21 = (int)key_isr;
  VIC0VECTADDR22 = (int)key_isr;
  VIC0VECTADDR23 = (int)key_isr;
  VIC0VECTADDR24 = (int)key_isr;
}

int in_music = -1;
int in_led = -1;

void clear_led() {
  GPJ2DAT |= B1111;
}

void press_sw4() {
  uart0_send_string("\r\nMarquee\r\n");
  in_led = 0;
  in_music = -1;
  pwm_init_timer1(1000000 / 8, 1);
  clear_led();
  pwm_untie_buzzer();
}

void press_sw6() {
}

void press_sw5() {
  uart0_send_string("\r\nSW5\r\n");
  press_sw6();
  // pwm_stop_timer1();
  // uart0_send_string("\r\nSW5\r\n");
  // pwm_untie_buzzer();
  // pwm_init_timer2(1000000 / 1433, 2);
  
  if (music_start == doremi) {
    music_set(doudizhu, doudizhu_end);
    uart0_send_string("\r\nswitch doudizhu\r\n");
  } else if (music_start == doudizhu) {
    music_set(xiaojiuwo, xiaojiuwo_end);
    uart0_send_string("\r\nswitch xiaojiuwo\r\n");
  } else {
    music_set(doremi, doremi_end);
    uart0_send_string("\r\nswitch doremi\r\n");
  }
}

void on_timer1() {
  uart0_send_string("\r\ntimer1\r\n");
  if (in_led >= 0) {
    if (in_led == 0x1f) {
      in_led = -1;
      pwm_stop_timer1();
      return;
    }
    if ((in_led & B0001) == B0001)
      GPJ2DAT ^= 1 << 0;
    if ((in_led & B0011) == B0011)
      GPJ2DAT ^= 1 << 1;
    if ((in_led & B0111) == B0111)
      GPJ2DAT ^= 1 << 2;
    if ((in_led & B1111) == B1111)
      GPJ2DAT ^= 1 << 3;
    in_led++;
  } else if (in_music >= 0) {
    int ret = music_next_i(2500);
  }
}

void on_timer2() { // 蜂鸣�?
  // uart0_send_string("\r\ntimer2\r\n");
}

void on_timer3() {
  uart0_send_string("\r\ntimer3\r\n");
}

void key_handle() {
  volatile unsigned int key_code = VIC0IRQSTATUS;
  VIC0ADDRESS = 0;
  EXT_INT_0_PEND |= B16(B0000, B1011, B0001, B1100);
  clear_irq_timer1();
  clear_irq_timer2();
  clear_irq_timer3();

  if (key_code & (1 << 2)) { // sw4
    press_sw4();
  } else if (key_code & (1 << 3)) { // sw5
    press_sw5();
  } else if (key_code & (1 << 4)) { // sw6
    press_sw6();
  } else if (key_code & (1 << 22)) {
    on_timer1();
  } else if (key_code & (1 << 23)) {
    on_timer2();
  } else if (key_code & (1 << 24)) {
    on_timer3();
  }
}

#define STR_T_00   "T+00"
#define STR_T_99   "T+99"
#define STR_T_PLAY "T+play"
#define STR_T_SWITCH "T+switch"

int main() {
  // buzzer_init();
  led_init();
  key_init();
  int_init();
  pwm_init();
  music_set(doremi, doremi_end);

  init_irq_timer1();
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
      uart0_send_string("\r\nPlaying music\r\n");
      p1 = p2 = 0;
      in_music = 1;
      in_led = -1;
      clear_led();
      if (music_start == doremi) {
        uart0_send_string("\r\nplay doremi\r\n");
      } else if (music_start == xiaojiuwo) {
        uart0_send_string("\r\nplay xiaojiuwo\r\n");
      } else if (music_start == doudizhu) {
        uart0_send_string("\r\nplay doudizhu\r\n");
      }
      pwm_init_timer1(10000, 1);
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
