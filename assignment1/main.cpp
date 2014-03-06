#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1415926

using namespace std;

int win_width = 512;
int win_height = 512;
int draw_mode=2;

int Spirograph_R = 60;
int Spirograph_r = 60;
int Spirograph_p = 60;

typedef struct RGB
{
    int r;
    int g;
    int b;
} RGB;

class Ball{
private:
    float Size;
    RGB Color;
    float x;
    float vx;
    float avx;
    float y;
    float vy;
    float avy;
    float attenx;
    float atteny;
public:
    Ball(float s)
    {
        Size = s;
        Color.r = rand()%255;
        Color.g = rand()%255;
        Color.b = rand()%255;
        x = 1.0*(rand()%100-50)/50;
        y = 1.0*(rand()%100-50)/50;
        vx = 0;
        avx = 0;
        vy = 0;
        avy = -0.0005;
        atteny = 0.002;
        attenx = 0.0005;
    };

    void draw()
    {
        x += vx;
        y += vy;
        vy += avy;
        vx += avx;
        if(x<=-1)
        {
            vx = -vx-attenx;
            if(vx<=0)
            {
                vx = 0;
                x = -1;
            }
        }
        if(x>=1)
        {
            vx = -vx+attenx;
            if(vx>=0)
            {
                vx = 0;
                x = 1;
            }
        }
        if(y<=-1)
        {
            vy = (-vy>atteny)? -vy-atteny : 0;
            y = -1;
        }
        glColor3ub(Color.r,Color.g,Color.b);
        glPointSize(Size);
        glBegin(GL_POINTS);
        glVertex3f(x,y,0);
        glEnd();
    };

    void push()
    {
        vx += (1.0*rand()/RAND_MAX)*0.02;
        vy += (1.0*rand()/RAND_MAX)*0.02;
    }

};

vector<Ball*> balls;

void drawSpirograph();
void drawBall();

void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT );

  switch(draw_mode){
      case 1:// draw Spirograph
          drawSpirograph();
          break;

      case 2:// MovingBall
          drawBall();
          break;

  }
  glutSwapBuffers();
}

void drawSpirograph()
{
    glColor3f(1.0,0,0);

    Spirograph_p++;
    if(Spirograph_p>100)
        Spirograph_p = -100;

    glBegin(GL_LINES);
    for(float t=0;t<500*PI;t+=0.01)
    {
        float x=1.0*(Spirograph_R+Spirograph_r)*cos(t) + 1.0*Spirograph_p*cos((Spirograph_R+t)/Spirograph_r);
        float y=1.0*(Spirograph_R+Spirograph_r)*sin(t) + 1.0*Spirograph_p*sin((Spirograph_R+t)/Spirograph_r);
        glVertex3f(x/180.0,y/180.0,0);
    }
    glEnd();
}

void drawBall()
{
    int j = balls.size();
    for(int i=0;i<balls.size();i++)
    {
        balls[i]->draw();
    }
}

void pushBall()
{
    int j = balls.size();
    for(int i=0;i<balls.size();i++)
    {
        balls[i]->push();
    }
}

void reshape( int w, int h )
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( -1.0, 1.0, -1.0, 1.0, -1., 1. );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport( 0, 0, w, h );

  win_width = w;
  win_height = h;

  glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) {
  switch(key) {
      case '1':// Press number keys to switch the draw mode
      {
        draw_mode=1;
        Spirograph_R = rand()%200-100;
        Spirograph_r = rand()%200-100;
        Spirograph_p = rand()%200-100;
        glutPostRedisplay();
        break;
      }
      case '2':
      case '3':
      case '4':
      case '5':
          draw_mode=key-'0';
          glutPostRedisplay();
          break;
      case 27: // Escape key
          exit(0);
          break;
  }
}

void help()
{
    printf("Press:\n");
    printf("1: Spirograph\n");
    printf("2: Drop balls\n");
}

void timer(int e)
{
    glutPostRedisplay();
    glutTimerFunc(10,timer,0);
}

void mouse(int button, int state, int x, int y)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_UP)
        {
            Ball *b = new Ball(1.0*(rand()%20));
            balls.push_back(b);
        }
        break;
    case GLUT_RIGHT_BUTTON:
        pushBall();
        break;
    }
}

void init()
{
    glClearColor(1,1,1,1);
    glEnable(GL_POINT_SMOOTH);

    for(int i=0;i<50;i++)
    {
        Ball *b = new Ball(1.0*(rand()%20));
        balls.push_back(b);
    }

}

int main (int argc, char *argv[]) {
  help();
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize( win_width, win_height );

  glutCreateWindow( "Intro Graphics Assignment 1" );

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( keyboard );
  glutMouseFunc(mouse);

  glutTimerFunc(10,timer,0);

  init();

  glutMainLoop();
}
