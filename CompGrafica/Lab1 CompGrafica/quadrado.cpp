#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

//Constants
#define TAMANHO_JANELA 500
#define INCREMENT_MOVE 0.005
#define OFFSET 0.25
#define SQUARE_SIZE 0.5

//Global Variables
float gX = 0.0;
float gY = 0.0;
int keyStatus[256];

//Drag event
int isDragging = 0;
float initialX = 0.0;
float initialY = 0.0;
float initialgX=0.0;
float initialgY=0.0;


void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
   glColor3f (1.0, 1.0, 1.0);
   /* Desenhar um polígono branco (retângulo) */
   glBegin(GL_POLYGON);
      glVertex3f (gX+OFFSET, gY+OFFSET, 0.0);
      glVertex3f (gX+OFFSET+SQUARE_SIZE, gY+OFFSET, 0.0);
      glVertex3f (gX+OFFSET+SQUARE_SIZE, gY+OFFSET+SQUARE_SIZE, 0.0);
      glVertex3f (gX+OFFSET, gY+OFFSET+SQUARE_SIZE, 0.0);
   glEnd();

   /* Desenhar no frame buffer! */
   glutSwapBuffers(); //Funcao apropriada para janela double buffer
}

void keyPress(unsigned char key, int x, int y){
   keyStatus[(int) key] = 1;
   glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y){
   keyStatus[(int) key] = 0;
   glutPostRedisplay();
}

void idle(void){
   //Move with keyboard
   if (keyStatus['w']) gY+=INCREMENT_MOVE;
   if (keyStatus['a']) gX-=INCREMENT_MOVE;
   if (keyStatus['s']) gY-=INCREMENT_MOVE;
   if (keyStatus['d']) gX+=INCREMENT_MOVE;

   
   glutPostRedisplay();
}

///mouse 3.2 
/*
void mouse(int button, int state, int x, int y){
   y = TAMANHO_JANELA - y;
   
   if(state==0){
      printf("Mouse: X %d, Y %d, State %d\n", x, y, state);
      gX = (float)x/(float)TAMANHO_JANELA - OFFSET;
      gY = (float)y/(float)TAMANHO_JANELA - OFFSET;
   }
   
   glutPostRedisplay();
} */

void mouse(int button, int state, int x, int y){
   y = TAMANHO_JANELA - y;
   
   if(state==0){
      printf("Mouse: X %d, Y %d, State %d\n", x, y, state);
      //Check if click is inside Square
      float X = (float)x/(float)TAMANHO_JANELA;
      float Y = (float)y/(float)TAMANHO_JANELA;
      if(X > gX+OFFSET && X < gX+OFFSET+SQUARE_SIZE &&
         Y > gY+OFFSET && Y < gY+OFFSET+SQUARE_SIZE){
            isDragging = 1;
            initialX = X;
            initialY = Y;
            initialgX = gX;
            initialgY = gY;
      }
   } else {
      isDragging = 0;
   }
   
   glutPostRedisplay();
}

void mouseMotion(int x, int y){
   y = TAMANHO_JANELA - y;

   //Move with dragging with mouse
   if(isDragging){
      float X = (float)x/(float)TAMANHO_JANELA;
      float Y = (float)y/(float)TAMANHO_JANELA;

      gX = initialgX + X-initialX;
      gY = initialgY + Y-initialY;
   }

   glutPostRedisplay();
}








///////////////////////////////////////////

void init (void) 
{
  /* selecionar cor de fundo (preto) */
  glClearColor (0.0, 0.0, 0.0, 0.0);

  /* inicializar sistema de visualizacao */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello world");

    //Register functions
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    init ();
    glutDisplayFunc(display); 

    glutMainLoop();

    return 0;
}
