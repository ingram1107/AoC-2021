#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFSIZE 100

int pow2(int n)
{
  if (n == 0)
    return 1;
  else
    return 2 << (n-1);
}

int binaryToInt(char *binary, int size)
{
  int result = 0;

  for (int i = 0, j = size-1; i < size && j >= 0; i++, j--)
    if (binary[i] == '1') result += pow2(j);

  return result;
}

int main(void)
{
  FILE *input = fopen("day3.txt", "r");
  if (!input) {
    perror("FileIOError: ");
    exit(EXIT_FAILURE);
  }

  int col = 0;
  char *buf = malloc(BUFSIZE * sizeof(char));
  if (fgets(buf, BUFSIZE, input) <= 0) {
    perror("FileIOError: ");
    exit(EXIT_FAILURE);
  }
  rewind(input);

  for (char *bufReader = buf; *bufReader != '\n'; bufReader++)
    col++;

  int row = 0;
  while (fgets(buf, BUFSIZE, input) != 0)
    row++;
  rewind(input);

  char **report = calloc(row, sizeof(char *));
  for (int i = 0; i < row; i++) {
    report[i] = malloc((col+1) * sizeof(char));
    if (fscanf(input, "%s\n", report[i]) != 1) {
      perror("FileIOError");
      exit(EXIT_FAILURE);
    }
  }

  char *gammaRateBin = calloc(col+1, sizeof(char));
  char *epsilonRateBin = calloc(col+1, sizeof(char));
  int gammaRate, epsilonRate;
  int *count0 = calloc(col, sizeof(int));
  int *count1 = calloc(col, sizeof(int));

  for (int i = 0; i < col; i++) {
    for (int j = 0; j < row; j++)
      if (report[j][i] == '0')
        count0[i]++;
      else
        count1[i]++;
    if (count0[i] > count1[i])
      strcat(&gammaRateBin[i], "0");
    else
      strcat(&gammaRateBin[i], "1");
  }

  for (int i = 0; i < col; i++) {
    if (count0[i] < count1[i])
      strcat(&epsilonRateBin[i], "0");
    else
      strcat(&epsilonRateBin[i], "1");
  }

  gammaRate = binaryToInt(gammaRateBin, col);
  epsilonRate = binaryToInt(epsilonRateBin, col);

  printf("Gamma Rate: %d\n", gammaRate);
  printf("Epsilon Rate: %d\n", epsilonRate);
  printf("Power Consumption: %d\n", gammaRate * epsilonRate);

  free(count1);
  free(count0);
  free(epsilonRateBin);
  free(gammaRateBin);
  for (int i = 0; i < row; i++)
    free(report[i]);
  free(report);
  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
