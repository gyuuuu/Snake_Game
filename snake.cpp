#include "snake.h"


Snake::Snake()
{

    initscr();
    start_color();
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    srand((unsigned int)time(NULL));

    y = 10;
    x = 20;

    direction = 'l';
    score = 0;
    gateCnt=0;
    mvCnt=0;
    eaten_fruit = 0, eaten_poison = 0;
    Check_body = ' ', Check_fruit = ' ', Check_poison = ' ', Check_Gate = ' ';
    Check_Complete = false;
    Max_length = 10;
    Check_time = 0;

    snake.clear();

    for (int i = 0; i < 3; i++)
    {
        Body body;
        body = {y, x};
        snake.push_back(body);
    }
}
Snake::~Snake()
{
    snake.clear();
    nodelay(stdscr, true);
    endwin();
}
Body::Body()
{
    Body(0, 0);
}

Body::Body(int yPos, int xPos)
{
    y = yPos;
    x = xPos;
}
void Snake::startGame()
{
    clear();
    createWindows(1);
    createWindows(2);
    createWindows(3);
    createFruit();
    createPoison();
   

    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(7, COLOR_YELLOW, COLOR_YELLOW);

    while (moveSnake())
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


bool Snake::moveSnake()
{
    if (!getKeyboard(getch()))
       return false;
    setDir();
    
    if (checkCollision() == 0 || snake.size() == 2)
    {
        return 0;
    }


    if (snake.size() >= 5)
        Check_body = 'v'; 
    if (gateCnt > 0) Check_Gate = 'v';
    if (mvCnt == snake.size())
    {
        DeleteGate();
    }

    snake.insert(snake.begin(),{y, x});
    mvaddch(snake.back().y, snake.back().x, ' ');
    snake.pop_back();


    vector<Body>::iterator it;
    for (it = snake.begin(); it != snake.end(); it++)
    {

        if (it == snake.begin())
            mvaddch(it->y, it->x, '#' | COLOR_PAIR(4));
        else
            mvaddch(it->y, it->x, 'O' | COLOR_PAIR(5));
    }
    if (CheckMission()) return false;
    setSpeed();
    if (Check_time==10) createGate();
    
    Checking(Check_speed,Check_time);
    Check_time++;
    return true;

}

int Snake::checkCollision()
{

    vector<Body>::iterator it;
    for (it = snake.begin(); it != snake.end(); it++)
    {

        if (it->x == fruit.x && it->y == fruit.y)
        {
            Body tail;
            tail.y = snake.back().y;
            tail.x = snake.back().x;
            snake.push_back(tail);
            createFruit();
            score += 10;
            eaten_fruit++;
            if (eaten_fruit == 2)
                Check_fruit = 'v';
            Renewall();
            return 1;
        }

        if (it->x == poison.x && it->y == poison.y)
        {
            Body tail;
            tail.y = snake.back().y;
            tail.x = snake.back().x;
            snake.pop_back();
            mvaddch(tail.y, tail.x, ' ');
            createPoison();
            score -= 10;
            eaten_poison++;
            if (eaten_poison >= 1)
                Check_poison = 'v';
            Renewall();
            return 1;
        }

        // 반대 방향으로 전환한 경우
        if (it->x == x && it->y == y)
        {
            return 0;
        }

        enterGate();
        
        if (it->x == (begX + sizeX) - 1 || it->x == begX || it->y == (begY + sizeY) - 1 || it->y == begY)
        {
            // Gate인 경우
            if (it->x == Gate1.x || it->x == Gate2.x)
            {
                mvCnt++;
                
                return 1;
            }
            else
                return 0;
        }
    }
}
void Snake::createFruit()
{
    fruit.y = rand() % (HEIGHT - 2) + 2;
    fruit.x = rand() % (WIDTH - 2) + 2;
    mvaddch(fruit.y, fruit.x, '*' | COLOR_PAIR(3));
}
void Snake::DeleteFruit()
{
    mvaddch(fruit.y, fruit.x, ' ');
}
void Snake::createPoison()
{
    do{
        poison.y = rand() % (HEIGHT - 2) + 2;
        poison.x = rand() % (WIDTH - 2) + 2;
    } while(fruit.x == poison.x || fruit.y == poison.y);
    mvaddch(poison.y, poison.x, 'X' | COLOR_PAIR(2));
}
void Snake::DeletePoison()
{
    mvaddch(poison.y, poison.x, ' ');
}
void Snake::createGate()
{
    gateFlag=false;
    do{
        Gate1.x = rand() % (WIDTH-2)+2;
        Gate1.y = rand() % (HEIGHT-2)+2 ;
    }while((Gate1.x==poison.x || Gate1.y == poison.y));

    do{
        Gate2.x = rand() % (WIDTH-2)+2;
        Gate2.y = rand() % (HEIGHT-2)+2;
    }while((Gate1.x==poison.x || Gate1.y == poison.y || Gate1.x==poison.x || Gate2.y == poison.y));

    mvaddch(Gate1.y, Gate1.x, 'q' | COLOR_PAIR(6));
    mvaddch(Gate2.y, Gate2.x, 'w' | COLOR_PAIR(6));
}

void Snake::DeleteGate()
{
    mvaddch(Gate1.y, Gate1.x, ' ');
    mvaddch(Gate2.y, Gate2.x, ' ');
}

void Snake::Checking(int check_speed, int Check_time)
{
    if (Check_time == 30) Renewall();
    else if (Check_speed == 2 && Check_time == 43) Renewall();
    else if (Check_speed == 3 && Check_time == 55) Renewall();
    else if (Check_speed == 4 && Check_time == 66) Renewall();
}
void Snake::Renewall()
{
    DeleteFruit();
    DeletePoison();
    DeleteGate();
    createFruit();
    createPoison();
    Check_time=0;
}

void Snake::scoreBoards()
{
    mvwprintw(scorewin, 2, 2, "Score Board");
    mvwprintw(scorewin, 3, 2, "%d / %d ", snake.size(), Max_length);
    mvwprintw(scorewin, 4, 2, "+ : %d", eaten_fruit);
    mvwprintw(scorewin, 5, 2, "- : %d", eaten_poison);
    mvwprintw(scorewin, 6, 2, "speed : %d", Check_speed);
    mvwprintw(scorewin, 7, 2, "g : %d", gateCnt);
}
void Snake::missionBoards()
{
    mvwprintw(missionwin, 2, 2, "Mission");
    mvwprintw(missionwin, 3, 2, "B : 5(%c)", Check_body);
    mvwprintw(missionwin, 4, 2, "+ : 2(%c)", Check_fruit);
    mvwprintw(missionwin, 5, 2, "- : 1(%c)", Check_poison);
    mvwprintw(missionwin, 6, 2, "G : 1(%c)", Check_Gate);

    if (snake.size() < 5 )
        Check_body = ' ';
    if (Check_body == 'v' && Check_fruit == 'v' && Check_poison == 'v' && Check_Gate == 'v')
    {
        Check_Complete = true;
    }
}

void Snake::createWindows(int windowNumber = 3)
{
    switch (windowNumber)
    {
    case 1: 
    {
        gamewin = newwin(HEIGHT, WIDTH, 1, 1);
        refresh();
        box(gamewin, '*', '*');
        wrefresh(gamewin);
        getmaxyx(gamewin, sizeY, sizeX);
        getbegyx(gamewin, begY, begX);
    }
    break;

    case 2: 
    {
        scorewin = newwin(10, 20, 2, 45);
        refresh();
        box(scorewin, 0, 0);
        wrefresh(scorewin);
        break;
    }
    case 3:
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

bool Snake::CheckMission()
{
    if (Check_body =='v' && Check_fruit == 'v' && Check_poison &&Check_Gate=='v')
        return missionComplete();
    return false;
}

void Snake::gameOver()
{
    mvwprintw(gamewin, sizeY / 2, sizeX / 2 - 8, "GAME IS OVER!!!");
    wrefresh(gamewin);
    usleep(2000000);

    delwin(gamewin);
    delwin(scorewin);
    delwin(missionwin);
    endwin();
}

bool Snake::missionComplete()
{
    mvwprintw(gamewin, sizeY / 2, sizeX / 2 - 8, "MISSION COMPLETE");
    wrefresh(gamewin);
    usleep(2000000);

    delwin(gamewin);
    delwin(scorewin);
    delwin(missionwin);
    endwin();
    return true;
}

bool Snake::getKeyboard(int ch)
{
    switch (ch)
    {
    case KEY_UP:
        if (direction != 'd')
            direction = 'u';
        else
            return false;
        break;
    case KEY_DOWN:
        if (direction != 'u')
            direction = 'd';
        else
            return false;
        break;
    case KEY_LEFT:
        if (direction != 'r')
            direction = 'l';
        else
            return false;
        break;
    case KEY_RIGHT:
        if (direction != 'l')
            direction = 'r';
        else
            return false;
        break;
    case 'q':
        return false;
        break; 
    default:
        break;
    }
    return true;
}

void Snake::setSpeed()
{
    if (snake.size() < 5 || eaten_fruit < 1)
    {
        Check_speed=1;
        usleep(SPEED);
    }
    else if (snake.size() <7 || eaten_fruit<2)
    {
        Check_speed=2;
        usleep(SPEED-15000);
    }
    else if (snake.size() < 9 || eaten_fruit <3 )
    {
        Check_speed=3;
        usleep(SPEED-25000);
    }
    else
    {
        Check_speed=4;
        usleep(SPEED-35000);
    }
}

void Snake::setDir()
{
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
}

void Snake::enterGate()
{

    if (y == Gate1.y && x == Gate1.x)
    {
        gateCnt++;
        gateFlag = true;
        if (Gate2.x == 1 || Gate2.x == WIDTH) 
        {
            direction = Gate2.x == 1? 'r' : 'l';
            x = Gate2.x ==1 ? 2 : WIDTH-1;
            y = Gate2.y;
        }
        else if(Gate2.y == 1 || Gate2.y == HEIGHT)
        {
            direction = Gate2.y == 1? 'd' : 'u';
            y = Gate2.y ==1 ? 2 : WIDTH-1;
            x = Gate2.x;
        }
        else if (direction=='l'|| direction=='r')
        {
            direction = direction == 'l'? 'r' : 'l';
            x = Gate2.x + (direction=='l'? -1 : 1);
            y = Gate2.y;
        }
        else
        {
            direction = direction == 'u'? 'd' : 'u';
            y = Gate2.y + (direction=='u'? -1 : 1);
            x = Gate2.x;
        }
    }

    if (y == Gate2.y && x == Gate2.x)
    {
        gateCnt++;
        gateFlag = true;
        if (Gate1.x == 1 || Gate1.x == WIDTH)
        {
            direction = Gate1.x == 1? 'r' : 'l';
            x = Gate1.x ==1 ? 2 : WIDTH-1;
            y = Gate1.y;
        }
        else if(Gate1.y == 1 || Gate1.y == HEIGHT)
        {
            direction = Gate1.y == 1? 'd' : 'u';
            y = Gate1.y ==1 ? 2 : WIDTH-1;
            x = Gate1.x;
        }
        else if (direction=='l'|| direction=='r')
        {
            direction = direction == 'l'? 'r' : 'l';
            x = Gate1.x + (direction=='l'? -1 : 1);
            y = Gate1.y;
        }
        else
        {
            direction = direction == 'u'? 'd' : 'u';
            y = Gate1.y + (direction=='u'? -1 : 1);
            x = Gate1.x;
        }
    }
}
