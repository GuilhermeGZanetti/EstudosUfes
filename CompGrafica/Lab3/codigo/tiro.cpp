#include "tiro.h"
#include "my_transforms.h"
#include <math.h>
#include <stdio.h>
#define DISTANCIA_MAX 500

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPointSize(1);
    glColor3f (R, G, B);

    glBegin(GL_POLYGON);
        int i;
        for(i=0; i<18; i++){
            float angulo = i*20;
            float xponto = radius*cos(angulo * (M_PI / 180.0));
            float yponto = radius*sin(angulo * (M_PI / 180.0));
            glVertex3f(xponto, yponto, 0.0);
        }            
    glEnd();
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x, y, 0);   
    DesenhaCirc(radiusTiro, 1, 1, 1);

    glPopMatrix();

}

void Tiro::Move(GLdouble timeDiference){
    float x=0.0, y=0.0, z;
    mSetIdentity();
    mTranslate(gX, gY, 0);
    mRotate(0.0, 0.0, gDirectionAng-90);
    mTranslate(0.0, timeDiference*gVel, 0.0);
    mApplyToPoint(&x, &y, &z);
    gX = x;
    gY = y;
}

bool Tiro::Valido()
{
    GLfloat dist = sqrt(pow(gX-gXInit, 2.0) + pow(gY-gYInit, 2.0));
    if(dist > DISTANCIA_MAX){
        return false;
    }
    return true;    
}
