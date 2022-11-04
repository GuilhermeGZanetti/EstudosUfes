#include "fighter.h"
#include "my_transforms.h"
#include <math.h>
#include <stdio.h>


void Fighter::DrawRect(  GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B){

}

void Fighter::DrawCirc(  GLint radius, GLfloat R, GLfloat G, GLfloat B){
    glPointSize(1);
    glColor3f (R, G, B);

    glBegin(GL_POLYGON);
        float angulo;
        for(angulo=0; angulo<360; angulo+=5){
            float xponto = radius*cos(angulo * (M_PI / 180.0));
            float yponto = radius*sin(angulo * (M_PI / 180.0));
            glVertex3f(xponto, yponto, 0.0);
        }            
    glEnd();
}

void Fighter::DrawFighter(){
    glPushMatrix();

    //Draw body
    glTranslatef(gX, gY, 0);
    DrawCirc(gRadiusBody, gR, gG, gB);

    //Draw nose
    glRotatef(gAngleDirection-90, 0, 0, 1);  
    glTranslatef(0, gRadiusBody*DISTANCE_NOSE, 0);
    DrawCirc(gRadiusBody*NOSE_SIZE, gR, gG, gB);


    glPopMatrix();
}
