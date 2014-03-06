#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "sgl.h"
#include "st.h"

using namespace std;

STImage* buff;

int win_w;
int win_h;

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT );

	// --- Make drawing calls here ---+

        sglLoadIdentity();

        sglBeginTriangles();
        sglColor(0.0f,1.0f,0.f);
        sglVertex(1.0*win_w/2,1.0*win_h*6/10);
        sglVertex(1.0*win_w/2-1.0*win_w/3,1.0*win_h*2/10);
        sglVertex(1.0*win_w/2+1.0*win_w/5,1.0*win_h*2/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(0.0f,0.5f,0.f);
        sglVertex(1.0*win_w/2,1.0*win_h*6/10);
        sglVertex(1.0*win_w/2+1.0*win_w/5,1.0*win_h*2/10);
        sglVertex(1.0*win_w/2+1.0*win_w/3,1.0*win_h*2/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(0.0f,1.0f,0.f);
        sglVertex(1.0*win_w/2,1.0*win_h*8/10);
        sglVertex(1.0*win_w/2-1.0*win_w/5,1.0*win_h*5/10);
        sglVertex(1.0*win_w/2+1.0*win_w/10,1.0*win_h*5/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(0.0f,0.5f,0.f);
        sglVertex(1.0*win_w/2,1.0*win_h*8/10);
        sglVertex(1.0*win_w/2+1.0*win_w/10,1.0*win_h*5/10);
        sglVertex(1.0*win_w/2+1.0*win_w/5,1.0*win_h*5/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(0.0f,1.0f,0.f);
        sglVertex(1.0*win_w/2,1.0*win_h*9/10);
        sglVertex(1.0*win_w/2-1.0*win_w/10,1.0*win_h*7/10);
        sglVertex(1.0*win_w/2+1.0*win_w/15,1.0*win_h*7/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(0.0f,0.5f,0.f);
        sglVertex(1.0*win_w/2,1.0*win_h*9/10);
        sglVertex(1.0*win_w/2+1.0*win_w/15,1.0*win_h*7/10);
        sglVertex(1.0*win_w/2+1.0*win_w/10,1.0*win_h*7/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(1.0f,0.f,0.f);
        sglVertex(1.0*win_w/2-1.0*win_w/10,1.0*win_h*2/10);
        sglVertex(1.0*win_w/2-1.0*win_w/10,1.0*win_h*1/10);
        sglVertex(1.0*win_w/2+(1.0*win_w/10-win_w/15),1.0*win_h*1/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(1.0f,0.f,0.f);
        sglVertex(1.0*win_w/2-1.0*win_w/10,1.0*win_h*2/10);
        sglVertex(1.0*win_w/2+(1.0*win_w/10-win_w/15),1.0*win_h*2/10);
        sglVertex(1.0*win_w/2+(1.0*win_w/10-win_w/15),1.0*win_h*1/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(0.5f,0.f,0.f);
        sglVertex(1.0*win_w/2+(1.0*win_w/10-win_w/15),1.0*win_h*2/10);
        sglVertex(1.0*win_w/2+(1.0*win_w/10-win_w/15),1.0*win_h*1/10);
        sglVertex(1.0*win_w/2+1.0*win_w/10,1.0*win_h*2/10);
        sglEnd();

        sglBeginTriangles();
        sglColor(0.5f,0.f,0.f);
        sglVertex(1.0*win_w/2+(1.0*win_w/10-win_w/15),1.0*win_h*1/10);
        sglVertex(1.0*win_w/2+1.0*win_w/10,1.0*win_h*2/10);
        sglVertex(1.0*win_w/2+1.0*win_w/10,1.0*win_h*1/10);
        sglEnd();
	// --- End of drawing calls ------+

	buff->Draw();
	glutSwapBuffers();
}

void reshape( int w, int h )
{
  win_w = w;
  win_h = h;
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  glOrtho( 0., w, 0., h, -1., 1. );
  glViewport( 0, 0, w, h );
  setBufferSize(w, h);

  glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y )
{
	switch(key)
	{
	case 27: // Escape key
		exit(0);
		break;
	case 's': // Save
		buff->Save("output.png");
		break;
	}
}

int main (int argc, char *argv[])
{
	int win_width = 512;
	int win_height = 512;
	buff = new STImage(win_width, win_height, STColor4ub(0, 0, 0, 255));
	setBuffer(buff);

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize( win_width, win_height );

	glutCreateWindow( "Intro Graphics Assignment 2" );

	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );

	glutMainLoop();

	delete buff;
}
