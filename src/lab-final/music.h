#include "../utils/pwm.h"

#define d0 (1000000 / 1)
#define d5 (1000000 / 1493)
#define d6 (1000000 / 1772)
#define d7 (1000000 / 1967)
#define z1 (1000000 / 2093)
#define z2 (1000000 / 2349)
#define z3 (1000000 / 2637)
#define z4 (1000000 / 2794)
#define z5 (1000000 / 3136)
#define z6 (1000000 / 3528)
#define z7 (1000000 / 3951)
#define g1 (1000000 / 4186)
#define g2 (1000000 / 4698)

#define g3 (1000000 / 5198)
#define g4 (1000000 / 5698)
#define g5 (1000000 / 6298)

int xiaojiuwo[] =
    {
        z3, 125, z3, 50,  z4, 100, z3, 50,  z2, 50,  z2, 125, z2, 50,
        z5, 100, z2, 50,  z1, 50,  z1, 125, z1, 50,  z6, 100, z5, 100,
        z3, 125, z4, 25,  z3, 25,  z2, 100, z2, 100, z3, 125, z3, 50,
        z4, 100, z5, 50,  z3, 50,  z5, 125, z5, 50,  g2, 100, z7, 50,
        g1, 50,  g1, 200, g1, 50,  z7, 50,  z6, 50,  z7, 50,  g1, 125,
        z7, 50,  g1, 200, g1, 50,  z7, 50,  g1, 50,  z7, 50,  g1, 100,
        z3, 50,  z5, 50,  z5, 250, z6, 50,  z7, 50,  g1, 50,  z7, 50,
        g1, 50,  z7, 50,  g1, 100, g2, 100, g3, 300, z6, 50,  z7, 50,
        g1, 50,  z7, 50,  g1, 50,  z7, 50,  g1, 100, g3, 100, z7, 50,
        z6, 50,  z7, 50,  z6, 50,  z7, 50,  z5, 50,  z5, 50,  g1, 50,
        g1, 200, g3, 75,  g4, 25,  g4, 50,  g3, 50,  g2, 200, d0, 50,
        z5, 50,  g1, 50,  g2, 50,                                     // 81
        g3, 50,  g1, 50,  g1, 50,  z5, 50,  z5, 100, z5, 50,  g3, 50, // 88
        g2, 50,  g3, 50,  g2, 50,  g1, 50,  z5, 50,  z5, 50,  z6, 50,
        z7, 50,                                                        // 96
        g1, 50,  z6, 50,  z6, 50,  z3, 50,  z3, 150, z7, 50,           // 103
        g1, 50,  z7, 50,  g1, 50,  g2, 50,  z6, 50,  z5, 50,  z5, 100, // 110
        z6, 100, z5, 50,  z6, 50,  z7, 100, g3, 50,  g4, 50,           // 116
        g3, 50,  g4, 50,  g3, 50,  g2, 50,  g2, 50,  g1, 100, z5, 50,  // 123
        z5, 50,  z4, 50,  z4, 50,  g1, 50,  g1, 50,  g3, 50,  g3, 50,
        g1, 50,                                                       // 131
        g2, 200, d0, 50,  z5, 50,  g1, 50,  g2, 50,                   // 136
        g3, 50,  g1, 50,  g1, 50,  z5, 50,  z5, 100, z5, 50,  g3, 50, // 143
        g2, 50,  g3, 50,  g2, 50,  g1, 50,  z5, 50,  z5, 50,  z6, 50,
        z7, 50,                                                       // 151
        g1, 50,  z6, 50,  z6, 50,  z3, 50,  z3, 100, z3, 50,  g1, 50, // 158
        z7, 50,  g1, 50,  z7, 50,  g2, 50,  z5, 200,                  // 165
        z6, 100, z7, 50,  g1, 50,  g2, 100, g3, 50,  g4, 50,          // 171
        g3, 50,  g4, 50,  g3, 50,  g2, 50,  g1, 150,                  // 178
        g2, 50,  g1, 50,  g2, 50,  z6, 50,  g1, 100, g2, 50,  g1, 50, // 185
        g1, 400,                                                      // 186
        g2, 50,  g1, 50,  g2, 50,  z6, 50,  z7, 100, z7, 50,  g1, 50, // 193
        g1, 400};

int doudizhu[] = {
    z3, 100, z3, 50,  z2, 50,  z1, 100, d5, 100,                         //
    z2, 50,  z3, 50,  z2, 50,  z3, 50,  d5, 200,                         //
    d6, 100, d6, 50,  d5, 50,  d6, 100, z1, 100,                         //
    z5, 50,  z6, 50,  z3, 50,  z5, 50,  z2, 200,                         //
    z3, 100, z3, 50,  z2, 50,  z3, 100, z5, 100,                         //
    z6, 50,  g1, 50,  z6, 50,  z5, 50,  z6, 100, z5, 50, z3, 50,         //
    z2, 100, z2, 50,  z3, 50,  z5, 100, d5, 100,                         //
    z2, 50,  z3, 50,  z2, 50,  z3, 50,  z1, 200,                         //
    z2, 75,  z3, 25,  z2, 50,  z3, 50,  z5, 50,  z6, 50, g1, 50, g2, 50, //
    g1, 100, z5, 100, g1, 200,
    // g1, 50,
    // d0, 50,
    // z5, 50,
    // d0, 150,
    // g1, 100,
    // d0, 100,
};

int doremi[] = {
    d5, 100,
    d6, 100,
    d7, 100,
    z1, 100,
    z2, 100,
    z3, 100,
    z4, 100,
    z5, 100,
    z6, 100,
    z7, 100,
    g1, 100,
    g2, 100,
    g3, 100,
    g4, 100,
    g5, 100,
};

int *doudizhu_end = doudizhu + sizeof(doudizhu) / sizeof(int);
int *xiaojiuwo_end = xiaojiuwo + sizeof(xiaojiuwo) / sizeof(int);
int *doremi_end = doremi + sizeof(doremi) / sizeof(int);

int *music_start;
int *music_end;

void music_set(int *start, int *end) {
  music_start = start;
  music_end = end;
}

int music_next_i(int rate) {
  if (music_start == music_end) {
    pwm_untie_buzzer();
    pwm_stop_timer2();
    pwm_stop_timer1();
    return 0;
  } else {
    int x = music_start[0];
    if (x == d5) {
      pwm_init_timer2(music_start[0], 8);
    } else {
      pwm_init_timer2(music_start[0], 8);
    }
    pwm_init_timer1(music_start[1] * rate, 1);
    music_start += 2;
    return 1;
  }
}