//#include <iostream>
#include <ncurses.h>
//#include <string>
//#include <cstdlib>
//#include <ctime>
#include "player.h"
#include "ball.h"
//#include <thread>
#include <vector>

using namespace std;

std::mutex mtx;
int maxBallsCount = 10;

void addThread(WINDOW *win, int y, int x, char c){
    Ball * b = new Ball(win, y, x, c);
    b->run();
}

int main()
{
    std::srand(std::time(nullptr));
    std::vector<std::thread> threads;

    /* NCURSES START */
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    // get screen size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // create a window for out input
    WINDOW * playwin = newwin(20, 50, (yMax/2)-10, 10);
    box(playwin, 0, 0);
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


    // ball thrower
    for(int i = 0 ; i < maxBallsCount ; i++){
        int delay = rand()%5+1; // lemme random time between each ball throw
        threads.push_back(std::thread(addThread, playwin, yMax/3, xMax/3, 'o')); // create new thread for a new ball
        this_thread::sleep_for(std::chrono::seconds(delay));
    }


    // wait for all threads to finish
    for (std::thread &thread : threads)
        thread.join();

    endwin();
    /* NCURSES END */

    return 0;
}