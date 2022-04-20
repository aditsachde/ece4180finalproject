#pragma once

#include "Quaternion.h"
#include "Serial.h"

#define TS_MS 5 // millisecond step

class Filter {
public:
  Quaternion state = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

  void step(float ax, float ay, float az, float gx, float gy, float gz);

  // This is a function of the Quaternion class
  // but this is a convenience wrapper around the state quaternion.
  inline void euler(float *roll, float *pitch, float *yaw) {
    state.euler(roll, pitch, yaw);
  };
};