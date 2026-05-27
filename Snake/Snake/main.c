#define _CRT_SECURE_NO_WARNINGS
#include "game.h"

typedef enum Menu {
    START = 1,
    OPEN_SCORE,
    CLEAR_SCORE,
    EXIT_GAME
} Menu;

int main() {
    int option;

    srand(time(NULL));

    while (1) {

        clearScreen();

        printf("===== SNAKE GAME =====\n");
        printf("1. START\n");
        printf("2. Open Scoresheet\n");
        printf("3. Clear Scoresheet\n");
        printf("4. Close Game\n");
        printf("======================\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {

        case START:

            gameOver = 0;

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

                if (!gameOver) {
                    readKeyboard();
                }
            }

            printf("Game Over, Final Score: %d\n", snake.len * 100);
            saveScore();
            printf("\nPress any key to return to menu...");
            _getch();

            break;

        case OPEN_SCORE:

            clearScreen();
            openScore();
            printf("\nPress any key to return to menu...");
            _getch();

            break;

        case CLEAR_SCORE:

            clearScreen();
            clearScore();
            printf("\nPress any key to return to menu...");
            _getch();

            break;

        case EXIT_GAME:
            return 0;

        default:
            printf("Invalid input");
            _getch();
        }
    }
    return 0;
}