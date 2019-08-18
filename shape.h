#ifndef SHAPE_H
#define SHAPE_H

#include "types.h"

#define MB_POINT_DX 0
#define MB_POINT_DY 1
#define MB_POINT_BRT 2

// A Mathbox vector point. Fields are deltaX, deltaY, and vector brightness.
typedef struct 
{
  int dX, dY;
  byte brightness;
} MathboxPoint;

typedef struct shape_t {
  MathboxPoint *geometry;
  MathboxPoint offset;
  byte number_of_points;
} Shape;

void SHAPE_SetOffset(Shape *shape, int _dx, int _dy);
void SHAPE_Init(Shape *shape, MathboxPoint *_geometry, byte _number_of_points);

#endif
