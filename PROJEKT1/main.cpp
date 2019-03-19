#include <ncurses.h>
#include "ball.h"
#include <vector>

using namespace std;

mutex mtx;
vector<thread> threads;

void throwBall(WINDOW *win, int y, int x, char c){
    Ball * b = new Ball(win, y, x, c);
    b->run();
}

void exitListener(){
    while(running){
        if(getch()==27) // if ESC pressed
            running = false;
    }
}

int main(){

    srand(time(nullptr));
    vector<thread> threads;

    /* NCURSES START */
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    // get screen size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // create a window for out input
    /* 20lines, 50cols, (yMax/2)-10 begin_y, 10 begin_x */
    WINDOW * playwin = newwin(20, 50, (yMax/2)-10, 10);
    box(playwin, 0, 0); // window*, vertical char, horizontal char
    refresh();
    wrefresh(playwin);

    // COLORS
    start_color();

    // definitions of each color pair
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_BLUE, COLOR_BLACK);

    threads.push_back(thread(exitListener)); // create new thread for exit listener


    // ball thrower
    while(running){
            int delay = rand()%4+1; // lemme random time between each ball throw
            threads.push_back(thread(throwBall, playwin, yMax / 3, xMax / 3, 'o')); // create new thread for a new ball
            this_thread::sleep_for(chrono::seconds(delay));
    }


    // wait for all threads to finish
    for (thread &thread : threads)
        thread.join();

    endwin();
    /* NCURSES END */

    return 0;
}