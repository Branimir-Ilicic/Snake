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

static inline int calculateScore() { //static funkcija zaizracun rezultata
	return snake.len * 100;
}
 
int compareScores(const void* a, const void* b) {
	const Score* s1 = (const Score*)a;
	const Score* s2 = (const Score*)b;

	return s2->score - s1->score;
}


void sortScores() { // Sortiranje spremljenih rezultata prema veličini, od najvećeg do najmanjeg
	FILE* f;
	struct Score* scores = malloc(sizeof(Score) * 100);

	int count = 0;

	if (scores == NULL) {
		perror("malloc");
		return;
	}

	f = fopen("Highscore.txt", "r");
	if (f == NULL) {
		perror("Highscore.txt");
		free(scores);
		return;
	}

	while (fscanf(f, "%3s - Score: %d", scores[count].name, &scores[count].score) == 2) {
		count++;
	}
	fclose(f);

	qsort(scores, count, sizeof(Score), compareScores);

	f = fopen("Highscore.txt", "w");
	if (f == NULL) {
		perror("Highscore.txt");
		free(scores);
		return;
	}
	for (int i = 0; i < count; i++) {
		fprintf(f, "%s - Score: %d\n", scores[i].name, scores[i].score);
	}
	fclose(f);
	free(scores);
	scores = NULL;
}

void searchScore() {

	FILE* f;
	Score* scores = malloc(sizeof(Score) * 100);

	int count = 0;
	char target[4];
	int i;

	if (scores == NULL) {
		perror("malloc");
		return;
	}

	printf("\nEnter initials to search: ");
	scanf("%3s", target);

	f = fopen("Highscore.txt", "r");
	if (f == NULL) {
		perror("Highscore.txt");
		free(scores);
		return;
	}

	while (fscanf(f, "%3s - Score: %d", scores[count].name, &scores[count].score) == 2) {
		count++;
	}
	fclose(f);

	for (i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			if (strcmp(scores[i].name, scores[j].name) > 0) {
				Score temp = scores[i];
				scores[i] = scores[j];
				scores[j] = temp;
			}
		}
	}

	int index = recursiveSearchScore(scores, 0, count - 1, target);

	if (index != -1) {
		printf("\nFOUND: %s - Score: %d\n",
			scores[index].name,
			scores[index].score);
	}
	else {
		printf("\nNo result found for %s\n", target);
	}

	free(scores);
	scores = NULL;
}

int recursiveSearchScore(Score* scores, int left, int right, char* target) {

	if (left > right) {
		return -1;
	}

	int mid = left + (right - left) / 2;
	int cmp = strcmp(scores[mid].name, target);

	if (cmp == 0) {
		return mid; 
	}

	if (cmp > 0) {
		return recursiveSearchScore(scores, left, mid - 1, target);
	}

	return recursiveSearchScore(scores, mid + 1, right, target);
}


void updateScore() {
	FILE* f;
	Score* scores = malloc(sizeof(Score) * 100);

	int count = 0;
	int i;
	char target[4];
	char newName[4];
	int score;
	int found = 0;

	if (scores == NULL) {
		perror("malloc");
		return;
	}

	printf("\nEnter initials to update: ");
	scanf("%3s", target);

	f = fopen("Highscore.txt", "r");
	if (f == NULL) {
		perror("Highscore.txt");
		free(scores);
		return;
	}

	while (fscanf(f, "%3s - Score: %d", scores[count].name, &scores[count].score) == 2) {
		count++;
	}
	fclose(f);

	for (i = 0; i < count; i++) {
		if (strcmp(scores[i].name, target) == 0) {
			printf("Enter new initials (3 CAPS letters): ");
			scanf("%3s", newName);

			int valid = 1;
			for (int j = 0; j < 3; j++) {
				if (!isupper(newName[j])) {
					valid = 0;
					break;
				}
			}

			if (!valid) {
				printf("Invalid initials!\n");
				return;
			}

			strcpy(scores[i].name, newName);
			found = 1;
		}
	}

	if (!found) {
		printf("No matching record found.\n");
		return;
	}

	f = fopen("Highscore.txt", "w");
	if (f == NULL) {
		perror("Highscore.txt");
		free(scores);
		return;
	}

	for (i = 0; i < count; i++) {
		fprintf(f, "%s - Score: %d\n", scores[i].name, scores[i].score);
	}

	fclose(f);
	free(scores);
	scores = NULL;
	printf("Score updated successfully.\n");
}

void saveScore() { //Spremanje rezultata igre u datoteku.
	static int saveCounter = 0;
	saveCounter++;

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
			perror("Highscore.txt");
			return;
		}
		fprintf(f, "%s - Score: %d\n", name, calculateScore());
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
	long fileSize;

	f = fopen("Highscore.txt", "r");
	if (f == NULL) {
		perror("Highscore.txt");
		return;
	}

	fseek(f, 0, SEEK_END);
	fileSize = ftell(f);
	rewind(f);

	printf("\n===== SCORE SHEET =====\n\n");
	printf("File size: %ld bytes\n\n", fileSize);

	while ((ch = fgetc(f)) != EOF) {
		putchar(ch);
	}
	fclose(f);
	printf("\n=======================\n");
}

void clearScore() { // Brisanje podataka iz datoteke Highscore.txt
	int result;

	result = remove("Highscore.txt");

	if (result == 0) {
		printf("\nScoresheet deleted successfully.\n");
	}
	else {
		perror("Highscore.txt");
		return;
	}

}
