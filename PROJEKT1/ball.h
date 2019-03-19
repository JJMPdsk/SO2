//
// Created by dsk on 3/16/19.
//

#ifndef PROJEKT1_BALL_H
#define PROJEKT1_BALL_H

#include <mutex>
#include "global.h"
#include <thread>

class Ball
{
public:
    Ball(WINDOW * win, int y, int x, char c);
    void run(); // main simulation method

private:
    int xLoc, yLoc, xMax, yMax;
    char character;
    WINDOW * curwin;
    int direction;
    int colorPair;
    int xLocOld, yLocOld;

    void display();
    /* MOVEMENT PURPOSE METHODS */
    void mvup();
    void mvdown();
    void mvleft();
    void mvright();

    void mvupleft();
    void mvupright();
    void mvdownleft();
    void mvdownright();

    void moveBall();

    void eraseBall();
};

/* CTOR */

Ball::Ball(WINDOW *win, int y, int x, char c) {
    curwin = win;
    yLoc = y;
    xLoc = x;

    xLocOld = xLoc;
    yLocOld = yLoc;

    getmaxyx(curwin, yMax, xMax);
    character = c;

    colorPair = rand() % 7 + 1; // gets 1/2/3/4/5

    do{
        direction = rand() % 8; // gets 0/1/2/3/4/5/6/7

    }while(direction == 2 || direction == 6 || direction == 7 );
    /*
     * 0 - UP
     * 1 - RIGHT
     * 2 - DOWN
     * 3 - LEFT
     *
     * 4 - UPPER LEFT
     * 5 - UPPER RIGHT
     * 6 - LOWER RIGHT
     * 7 - LOWER LEFT
     */

    display();
}

void Ball::display() {
    std::lock_guard<std::mutex> guard(mtx);
    mvwaddch(curwin, yLocOld, xLocOld, ' ');
    wattron(curwin, COLOR_PAIR(colorPair));
    mvwaddch(curwin, yLoc, xLoc, character);
    wattroff(curwin, COLOR_PAIR(colorPair));
    wrefresh(curwin);
}

/* MOVEMENT */

void Ball::mvup() {
    xLocOld = xLoc;
    yLocOld = yLoc;
    yLoc--;
    if(yLoc < 1)
    {
        yLoc = 2;
        direction = 2; // if can't go up any further, go down
    }
}

void Ball::mvdown() {
    xLocOld = xLoc;
    yLocOld = yLoc;
    yLoc++;
    if(yLoc > yMax-2)
    {
        yLoc = yMax-3;
        direction = 0; // if can't go down any further, go up
    }
}

void Ball::mvleft() {
    xLocOld = xLoc;
    yLocOld = yLoc;
    xLoc--;
    if(xLoc < 1)
    {
        xLoc = 2;
        direction = 1; // if can't go left any further, go right
    }
}

void Ball::mvright() {
    xLocOld = xLoc;
    yLocOld = yLoc;
    xLoc++;
    if(xLoc > xMax-2)
    {
        xLoc = xMax-2;
        direction = 3; // if can't go left any further, go left
    }
}

void Ball::mvupleft() {
    xLocOld = xLoc;
    yLocOld = yLoc;
    xLoc--;
    yLoc--;

    if(yLoc < 1){
        yLoc = 2;
        direction = 7;
    }

    if(xLoc < 1){
        xLoc = 2;
        direction = 5;
    }
}

void Ball::mvdownleft() {
    xLocOld = xLoc;
    yLocOld = yLoc;
    xLoc--;
    yLoc++;

    if(yLoc > yMax-2){
        yLoc = yMax-3;
        direction = 4;
    }

    if(xLoc < 1){
        xLoc = 2;
        direction = 6;
    }
}

void Ball::mvdownright(){
    xLocOld = xLoc;
    yLocOld = yLoc;
    xLoc++;
    yLoc++;

    if(yLoc > yMax-2){
        yLoc = yMax-3;
        direction = 5;
    }

    if(xLoc > xMax-2){
        xLoc = xMax-2;
        direction = 7;
    }
}

void Ball::mvupright(){
    xLocOld = xLoc;
    yLocOld = yLoc;
    xLoc++;
    yLoc--;

    if(yLoc < 1){
        yLoc = 2;
        direction = 6;
    }

    if(xLoc > xMax-2){
        xLoc = xMax-2;
        direction = 4;
    }
}

void Ball::moveBall(){
    switch(direction)
    {
        case 0: // UP
            mvup();
            break;
        case 1: // RIGHT
            mvright();
            break;
        case 2: // DOWN
            mvdown();
            break;
        case 3: // LEFT
            mvleft();
            break;
        case 4: // UPPER LEFT
            mvupleft();
            break;
        case 5: // UPPER RIGHT
            mvupright();
            break;
        case 6: // LOWER RIGHT
            mvdownright();
            break;
        case 7: // LOWER LEFT
            mvdownleft();
            break;

        default:
            break;
    }
}

/* SIMULATION PURPOSE */

void Ball::run() {
    int i = 1, j = 0;

    do
    {
        moveBall();
        display();

        if(direction == 1 || direction == 3)
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 32));
         else
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 80));

        if(j%3==0) // increment i every 3 "turns"
            i++;

        j++; // j is incremented eitherway

        // after 15 "slows downs" - get rid of that ball
        if(i == 15){
            // get me that ball of my screen!
            eraseBall();
            return;
        }
    }while(running);
}

void Ball::eraseBall() {
    std::lock_guard<std::mutex> guard(mtx);
    mvwaddch(curwin, yLoc, xLoc, ' ');
    wrefresh(curwin);
}

#endif //PROJEKT1_BALL_H
