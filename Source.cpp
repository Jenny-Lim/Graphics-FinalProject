// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

// Source.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//hello
#define FILENAME "img.tif"

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>

int a[3] = { 10,10,10 }, b[3] = { 10,-10,10 }, c[3] = { -10,-10,10 }, d[3] = { -10,10,10 },
e[3] = { 10,10,-10 }, f[3] = { 10,-10,-10 }, g[3] = { -10,-10,-10 }, h[3] = { -10,10,-10 };

float angle = 0.0;

void drawcube(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex3iv(a);
    glVertex3iv(b);
    glVertex3iv(c);
    glVertex3iv(d);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3iv(a);
    glVertex3iv(e);
    glVertex3iv(f);
    glVertex3iv(b);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3iv(d);
    glVertex3iv(h);
    glVertex3iv(g);
    glVertex3iv(c);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3iv(e);
    glVertex3iv(f);
    glVertex3iv(g);
    glVertex3iv(h);
    glEnd();
    glutSwapBuffers();
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case 0x1B:
    case 'q':
    case 'Q':
        exit(0);
        break;
    }
}

void mouse(int btn, int state, int x, int y) {

    printf("%3d, %3d, %f\n", btn, state, angle);
    if (state == GLUT_DOWN) {
        if (btn == GLUT_LEFT_BUTTON) {
            angle = angle + 1.0;
        }
        else if (btn == GLUT_RIGHT_BUTTON) {
            angle = angle - 1.0;
        }
        else {
            angle = 0.0;
        }
    }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Glut rotate");
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawcube);
    glutIdleFunc(drawcube);

    glMatrixMode(GL_PROJECTION);
    glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);

    glMatrixMode(GL_MODELVIEW);
    glRotatef(30.0, 1.0, 0.0, 0.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutMainLoop();
}

