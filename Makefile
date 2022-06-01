all:
	g++ -std=c++2a src/main.cpp -Iinclude/ -o tetris -lX11 -lsfml-graphics -lsfml-window -lsfml-system -pthread