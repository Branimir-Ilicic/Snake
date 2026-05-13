#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define STUP 20
#define RED 20

char board[STUP * RED];

int GameOver = 0;

void fillBoard() {
	int i, j;
	
	for (j = 0; j < RED; j++) {
		for (i = 0; i < STUP; i++) {
			if (i == 0 || j == 0 || i == STUP-1 || j == RED-1) {
				board[j * STUP + i] = '#';
			}
			else {
				board[j * STUP + i] = ' ';
			}
		}
	}
}

void clearScreen() {
	system("cls");
}

void printBoard() {
	int i, j;

	clearScreen();

	for (j = 0; j < RED; j++) {
		for (i = 0; i < STUP; i++) {
			putchar(board[j * STUP + i]);
		}
		putchar('\n');
	}
}

#define SNAKE_MAX 256
struct SnakePart {
	int x, y;
};
struct Snake {
	int len;
	struct SnakePart part[SNAKE_MAX];
};
struct Snake snake;

struct Food {
	int x, y;
};

struct Food food ;

void drawSnake() {
	int i;

	for (i = snake.len - 1; i > 0; i--) {
		board[snake.part[i].y * STUP + snake.part[i].x] = '*';
		snake.part[i];
	}
	board[snake.part[0].y * STUP + snake.part[0].x] = '@';

}

void moveSnake(int x, int y) {
	int i;
	for (i = snake.len - 1; i > 0; i--) {
		snake.part[i] = snake.part[i - 1];
	}

	snake.part[0].x += x;
	snake.part[0].y += y;

}

void readKeyboard() {
	int ch = getchar();

	switch (ch) {
	case 'w': moveSnake(0, -1);
		break;
	case 's': moveSnake(0, 1);
		break;
	case 'a': moveSnake(-1, 0); 
		break;
	case 'd': moveSnake(1, 0); 
		break;
	}
}

void placeFood() {
	board[3 * STUP + 5] = '+';
}

int main(int argc, char** argv) {

	snake.len = 3;
	snake.part[0].x = 5;
	snake.part[0].y = 5;
	snake.part[1].x = 5;
	snake.part[1].y = 6;
	snake.part[2].x = 5;
	snake.part[2].y = 7;

	while (!GameOver) {
		fillBoard();
		placeFood();
		drawSnake();
		printBoard();
		printf("Duljina zmije: %d\n", snake.len);
		printf("%d %d\n", snake.part[0].x, snake.part[0].y);
		printf("%d %d\n", snake.part[1].x, snake.part[1].y);
		printf("%d %d\n", snake.part[2].x, snake.part[2].y);
		readKeyboard();
	}
	
	return 0;
}