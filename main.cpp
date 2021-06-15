#include <ncurses.h>

int main(){
    int xMax, yMax;
    WINDOW *menuwin;

    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, true);
    // initscr();
    // keypad(stdscr, TRUE);
    // curs_set(0);
    // noecho();

    getmaxyx(stdscr, yMax, xMax);
    menuwin = newwin(21, 30, 1, 1);

    box(menuwin, '*','*');
    refresh();
    wrefresh(menuwin);
    getch();
    // clear();
    // refresh();

    // getch();
    endwin();

    return 0;
}