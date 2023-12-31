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

// Jenny Lim, Patrick Leonard

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <iostream>
#include <vector>
#include <string>
#include <mmsystem.h>

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

struct asteroid { // asteroid data structure
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

struct bullet { // bullet data structure
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

void reset() { // reset
    gameOver = false;
    asteroids.clear();
    bullets.clear();
    health = 5;
    points = 0;
} // reset

void drawsquare(int size) { // draw player / bullets
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(0, size);
    glVertex2i(size, size);
    glVertex2i(size, 0);

    glEnd();
    glFlush();
} // drawsquare

void drawasteroid(vector color) { // draw asteroid
    glColor3f(color.x, color.y, color.z);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 8; i++) {
        glVertex2f(sin(i / 8.0 * 2 * M_PI), cos(i / 8.0 * 2 * M_PI));
    }
    glEnd();
    glFlush();
} // drawasteroid

float getlength(vector v) { // returns euclidian distance
    return sqrt(v.x * v.x + v.y * v.y);
} // getlength

vector getbounds(float angle, float right, float left, float top, float bottom) // returns bounds of a box
{
    // 1/2 width and height -- 0,0 is at bottom left
    vector bounds( (right - left) * 0.5f, (top - bottom) * 0.5f, 0 );

    // if the object is rotated -- rotate vector by angle
    if (angle != 0)
    {
        vector rotated = vector( bounds.x * cosf(angle) - bounds.y * sinf(angle), bounds.x * sinf(angle) + bounds.y * cosf(angle), 0 );

        bounds.x = fabsf(rotated.x);
        bounds.y = fabsf(rotated.y);
    }

    return bounds;
} // getbounds

bool collision(vector boxBounds, float radius, vector boxPos, vector circlePos) // collision detector
{
    vector distance = { circlePos.x - boxPos.x, circlePos.y - boxPos.y, 0 }; // distance of box center (pos) to circle center

    // if that distance is outside on the axes + accounting for corners
    if (fabsf(distance.x) > (boxBounds.x + radius) || fabsf(distance.y) > (boxBounds.y + radius) || getlength(distance) > (getlength(boxBounds) + radius)) {
        return false;
    }

    return true;
} // collision

float randomfloat(int min, int max) { // random float generation
    return (float)rand() / RAND_MAX * (max - min) + min;
} // randomfloat

void createbullet() { // bullet initialization
    bullet b;

    vector pos = { xpos, ypos, 0 };
    float speed = 1;
    float angle = 0;

    bool rotPos = (int)randomfloat(0, 2);

    b = { pos, speed, angle, rotPos };
    bullets.push_back(b);
} // createbullet

void createasteroid() { // asteroid initialization
    asteroid a;

    vector color{ randomfloat(0, 1), randomfloat(0, 1), randomfloat(0, 1) };
    vector pos = { randomfloat(0, (float)glutGet(GLUT_WINDOW_WIDTH)), (float)glutGet(GLUT_WINDOW_HEIGHT) + 10, 0 }; // 510 -- above the screen
    float speed = randomfloat(1, 2);
    float size = randomfloat(10, 30);
    float angle = 0;

    a = { pos, speed, color, size, angle };
    asteroids.push_back(a);
} // createasteroid

void draw() { // update
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
            if (collision(getbounds(0, shipSize, 0, shipSize, 0), a->size / 2, vector{ xpos, ypos, 0 }, a->pos)) {
                // decr health
                asteroids.erase(asteroids.begin() + i);
                health--;
                PlaySound(TEXT("hurt.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                //std::cout << "health: " << health << std::endl;
                if (health <= 0) {
                    health = 0;
                    //std::cout << "lost" << std::endl;

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
                if (collision(getbounds(b->angle, bulletSize, 0, bulletSize, 0), a->size / 2, b->pos, a->pos)) {
                    // destroy asteroid
                    bullets.erase(bullets.begin() + i);
                    asteroids.erase(asteroids.begin() + j);
                    // incr points
                    points += 10;
                    PlaySound(TEXT("explode.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
                    //std::cout << "points: " << points << std::endl;
                    if (points >= 100) {
                        points = 100;
                        //std::cout << "won" << std::endl;

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
    case 'r':
    case 'R':
        reset();
        break;
    }
} // keyboard

void show_keys() {
    std::cout << "|| R: reset || Q: quit ||" << std::endl;
} // show_keys

void mouse(int btn, int state, int x, int y) {

    if (state == GLUT_DOWN) {
        if (btn == GLUT_LEFT_BUTTON) {
            // shoot
            if (!gameOver) {
                //std::cout << "shot" << std::endl;
                createbullet();
            }
        }
        //else if (btn == GLUT_RIGHT_BUTTON) {
        //    // idk
        //}
    }
} // mouse

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Meteor Shoot");
    show_keys();
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    srand(time(0));
    glutDisplayFunc(draw);
    glutIdleFunc(draw);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 499.0, 0.0, 499.0);

    glutMainLoop();
} // main