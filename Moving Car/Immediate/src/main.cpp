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
    //glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
}

void Display(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // exhaust
    glBegin(GL_POLYGON);              
        glColor3f(0.566f, 0.566f, 0.566f);
        glVertex2f(-0.7f, -0.1f);    
        glVertex2f(-0.7f, -0.2f);
        glVertex2f(-0.6f, -0.2f);
        glVertex2f(-0.6f, -0.1f);
    glEnd();

    // body
    glBegin(GL_POLYGON);              
        glColor3f(1.0f, 0.0f, 0.0f); 
        glVertex2f(-0.6f, 0.15f);    
        glVertex2f(-0.6f, -0.25f);
        glVertex2f(0.75f, -0.25f);
        glVertex2f(0.75f, 0.15f);
    glEnd();

    // upper body
    glBegin(GL_POLYGON);              
      glColor3f(1.0f, 0.0f, 0.0f); 
      glVertex2f(-0.45f, 0.15f);    
      glVertex2f(0.5f, 0.15f);
      glVertex2f(0.35f, 0.4f);
      glVertex2f(-0.3f, 0.4f);
    glEnd();

    // back window
    glBegin(GL_POLYGON);              
      glColor3f(0.8f, 0.8f, 0.8f); 
      glVertex2f(-0.365f, 0.15f);    
      glVertex2f(0.0f, 0.15f);
      glVertex2f(0.0f, 0.35f);
      glVertex2f(-0.25f, 0.35f);
    glEnd();

    // front window
    glBegin(GL_POLYGON);              
      glColor3f(0.8f, 0.8f, 0.8f); 
      glVertex2f(0.075f, 0.15f);    
      glVertex2f(0.415f, 0.15f);
      glVertex2f(0.3f, 0.35f);
      glVertex2f(0.075f, 0.35f);
    glEnd();

    // back door
    glBegin(GL_POLYGON);              
      glColor3f(0.85f, 0.0f, 0.0f); 
      glVertex2f(-0.365f, 0.15f);    
      glVertex2f(-0.365f, -0.1f);
      glVertex2f(0.0f, -0.1f);
      glVertex2f(0.0f, 0.15f);
    glEnd();

    glBegin(GL_POLYGON);              
      glColor3f(0.85f, 0.0f, 0.0f); 
      glVertex2f(0.075f, 0.15f);    
      glVertex2f(0.075f, -0.1f);
      glVertex2f(0.415f, -0.1f);
      glVertex2f(0.415f, 0.15f);
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