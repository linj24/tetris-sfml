all:
	clang++ -std=c++2a src/main.cpp -Iinclude/ -o tetris -lsfml-graphics -lsfml-window -lsfml-system -pthread