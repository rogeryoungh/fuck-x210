#ifndef RY_DEFS_H
#define RY_DEFS_H

typedef unsigned int u32;
typedef unsigned long u64;
typedef unsigned short u16;
typedef unsigned char u8;

#include "./bin.h"

#define DEF(X, Y) (*(volatile X *)(Y))

// gpio

#define GPA0CON        DEF(u32, 0xE0200000)
#define GPJ2CON        DEF(u64, 0xE0200280)
#define GPJ2DAT        DEF(u64, 0xE0200284)
#define GPH0CON        DEF(u32, 0xE0200C00)
#define GPH0DAT        DEF(u32, 0xE0200C04)
#define GPD0CON        DEF(u64, 0xE02000A0)
#define GPD0DAT        DEF(u64, 0xE02000A4)
#define GPC1CON        DEF(u64, 0xE0200060)
#define GPC1DAT        DEF(u64, 0xE0200064)
#define GPC1PUD        DEF(u64, 0xE0200068)
#define EXT_INT_0_CON  DEF(u32, 0xE0200E00)
#define EXT_INT_0_MASK DEF(u32, 0xE0200F00)
#define EXT_INT_0_PEND DEF(u32, 0xE0200F40)

#define VIC0IRQSTATUS  DEF(u32, 0xF2000000)
#define VIC0INTSELECT  DEF(u32, 0xF200000C)
#define VIC0INTENABLE  DEF(u32, 0xF2000010)
#define VIC0ADDRESS    DEF(u32, 0xF2000F00)
#define VIC0VECTADDR2  DEF(u32, 0xF2000108)
#define VIC0VECTADDR3  DEF(u32, 0xF200010C)
#define VIC0VECTADDR4  DEF(u32, 0xF2000110)
#define VIC0VECTADDR23 DEF(u32, 0xF200015C)
#define LEDS           1

// pwn

#define TCFG0      DEF(u64, 0xE2500000)
#define TCFG1      DEF(u64, 0xE2500004)
#define TCNTB2     DEF(u64, 0xE2500024)
#define TCMPB2     DEF(u64, 0xE2500028)
#define TCON       DEF(u64, 0xE2500008)
#define TCFG0      DEF(u64, 0xE2500000)
#define TCFG1      DEF(u64, 0xE2500004)
#define TCNTB2     DEF(u64, 0xE2500024)
#define TCMPB2     DEF(u64, 0xE2500028)
#define TCON       DEF(u64, 0xE2500008)
#define TINT_CSTAT DEF(u32, 0xE2500044)

#define TCFG0      DEF(u64, 0xE2500000)
#define TCFG1      DEF(u64, 0xE2500004)
#define TCNTB2     DEF(u64, 0xE2500024)
#define TCMPB2     DEF(u64, 0xE2500028)
#define TCON       DEF(u64, 0xE2500008)
#define TINT_CSTAT DEF(u32, 0xE2500044)

// uart

#define UART0_BASE(x) 0xE29000##x
#define UART1_BASE(x) 0xE29004##x

#define UART(x) UART0_BASE(x)

#define ULCON0    DEF(u32, UART(00))
#define UCON0     DEF(u32, UART(04))
#define UFCON0    DEF(u32, UART(08))
#define UTRSTAT0  DEF(u32, UART(10))
#define UTXH0     DEF(u32, UART(20))
#define URXH0     DEF(u32, UART(24))
#define UBRDIV0   DEF(u32, UART(28))
#define UDIVSLOT0 DEF(u32, UART(2C))

// clock

#define APLLCON0 DEF(u32, 0xE0100100)
#define MPLLCON  DEF(u32, 0xE0100108)
#define EPLLCON0 DEF(u32, 0xE0100110)
#define VPLLCON  DEF(u32, 0xE0100120)
#define CLK_SRC0 DEF(u32, 0xE0100200)
#define CLK_DIV0 DEF(u32, 0xE0100300)
#define CLK_DIV1 DEF(u32, 0xE0100304)
#define CLK_DIV2 DEF(u32, 0xE0100308)
#define CLK_DIV3 DEF(u32, 0xE010030C)

#endif
