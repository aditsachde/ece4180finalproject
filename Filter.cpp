#include "Filter.h"
#include "Quaternion.h"
#include <cmath>

void Filter::step(float ax, float ay, float az, float gx, float gy, float gz) {
  // TODO Finish implementation
  Quaternion accel(0, ax, ay, az);
  accel.normalize();
}