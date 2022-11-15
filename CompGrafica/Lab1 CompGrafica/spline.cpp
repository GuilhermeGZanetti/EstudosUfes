#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define TAMANHO_JANELA 500
#define DISTANCE_DRAG_POINT 0.6

float size = 5.0;

int isDragging=0;
int pointDragged=-1;

//Pontos de controle da Spline
int numPoints = 4;
GLfloat ctrlpoints[4][3] = {
        { -4.0, -4.0, 0.0}, { -2.0, 4.0, 0.0}, 
        {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};

///////////////Funções auxiliares
int isCloseToPoint(float* point1, float* point2, float minDist){
   float distance = sqrtf(pow(point1[0]-point2[0], 2.0) + pow(point1[1]-point2[1], 2.0));
   //printf("Distância pro ponto: %f\n", distance);

   if(distance <= minDist){
      return 1;
   } 
   return 0;
}



///////////////Callbacks


void mouse(int button, int state, int x, int y){
   y = TAMANHO_JANELA - y;
   //Convert X and Y from 0-TAMANHO_JANELA to -5.0 to 5.0
   float X = x/50.0 - 5.0;
   float Y = y/50.0 - 5.0; 
   float click_point[2] = {X, Y};
   
   if(state==0){
      printf("Mouse: X %f, Y %f, State %d\n", X, Y, state);
      //Check if click is near a point (0,3 de diferença)
      int point = 0;
      for (point =0; point<numPoints; point++){
         if(isCloseToPoint(ctrlpoints[point] , click_point, DISTANCE_DRAG_POINT)){
            printf("Início drag ponto %d\n", point);
            isDragging = 1;
            pointDragged = point;
         }
      }
   } else {
      isDragging = 0;
      printf("Fim drag ponto %d\n", pointDragged);
   }
   
   glutPostRedisplay();
}

void mouseMotion(int x, int y){
   y = TAMANHO_JANELA - y;
   float X = x/50.0 - 5.0;
   float Y = y/50.0 - 5.0; 

   //Move with dragging with mouse
   if(isDragging){
      //Change point position to mouse position
      ctrlpoints[pointDragged][0] = X;
      ctrlpoints[pointDragged][1] = Y;
   }
   
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   glutPostRedisplay();
}




void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_MAP1_VERTEX_3);

   //Definicao do polinomio com os pontos de controle
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   
   //Muda para a matriz de projecao (aulas futuras)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-size, size, -size, size, -size, size);
}

void display(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);
   
   /* Desenha a curva aproximada por n+1 pontos. */
   int n = 30;
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= n; i++){
        //Avaliacao do polinomio, retorna um vertice (equivalente a um glVertex3fv) 
        glEvalCoord1f((GLfloat) i/(GLfloat)n);
      }
   glEnd();
   
   /* Desenha os pontos de controle. */
   glPointSize(5.0);
   glColor3f(1.0, 0.6, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();
   
   glutSwapBuffers();
}

void reshape(int w, int h)
{
   //Define a porcao visivel da janela
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   
   //Muda para a matriz de projecao (aulas futuras)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   //Controla o redimensionamento da janela mantendo o aspect ration do objeto
   if (w <= h)
      glOrtho(-size, size, -size*(GLfloat)h/(GLfloat)w, 
               size*(GLfloat)h/(GLfloat)w, -size, size);
   else
      glOrtho(-size*(GLfloat)w/(GLfloat)h, 
               size*(GLfloat)w/(GLfloat)h, -size, size, -size, size);
   
   //Muda para a matriz de trasformacoes (aulas futuras)
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutMainLoop();
   return 0;
}
