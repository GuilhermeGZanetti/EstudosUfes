#include "alvo.h"
#include <stdio.h>
#include <math.h>

void Alvo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
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

void Alvo::DesenhaAlvo(GLfloat x, GLfloat y)
{
    glPushMatrix();

    glTranslatef(x, y, 0);  
    switch(gColor){
        case 0:
            DesenhaCirc(radiusAlvo, 1, 0, 0);
            break;
        case 1:
            DesenhaCirc(radiusAlvo, 0, 1, 0);
            break;
        case 2:
            DesenhaCirc(radiusAlvo, 0, 0, 1);
            break;
    } 
    

    glPopMatrix();
}

void Alvo::Recria(GLfloat x, GLfloat y)
{
    gX=x;
    gY=y;
    gColor=(gColor+1)%3;
}

bool Alvo::Atingido(Tiro *tiro)
{
    GLfloat xtiro, ytiro;
    tiro->GetPos(xtiro, ytiro);

    GLfloat dist_tiro_alvo = sqrt(pow(gX-xtiro, 2.0) + pow(gY-ytiro, 2.0));
    if(dist_tiro_alvo < (radiusAlvo+radiusTiro)){
        return true;
    }
    return false;
}
