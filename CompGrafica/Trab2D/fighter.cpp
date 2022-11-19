#include "fighter.h"
#include "my_transforms.h"
#include "auxFunctions.h"
#include <math.h>
#include <stdio.h>


void Fighter::DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B){
    glColor3f (R, G, B);
    glBegin(GL_POLYGON);
      glVertex3f (-width/2.0, 0.0, 0.0);
      glVertex3f (width/2.0, 0.0, 0.0);
      glVertex3f (width/2.0, height, 0.0);
      glVertex3f (-width/2.0, height, 0.0);
    glEnd();
}

void Fighter::DrawCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B){
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

    // Go to body center
    glTranslatef(gX, gY, 0);
    glRotatef(gAngleDirection-90, 0, 0, 1);  

    //Draw arms
    DrawRightArm(gRadiusBody, rightPunchStatus);
    DrawLeftArm(gRadiusBody, leftPunchStatus);

    //Draw body
    DrawCirc(gRadiusBody, gR, gG, gB);

    //Draw nose
    glPushMatrix();
    glTranslatef(0, gRadiusBody*DISTANCE_NOSE, 0);
    DrawCirc(gRadiusBody*NOSE_SIZE, gR, gG, gB);
    glPopMatrix();

    glPopMatrix();
}


void Fighter::DrawRightArm(GLfloat bodySize, GLfloat punchStatus){
    glPushMatrix();
    glTranslatef(bodySize*DISTANCE_ARM, 0, 0); //Goes to base of arm
    glRotatef(getShoulderAngle(punchStatus), 0, 0, 1);
    DrawRect(ARM_LENGTH*bodySize, ARM_WIDTH*bodySize, ARM_COLOR_R, ARM_COLOR_G, ARM_COLOR_B);
    glTranslatef(0, ARM_LENGTH*bodySize*DISTANCE_ARM, 0); //Goes to the elbow

    glRotatef(getElbowAngle(punchStatus), 0, 0, 1);
    DrawRect(ARM_LENGTH*bodySize, ARM_WIDTH*bodySize, ARM_COLOR_R, ARM_COLOR_G, ARM_COLOR_B);
    glTranslatef(0, ARM_LENGTH*bodySize, 0); //Goes to the hand

    //Draw glove
    DrawCirc(GLOVE_RADIUS*bodySize, GLOVE_COLOR_R, GLOVE_COLOR_G, GLOVE_COLOR_B);

    glPopMatrix();
}
    
void Fighter::DrawLeftArm(GLfloat bodySize, GLfloat punchStatus){
    glPushMatrix();
    glTranslatef(-bodySize*DISTANCE_ARM, 0, 0); //Goes to base of arm
    glRotatef(-getShoulderAngle(punchStatus), 0, 0, 1);
    DrawRect(ARM_LENGTH*bodySize, ARM_WIDTH*bodySize, ARM_COLOR_R, ARM_COLOR_G, ARM_COLOR_B);
    glTranslatef(0, ARM_LENGTH*bodySize*DISTANCE_ARM, 0); //Goes to the elbow

    glRotatef(-getElbowAngle(punchStatus), 0, 0, 1);
    DrawRect(ARM_LENGTH*bodySize, ARM_WIDTH*bodySize, ARM_COLOR_R, ARM_COLOR_G, ARM_COLOR_B);
    glTranslatef(0, ARM_LENGTH*bodySize, 0); //Goes to the hand

    //Draw glove
    DrawCirc(GLOVE_RADIUS*bodySize, GLOVE_COLOR_R, GLOVE_COLOR_G, GLOVE_COLOR_B);

    glPopMatrix();
}

GLfloat Fighter::getShoulderAngle(GLfloat punchStatus){
    //punchStatus = 0 - Angle = Base Angle
    //PunchStatus = 1 - Angle = 1/8 Base Angle
    return ((-7.0/8.0)*punchStatus + 1)*SHOULDER_BASE_ANGLE;
}

GLfloat Fighter::getElbowAngle(GLfloat punchStatus){
    //punchStatus = 0 - Angle = Base Angle
    //PunchStatus = 1 - Angle = 1/3 Base Angle
    return ((-2.0/3.0)*punchStatus + 1)*ELBOW_BASE_ANGLE;
}

void Fighter::RotateFighter(GLfloat rotationDegrees){
    gAngleDirection += rotationDegrees;
}

void Fighter::MoveFighter(GLfloat increment, GLfloat ViewingWidth, GLfloat ViewingHeight, Fighter* opponent){
    mSetIdentity();

    mTranslate(gX, gY, 0);
    mRotate(0, 0, gAngleDirection-90);
    mTranslate(0, increment, 0);

    float X=0, Y=0, Z=0;
    mApplyToPoint(&X, &Y, &Z);
    //printf("Antes: gX: %f gY: %f Increment: %f\n", gX, gY, increment);

    //Check collision with opponent
    float minDist = (opponent->ObtemRaio() + gRadiusBody*COLLISION_CIRCLE_RADIUS);
    if(getDistancePoints(X, Y, opponent->ObtemX(), opponent->ObtemY()) < minDist){
        //Get unitary vector pointing from opponent to my new (wrong) position
        float dx = X - opponent->ObtemX();
        float dy = Y - opponent->ObtemY();
        double dxUnitary = dx/getModuleVector(dx, dy);
        double dyUnitary = dy/getModuleVector(dx, dy);

        //Multiply it by the min Dist and get the resulting X and Y
        X = dxUnitary*minDist + opponent->ObtemX();
        Y = dyUnitary*minDist + opponent->ObtemY();
    }

    //Check collision with border
    if(X - gRadiusBody <= 0){ //Se passar da borda na esquerda
        X = gRadiusBody;
    }
    if(X + gRadiusBody >= ViewingWidth){ //Se passar da borda na direita
        X = ViewingWidth - gRadiusBody;
    }
    if(Y - gRadiusBody <= 0){ //Se passar da borda embaixo
        Y = gRadiusBody;
    }
    if(Y + gRadiusBody >= ViewingHeight){ //Se passar da borda embaixo
        Y = ViewingHeight - gRadiusBody;
    }
    
    gX = X;
    gY = Y;
    //printf("Depois: gX: %f gY: %f\n", gX, gY);
}

void Fighter::RecolheSoco(GLfloat timeDiference){
    if(rightPunchStatus > 0.0){
        this->DefineRightPunchStatus(rightPunchStatus - timeDiference*INC_RETURN_PUNCH);
    }
    if(leftPunchStatus > 0.0){
        this->DefineLeftPunchStatus(leftPunchStatus - timeDiference*INC_RETURN_PUNCH);
    }
}



void Fighter::DrawColisionCircle(){
    glPushMatrix();

    // Go to body center
    glTranslatef(gX, gY, 0);

    glPointSize(4);
    glColor3f (0, 0, 0);

    GLfloat radius = gRadiusBody*COLLISION_CIRCLE_RADIUS;
    glBegin(GL_POINTS);
        float angulo;
        for(angulo=0; angulo<360; angulo+=8){
            float xponto = radius*cos(angulo * (M_PI / 180.0));
            float yponto = radius*sin(angulo * (M_PI / 180.0));
            glVertex3f(xponto, yponto, 0.0);
        }     
    glEnd();


    glPopMatrix();
}