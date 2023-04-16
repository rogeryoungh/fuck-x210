#define B0000 0x0
#define B0001 0x1
#define B0010 0x2
#define B0011 0x3
#define B0100 0x4
#define B0101 0x5
#define B0110 0x6
#define B0111 0x7
#define B1000 0x8
#define B1001 0x9
#define B1010 0xA
#define B1011 0xB
#define B1100 0xC
#define B1101 0xD
#define B1110 0xE
#define B1111 0xF

#define B8(x1, x0) ((x1) << 8 | (x0))
#define B16(x3, x2, x1, x0) ((x3) << 24 | (x2) << 16 |(x1) << 8 | (x0))
