#include "menu.h"

Intro::Intro(){
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, true);

    getmaxyx(stdscr, max_y, max_x);
    introwin = newwin(6, max_x-12, max_y-8, 5);
    box(introwin, 0,0);
    refresh();
    wrefresh(introwin);
}
// menu 보여주기.
void Intro::showIntro(){
    string menu[] = {"Play", "Help", "Exit"};

    int highlight = 0;
    int ch = ' ';
    do {
        switch (ch){
            case KEY_UP:
                highlight--;
                break;
            case KEY_DOWN:
                highlight++;
                break;
            default:
                break;
        }
        highlight += 3;
        highlight %= 3;

        if( ch == 10){
            if( highlight == 0 ){
                Snake snake;
                snake.startGame();
                clear();
                box(introwin, 0,0);
                refresh();
                wrefresh(introwin);
                
            }
            if( highlight == 1){
                mvwprintw(introwin, 1,20, "Classic snake game.");
                mvwprintw(introwin, 2,20, "Just use arrow keys.");
                mvwprintw(introwin, 3,20, "Eat meats (*), and grow up!");
            }
            if( highlight == 2 ){
                endwin();
                return;
            }
        }
        for(int i=0; i<3; i++){
            if( i == highlight)
                wattron(introwin, A_REVERSE);
            mvwprintw(introwin, i+1, 3, "%s", menu[i].c_str() );
            wattroff(introwin, A_REVERSE);
            wrefresh(introwin);
        }
    } while( (ch = getch()) != 'q' );

    endwin();
}
