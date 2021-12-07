#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 10000

int findLeastFuelCost(int *crabPosition, int size)
{
  int leastCostedFuel = 0;
  int distance;

  for (int i = 0; i < size; i++) {
    int fuelSum = 0;
    int *sortedPosition = calloc(size, sizeof(int));
    sortedPosition[0] = crabPosition[i];
    for (int j = 0; j < size; j++) {
      distance = (j > i) ? j - i : i - j;
      if (distance == 0) continue;
      else sortedPosition[distance] = crabPosition[j];
      fuelSum += sortedPosition[distance] * distance;
    }
    if (i == 0 || leastCostedFuel > fuelSum)
      leastCostedFuel = fuelSum;
    free(sortedPosition);
  }

  return leastCostedFuel;
}

int main(void)
{
  FILE *input = fopen("day7.txt", "r");
  if (!input) {
    perror("FileIOError: ");
    exit(EXIT_FAILURE);
  }

  int furthestCrab = 0;
  char *buf = malloc(BUFSIZE * sizeof(char));
  fgets(buf, BUFSIZE, input);
  int readPosition;
  for (char *bufReader = buf; *bufReader != '\n';) {
    readPosition = strtol(bufReader, &bufReader, 10);
    if (furthestCrab < readPosition)
      furthestCrab = readPosition;
    if (*bufReader == ',')
      bufReader++;
  }

  int *crabPosition = calloc((furthestCrab+1), sizeof(int));
  for (char *bufReader = buf; *bufReader != '\n';) {
    readPosition = strtol(bufReader, &bufReader, 10);
    crabPosition[readPosition]++;
    if (*bufReader == ',')
      bufReader++;
  }

  int fuel = findLeastFuelCost(crabPosition, furthestCrab+1);
  printf("Costed Fuel: %d\n", fuel);

  free(crabPosition);
  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
