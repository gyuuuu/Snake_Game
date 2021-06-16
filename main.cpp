#include "map.h"
#include<cstdlib>

int main(){
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    start_color();
    keypad(stdscr, true);

    map map;
    map.createItem();
    getch();
    return 0;
}