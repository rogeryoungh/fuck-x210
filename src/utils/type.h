#ifndef RY_TYPE_H
#define RY_TYPE_H

typedef unsigned int u32;
typedef unsigned long u64;
typedef unsigned short u16;
typedef unsigned char u8;

#define DEF(X, Y) (*(volatile X *)(Y))

#endif // RY_TYPE_H