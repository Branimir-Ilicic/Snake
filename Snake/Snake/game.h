#define _CRT_SECURE_NO_WARNINGS

#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define STUP 25
#define RED 25
#define FOODS 50
#define SNAKE_MAX 256

extern char board[STUP * RED];
extern int gameOver;

typedef struct SnakePart {
    int x;
    int y;
} SnakePart;

typedef struct Snake {
    int len;
    SnakePart part[SNAKE_MAX];
} Snake;

typedef struct Food {
    int x;
    int y;
    int consumed;
} Food;

typedef struct Score {
    char name[4];
    int score;
} Score;

extern Snake snake;
extern Food food[FOODS];

void fillBoard();
void clearScreen();
void printBoard();

void setupSnake();
void drawSnake();
void moveSnake(int x, int y);
void readKeyboard();

void setupFood();
void drawFood();

void gameRules();

void saveScore();
void openScore();
void clearScore();
void sortScores();

#endif