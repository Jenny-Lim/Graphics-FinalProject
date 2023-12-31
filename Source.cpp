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

// Jenny Lim (6978118), Patrick Leonard (stud#)

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <iostream>
#include <vector>
#include <string>

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

struct asteroid {
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

struct bullet {
    vector pos;
    float speed;
    float angle;
    bool rotPos;

    bullet() {
        // default
    }

    bullet(vector _pos, float _speed, float _angle, int _rotPos)
    {
        pos = _pos;
        speed = _speed;
        angle = _angle;
        rotPos = _rotPos;
    }
}; // bullet

// player variables
float playerSpeed = 1;
float xpos = 240;
float ypos = 20;
int shipSize = 20;
int health = 5;
int points = 0;

bool gameOver = false;

// asteroid variables
int maxAsteroids = 7;
std::vector<asteroid> asteroids;

// bullet variables
int bulletSize = 10;
std::vector<bullet> bullets;

void Reset()
{

}

void drawsquare(int size) {
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(0, size);
    glVertex2i(size, size);
    glVertex2i(size, 0);

    glEnd();
    glFlush();
} // drawcube

void drawasteroid(vector color) {
    glColor3f(color.x, color.y, color.z);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; i++) {
        glVertex2f(sin(i / 6.0 * 2 * M_PI), cos(i / 6.0 * 2 * M_PI));
    }
    glEnd();
    glFlush();
} // drawasteroid

float getlength(vector v) {
    return sqrt(v.x * v.x + v.y * v.y);
} // getlength

//	Extents are 1/2 width and height of the transformed object
vector getextents(float angle, float right, float left, float top, float bottom)
{
    vector extents(
        (right - left) * 0.5f,
        (top - bottom) * 0.5f, // 0,0 is at bottom left
        0
    );

    // need to adjust it for rotation
    if (angle != 0)
    {
        float cosTheta = cosf(angle);
        float sinTheta = sinf(angle);

        vector rotated = vector(
            extents.x * cosTheta - extents.y * sinTheta,
            extents.x * sinTheta + extents.y * cosTheta,
            0
        );

        extents.x = fabsf(rotated.x);
        extents.y = fabsf(rotated.y);
    }

    return extents;
} // getextents

bool boxcirclecollision(vector boxExtents, float radius, vector boxPos, vector circlePos)
{
    vector distance = { circlePos.x - boxPos.x, circlePos.y - boxPos.y, 0 }; // centers are the objects position

    // check the x axis distance
    if (fabsf(distance.x) > (boxExtents.x + radius))
        return false;

    // check the y axis distance
    if (fabsf(distance.y) > (boxExtents.y + radius))
        return false;

    // straight line distance from 
    if (getlength(distance) > (getlength(boxExtents) + radius))
        return false;

    return true;
} // boxcirclecollision

float randomfloat(int min, int max) {
    return (float)rand() / RAND_MAX * (max - min) + min;
} // randomfloat

void createbullet() {
    bullet b;

    vector pos = { xpos, ypos, 0 };
    float speed = 1;
    float angle = 0;

    bool rotPos = (int)randomfloat(0, 2);

    b = { pos, speed, angle, rotPos };
    bullets.push_back(b);
} // createbullet

void createasteroid() {
    asteroid a;

    vector color{ randomfloat(0, 1), randomfloat(0, 1), randomfloat(0, 1) };
    vector pos = { randomfloat(0, (float)glutGet(GLUT_WINDOW_WIDTH)), (float)glutGet(GLUT_WINDOW_HEIGHT) + 10, 0 }; // 510 -- above the screen
    float speed = randomfloat(1, 2);
    float size = randomfloat(10, 30);
    float angle = 0;

    a = { pos, speed, color, size, angle };
    asteroids.push_back(a);
} // createasteroid

void draw() {
    Sleep(10);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    if (!gameOver) {

        if ((GetKeyState('A') & 0x8000) && xpos > 0) { // left
            xpos = xpos - playerSpeed;
        }
        if ((GetKeyState('D') & 0x8000) && xpos < 470) { // right
            xpos = xpos + playerSpeed;
        }
        if ((GetKeyState('W') & 0x8000) && ypos < 470) { // up
            ypos = ypos + playerSpeed;
        }
        if ((GetKeyState('S') & 0x8000) && ypos > 0) { // down
            ypos = ypos - playerSpeed;
        }

        //player
        glPushMatrix();
        glTranslatef(xpos, ypos, 0);
        drawsquare(shipSize);
        glPopMatrix();

        if (asteroids.size() < maxAsteroids) {
            createasteroid();
        }

        // draw asteroids
        for (int i = 0; i < asteroids.size(); i++) {
            asteroid* a = &asteroids.at(i);

            if (a->pos.y <= -1 * glutGet(GLUT_WINDOW_HEIGHT)) {
                asteroids.erase(asteroids.begin() + i);
            }

            a->pos.y -= a->speed; // move down
            a->angle += a->speed; // rotate

            // player, asteroid
            if (boxcirclecollision(getextents(0, shipSize, 0, shipSize, 0), a->size / 2, vector{ xpos, ypos, 0 }, a->pos)) {
                // decr health
                asteroids.erase(asteroids.begin() + i);
                health--;
                std::cout << "health: " << health << std::endl;
                if (health <= 0) {
                    health = 0;
                    std::cout << "lost" << std::endl;

                    // end game
                    gameOver = true;

                    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);  //RGBA values of text color
                    glRasterPos2i(10, 10);            //Top left corner of text

                    std::string loseStr = "Lost!";

                    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)loseStr.c_str());
                }
            } // end if

            glPushMatrix();
            glTranslatef(a->pos.x, a->pos.y, 0);
            glRotatef(a->angle, 0, 0, 1);
            glScalef(a->size, a->size, a->size);
            drawasteroid(a->color);
            glPopMatrix();
        }

        // draw bullets
        for (int i = 0; i < bullets.size(); i++) {
            bullet* b = &bullets.at(i);

            if (b->pos.y >= 500) {
                bullets.erase(bullets.begin() + i);
            }

            b->pos.y += b->speed;

            // some rotate the other way
            if (b->rotPos) {
                b->angle += b->speed;
            }
            else {
                b->angle -= b->speed;
            }

            // checking asteroid collisions
            for (int j = 0; j < asteroids.size(); j++) {
                asteroid* a = &asteroids.at(j);
                if (boxcirclecollision(getextents(b->angle, bulletSize, 0, bulletSize, 0), a->size / 2, b->pos, a->pos)) {
                    // destroy asteroid
                    bullets.erase(bullets.begin() + i);
                    asteroids.erase(asteroids.begin() + j);
                    // incr points
                    points += 10;
                    std::cout << "points: " << points << std::endl;
                    if (points >= 100) {
                        points = 100;
                        std::cout << "won" << std::endl;

                        // end game
                        gameOver = true;

                        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);  //RGBA values of text color
                        glRasterPos2i(10, 10);            //Top left corner of text

                        std::string winStr = "Won!";

                        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)winStr.c_str());
                    }
                }
            } // end for

            glPushMatrix();
            glTranslatef(b->pos.x + bulletSize / 2, b->pos.y, 0); // + half bulletSize for centering
            glRotatef(b->angle, 0, 0, 1);
            drawsquare(bulletSize);
            glPopMatrix();
        }

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  //RGBA values of text color
        glRasterPos2i(200, 460);            //Top left corner of text

        std::string score = "SCORE: " + std::to_string(points);

        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)score.c_str());

        glRasterPos2i(210, 430);            //Top left corner of text

        std::string hp = "HP: " + std::to_string(health);

        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)hp.c_str());

        glutSwapBuffers();

    } // end gameover if

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
            if (!gameOver) {
                std::cout << "shot" << std::endl;
                createbullet();
            }
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