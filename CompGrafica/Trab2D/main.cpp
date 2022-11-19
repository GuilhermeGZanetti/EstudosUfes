#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include "my_transforms.h"
#include "auxFunctions.h"
#include "tinyxml.h"
#include "fighter.h"
#define INC_KEY 1
#define INC_KEYIDLE 0.2

//Key status
int keyStatus[256];
int showColisionCircle = 0;

// Window dimensions
GLint Width;
GLint Height;
// Viewing dimensions
GLint ViewingWidth;
GLint ViewingHeight;


//Componentes do mundo virtual sendo modelado
Fighter *player;
Fighter *enemy;


void getInitialPosition(const char *filename){
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
        TiXmlElement *root = doc.FirstChildElement("svg");
        if(root){
            TiXmlElement *rectArena = root->FirstChildElement("rect");
            int rectX = std::stoi(rectArena->Attribute("x"));
            int rectY = std::stoi(rectArena->Attribute("y"));
            int rectwidth = std::stoi(rectArena->Attribute("width"));
            int rectheight = std::stoi(rectArena->Attribute("height"));
            //printf("Rect:\nx:%d y:%d width:%d height:%d\n", rectX, rectY, rectwidth, rectheight);

            //Get first circle
            TiXmlElement *circle1 = root->FirstChildElement("circle");
            int c1X = std::stoi(circle1->Attribute("cx")) - rectX;
            int c1Y = rectheight - (std::stoi(circle1->Attribute("cy")) - rectY);
            int c1R = std::stoi(circle1->Attribute("r"));
            const char *c1fill = circle1->Attribute("fill");
            //printf("Circle 1:\nx:%d y:%d radius:%d fill:%s\n", c1X, c1Y, c1R, c1fill);

            //Get second circle
            TiXmlElement *circle2 = circle1->NextSiblingElement("circle");
            int c2X = std::stoi(circle2->Attribute("cx")) - rectX;
            int c2Y = rectheight - (std::stoi(circle2->Attribute("cy")) - rectY);
            int c2R = std::stoi(circle2->Attribute("r"));
            const char *c2fill = circle2->Attribute("fill");
            //printf("Circle 2:\nx:%d y:%d radius:%d fill:%s\n", c2X, c2Y, c2R, c2fill);

            //Define as dimensões da tela e do viewport
            Width = rectwidth;
            Height = rectheight;
            ViewingWidth = Width;
            ViewingHeight = Height;

            //Encontra os angulos iniciais dos lutadores (se encarando)
            GLfloat angle1, angle2=0;
            //Encontra vetor do c1 pro c2
            GLfloat dx = c2X - c1X;
            GLfloat dy = c2Y - c1Y;
            angle1 = getAngleVector(dx, dy);
            angle2 = angle1 - 180;            


            if(strcmp(c1fill, "green") == 0){ //Se o prímeiro circulo for o jogador
                //Cria o jogador e o inimigo
                player = new Fighter(c1X, c1Y, c1R, angle1, 0.2, 0.64, 0);
                enemy = new Fighter(c2X, c2Y, c2R, angle2, 0.86, 0.39, 0.015);
            } else { //Se o segundo círculo for o jogador
                //Cria o jogador e o inimigo
                enemy = new Fighter(c1X, c1Y, c1R, angle1, 0.86, 0.39, 0.015);
                player = new Fighter(c2X, c2Y, c2R, angle2, 0.2, 0.64, 0);
            }
        } else {
            printf("ERRO na leitura do SVG!\n");
            exit(2);
        }
	}
	else
	{
		printf("Failed to load file \"%s\"\n", filename);
	}
    
}

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw player
    player->Desenha();
    //Draw enemy
    enemy->Desenha();

    if(showColisionCircle){
        player->DrawColisionCircle();
        enemy->DrawColisionCircle();
    }


    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
             keyStatus[(int)('a')] = 1; //Using keyStatus trick
             break;
        case 'd':
        case 'D':
             keyStatus[(int)('d')] = 1; //Using keyStatus trick
             break;
        case 'w':
        case 'W':
             keyStatus[(int)('w')] = 1; //Using keyStatus trick
             break;
        case 's':
        case 'S':
             keyStatus[(int)('s')] = 1; //Using keyStatus trick
             break;
        case 'c':
        case 'C':
             //Mostra círculo de colisão
             showColisionCircle = !showColisionCircle;
             break;
        case 27 :
             exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}

void init(void)
{
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.78f, 0.78f, 0.78f, 1.0f); // Black, no opacity(alpha).
 
    glMatrixMode(GL_PROJECTION); // Select the projection matrix 
    glOrtho(0,     // X coordinate of left edge            
            (ViewingWidth),     // X coordinate of right edge           
            0,     // Y coordinate of bottom edge      
            (ViewingHeight),     // Y coordinate of top edge            
            -100,     // Z coordinate of the “near” plane           
            100);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix  
    glLoadIdentity();
      
}

void idle(void)
{
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime, timeDiference;
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDiference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;

    //Treat keyPress
    if(keyStatus[(int)('a')])
    {
        player->Gira(timeDiference);
    }
    if(keyStatus[(int)('d')])
    {
        player->Gira(-timeDiference);
    }
    if(keyStatus[(int)('w')])
    {
        player->Move(timeDiference, ViewingWidth, ViewingHeight, enemy);
    }
    if(keyStatus[(int)('s')])
    {
        player->Move(-timeDiference, ViewingWidth, ViewingHeight, enemy);
    }

    

    glutPostRedisplay();
}
 
int main(int argc, char *argv[])
{
    if(argc<2){
        printf("ERRO: Forneça o arquivo SVG para configuração inicial\n");
        return 1;
    }
    getInitialPosition(argv[1]);


    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 
    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Trab 2D");
 
    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    
    init();
 
    glutMainLoop();
 
    return 0;
}
