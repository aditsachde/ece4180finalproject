#include "Filter.h"
#include "Quaternion.h"
#include <cmath>

void Filter::step(float ax, float ay, float az, float gx, float gy, float gz) {
  // TODO Finish implementation
  Quaternion accel(0, ax, ay, az);
  accel.normalize();
  Quaternion step = state.clone();

  // step = 0.5 * current ⊗ accel
  step.scale(0.5);
  step.mult(accel);

  // next = current + time delta * step
  step.scale(TS_MS / (float) 1000);
  state.add(step);
}