
#include <string.h>
#include "shape.h"
#include "types.h"

word __at(0xa000) dvgram[0x1000];
byte __at(0x8840) _dvgstart;

volatile int __at(0x8100) mathbox_sum;
sbyte __at(0x8102) mathbox_arg1;
sbyte __at(0x8103) mathbox_arg2;
byte __at(0x810f) mathbox_go_mul;

volatile byte __at (0x8000) input0;
volatile byte __at (0x8001) input1;
volatile byte __at (0x8002) input2;
volatile byte __at (0x800f) vidframe;
byte __at (0x8980) watchdog;

#define LEFT1 !(input1 & 0x8)
#define RIGHT1 !(input1 & 0x4)
#define UP1 !(input1 & 0x10)
#define DOWN1 !(input1 & 0x20)
#define FIRE1 !(input1 & 0x2)
#define BOMB1 !(input1 & 0x1)
#define COIN1 (input0 & 0x2)
#define COIN2 (input0 & 0x1)
#define START1 (input2 & 0x20)
#define START2 (input2 & 0x40)

//

void main();

void start() {
__asm
        LD      SP,#0x0
        DI
; copy initialized data
        LD    BC, #l__INITIALIZER
        LD    A, B
        LD    DE, #s__INITIALIZED
        LD    HL, #s__INITIALIZER
        LDIR
__endasm;
  main();
}

//#link "shape.c"

// VECTOR ROUTINES

int dvgwrofs; // write offset for DVG buffer

inline word ___swapw(word j) {
  return ((j << 8) | (j >> 8));
}

inline void dvgreset() {
  dvgwrofs = 0;
}

inline void dvgstart() {
  _dvgstart = 0;
}

void dvgwrite(word w) {
  dvgram[dvgwrofs++] = w;
}

inline void VCTR(int dx, int dy, byte bright) {
  dvgwrite((dy & 0x1fff));
  dvgwrite(((bright & 7) << 13) | (dx & 0x1fff));
}

inline void SVEC(signed char dx, signed char dy, byte bright) {
  dvgwrite(0x4000 | (dx & 0x1f) | ((bright&7)<<5) | ((dy & 0x1f)<<8));
}

inline void JSRL(word offset) {
  dvgwrite(0xa000 | offset);
}

inline void JMPL(word offset) {
  dvgwrite(0xe000 | offset);
}

inline void RTSL() {
  dvgwrite(0xc000);
}

inline void CNTR() {
  dvgwrite(0x8000);
}

inline void HALT() {
  dvgwrite(0x2000);
}

inline void STAT(byte rgb, byte intens) {
  dvgwrite(0x6000 | ((intens & 0xf)<<4) | (rgb & 7));
}

inline void STAT_sparkle(byte intens) {
  dvgwrite(0x6800 | ((intens & 0xf)<<4));
}

inline void SCAL(word scale) {
  dvgwrite(0x7000 | scale);
}

enum {
  BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE
} Color;

MathboxPoint square_points[4] = {
  { 200, 0, 4 },
  { 0, 200, 4 },
  {-200, 0, 4 },
  {0, -200, 4 },
};
byte square_point_count = 4;
MathboxPoint square_pos_offset = { -50, -50, 0 };

#define MB_POINT_DX 0
#define MB_POINT_DY 1
#define MB_POINT_BRT 2

//#link shape.c

void main() {
  Shape shape;
  word scaleFactor = 0x0000;
  
  SHAPE_Init(&shape, square_points, square_point_count);
  SHAPE_SetOffset(&shape, -100, -100);
  
  memset(dvgram, 0x20, sizeof(dvgram)); // HALTs
    
  while (1) {
    dvgreset();
    CNTR();
    
    scaleFactor = (scaleFactor + 0x01) & 0x00FF;
    
    SCAL(scaleFactor);
    STAT(RED, 3);
    {
        byte i;
      
      	VCTR(shape.offset.dX,
             shape.offset.dY,
             0);
      
      	for(i=0; i<4; i++)
        {
          VCTR(shape.geometry[i].dX,
               shape.geometry[i].dY, 
               shape.geometry[i].brightness);
        }
    }
    
    CNTR();
    HALT();
    dvgstart();
    watchdog=0;
  }
  main();
}
