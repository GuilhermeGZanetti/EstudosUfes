#ifndef FIGHTER_H
#define	FIGHTER_H
#include <GL/gl.h>
#include <GL/glu.h>

// Dimensions
#define DISTANCE_NOSE 1.1 //Proportion from body radius
#define NOSE_SIZE 0.25 //Proportion from body radius
#define DISTANCE_ARM 0.95
#define ARM_LENGTH 1.85 //Proportion from body radius
#define ARM_WIDTH 0.3 //Proportion from body radius
#define GLOVE_RADIUS 0.5 //Proportion from body radius

#define SHOULDER_BASE_ANGLE -125
#define ELBOW_BASE_ANGLE 130

//Colors
#define ARM_COLOR_R 0
#define ARM_COLOR_G 0
#define ARM_COLOR_B 0
#define GLOVE_COLOR_R 0.73
#define GLOVE_COLOR_G 0.0
#define GLOVE_COLOR_B 0.0




class Fighter {
    //Position
    GLfloat gX; 
    GLfloat gY; 
    GLfloat gAngleDirection;

    //Size
    GLfloat gRadiusBody;

    //Punch Status 0 - not punching / 1 - arm fully extended
    GLfloat rightPunchStatus;
    GLfloat leftPunchStatus;

    //Colors
    GLfloat gR; 
    GLfloat gG; 
    GLfloat gB; 
private:
    void DrawRect(  GLint height, GLint width, 
                    GLfloat R, GLfloat G, GLfloat B);
    void DrawCirc(  GLint radius, GLfloat R, 
                    GLfloat G, GLfloat B);
    void DrawFighter();

    void DrawRightArm(GLfloat bodySize, GLfloat punchStatus);
    
    void DrawLeftArm(GLfloat bodySize, GLfloat punchStatus);

    GLfloat getShoulderAngle(GLfloat punchStatus);
    GLfloat getElbowAngle(GLfloat punchStatus);

public:
    Fighter(GLfloat x, GLfloat y, GLfloat radius, GLfloat angle, GLfloat R, GLfloat G, GLfloat B){
        gX = x; 
        gY = y; 
        gRadiusBody = radius;
        gAngleDirection = angle;
        leftPunchStatus = 0.0;
        rightPunchStatus = 0.0;
        gR = R; 
        gG = G; 
        gB = B; 
    };
    void Desenha(){ 
        DrawFighter();
    };
    GLfloat ObtemX(){
        return gX;
    };
    GLfloat ObtemY(){
        return gY;
    };
};

#endif	/* FIGHTER_H */