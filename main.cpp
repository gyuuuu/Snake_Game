#include "map.h"
#include<cstdlib>

int main(){
    map();
    // int y = rand() % (21 - 2) + 2;
    // int x = rand() % (30 - 2) + 2;
    mvaddch(15, 15, '*' | COLOR_PAIR(3));
    return 0;
}