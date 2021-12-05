#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFSIZE 1000

static int passedGuess = 0;

int ***newBingoBoard(int numOfBoard, int row, int col)
{
  int ***bingoBoard = calloc(numOfBoard, sizeof(int **));
  for (int i = 0; i < numOfBoard; i++) {
    bingoBoard[i] = calloc(row, sizeof(int *));
    for (int j = 0; j < row; j++)
      bingoBoard[i][j] = calloc(col, sizeof(int));
  }

  return bingoBoard;
}

void destroyBingoBoard(int ***bingoBoard, int numOfBoard, int row)
{
  for (int i = 0; i < numOfBoard; i++) {
    for (int j = 0; j < row; j++)
      free(bingoBoard[i][j]);
    free(bingoBoard[i]);
  }
  free(bingoBoard);
}

int **newBoard(int row, int col)
{
  int **board = calloc(row, sizeof(int *));
  for (int i = 0; i < row; i++)
    board[i] = calloc(col, sizeof(int));

  return board;
}

void copyBoard(int **srcBoard, int **destBoard, int row, int col)
{
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      destBoard[i][j] = srcBoard[i][j];
}

void destroyBoard(int **board, int row)
{
  for (int i = 0; i < row; i++)
    free(board[i]);
  free(board);
}


void markBingoBoard(int guess, int **board, int row, int col)
{
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      if (board[i][j] == guess)
        board[i][j] = -1;
}

bool checkBingoBoard(int **board, int row, int col)
{
  bool bingo = false;
  int match = 0;

  // check horizontal
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      if (match == 5) {
        bingo = true;
        return bingo;
      } else if (board[i][j] == -1) match++;
      else {
        match = 0;
        break;
      }

  // check vertical
  for (int j = 0; j < col; j++)
    for (int i = 0; i < row; i++)
      if (match == 5) {
        bingo = true;
        return bingo;
      } else if (board[i][j] == -1) match++;
      else {
        match = 0;
        break;
      }

  return bingo;
}

int guessBingo(int *guesses, int guessesSize, int ***boards, int numOfBoard, int row, int col, int **winningBoard)
{
  for (int g = 0; g < guessesSize; g++) {
    for (int i = 0; i < numOfBoard; i++)
      markBingoBoard(guesses[g], boards[i], row, col);

    if (++passedGuess < 5) continue;
    for (int i = 0; i < numOfBoard; i++)
      if (checkBingoBoard(boards[i], row, col)) {
        copyBoard(boards[i], winningBoard, row, col);
        return guesses[g];
      }
  }

  return -1;
}

int calculateBoardScore(int bingoNum, int **board, int row, int col)
{
  int sum = 0;

  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      if (board[i][j] == -1) continue;
      else sum += board[i][j];

  return sum * bingoNum;
}

int main(void)
{
  FILE *input = fopen("day4.txt", "r");
  int *guesses;
  char *guessesBuf = calloc(BUFSIZE, sizeof(char));
  // 5 x 5
  int numOfBingoBoard = 0;
  size_t bingoRow = 5, bingoCol = 5;
  int ***bingoBoard;

  if (!input) {
    fprintf(stderr, "error: Cannot open the file");
    return 1;
  }

  if(!fgets(guessesBuf, BUFSIZE, input)) {
    fprintf(stderr, "error: Read error when reading the first line");
    return 2;
  }

  int guessesSize = 0;
  char *guessesBufCounter = guessesBuf;
  while(true) {
    if (*guessesBufCounter == '\n') {
      guessesSize++;
      break;
    } else if (*guessesBufCounter == ',') guessesSize++;
    guessesBufCounter++;
  }

  guessesBufCounter = guessesBuf;
  guesses = calloc(guessesSize, sizeof(int));
  for(int i = 0; i < guessesSize;) {
    if (*guessesBufCounter == '\n') break;
    else if (*guessesBufCounter == ',') {
      guessesBufCounter++;
      continue;
    }
    guesses[i] = strtol(guessesBufCounter, &guessesBufCounter, 10);
    i++;
  }

  char *bingoBuf = calloc(BUFSIZE, sizeof(char));
  int bingoBoardStartPos = ftell(input);
  while (fgets(bingoBuf, BUFSIZE, input)) {
    if (bingoBuf[0] == '\n') {
      numOfBingoBoard++;
      continue;
    }
  }

  if (fseek(input, bingoBoardStartPos, SEEK_SET) != 0) {
    fprintf(stderr, "error: Fail to reposition the stream pointer");
    return 3;
  }

  bingoBoard = newBingoBoard(numOfBingoBoard, bingoRow, bingoCol);
  char* bingoBufCounter;
  int i = 0, j = 0;
  while (fgets(bingoBuf, BUFSIZE, input)) {
    if (bingoBuf[0] == '\n') continue;
    bingoBufCounter = bingoBuf;
    for (int k = 0; k < bingoCol; k++)
      bingoBoard[i][j][k] = strtol(bingoBufCounter, &bingoBufCounter, 10);

    if (++j >= bingoRow) {
      i++;
      j = 0;
    }
  }

  int **winningBoard = newBoard(bingoRow, bingoCol);
  int bingoNum = guessBingo(guesses, guessesSize, bingoBoard, numOfBingoBoard, bingoRow, bingoCol, winningBoard);
  if (bingoNum == -1) fprintf(stderr, "Something wrong here\n");
  printf("Bingo Number: %d\n", bingoNum);
  int score = calculateBoardScore(bingoNum, winningBoard, bingoRow, bingoCol);
  printf("Score: %d\n", score);

  destroyBoard(winningBoard, bingoRow);
  free(bingoBuf);
  destroyBingoBoard(bingoBoard, numOfBingoBoard, bingoRow);
  free(guesses);
  free(guessesBuf);
  fclose(input);
  return 0;
}
