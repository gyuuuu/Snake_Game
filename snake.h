#include <ncurses.h>
#include <list>
#include <unistd.h>
#include <time.h>
#include <string>
#include<cstdlib>

using namespace std;
// 맵 높이 넓이, 처음 스피드 설정
#define WIDTH 38
#define HEIGHT 22
#define SPEED 250000

struct Part{
    int y, x;
    Part(int y, int x);
    Part();
};

class Snake{
    public:
        Snake();
        ~Snake();

        void startGame();
        bool mvSnake();
        int checkCollision();
        void createFruit();
        void createPoison();
        void createGate();
        void DeleteFruit();
        void DeletePoison();
        void DeleteGate();
        void Renewall();
        void Checking(int,int);
        void scoreBoards();
        void missionBoards();
        void missionComplete();
        void createWindows(int windowNumber);
        void gameOver();

    private:
        int start_y, start_x;
        int y, x;
        list<Part> snake;
        
        Part meat;
        Part poison;
        Part Gate1,Gate2;
        char direction;
        int score;
        int eaten_fruit,eaten_poison;
        int Max_length;
        int GateFlag,Gatecnt;
        char Check_body,Check_fruit,Check_poison,Check_Gate;
        int Check_time;
        bool Check_Complete;
        int Check_speed;
        WINDOW *gamewin, *scorewin, *menuwin, *missionwin;
        int sizeX, sizeY, begX, begY;   // gamewin borders
        char chBody, chMeat, chPoison;
};
