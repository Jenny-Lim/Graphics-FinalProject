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

// hello

// Jenny Lim (6978118), Patrick Leonard (stud#)

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <iostream>

float playerSpeed = 1;
float xpos = 0.0;
float ypos = 0.0;
float startingPos = -200;
float angle = 0.0;

void drawship() {
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    glVertex2i(240, 240);
    glVertex2i(240, 260);
    glVertex2i(260, 260);
    glVertex2i(260, 240);

    glEnd();
    glFlush();
} // drawcube

void drawasteroid() {
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; i++) {
        glVertex2f(sin(i / 6.0 * 2 * M_PI) * 10, cos(i / 6.0 * 2 * M_PI) * 10);
    }
    glEnd();
    glFlush();
} // drawasteroid

//bool Collision2D::BoxCircleCheck(Box2D box, Circle circle)
//{
//    Vector2 distance = circle.center - box.center;
//
//    // check the x axis distance
//    if (fabsf(distance.x) > (box.extents.x + circle.radius))
//        return false;
//
//    // check the y axis distance
//    if (fabsf(distance.y) > (box.extents.y + circle.radius))
//        return false;
//
//    // straight line distance from 
//    if (distance.Length() > (box.extents.Length() + circle.radius))
//        return false;
//
//    return true;
//}

void draw() {
    Sleep(10);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    if (GetKeyState('A') & 0x8000) { // left
        xpos = xpos - playerSpeed;
    }
    if (GetKeyState('D') & 0x8000) { // right
        xpos = xpos + playerSpeed;
    }
    if (GetKeyState('W') & 0x8000) { // up
        ypos = ypos + playerSpeed;
    }
    if (GetKeyState('S') & 0x8000) { // down
        ypos = ypos - playerSpeed;
    }

    glPushMatrix();
    glTranslatef(xpos, startingPos + ypos, 0);
    drawship();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(250, 200, 0); // get random pos + make list of asteroids
    glRotatef(angle, 0, 0, 1);
    drawasteroid();
    glPopMatrix();

    angle++;

    //if (Collision2D::BoxCircleCheck(paddleCollision, ballCollision)) {
    //    ballCollision.center -= ballVelocity * deltaTime;
    //    pos = Collision2D::ReflectCircleBox(ballCollision, ballVelocity, deltaTime, paddleCollision);
    //    ballSprite.SetPosition(pos);
    //}
    
    glutSwapBuffers();
} // draw

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
    case 0x1B:
    case 'q':
    case 'Q':
        exit(0);
        break;
    }
} // keyboard

void mouse(int btn, int state, int x, int y) {

    if (state == GLUT_DOWN) {
        if (btn == GLUT_LEFT_BUTTON) {
            // shoot
            std::cout << "shot" << std::endl;
        }
        else if (btn == GLUT_RIGHT_BUTTON) {
            // idk
        }
    }
} // mouse

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Asteroid Shooter");

    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutDisplayFunc(draw);
    glutIdleFunc(draw);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 499.0, 0.0, 499.0);

    glutMainLoop();
} // main