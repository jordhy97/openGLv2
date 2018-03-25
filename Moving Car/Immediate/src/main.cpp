#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

GLint WINDOW_WIDTH = 800;
GLint WINDOW_HEIGHT = 800;
const float PI = 3.14159265;
const int FPS = 20;

void display(void);
void keyboard(unsigned char key, int x, int y);
void timer(int);

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Moving Car");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1000.0/FPS, timer, 0);
    glutMainLoop();
    return 0;
}

void display(void) {
	  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
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


    // wheels
    int num_segments = 360;

    // back wheel
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(-0.365f, -0.25f, 0.0f);
    glRotatef(glutGet(GLUT_ELAPSED_TIME), 0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
      glColor3f(0.0f, 0.0f, 0.0f);
      for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * i / float(num_segments);
        float x = 0.13f * cos(theta);
        float y = 0.13f * sin(theta);

        glVertex2f(x, y);
      }
    glEnd();

    for (int i = 0; i < 6; i++) {
      glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        for (int j = i * 60 + 8; j <= i * 60 + 52; j++) {
          float theta = 2.0f * PI * j / float(num_segments);
          float x = 0.09f * cos(theta);
          float y = 0.09f * sin(theta);
          glVertex2f(x, y);
        }
      glEnd();
    }

    glBegin(GL_POLYGON);
      glColor3f(0.0f, 0.0f, 0.0f);
      for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * i / float(num_segments);
        float x = 0.025f * cos(theta);
        float y = 0.025f * sin(theta);

        glVertex2f(x, y);
      }
    glEnd();
    glPopMatrix();

    // front wheel
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.415f, -0.25f, 0.0f);
    glRotatef(glutGet(GLUT_ELAPSED_TIME), 0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
      glColor3f(0.0f, 0.0f, 0.0f);
      for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * i / float(num_segments);
        float x = 0.13f * cos(theta);
        float y = 0.13f * sin(theta);

        glVertex2f(x, y);
      }
    glEnd();

    for (int i = 0; i < 6; i++) {
      glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        for (int j = i * 60 + 8; j <= i * 60 + 52; j++) {
          float theta = 2.0f * PI * j / float(num_segments);
          float x = 0.09f * cos(theta);
          float y = 0.09f * sin(theta);
          glVertex2f(x, y);
        }
      glEnd();
    }

    glBegin(GL_POLYGON);
      glColor3f(0.0f, 0.0f, 0.0f);
      for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * i / float(num_segments);
        float x = 0.025f * cos(theta);
        float y = 0.025f * sin(theta);

        glVertex2f(x, y);
      }
    glEnd();
    glPopMatrix();

    glFinish();
}

void keyboard(unsigned char key, int x, int y) {
	#define ESCAPE '\033'

    if( key == ESCAPE )
        exit(0);
}

void timer(int) {
    /* update animation */
    glutPostRedisplay();
    glutTimerFunc(1000.0/FPS, timer, 0);
}
