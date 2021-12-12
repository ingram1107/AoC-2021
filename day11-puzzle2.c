#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFSIZE 100

void cuteOctupusesFlash(char octupuses[][10], int i, int j, int *flashes, bool flashed[][10])
{
  if (i >= 10 || j >= 10 || octupuses[i][j] == '0' && flashed[i][j]) return;
  else if (octupuses[i][j] == '9') {
    (*flashes)++;
    octupuses[i][j] = '0';
    flashed[i][j] = true;
    if (i+1 < 10) {
      cuteOctupusesFlash(octupuses, i+1, j, flashes, flashed);
    }
    if (j+1 < 10) {
      cuteOctupusesFlash(octupuses, i, j+1, flashes, flashed);
    }
    if (i > 0) {
      cuteOctupusesFlash(octupuses, i-1, j, flashes, flashed);
    }
    if (j > 0) {
      cuteOctupusesFlash(octupuses, i, j-1, flashes, flashed);
    }
    if (i+1 < 10 && j+1 < 10) {
      cuteOctupusesFlash(octupuses, i+1, j+1, flashes, flashed);
    }
    if (i+1 < 10 && j > 0) {
      cuteOctupusesFlash(octupuses, i+1, j-1, flashes, flashed);
    }
    if (i > 0 && j+1 < 10) {
      cuteOctupusesFlash(octupuses, i-1, j+1, flashes, flashed);
    }
    if (i > 0 && j > 0) {
      cuteOctupusesFlash(octupuses, i-1, j-1, flashes, flashed);
    }
  } else octupuses[i][j]++;
}

int main(void)
{
  FILE *input = fopen("day11.txt", "r");
  if (!input) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }

  char octopuses[10][10] = { 0 };
  int i, j;
  i = j = 0;
  char *buf = malloc(BUFSIZE * sizeof(char));
  while (fgets(buf, BUFSIZE, input) != 0) {
    for (char *bufReader = buf; *bufReader != '\n'; bufReader++) {
      if (j >= 10) {
        j = 0;
        i++;
      }
      octopuses[i][j++] = *bufReader;
    }
  }

  int numOfFlashes = 0;
  for (int s = 0; s < 500; s++) {
    bool octopusesFlashed[10][10] = { false };
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        cuteOctupusesFlash(octopuses, i, j, &numOfFlashes, octopusesFlashed);
      }
    }

    bool allFlash;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        allFlash = octopusesFlashed[i][j];
        if (!allFlash) break;
      }
      if (!allFlash) break;
    }

    if (allFlash) {
      printf("First step where all octupuses flash: %d\n", s+1);
      break;
    }
  }

  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
