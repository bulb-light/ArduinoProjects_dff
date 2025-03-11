#include "Algorithms.h"

float maxElement(float* array, int array_size, int &max_index)
{
    float largest = array[0];
    for (int i = 0; i < array_size; i++) {
        if(array[i] > largest) {
            largest = array[i];
            max_index = i;
        }
    }
    return largest;
}