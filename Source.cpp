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
#include <vector>

struct vector {
    float x;
    float y;
    float z;

    vector() {
        // default
    }

    vector(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
}; // vector

struct asteroid{
    vector pos;
    float speed;
    vector color;
    float size;
    float angle;

    asteroid() {
        // default
    }

    asteroid(vector _pos, float _speed, vector _color, float _size, float _angle)
    {
        pos = _pos;
        speed = _speed;
        color = _color;
        size = _size;
        angle = _angle;
    }
}; // asteroid

// player variables
float playerSpeed = 1;
float xpos = 0.0;
float ypos = 0.0;
float startingPos = -200;

// asteroid variables
int maxAsteroids = 20;
int lifetime = 400;
std::vector<asteroid> asteroids;

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

void drawasteroid(vector color) {
    glColor3f(color.x, color.y, color.z);
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

float randomfloat(int min, int max) {
    return (float)rand() / RAND_MAX * (max - min) + min;
} // randomfloat

void createAsteroid(){
    asteroid a;

    vector color{ randomfloat(0, 1), randomfloat(0, 1), randomfloat(0, 1) };
    vector pos = { randomfloat(0, 500), 510, 0 }; // 510 -- above the screen
    float speed = randomfloat(1, 10);
    float size = randomfloat(1, 3);
    float angle = 0;

    a = { pos, speed, color, size, angle};
    asteroids.push_back(a);
} // createasteroid

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

    //player
    glPushMatrix();
    glTranslatef(xpos, startingPos + ypos, 0);
    drawship();
    glPopMatrix();

    if (asteroids.size() < maxAsteroids) {
        createAsteroid();
    }
    
    // draw asteroids
    for (int i = 0; i < asteroids.size(); i++) {
        asteroid* a = &asteroids.at(i);

        if (a->pos.y <= -500) {
            asteroids.erase(asteroids.begin() + i);
        }

        a->pos.y -= a->speed; // move down
        a->angle += a->speed; // rotate

        glPushMatrix();
        glTranslatef(a->pos.x, a->pos.y, 0);
        glRotatef(a->angle, 0, 0, 1);
        glScalef(a->size, a->size, a->size);
        drawasteroid(a->color);
        glPopMatrix();
    }

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