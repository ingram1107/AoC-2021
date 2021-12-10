#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1000

int findRiskLevelForGeneralCase(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char left = heightMap[row][col-1];
  char right = heightMap[row][col+1];
  char top = heightMap[row-1][col];
  char bottom = heightMap[row+1][col];

  if (current < left && current < right && current < top && current < bottom) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForLeftCol(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char right = heightMap[row][col+1];
  char top = heightMap[row-1][col];
  char bottom = heightMap[row+1][col];

  if (current < right && current < top && current < bottom) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForRightCol(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char left = heightMap[row][col-1];
  char top = heightMap[row-1][col];
  char bottom = heightMap[row+1][col];

  if (current < left && current < top && current < bottom) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForTopRow(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char left = heightMap[row][col-1];
  char right = heightMap[row][col+1];
  char bottom = heightMap[row+1][col];

  if (current < left && current < right && current < bottom) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForBottomRow(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char left = heightMap[row][col-1];
  char right = heightMap[row][col+1];
  char top = heightMap[row-1][col];

  if (current < left && current < right && current < top) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForTopLeftEdge(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char right = heightMap[row][col+1];
  char bottom = heightMap[row+1][col];

  if (current < right && current < bottom) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForTopRightEdge(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char left = heightMap[row][col-1];
  char bottom = heightMap[row+1][col];

  if (current < left && current < bottom) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForBottomLeftEdge(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char right = heightMap[row][col+1];
  char top = heightMap[row-1][col];

  if (current < right && current < top) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findRiskLevelForBottomRightEdge(char **heightMap, int row, int col)
{
  char current = heightMap[row][col];
  char left = heightMap[row][col-1];
  char top = heightMap[row-1][col];

  if (current < left && current < top) {
    return current - '0' + 1;
  } else {
    return 0;
  }
}

int findSumOfRiskLevelsOfAllLowPoints(char **heightMap, int row, int col)
{
  int sum = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (i == 0 && j == 0) {
        sum += findRiskLevelForTopLeftEdge(heightMap, i, j);
      } else if (i == 0 && j == col-1) {
        sum += findRiskLevelForTopRightEdge(heightMap, i, j);
      } else if (i == 0) {
        sum += findRiskLevelForTopRow(heightMap, i, j);
      } else if (i == row-1 && j == 0) {
        sum += findRiskLevelForBottomLeftEdge(heightMap, i, j);
      } else if (i == row-1 && j == col-1) {
        sum += findRiskLevelForBottomRightEdge(heightMap, i, j);
      } else if (i == row-1) {
        sum += findRiskLevelForBottomRow(heightMap, i, j);
      } else if (j == 0) {
        sum += findRiskLevelForLeftCol(heightMap, i, j);
      } else if (j == col-1) {
        sum += findRiskLevelForRightCol(heightMap, i, j);
      } else {
        sum += findRiskLevelForGeneralCase(heightMap, i, j);
      }
    }
  }

  return sum;
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

  int sum = findSumOfRiskLevelsOfAllLowPoints(heightMap, row, col);
  printf("Sum = %d\n", sum);

  for (int i = 0; i < row; i++)
    free(heightMap[i]);
  free(heightMap);
  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
