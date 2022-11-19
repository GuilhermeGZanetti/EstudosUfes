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
#define COLLISION_CIRCLE_RADIUS 3

//Colors
#define ARM_COLOR_R 0
#define ARM_COLOR_G 0
#define ARM_COLOR_B 0
#define GLOVE_COLOR_R 0.73
#define GLOVE_COLOR_G 0.0
#define GLOVE_COLOR_B 0.0

//Increments
#define INC_ROTATE 0.2
#define INC_MOVE 0.2
#define INC_RETURN_PUNCH 0.01




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
    void RotateFighter(GLfloat rotationDegrees);
    void MoveFighter(GLfloat increment, GLfloat ViewingWidth, GLfloat ViewingHeight, Fighter* opponent);

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
    void Gira(GLfloat timeDiference){ 
        RotateFighter(timeDiference*INC_ROTATE);
    };
    void Move(GLfloat timeDiference, GLfloat ViewingWidth, GLfloat ViewingHeight, Fighter* opponent){ 
        MoveFighter(timeDiference*INC_MOVE, ViewingWidth, ViewingHeight, opponent);
    };
    void RecolheSoco(GLfloat timeDiference);
    int EstaAtingindoOponente(Fighter *opponent);

    void DrawColisionCircle();

    GLfloat ObtemX(){
        return gX;
    };
    GLfloat ObtemY(){
        return gY;
    };
    GLfloat ObtemRaio(){
        return gRadiusBody;
    };

    void MudaCor(GLfloat R, GLfloat G, GLfloat B){ gR = R; gG = G; gB=B; };


    void DefineRightPunchStatus(GLfloat status){
        if(status > 1) status = 1;
        if(status < 0) status = 0;
        rightPunchStatus = status;
    };
    void DefineLeftPunchStatus(GLfloat status){
        if(status > 1) status = 1;
        if(status < 0) status = 0;
        leftPunchStatus = status;
    };
};

#endif	/* FIGHTER_H */