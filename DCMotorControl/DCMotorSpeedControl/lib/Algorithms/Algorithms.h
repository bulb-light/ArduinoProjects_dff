#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <Arduino.h>

// NOTE:this functions returns the maximum value of an array
// and the maxIndex parameter is passed as reference
// that stores the maximum index of the array
float maxElement(float* array, int array_size, int &max_index);

#endif