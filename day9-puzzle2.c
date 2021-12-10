#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 1000

void floodFill(char **heightMap, int row, int col)
{
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (heightMap[i][j] < '9') {
        heightMap[i][j] = 'a';
      }
    }
  }
}

int findBasins(char **heightMap, int totalRow, int totalCol, int row, int col, bool start, int *basinSize)
{
  if (start)
    *basinSize = 0;

  if (heightMap[row][col] != '9' && heightMap[row][col] != 'b') {
    heightMap[row][col] = 'b';
    (*basinSize)++;

    if (row+1 < totalRow)
      findBasins(heightMap, totalRow, totalCol, row+1, col, false, basinSize);
    if (col+1 < totalCol)
      findBasins(heightMap, totalRow, totalCol, row, col+1, false, basinSize);
    if (row > 0)
      findBasins(heightMap, totalRow, totalCol, row-1, col, false, basinSize);
    if (col > 0)
      findBasins(heightMap, totalRow, totalCol, row, col-1, false, basinSize);
  }

  return *basinSize;
}

int totalSizeOfThreeLargestBasins(int basinSize, int *top, int *mid, int *low)
{
  if (basinSize > *top) {
    *low = *mid;
    *mid = *top;
    *top = basinSize;
  } else if (basinSize > *mid) {
    *low = *mid;
    *mid = basinSize;
  } else if (basinSize > *low) {
    *low = basinSize;
  }

  return *top * *mid * *low;
}

int main(void)
{
  FILE *input = fopen("day9.txt", "r");
  if (!input) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }

  int col = 0;
  char *buf = malloc(BUFSIZE * sizeof(char));
  if (fgets(buf, BUFSIZE, input) <= 0) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }
  rewind(input);

  for (char *bufReader = buf; *bufReader != '\n'; bufReader++)
    col++;

  int row = 0;
  while (fgets(buf, BUFSIZE, input) != 0)
    row++;
  rewind(input);

  char **heightMap = calloc(row, sizeof(char *));
  for (int i = 0; i < row; i++) {
    heightMap[i] = malloc((col+1) * sizeof(char));
    if (fscanf(input, "%s\n", heightMap[i]) != 1) {
      perror("FileIOError");
      exit(EXIT_FAILURE);
    }
  }

  floodFill(heightMap, row, col);

  int totalSize, basinSize = 0;
  int top, mid, low;
  top = mid = low = 0;
  for (int i = 0; i < row; i ++)
    for (int j = 0; j < col; j++) {
      totalSize = totalSizeOfThreeLargestBasins(findBasins(heightMap, row, col, i, j, true, &basinSize), &top, &mid, &low);
    }

  printf("Total Size: %d\n", totalSize);

  for (int i = 0; i < row; i++)
    free(heightMap[i]);
  free(heightMap);
  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
