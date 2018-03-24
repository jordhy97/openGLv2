#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

GLint WINDOW_WIDTH = 800;
GLint WINDOW_HEIGHT = 800;

void Display(void);
void Reshape(GLint w, GLint h);
void Keyboard(unsigned char key, int x, int y);

main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Moving Car");

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
}

void Display(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3ub(255,0,0);
    glBegin(GL_TRIANGLES);
      glVertex2f(0,0);
      glVertex2f(800,800);
      glVertex2f(160,360);
    glEnd();

    glFinish();
}

void Reshape(GLint w, GLint h)
{
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	#define ESCAPE '\033'

    if( key == ESCAPE )
        exit(0);
}