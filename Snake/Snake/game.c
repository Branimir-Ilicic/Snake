#define _CRT_SECURE_NO_WARNINGS
#include "game.h"

char board[STUP * RED];

int gameOver = 0;

Snake snake;
Food food[FOODS];

void fillBoard() { // Postavljanje igraće ploče / gdje se igra odvija
	int x, y;

	for (y = 0; y < RED; y++) {
		for (x = 0; x < STUP; x++) {
			if (x == 0 || y == 0 || x == STUP - 1 || y == RED - 1) {
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
		if (snake.len < SNAKE_MAX) {
			snake.part[i] = snake.part[i - 1];
		}
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

void sortScores() { // Sortiranje spremljenih rezultata prema veličini, od najvećeg do najmanjeg
	FILE* f;
	struct Score* scores = malloc(sizeof(Score) * 100);

	int count = 0;
	int i, j;

	f = fopen("Highscore.txt", "r");
	if (f == NULL) {
		return;
	}

	while (fscanf(f, "%3s - Score: %d", scores[count].name, &scores[count].score) == 2) {
		count++;
	}
	fclose(f);

	for (i = 0; i < count - 1; i++) {
		for (j = i + 1; j < count; j++) {
			if (scores[j].score > scores[i].score) {
				struct Score temp = scores[i];
				scores[i] = scores[j];
				scores[j] = temp;
			}
		}
	}

	f = fopen("Highscore.txt", "w");
	if (f == NULL) {
		return;
	}
	for (i = 0; i < count; i++) {
		fprintf(f, "%s - Score: %d\n", scores[i].name, scores[i].score);
	}
	fclose(f);
	free(scores);
	scores = NULL;
}

void saveScore() { //Spremanje rezultata igre u datoteku.
	char choice;
	char name[4];
	int i;

	printf("\nDo you want to save you score? (Y/N): ");
	scanf(" %c", &choice);

	choice = toupper(choice);

	if (choice == 'Y') {
		printf("Write your initials (3 CAPS letters): ");

		while (1) {
			scanf("%3s", name);

			if (strlen(name) != 3) {
				printf("Must contain exactly 3 letters. Try again: ");
				continue;
			}
			int valid = 1;

			for (i = 0; i < 3; i++) {
				if (!isupper(name[i])) {
					valid = 0;
					break;
				}
			}
			if (valid) {
				break;
			}
			else {
				printf("Must be only capital letters. Try again: ");
			}
		}

		FILE* f = fopen("Highscore.txt", "a");
		if (f == NULL) {
			printf("Error while opening document!\n");
			return 1;
		}
		fprintf(f, "%s - Score: %d\n", name, snake.len * 100);
		fclose(f);
		sortScores();
		printf("Score has been saved.\n");
	}
	else {
		printf("Score has not been saved.\n");
	}
}

void openScore() { //Prikazivanje podataka iz datoteke Highscore.txt u konzoli.
	FILE* f;
	char ch;

	f = fopen("Highscore.txt", "r");
	if (f == NULL) {
		printf("\nThere is no saved scores!\n");
		return 1;
	}
	printf("\n===== SCORE SHEET =====\n\n");

	while ((ch = fgetc(f)) != EOF) {
		putchar(ch);
	}
	fclose(f);
	printf("\n=======================\n");
}

void clearScore() { // Brisanje podataka iz datoteke Highscore.txt
	FILE* f = fopen("Highscore.txt", "w");
	if (f == NULL) {
		printf("\nError occured while clearing scoresheet.\n");
		return;
	}
	fclose(f);
	printf("\nScoresheet cleared successfully.\n");
}
