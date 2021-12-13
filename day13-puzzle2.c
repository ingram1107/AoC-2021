#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int findMax(int l, int r)
{
  if (l > r) return l;
  else return r;
}

void markTransparentPaper(int *paper, int x, int y, int col)
{
  paper[y*col + x]++;
}

int *foldTransparentPaperInRow(int *paper, int y, int *row, int col)
{
  int foldStart = y*col;
  int foldEnd = y*col + col;
  int startPos = (*row)*col - col;
  int lastPos = (*row)*col - 1;
  int distance = (lastPos - foldEnd + 1) * 2;
  int mirror = startPos - distance;

  for (int i = 0; i < y; i++) {
    for (int j = 0; j < col; j++) {
      if (paper[mirror] == 0) paper[mirror] = paper[startPos + j];
      mirror++;
    }
    startPos -= col;
  }

  int *newPaper = calloc(foldStart, sizeof(int));
  for (int i = 0; i < y; i++) {
    for (int j = 0; j < col; j++) {
      newPaper[i*col + j] = paper[i*col + j];
    }
  }

  *row = y;
  free(paper);
  return newPaper;
}

int *foldTransparentPaperInCol(int *paper, int x, int row, int *col)
{
  int foldEnd = row*(*col) - x;
  int startPos = *col - 1;
  int lastPos = row*(*col) - 1;
  int distance = (lastPos - foldEnd + 1) * 2;
  int mirror = startPos - distance;

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < x; j++) {
      if (paper[mirror] == 0) paper[mirror] = paper[startPos - j];
      mirror++;
    }
    startPos += *col;
    mirror = startPos - distance;
  }

  int *newPaper = calloc(foldEnd, sizeof(int));
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < x; j++) {
      newPaper[i*x + j] = paper[i*(*col) + j];
    }
  }

  *col = x;
  free(paper);
  return newPaper;
}

int main(void)
{
  FILE *input = fopen("day13.txt", "r");
  if (!input) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }

  int x, y;
  int maxX = 0, maxY = 0;
  while (fscanf(input, "%d,%d\n", &x, &y) == 2) {
    maxX = findMax(maxX, x);
    maxY = findMax(maxY, y);
  }
  rewind(input);

  int col = maxX + 1, row = maxY + 1;
  int transparentPaperSize = col * row;
  int *transparentPaper = calloc(transparentPaperSize, sizeof(int));
  while (fscanf(input, "%d,%d\n", &x, &y) == 2) {
    markTransparentPaper(transparentPaper, x, y, col);
  }

  char *instruction = malloc(13 * sizeof(char));
  int pos;
  while (fscanf(input, "%[^=]=%d\n", instruction, &pos) == 2) {
    if (strcmp(instruction, "fold along y") == 0) {
      transparentPaper = foldTransparentPaperInRow(transparentPaper, pos, &row, col);
    } else if (strcmp(instruction, "fold along x") == 0) {
      transparentPaper = foldTransparentPaperInCol(transparentPaper, pos, row, &col);
    }
  }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if(transparentPaper[i*col+j] == 1)
        printf("#");
      else
        printf(".");
    }
    printf("\n");
  }

  free(instruction);
  free(transparentPaper);
  fclose(input);
  exit(EXIT_SUCCESS);
}
