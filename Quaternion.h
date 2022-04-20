#pragma once
#include <cmath>

#define PI 3.14159265358979f
#define DEG_TO_RAD (PI / 180)
#define RAD_TO_DEG (180 / PI)

class Quaternion {
public:
  float w;
  float x;
  float y;
  float z;

  inline Quaternion(float w, float x, float y, float z);
  inline void mult(Quaternion q);
  inline void scale(float scalar);
  inline void add(Quaternion q);
  inline void sub(Quaternion q);
  inline void conjugate(void);
  inline float normal(void);
  inline void normalize(void);
  inline Quaternion clone();
  void euler(float *roll, float *pitch, float *yaw);
};

// This isn't very clean, but inline functions must be defined alongside the 
// function declaration in the header file.
// These functions do a very tiny bit of math and they should ideally be inlined.

inline Quaternion::Quaternion(float w, float x, float y, float z) {
  this->w = w;
  this->x = x;
  this->y = y;
  this->z = z;
};

inline void Quaternion::mult(Quaternion q) {
  float pw = w * q.w - x * q.x - y * q.y - z * q.z;
  float px = w * q.x + x * q.w + y * q.z - z * q.y;
  float py = w * q.y - x * q.z + y * q.w + z * q.x;
  float pz = w * q.z + x * q.y - y * q.x + z * q.w;

  w = pw;
  x = px;
  y = py;
  z = pz;
};

inline void Quaternion::scale(float scalar) {
  w *= scalar;
  x *= scalar;
  y *= scalar;
  z *= scalar;
};

inline void Quaternion::add(Quaternion q) {
  w += q.w;
  x += q.x;
  y += q.y;
  z += q.z;
};

inline void Quaternion::sub(Quaternion q) {
  w -= q.w;
  x -= q.x;
  y -= q.y;
  z -= q.z;
}

inline void Quaternion::conjugate(void) {
  x *= -1;
  y *= -1;
  z *= -1;
};

inline float Quaternion::normal(void) {
  return sqrt(w * w + x * x + y * y + z * z);
};

inline void Quaternion::normalize(void) {
  float normal = this->normal();
  w /= normal;
  x /= normal;
  y /= normal;
  z /= normal;
};

inline Quaternion Quaternion::clone() { return Quaternion(w, x, y, z); };