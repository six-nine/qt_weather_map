#include "PointProcessor.h"

#include <iostream>

void PointProcessor::get_point(int &x, int &y, double xx, double yy) {
  x = (int)((xx - 40) * 15) - 400;
  y = (int)((yy) * -15) + 900;
}


