#include "robo.h"
#include "my_transforms.h"
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

//Funcao auxiliar para encontrar a posição do ponto de tiro e a direção
void encontraPosicaoInicialTiro(float xRobo, float theta1, float theta2, float theta3, float *gXInit, float *gYInit, float *gDirectionAng){
    ///// TESTE //////////
    float X=0, Y=0, Z=0;
    float X2=0, Y2=0, Z2=0;

    mSetIdentity();
    mTranslate(xRobo, -200, 0.0);
    mTranslate(0.0, baseHeight, 0.0);
    mRotate(0,0,theta1);
    mTranslate(0.0, paddleHeight, 0);
    mRotate(0,0,theta2);
    mTranslate(0.0, paddleHeight, 0);
    mApplyToPoint(&X, &Y, &Z);

    /*
    glPointSize(6);
    glColor3f (1, 0, 0);
    glBegin(GL_POINTS);
        glVertex3f(X, Y, Z);    
    glEnd();*/

    mRotate(0,0,theta3);
    mTranslate(0.0, paddleHeight, 0);
    mApplyToPoint(&X2, &Y2, &Z2);

    /*
    glPointSize(6);
    glColor3f (1, 1, 1);
    glBegin(GL_POINTS);
        glVertex3f(X2, Y2, Z2);
    glEnd();*/

    *gXInit = X2;
    *gYInit = Y2;
    float dx = X2-X;
    float dy = Y2-Y;
    *gDirectionAng = (atan(dy/dx) * 180/M_PI);
    if(*gDirectionAng <0){
        *gDirectionAng += 180;
    } else if(*gDirectionAng == 0.0){
        if(dx < 0){
            *gDirectionAng = 180;
        }
    }
}

Tiro* Robo::Atira()
{
    float gXInit, gYInit, gDirectionAng;
    encontraPosicaoInicialTiro(gX, gTheta1, gTheta2, gTheta3, &gXInit, &gYInit, &gDirectionAng);

    Tiro *tiro = new Tiro(gXInit, gYInit, gDirectionAng);
    return tiro;    
}
