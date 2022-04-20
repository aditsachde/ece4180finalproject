#include "Quaternion.h"

void Quaternion::euler(float *roll, float *pitch, float *yaw) {
  *roll =
      atan2((2 * x * y - 2 * w * z), (2 * w * w + 2 * x * x - 1)) * RAD_TO_DEG;
  *pitch = -1 * asinf(2 * x * z + 2 * w * y) * RAD_TO_DEG;
  *yaw =
      atan2((2 * y * z - 2 * w * x), (2 * w * w + 2 * z * z - 1)) * RAD_TO_DEG;
}
