#include "robo.h"
#include <math.h>
#include <stdio.h>

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);
    glBegin(GL_POLYGON);
      glVertex3f (-width/2.0, 0.0, 0.0);
      glVertex3f (width/2.0, 0.0, 0.0);
      glVertex3f (width/2.0, height, 0.0);
      glVertex3f (-width/2.0, height, 0.0);
    glEnd();
}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPointSize(2);
    glColor3f (R, G, B);

    glBegin(GL_POINTS);
        int i;
        for(i=0; i<18; i++){
            float angulo = i*20;
            float xponto = radius*cos(angulo * (M_PI / 180.0));
            float yponto = radius*sin(angulo * (M_PI / 180.0));
            glVertex3f(xponto, yponto, 0.0);
        }            
    glEnd();

}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();

    glTranslatef(x, y, 0);
    glRotatef(thetaWheel, 0, 0, 1);    
    DesenhaCirc(radiusWheel, R, G, B);

    glPopMatrix();
}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();

    //Desenha pá 1    
    glTranslatef(x, y, 0.0);
    glRotatef(theta1, 0, 0, 1);
    DesenhaRect(paddleHeight, paddleWidth, 0, 0, 1);
    //Desenha pá 2 
    glTranslatef(0, paddleHeight, 0.0);
    glRotatef(theta2, 0, 0, 1);   
    DesenhaRect(paddleHeight, paddleWidth, 1, 1, 0);
    //Desenha pá 3 
    glTranslatef(0, paddleHeight, 0.0);
    glRotatef(theta3, 0, 0, 1);  
    DesenhaRect(paddleHeight, paddleWidth, 0, 1, 0);

    glPopMatrix();

}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();

    glTranslatef(x, y, 0.0);
    //Desenha corpo
    DesenhaRect(baseHeight, baseWidth, 1, 0, 0);

    //Desenha braço
    DesenhaBraco(0.0, baseHeight, theta1, theta2, theta3);

    //Desenha rodas
    DesenhaRoda(baseWidth/2.0, 0.0, thetaWheel, 1, 1, 1);
    DesenhaRoda(-baseWidth/2.0, 0.0, thetaWheel, 1, 1, 1);

    glPopMatrix();
}

void Robo::RodaBraco1(GLfloat inc)
{
    gTheta1 += inc;
}

void Robo::RodaBraco2(GLfloat inc)
{
    gTheta2 += inc;
}

void Robo::RodaBraco3(GLfloat inc)
{
    gTheta3 += inc;
}

void Robo::MoveEmX(GLfloat dx)
{
    //Move robo
    gX += dx;

    //Gira roda
    // 360 - 2*M_PI*radiusWheel
    // x radianos - dx
    GLfloat dgraus = - (dx * 360 / (2*M_PI*radiusWheel));
    gThetaWheel += dgraus;
}

//Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut){

}

Tiro* Robo::Atira()
{

}
