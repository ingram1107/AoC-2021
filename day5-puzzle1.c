#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 100

void foundHydrothermalVent(int *diagram, int x, int y, int width)
{
  diagram[x*width + y]++;
}

void retriveXAndYFromBuf(char **b, int *x, int *y)
{
  *x = strtol(*b, *&b, 10);
  (*b)++;
  *y = strtol(*b, *&b, 10);
}

void skipArrow(char **b)
{
  (*b)++; (*b)++; (*b)++; (*b)++;
}

int findMaxX(int x, int X)
{
  if (x > X) return x;
  return X;
}

int findMaxY(int y, int Y)
{
  if (y > Y) return y;
  return Y;
}

int main(void)
{
  FILE *input = fopen("day5.txt", "r");
  if (!input) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }

  char *line = malloc(sizeof(char) * BUFSIZE);
  int maxX = 0, maxY = 0;
  while (fgets(line, BUFSIZE, input) != 0) {
    int x, y;
    char *lineReader = line;
    retriveXAndYFromBuf(&lineReader, &x, &y);
    maxX = findMaxX(maxX, x);
    maxY = findMaxY(maxY, y);
    skipArrow(&lineReader);
    retriveXAndYFromBuf(&lineReader, &x, &y);
    maxX = findMaxX(maxX, x);
    maxY = findMaxY(maxY, y);
  }
  rewind(input);

  int width = maxY + 1, height = maxX + 1;
  int hydrothermalVentsDiagramSize = height * width;
  int *hydrothermalVentsDiagram = calloc(hydrothermalVentsDiagramSize, sizeof(int));
  while (fgets(line, BUFSIZE, input) != 0) {
    int lastX, lastY, nextX, nextY;
    char *lineReader = line;
    retriveXAndYFromBuf(&lineReader, &lastX, &lastY);
    skipArrow(&lineReader);
    retriveXAndYFromBuf(&lineReader, &nextX, &nextY);
    if (lastX == nextX) {
      if (lastY > nextY) {
        for (int y = nextY; y < lastY+1; y++)
          foundHydrothermalVent(hydrothermalVentsDiagram, lastX, y, width);
      } else {
        for (int y = lastY; y < nextY+1; y++)
          foundHydrothermalVent(hydrothermalVentsDiagram, lastX, y, width);
      }
    } else if (lastY == nextY) {
      if (lastX > nextX) {
        for (int x = nextX; x < lastX+1; x++)
          foundHydrothermalVent(hydrothermalVentsDiagram, x, lastY, width);
      } else {
        for (int x = lastX; x < nextX+1; x++)
          foundHydrothermalVent(hydrothermalVentsDiagram, x, lastY, width);
      }
    }
  }

  int numOfOverlappingHydrothermalVent = 0;
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (*(hydrothermalVentsDiagram+(i*width+j)) > 1)
        numOfOverlappingHydrothermalVent++;
  printf("Overlapping Lines: %d\n", numOfOverlappingHydrothermalVent);

  free(hydrothermalVentsDiagram);
  free(line);
  fclose(input);
}
