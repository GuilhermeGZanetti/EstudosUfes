#include "my_transforms.h"
#include <math.h>
#include <stdio.h>

static float matTransform[4][4];


void mSetIdentity(){
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(i==j){
                matTransform[i][j] = 1.0;    
            } else {
                matTransform[i][j] = 0.0;
            }            
        }
    }
}

void mPrintMTransform(){
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            printf("%f ", matTransform[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void mTranslate(float dx, float dy, float dz){
    //Matriz 4x4
    // 1 0 0 dx
    // 0 1 0 dy
    // 0 0 1 dz
    // 0 0 0 1
    float matrizT[4][4] = 
    {{1.0, 0.0, 0.0, dx},
     {0.0, 1.0, 0.0, dy},
     {0.0, 0.0, 1.0, dz},
     {0.0, 0.0, 0.0, 1.0}
    };

    float matrizResult[4][4];

    //multiplica matTransform x matriz
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            matrizResult[i][j] = matTransform[i][0]*matrizT[0][j] + matTransform[i][1]*matrizT[1][j]+
                                 matTransform[i][2]*matrizT[2][j] + matTransform[i][3]*matrizT[3][j];
        }
    }

    //Copy result to matTransform
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            matTransform[i][j] = matrizResult[i][j];
        }
    }
}

void mRotate(float thetaX, float thetaY, float thetaZ){
    //Convertendo de graus pra radianos
    thetaZ = thetaZ*M_PI/180.0;

    float matrizRz[4][4] = 
    {{cos(thetaZ), -sin(thetaZ), 0.0, 0.0},
     {sin(thetaZ), cos(thetaZ), 0.0, 0.0},
     {0.0, 0.0, 1.0, 0.0},
     {0.0, 0.0, 0.0, 1.0}
    };

    float matrizResult[4][4];

    //multiplica matTransform x matriz
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            matrizResult[i][j] = matTransform[i][0]*matrizRz[0][j] + matTransform[i][1]*matrizRz[1][j]+
                                 matTransform[i][2]*matrizRz[2][j] + matTransform[i][3]*matrizRz[3][j];
        }
    }

    //Copy result to matTransform
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            matTransform[i][j] = matrizResult[i][j];
        }
    }
}

void mApplyToPoint(float *x, float *y, float *z){
    float newX = matTransform[0][0]*(*x) + matTransform[0][1]*(*y) + matTransform[0][2]*(*z) + matTransform[0][3]*(1);
    float newY = matTransform[1][0]*(*x) + matTransform[1][1]*(*y) + matTransform[1][2]*(*z) + matTransform[1][3]*(1);
    float newZ = matTransform[2][0]*(*x) + matTransform[2][1]*(*y) + matTransform[2][2]*(*z) + matTransform[2][3]*(1);

    *x=newX;
    *y=newY;
    *z=newZ;
}