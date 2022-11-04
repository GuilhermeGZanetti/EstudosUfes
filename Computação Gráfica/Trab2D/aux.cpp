#include "aux.h"

float getAngleVector(float x, float y){
    return 180*atan2f(y, x)/M_PI;
}