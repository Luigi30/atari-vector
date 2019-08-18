#include "shape.h"

void SHAPE_SetOffset(Shape *shape, int _dx, int _dy)
{
  shape->offset.dX = _dx;
  shape->offset.dY = _dy;
  shape->offset.brightness = 0;
}

void SHAPE_Init(Shape *shape, MathboxPoint *_geometry, byte _number_of_points)
{
  shape->geometry = _geometry;
  shape->number_of_points = _number_of_points;
}
