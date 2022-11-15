#ifndef FIGHTER_H
#define	FIGHTER_H
#include <GL/gl.h>
#include <GL/glu.h>

// Dimensions
#define DISTANCE_NOSE 1.1 //Proportion from body radius
#define NOSE_SIZE 0.25 //Proportion from body radius


class Fighter {
    GLfloat gX; 
    GLfloat gY; 
    GLfloat gRadiusBody;
    GLfloat gAngleDirection;
    GLfloat gR; 
    GLfloat gG; 
    GLfloat gB; 
private:
    void DrawRect(  GLint height, GLint width, 
                    GLfloat R, GLfloat G, GLfloat B);
    void DrawCirc(  GLint radius, GLfloat R, 
                    GLfloat G, GLfloat B);
    void DrawFighter();

public:
    Fighter(GLfloat x, GLfloat y, GLfloat radius, GLfloat angle, GLfloat R, GLfloat G, GLfloat B){
        gX = x; 
        gY = y; 
        gRadiusBody = radius;
        gAngleDirection = angle;
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