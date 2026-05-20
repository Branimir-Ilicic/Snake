#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define STUP 25
#define RED 25
#define FOODS 40

char board[STUP * RED]; 

int gameOver = 0;

void fillBoard() { // Postavljanje igraće ploče / gdje se igra odvija
	int x, y;
	
	for (y = 0; y < RED; y++) {
		for (x = 0; x < STUP; x++) {
			if (x == 0 || y == 0 || x == STUP-1 || y == RED-1) {
				board[y * STUP + x] = '#';
			}
			else {
				board[y * STUP + x] = ' ';
			}
		}
	}
}

void clearScreen() { // Čišćenje konzole kako bi prikazalo samo jedno, trenutačno stanje
	system("cls");
}

void printBoard() { // Printanje postavljene ploče u konzolu 
	int x, y;

	for (y = 0; y < RED; y++) {
		for (x = 0; x < STUP; x++) {
			putchar(board[y * STUP + x]);
		}
		putchar('\n');
	}
}

#define SNAKE_MAX 256 // Definiranje struktura zmije i hrane 
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
	int consumed;
};

struct Food food[FOODS];

void drawSnake() { // Prikaz same zmije (glave i tijela)
	int i;

	for (i = snake.len - 1; i > 0; i--) {
		board[snake.part[i].y * STUP + snake.part[i].x] = '*';
		snake.part[i];
	}
	board[snake.part[0].y * STUP + snake.part[0].x] = '@';

}

void moveSnake(int x, int y) { // Postupak kretanja zmije nakon inputa s tipkovnice 
	int i;
	for (i = snake.len - 1; i > 0; i--) {
		snake.part[i] = snake.part[i - 1];
	}

	snake.part[0].x += x;
	snake.part[0].y += y;

}

void readKeyboard() { // Detekcija inputa sa tipkovnice u svrhu kontroliranja zmije
	int ch = _getch();

	switch (ch) {
	case 'w': moveSnake(0, -1); break;
	case 's': moveSnake(0, 1); break;
	case 'a': moveSnake(-1, 0); break;
	case 'd': moveSnake(1, 0); break;
	}
}

void drawFood() { // Prikaz hrane u konzoli
	int i = 0;

	for (i = 0; i < FOODS; i++) {
		if (!food[i].consumed) {
			board[food[i].y * STUP + food[i].x] = '+';
		}
	}
}

void setupFood() { // Postavljanje hrane na random pozicije u ploči
	int i;

	for (i = 0; i < FOODS; i++) {
		food[i].x = 1 + rand() % (STUP - 2);
		food[i].y = 1 + rand() % (RED - 2);
		food[i].consumed = 0;
	}
}

void setupSnake() { // Postavljanje početne veličine i pozicije zmije 
	snake.len = 1;
	snake.part[0].x = 1 + rand() % (STUP - 2);
	snake.part[0].y = 1 + rand() % (RED - 2);
}

void gameRules() { // Osnovna pravila, detektira kada igra završava i kada je hrana pojedena
	int i;

	for (i = 0; i < FOODS; i++) {
		if (!food[i].consumed) {
			if (food[i].x == snake.part[0].x && food[i].y == snake.part[0].y) {
				food[i].consumed = 1;
				snake.len++;
			}
		}
	}

	if (snake.part[0].x == 0 || snake.part[0].x == STUP - 1 || snake.part[0].y == 0 || snake.part[0].y == RED - 1) {
		gameOver = 1;
	}

	for (i = 1; i < snake.len; i++) {
		if (snake.part[0].x == snake.part[i].x && snake.part[0].y == snake.part[i].y) {
			gameOver = 1;
		}
	}
}

int main(int argc, char** argv) {

	srand(time(NULL));

	setupSnake();
	setupFood();

	while (!gameOver) {
		fillBoard();
		drawFood();
		drawSnake();
		gameRules();
		clearScreen();
		printf("Snake Game, Score: %d\n", snake.len * 100);
		printBoard();
		if (!gameOver) readKeyboard();
	}
	
	printf(" Game Over, Final Score: %d\n", snake.len * 100);

	while (1) _getch();
	return 0;
}