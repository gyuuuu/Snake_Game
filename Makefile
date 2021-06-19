all : main

main : main.o Menu.o Snake.o
	g++ -o main main.o Menu.o Snake.o -lncurses

Menu.o : menu.cpp
	g++ -std=c++11 -c -o Menu.o menu.cpp

Snake.o : snake.cpp
	g++ -std=c++11 -c -o Snake.o snake.cpp

main.o : main.cpp
	g++ -std=c++11 -c -o main.o main.cpp
