#include <ncurses.h>

class map
{
private:
    /* data */
    int xMax, yMax;
    WINDOW *menuwin;
public:
    map(int map_row=21, int map_col=30);
    ~map();
    void createItem();
};