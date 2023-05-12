#include "../utils/defs.h"
#include "../utils/buzzer.h"
#include "../utils/delay.h"

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

void led_init(void) {
  GPJ2CON &= ~(0xFFFF << 0);
  GPJ2CON |= ((0x1 << 0) | (0x1 << 4) | (0x1 << 8) | (0x1 << 12));
  GPJ2DAT |= 0xF << 0;
}

void init_irq() {
  TINT_CSTAT |= (1 << 2);
}
void clear_irq() {
  TINT_CSTAT |= (1 << 7);
}
void key_init(void) {
  GPH0CON |= 0xF << 4 * 3;
  // 配置2，4，8为外部中断，分别为SW467
  GPH0CON |= (0xF << 4 * 2) | (0xF << 4 * 4);
  /* 清空相应位*/
  EXT_INT_0_CON &= ~((0xF << 4 * 3) | (0xF << 4 * 2) | (0xF << 4 * 4));
  /* 配置EXT_INT[2,3,4,8]为下降沿触发 0b0 010 0 010 */
  EXT_INT_0_CON |= (2 << 4 * 3) | (2 << 4 * 2) | (2 << 4 * 4);

  /* 取消屏蔽外部中断EXT_INT[3] */
  EXT_INT_0_MASK &= ~0x1C;
}

#define VIC0VECTADDR23 *((volatile unsigned int *)0xF200015C)

void int_init(void) {
  VIC0INTSELECT &= ~(0x1C | (1 << 23));
  VIC0INTENABLE |= (0x1C | (1 << 23));
  VIC0ADDRESS = 0;
  VIC0VECTADDR2 = (int)key_isr;
  VIC0VECTADDR3 = (int)key_isr;
  VIC0VECTADDR4 = (int)key_isr;
  VIC0VECTADDR23 = (int)key_isr;
}

int sw4_1 = -1, sw5_1 = -1;
int flag = 0, x2 = 0;

void key_handle() {
  volatile unsigned int key_code = VIC0IRQSTATUS;
  VIC0ADDRESS = 0;
  EXT_INT_0_PEND |= 0x1C;
  clear_irq();
  if (key_code & 0x4) { // sw4
    sw4_1 = 0;

  } else if (key_code & 0x8) { // sw5
    sw5_1 = 0;
  } else if (key_code & 0x10) {
    flag ^= 1;
  } else if (key_code & (1 << 23)) {
    int exec = 0;
    if (flag == 0) {
      exec = 1;
    } else {
      x2 ^= 1;
      exec = x2;
    }

    if (exec) {
      if (sw4_1 >= 0) {
        if (sw4_1 % 2 == 0) {
          buzzer_on();
        } else {
          buzzer_off();
        }
        sw4_1 += 1;
        if (sw4_1 >= 10) {
          sw4_1 = -1;
        }
      }
      if (sw5_1 >= 0) {
        GPJ2DAT ^= 1 << (sw5_1 % 4);
        if (sw5_1 >= 16) {
          sw5_1 = -1;
        }
      }
    }
  }
  //   if (key_code == 0x4) /* sw4 */
  //   {
  //     int num = 10;
  //     while (num--) {

  //       buzzer_on();
  //       delay(0x50000);
  //       buzzer_off();
  //       delay(0x50000);
  //     }
  //   } else if (key_code == 0x10) /* SW6 */
  //   {
  //     int num = 10;
  //     while (num--) {
  //       GPJ2DAT ^= 1 << 0;
  //       delay(0x50000);
  //       GPJ2DAT ^= 1 << 1;
  //       delay(0x50000);
  //       GPJ2DAT ^= 1 << 2;
  //       delay(0x50000);
  //       GPJ2DAT ^= 1 << 3;
  //       delay(0x50000);
  //     }
  //   }
}

int main() {
  buzzer_init();
  led_init();
  key_init();
  int_init();
  init_irq();
  sw4_1 = -1, sw5_1 = -1;
  flag = 0, x2 = 0;
  while (1)
    ;
  return 0;
}
