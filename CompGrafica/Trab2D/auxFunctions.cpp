#include "auxFunctions.h"

float getAngleVector(float x, float y){
    return 180*atan2f(y, x)/M_PI;
}

float getDistancePoints(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

double getModuleVector(float x, float y){
    return sqrt(pow(x, 2) + pow(y, 2));
}