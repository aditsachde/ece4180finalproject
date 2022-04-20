#include "Filter.h"
#include "Quaternion.h"
#include "Serial.h"
#include <cmath>

void Filter::step(float ax, float ay, float az, float gx, float gy, float gz) {
  // Adapted from https://www.samba.org/tridge/UAV/madgwick_internal_report.pdf

  // Local system variables
  float f_1, f_2, f_3; // obj function elements
  float J_11or24, J_12or23, J_13or22, J_14or21, J_32,
      J_33; // Jacobian intermediates

  // Auxiliary variables
  Quaternion c_double = state.clone();
  c_double.scale(2);

  // Normalize accel measurement
  Quaternion accel(0, ax, ay, az);
  accel.normalize();

  // Compute objective function and Jacboian
  f_1 = c_double.x * state.z - c_double.w * state.y - accel.x;
  f_2 = c_double.w * state.x + c_double.y * state.z - accel.y;
  f_3 = 1.0f - c_double.x * state.x - c_double.y * state.y - accel.z;

  J_11or24 = c_double.y;
  J_12or23 = c_double.z;
  J_13or22 = c_double.w;
  J_14or21 = c_double.x;
  J_32 = 2.0f * J_14or21;
  J_33 = 2.0f * J_11or24;

  // Compute gradient
  Quaternion accel_step(J_14or21 * f_2 - J_11or24 * f_1,
                        J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3,
                        J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1,
                        J_14or21 * f_1 + J_11or24 * f_2);

  // Normalize gradient
  accel_step.normalize();

  // Compute quaternion derivative measured by gyroscope
  Quaternion gyro(0, gx, gy, gz);
  Quaternion gyro_step = state.clone();

  gyro_step.mult(gyro);
  gyro_step.scale(0.5);

  // Compute then integrate the estimated quaternion derivative
  accel_step.scale(sqrt(0.75f) * 35.0f * DEG_TO_RAD); // Assume x deg/s gyro drift

  gyro_step.sub(accel_step);
  gyro_step.scale(TS_MS / 1000.0f);
  state.add(gyro_step);

  // Normalize quaternion
  state.normalize();
}