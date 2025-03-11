#include "MagneticEncoder.h"

// globals method 1
long prevT = 0;
int posPrev = 0;
// filter global vars
float vFilt = 0;
float vPrev = 0;

float readSpeed(int pos)
{
  long currT = micros();
  float deltaT = ((float) (currT - prevT)) / 1.0e6;
  float velocity1 = (pos - posPrev)/deltaT;
  posPrev = pos;
  prevT = currT;

  // convert counts/s to RPM
  // 11.0 PPR (encoder) x 2 - Ratio (9.3:1) - 60.0 seconds
  velocity1 = (velocity1/22.0) * (1.0/9.3) * 60.0;
  // lowpass filter
  vFilt = 0.7265*vFilt + 0.1367*velocity1 + 0.1367*vPrev;
  // vFilt = 0.8816*vFilt + 0.056*velocity1 + 0.056*vPrev;
  vPrev = velocity1;

  // store unfiltered and filtered velocity
  //velocity[0] = velocity1; velocity[1] = vFilt;

  // return filtered velocity
  return vFilt;
}