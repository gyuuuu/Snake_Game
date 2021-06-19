#include "snake.h"

Part::Part()
{
    Part(0, 0);
}
Part::Part(int yPos, int xPos)
{
    y = yPos;
    x = xPos;
}
Snake::Snake()
{

    initscr();
    start_color();
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    srand(time(NULL));

    //getmaxyx(stdscr, yMax, xMax);
    //start_x = WIDTH/2;
    //start_y = HEIGHT/2;
    y = 10;
    x = 20;

    direction = 'l';
    score = 0;
    eaten_fruit = 0, eaten_poison = 0;
    Check_body = ' ', Check_fruit = ' ', Check_poison = ' ', Check_Gate = ' ';
    Check_Complete = false;
    Max_length = 10;
    Check_time = 0;
    //GateFlag=0;
    //Gatecnt=0;

    snake.clear();
    // creating snake
    for (int i = 0; i < 3; i++)
    {
        Part part;
        part = {y, x};
        snake.push_back(part);
    }
}
Snake::~Snake()
{
    snake.clear();
    nodelay(stdscr, true);
    endwin();
}
void Snake::startGame()
{
    clear();
    createWindows(1);
    createWindows(2);
    createWindows(3);
    createFruit();
    createPoison();
    createGate();

    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(7, COLOR_YELLOW, COLOR_YELLOW);

    while (mvSnake())
    {
        refresh();
        scoreBoards();
        missionBoards();
        wrefresh(gamewin);
        wrefresh(scorewin);
        wrefresh(missionwin);
    };

    gameOver();
}
// 전체적인 스네이크 표현 및 조작 함수.
bool Snake::mvSnake()
{
    //chBody = '@';
    int ch;
    ch = getch();
    // 방향키 이벤트 처리 진행방향과 반대방향일시 return 0;
    switch (ch)
    {
    case KEY_UP:
        if (direction != 'd')
            direction = 'u';
        else
            return 0;
        break;
    case KEY_DOWN:
        if (direction != 'u')
            direction = 'd';
        else
            return 0;
        break;
    case KEY_LEFT:
        if (direction != 'r')
            direction = 'l';
        else
            return 0;
        break;
    case KEY_RIGHT:
        if (direction != 'l')
            direction = 'r';
        else
            return 0;
        break;
    case 'q':
        return 0;
        break; // meanless
    default:
        break;
    }

    switch (direction)
    {
    case 'u':
        y--;
        break;
    case 'd':
        y++;
        break;
    case 'l':
        x--;
        break;
    case 'r':
        x++;
        break;
    }
    // 벽과 충돌했을 경우와 size가 2 이하가 됬을 경우 게임종료.
    if (checkCollision() == 0 || snake.size() == 2)
    {
        return 0;
    }
    // Gate 1을 입구로 들어갔을 경우 방향, 좌표 설정.
    if (y == Gate1.y && x == Gate1.x)
    {

        GateFlag = 1;
        if (Gate2.x == 1)
        {
            direction = 'r';
            x = Gate2.x + 1;
            y = Gate2.y;
        }
        else
        {
            direction = 'l';
            x = Gate2.x - 1;
            y = Gate2.y;
        }
    }
    // Gate 2을 입구로 들어갔을 때
    else if (y == Gate2.y && x == Gate2.x)
    {
        GateFlag = 1;
        if (Gate1.y == 1)
        {
            direction = 'd';
            x = Gate1.x;
            y = Gate1.y + 1;
        }
        else
        {
            direction = 'u';
            x = Gate1.x;
            y = Gate1.y - 1;
        }
    }
    // size 10이면 미션 보드에 있는 사이즈 미션 체크표시
    if (snake.size() == 10)
        Check_body = 'v'; 
    // 게이트 들어갔을 때 Flag 1로하여서 게이트 표현하는데 이용하였다.
    if (GateFlag == 1)
    {
        Gatecnt++;
    }
    // 스네이크가 게이트가 다 나왔을 경우 갱신.
    if (Gatecnt == snake.size())
    {
        mvaddch(Gate2.y, Gate2.x, '*');
        mvaddch(Gate1.y, Gate1.x, '*');
        Check_Gate = 'v';
        createGate();
    }
    // 실시간 갱신.
    snake.push_front({y, x});
    mvaddch(snake.back().y, snake.back().x, ' ');
    snake.pop_back();

    // 반복자를 사용해 스네이크 본체 화면에 표시함.
    list<Part>::iterator i;
    for (i = snake.begin(); i != snake.end(); i++)
    {

        if (i == snake.begin())
            mvaddch(i->y, i->x, '#' | COLOR_PAIR(4));
        else
            mvaddch(i->y, i->x, 'O' | COLOR_PAIR(5));
    }
    // 사이즈에 따른 레벨, 속도 처리
    if (snake.size() < 5)
    {
        Check_speed=1;
        usleep(SPEED);
    }
    else if (snake.size() == 5 || snake.size() == 6)
    {
        Check_speed=2;
        usleep(180000);
    }
    else if (snake.size() == 7 || snake.size() == 8)
    {
        Check_speed=3;
        usleep(130000);
    }
    else
    {
        Check_speed=4;
        usleep(90000);
    }

    Checking(Check_speed,Check_time);
    Check_time++;
    return 1;
}
// 충돌 경우 정리한 함수
int Snake::checkCollision()
{
    // 반복자 사용해서
    list<Part>::iterator i;
    for (i = snake.begin(); i != snake.end(); i++)
    {
        // Growth 만났을 때
        if (i->x == meat.x && i->y == meat.y)
        {
            Part tail;
            tail.y = snake.back().y;
            tail.x = snake.back().x;
            snake.push_back(tail);
            createFruit();
            score += 10;
            eaten_fruit++;
            if (eaten_fruit == 5)
                Check_fruit = 'v';
            return 1;
        }
        // Poison 만났을 때
        if (i->x == poison.x && i->y == poison.y)
        {
            Part tail;
            tail.y = snake.back().y;
            tail.x = snake.back().x;
            snake.pop_back();
            mvaddch(tail.y, tail.x, ' ');
            createPoison();
            score -= 10;
            eaten_poison++;
            if (eaten_poison == 2)
                Check_poison = 'v';
            return 1;
        }

        // 꼬리 물기 했을 경우
        if (i->x == x && i->y == y)
        {
            return 0;
        }

        // 벽과 충돌했을 경우
        if (i->x == (begX + sizeX) - 1 || i->x == begX || i->y == (begY + sizeY) - 1 || i->y == begY)
        {
            // Gate 예외 처리
            if (i->x == Gate1.x || i->x == Gate2.x)
                return 1;
            else
                return 0;
        }
    }
}
// Growth 생성
void Snake::createFruit()
{
    meat.y = rand() % (HEIGHT - 2) + 2;
    meat.x = rand() % (WIDTH - 2) + 2;
    mvaddch(meat.y, meat.x, '*' | COLOR_PAIR(3));
}
// Growth 제거
void Snake::DeleteFruit()
{
    mvaddch(meat.y, meat.x, ' ');
}
// Poison 생성
void Snake::createPoison()
{
    poison.y = rand() % (HEIGHT - 2) + 2;
    poison.x = rand() % (WIDTH - 2) + 2;
    mvaddch(poison.y, poison.x, 'X' | COLOR_PAIR(2));
}
// Poison 제거
void Snake::DeletePoison()
{
    mvaddch(poison.y, poison.x, ' ');
}
// Gate 생성
void Snake::createGate()
{
    GateFlag = 0;
    Gatecnt = 0;
    Gate1.y = rand() % 11;

    if (Gate1.y < 5)
        Gate1.y = 1;
    else
        Gate1.y = HEIGHT;

    Gate1.x = (rand() % (WIDTH - 2)) + 2;
    Gate2.y = (rand() % (HEIGHT - 2)) + 2;

    Gate2.x = rand() % 11;
    if (Gate2.x < 5)
        Gate2.x = 1;
    else
        Gate2.x = WIDTH;
    mvaddch(Gate1.y, Gate1.x, 'q' | COLOR_PAIR(6));
    mvaddch(Gate2.y, Gate2.x, 'w' | COLOR_PAIR(6));
}
// Gate 제거
void Snake::DeleteGate()
{
    mvaddch(Gate1.y, Gate1.x, '*');
    mvaddch(Gate2.y, Gate2.x, '*');
}
// 시간에 따른 아이템, 게이트 위치 바꿔주는 함수.
void Snake::Checking(int check_speed, int Check_time)
{
    if (Check_speed == 1 && Check_time == 23) Renewall();
    else if (Check_speed == 2 && Check_time == 37) Renewall();
    else if (Check_speed == 3 && Check_time == 50) Renewall();
    else if (Check_speed == 4 && Check_time == 60) Renewall();
}
// 위의 함수의 재사용하기 위해 만든 함수.
void Snake::Renewall()
{
    DeleteFruit();
    DeletePoison();
    DeleteGate();
    createFruit();
    createGate();
    createPoison();
    Check_time=0;
}
// 점수 표시하는 윈도우 창.
void Snake::scoreBoards()
{
    mvwprintw(scorewin, 2, 2, "Score Board");
    mvwprintw(scorewin, 3, 2, "%d / %d ", snake.size(), Max_length);
    mvwprintw(scorewin, 4, 2, "+ : %d", eaten_fruit);
    mvwprintw(scorewin, 5, 2, "- : %d", eaten_poison);
    //mvwprintw(scorewin, 7, 2, "G : %d", Check_time);
    //mvwprintw(scorewin, 6, 2, "GFFF : %d", GateFlag);
    //mvwprintw(scorewin, , , "Press 'q' to quit...");
}
// 미션체크하는 윈도우 창.
void Snake::missionBoards()
{
    mvwprintw(missionwin, 2, 2, "Mission");
    mvwprintw(missionwin, 3, 2, "B : 10(%c)", Check_body);
    mvwprintw(missionwin, 4, 2, "+ : 5(%c)", Check_fruit);
    mvwprintw(missionwin, 5, 2, "- : 2(%c)", Check_poison);
    mvwprintw(missionwin, 6, 2, "G : 1(%c)", Check_Gate);

    if (snake.size() != 10)
        Check_body = ' ';
    if (Check_body == 'v' && Check_fruit == 'v' && Check_poison == 'v' && Check_Gate == 'v')
    {
        Check_Complete = true;
    }
}
// 윈도우 세개 별로 따로 설정하는 함수.
void Snake::createWindows(int windowNumber = 3)
{
    switch (windowNumber)
    {
    case 1: // 본 게임 창
    {
        gamewin = newwin(HEIGHT, WIDTH, 1, 1);
        refresh();
        box(gamewin, '*', '*');
        //mvwaddch(gamewin,20,36,'2');
        wrefresh(gamewin);
        getmaxyx(gamewin, sizeY, sizeX);
        getbegyx(gamewin, begY, begX);
    }
    break;
    case 2: // 점수 창
    {
        scorewin = newwin(10, 20, 2, 45);
        refresh();
        box(scorewin, 0, 0);
        wrefresh(scorewin);
        break;
    }
    case 3: // 미션 창
    {
        missionwin = newwin(10, 20, 12, 45);
        refresh();
        box(missionwin, 0, 0);
        wrefresh(missionwin);
        break;
    }
    default:
        break;
    }
}
// 게임 종료 표시하는 함수
void Snake::gameOver()
{
    mvwprintw(gamewin, sizeY / 2, sizeX / 2 - 8, "GAME IS OVER!!!");
    wrefresh(gamewin);
    usleep(2000000);

    delwin(gamewin);
    delwin(scorewin);
    //delwin(missionwin);
    endwin();
}
// 게임 완료 표시하는 함수
void Snake::missionComplete()
{
    mvwprintw(gamewin, sizeY / 2, sizeX / 2 - 8, "MISSION COMPLETE");
    wrefresh(gamewin);
    usleep(2000000);

    delwin(gamewin);
    delwin(scorewin);
    //delwin(missionwin);
    endwin();
}
