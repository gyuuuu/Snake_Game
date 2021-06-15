#include "map.h"

map::map(int map_row, int map_col)
{
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    start_color();
    keypad(stdscr, true);

    init_pair(1, COLOR_BLUE, COLOR_YELLOW);

    // getmaxyx(stdscr, yMax, xMax);
    menuwin = newwin(map_row, map_col, 1, 1);
    wbkgd(menuwin, COLOR_PAIR(1));
    wattron(menuwin, COLOR_PAIR(1));

    box(menuwin, '*','*');
    refresh();
    wrefresh(menuwin);
    getch();
}

map::~map()
{
}