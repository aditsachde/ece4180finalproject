#pragma once

#include "Quaternion.h"

#define TS_MS 100 // 100 millisecond step

class Filter {
  Quaternion state = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

public:
  void step(float ax, float ay, float az, float gx, float gy, float gz);

  // This is a function of the Quaternion class
  // but this is a convenience wrapper around the state quaternion.
  inline void euler(float *roll, float *pitch, float *yaw) {
    state.euler(roll, pitch, yaw);
  };
};