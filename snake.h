#include <ncurses.h>
#include <vector>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;
// 맵 높이 넓이, 처음 스피드 설정
#define WIDTH 38
#define HEIGHT 22
#define SPEED 200000

struct Body{
    int x, y;
    Body(int x, int y);
    Body();
};

class Snake{
    public:
        Snake();
        ~Snake();

        void startGame();
        bool moveSnake();
        int checkCollision();
        void setSpeed();
        void setDir();
        void enterGate();
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
        //void CheckMission();
        void createWindows(int windowNumber);
        void gameOver();
        bool CheckMission();
        bool missionComplete();
        bool getKeyboard(int ch);

    private:
        int start_y, start_x;
        int y, x;
        vector<Body> snake;
        
        Body fruit;
        Body poison;
        Body Gate1,Gate2;
        char direction;
        int score;
        int eaten_fruit,eaten_poison;
        int Max_length;
        bool gateFlag;
        int gateCnt;
        int mvCnt;
        char Check_body,Check_fruit,Check_poison,Check_Gate;
        int Check_time;
        bool Check_Complete;
        int Check_speed;
        WINDOW *gamewin, *scorewin, *menuwin, *missionwin;
        int sizeX, sizeY, begX, begY;   // gamewin borders
        char chBody, chMeat, chPoison;
};
