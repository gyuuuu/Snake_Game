#include "snake.h"

class Intro{
    private:
        int max_x, max_y;
        WINDOW *introwin;
    public:
        Intro();
        void showIntro();
};