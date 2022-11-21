#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <time.h>       /* time */
#include "my_transforms.h"
#include "auxFunctions.h"
#include "tinyxml.h"
#include "fighter.h"
#define INC_KEY 1
#define INC_KEYIDLE 0.2
#define TIME_RUN_AWAY 2000


//Key status
int keyStatus[256];
int showColisionCircle = 0;
int enemyAIOn=1;

//Punch control
int isDragging = 0;
GLfloat initialX = 0;
int playerIsPunchingForward = 0;
int enemyState=0; 

// Window dimensions
GLint Width;
GLint Height;
// Viewing dimensions
GLint ViewingWidth;
GLint ViewingHeight;

//Score
int playerScore = 0;
int enemyScore = 0;
#define FONT_SIZE 0.1


//Componentes do mundo virtual sendo modelado
#define COLOR_HIT 0.5
Fighter *player;
#define PLAYER_INIT_R 0.2
#define PLAYER_INIT_G 0.64
#define PLAYER_INIT_B 0
Fighter *enemy;
#define ENEMY_INIT_R 0.86
#define ENEMY_INIT_G 0.39
#define ENEMY_INIT_B 0.015

//Main functions
void getInitialPosition(const char *filename);
void ResetKeyStatus();
void drawScores();
void controlEnemy(GLfloat timeDiference);

//Calbacks events
void keyPress(unsigned char key, int x, int y);
void keyup(unsigned char key, int x, int y);
void mouseClick(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void renderScene(void);
void idle(void);

//Init
void init(void);
 
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
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);

    init();
 
    glutMainLoop();
 
    return 0;
}


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
                player = new Fighter(c1X, c1Y, c1R, angle1, PLAYER_INIT_R, PLAYER_INIT_G, PLAYER_INIT_B);
                enemy = new Fighter(c2X, c2Y, c2R, angle2, ENEMY_INIT_R, ENEMY_INIT_G, ENEMY_INIT_B);
            } else { //Se o segundo círculo for o jogador
                //Cria o jogador e o inimigo
                enemy = new Fighter(c1X, c1Y, c1R, angle1, ENEMY_INIT_R, ENEMY_INIT_G, ENEMY_INIT_B);
                player = new Fighter(c2X, c2Y, c2R, angle2, PLAYER_INIT_R, PLAYER_INIT_G, PLAYER_INIT_B);
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

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}

void drawScores(){
    glPushMatrix();
    glPointSize(1);
    glColor3f (0, 0, 0);
    glScalef(FONT_SIZE, FONT_SIZE, FONT_SIZE);
    
    //Draw player score
    glPushMatrix();
    glTranslatef(50, 300, 0);
    char textScore[50];
    sprintf(textScore, "Player: %d", playerScore);
    int i=0;
    for(i=0; textScore[i]!= '\0'; i++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, textScore[i]);
        glTranslatef(50, 0, 0);
    }
    glPopMatrix();

    //Draw enemy score
    glPushMatrix();
    glTranslatef(50, 100, 0);
    sprintf(textScore, "Enemy: %d", enemyScore);
    i=0;
    for(i=0; textScore[i]!= '\0'; i++){
        glutStrokeCharacter(GLUT_STROKE_ROMAN, textScore[i]);
        glTranslatef(50, 0, 0);
    }
    glPopMatrix();

    glPopMatrix();
}

void controlEnemy(GLfloat timeDiference){
    //Enemy state //0 walking towards player // 1 Punching Right // 2 returning punch // 3 Punching Left // 4 returning punch //  5 Walking away from player
    static GLdouble timeStart;
    GLfloat dX, dY;

    switch (enemyState){ 
        case 0: //0 walking towards player
        case 5: //5 Walking away from player
            static GLfloat changeRand = rand()%(30);
            dX = player->ObtemX() - enemy->ObtemX();
            dY = player->ObtemY() - enemy->ObtemY();
            static GLfloat angleTarget = getAngleVector(dX, dY) + changeRand;
            if(enemy->TurnTowards(angleTarget, timeDiference/2)){
                if(changeRand > 0) changeRand = -(rand()%30);
                else changeRand = (rand()%30);

                dX = player->ObtemX() - enemy->ObtemX();
                dY = player->ObtemY() - enemy->ObtemY();
                angleTarget = getAngleVector(dX, dY) + changeRand;
            }

            if(enemyState == 0){
                //If distance to the player is less than a constant, stop walking towards him and starts punching
                if(getDistancePoints(player->ObtemX(), player->ObtemY(), enemy->ObtemX(), enemy->ObtemY()) <= 1.1*enemy->ObtemDistMinimaOponente(player)){
                    enemyState = 1;
                } else { //Walks towards player
                    enemy->Move(timeDiference, ViewingWidth, ViewingHeight, player);
                }
            } else { //state = 5
                //If distance to the player is more than a constant, stop walking away from him and starts approaching
                if(glutGet(GLUT_ELAPSED_TIME) - timeStart >= TIME_RUN_AWAY){
                    enemyState = 0;
                } else { //Walks away from player
                    enemy->Move(-timeDiference, ViewingWidth, ViewingHeight, player);
                }
            }

            break;
        case 1: // 1 Punching Right
            if(enemy->Soca(timeDiference, 0)){
                //finished extending punch
                enemyState = 2;
            }
            break;
        case 2: // 2 returning Punching Right
            if(enemy->RecolheSoco(timeDiference)){
                //Finished retruning punch
                enemyState = 3;
            }
            break;
        case 3: // 1 Punching Left
            if(enemy->Soca(timeDiference, 1)){
                //finished extending punch
                enemyState = 4;
            }
            break;
        case 4: // 4 returning Punching Left
            if(enemy->RecolheSoco(timeDiference)){
                //Finished retruning punch
                enemyState = 5;
                timeStart = glutGet(GLUT_ELAPSED_TIME);
            }
            break;
        default:
            break;
    }
}

void keyPress(unsigned char key, int x, int y) {
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
            isDragging = 0;
            break;
        case 's':
        case 'S':
            keyStatus[(int)('s')] = 1; //Using keyStatus trick
            isDragging = 0;
            break;
        case 'c':
        case 'C':
            //Mostra círculo de colisão
            showColisionCircle = !showColisionCircle;
            break;
        case 'v':
        case 'V':
            enemyAIOn = !enemyAIOn;
            break;
        case 27 :
            exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y) {
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y){
    if(state==0 && button == 0){
        //printf("SOCANDO! Mouse: X %d, Y %d, Button %d\n", x, y, button);
        if(keyStatus[(int)('w')] != 1 && keyStatus[(int)('s')] != 1){//Is not walking
            isDragging = 1;
            initialX = x; 
        }           
    } else {
        isDragging = 0;
        playerIsPunchingForward = 0;
    }

    glutPostRedisplay();
}

void mouseMotion(int x, int y){
    static GLfloat lastStatus = 0;
    //Move punch when dragging with mouse
    if(isDragging){
        GLfloat distancePunch = x - initialX;
        GLfloat status = abs(distancePunch)/(ViewingWidth/3);
        if(status > lastStatus){
            playerIsPunchingForward = 1;
        } else {
            playerIsPunchingForward = 0;
        }
        lastStatus = status;
        if(distancePunch > 0){ //Right punch
            player->DefineRightPunchStatus(status);
        }
        if(distancePunch < 0){ //Left punch
            player->DefineLeftPunchStatus(status);
        }
    }
    glutPostRedisplay();
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

    drawScores();


    glutSwapBuffers(); // Desenha the new frame of the game.
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

    //Recolhe soco se não estiver apertando o mouse
    if(!isDragging){
        player->RecolheSoco(timeDiference);
    }

    //Control the enemy (IA)
    if(enemyAIOn) controlEnemy(timeDiference);

    //Avalia colisão soco player
    static int playerHitting = 0;
    int isColliding = player->EstaAtingindoOponente(enemy);
    if(playerIsPunchingForward){
        if(isColliding && !playerHitting){ //Started collision
            //HIT!
            enemy->MudaCor(ENEMY_INIT_R*COLOR_HIT, ENEMY_INIT_G*COLOR_HIT, ENEMY_INIT_B*COLOR_HIT);
            playerScore++;
            printf("Player Score: %d\n", playerScore);
        }
    } 
    if(!isColliding && playerHitting){
        enemy->MudaCor(ENEMY_INIT_R, ENEMY_INIT_G, ENEMY_INIT_B);
    }
    playerHitting = isColliding;

    //Avalia colisão soco enemy
    static int enemyHitting = 0;
    isColliding = enemy->EstaAtingindoOponente(player);
    if(enemyState == 1 || enemyState == 3){ //If it is on a punching state
        if(isColliding && !enemyHitting){ //Started collision
            //HIT!
            player->MudaCor(PLAYER_INIT_R*COLOR_HIT, PLAYER_INIT_G*COLOR_HIT, PLAYER_INIT_B*COLOR_HIT);
            enemyScore++;
            printf("Enemy Score: %d\n", enemyScore);
        }
    } 
    if(!isColliding && enemyHitting){
        player->MudaCor(PLAYER_INIT_R, PLAYER_INIT_G, PLAYER_INIT_B);
    }
    enemyHitting = isColliding;    


    glutPostRedisplay();
}


void init(void)
{
    /* initialize random seed: */
    srand (time(NULL));

    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.78f, 0.78f, 0.78f, 1.0f);
    //glClearColor(0.0f, 0.0f, 0.0, 1.0f); // Black, no opacity(alpha).
 
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
