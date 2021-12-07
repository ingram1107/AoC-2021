#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

char **newStrArray(int row, int col)
{
  char **array = calloc(row, sizeof(char *));
  for (int i = 0; i < row; i++)
    array[i] = calloc(col + 1, sizeof(char));

  return array;
}

void cleanupStrArray(char** array, int row)
{
  for (int i = 0; i < row; i++)
    free(array[i]);
  free(array);
}

bool rateOxygenGenerator(char **report, char **subreport, int row, int nCol, int col)
{
  int count0 = 0, count1 = 0;
  char **binaryMatch0 = newStrArray(row, nCol);

  if (!binaryMatch0) {
    fprintf(stderr, "error: Not enough memory");
    goto end;
  }

  char **binaryMatch1 = newStrArray(row, nCol);

  if (!binaryMatch1) {
    fprintf(stderr, "error: Not enough memory");
    goto end;
  }

  for (int i = 0; i < row; i++)
    if (report[i][col] == '0') strcpy(binaryMatch0[count0++], report[i]);
    else if (report[i][col] == '1') strcpy(binaryMatch1[count1++], report[i]);

  if (count0 > count1)
    for (int i = 0; i < count0; i++)
      strcpy(subreport[i], binaryMatch0[i]);
  else
    for (int i = 0; i < count1; i++)
      strcpy(subreport[i], binaryMatch1[i]);

  cleanupStrArray(binaryMatch1, row);
  cleanupStrArray(binaryMatch0, row);

  if ((count0 == 1 && count1 == 0) || (count1 == 1 && count0 == 0))
    return true;
end:
  return false;
}

bool rateCO2Scrubber(char **report, char **subreport, int row, int nCol, int col)
{
  int count0 = 0, count1 = 0;
  char **binaryMatch0 = newStrArray(row, nCol);

  if (!binaryMatch0) {
    fprintf(stderr, "error: Not enough memory");
    goto end;
  }

  char **binaryMatch1 = newStrArray(row, nCol);

  if (!binaryMatch1) {
    fprintf(stderr, "error: Not enough memory");
    goto end;
  }

  for (int i = 0; i < row; i++)
    if (report[i][col] == '0') strcpy(binaryMatch0[count0++], report[i]);
    else if (report[i][col] == '1') strcpy(binaryMatch1[count1++], report[i]);

  if (count1 < count0 && !(count1 < 1))
    for (int i = 0; i < count1; i++)
      strcpy(subreport[i], binaryMatch1[i]);
  else
    for (int i = 0; i < count0; i++)
      strcpy(subreport[i], binaryMatch0[i]);

  cleanupStrArray(binaryMatch1, row);
  cleanupStrArray(binaryMatch0, row);

  if ((count0 == 1 && count1 == 0) || (count1 == 1 && count0 == 0))
    return true;
end:
  return false;
}

void genRating(char **report, char *binary, int row, int col, bool rateFunc(char **, char **, int, int, int)) {
  char **subReport = newStrArray(row, col);

  if (!subReport) {
    fprintf(stderr, "error: Not enough memory");
    return;
  }

  char **finalReport = newStrArray(row, col);

  if (!finalReport) {
    fprintf(stderr, "error: Not enough memory");
    return;
  }

  rateFunc(report, subReport, row, col, 0);

  for (int i = 1; i < col; i++)
    if (rateFunc(subReport, finalReport, row, col, i))
      strcpy(binary, finalReport[0]);
    else {
      cleanupStrArray(subReport, row);
      subReport = finalReport;
      finalReport = newStrArray(row, col);
    }

  cleanupStrArray(finalReport, row);
  cleanupStrArray(subReport, row);
}

void genOxygenGeneratorRating(char **report, char *binary, int row, int col)
{
  genRating(report, binary, row, col, rateOxygenGenerator);
}

void genCO2ScrubberRating(char **report, char *binary, int row, int col)
{
  genRating(report, binary, row, col, rateCO2Scrubber);
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

  char *oxygenGeneratorRatingBin = calloc(col + 1, sizeof(char));
  char *co2ScrubberRatingBin = calloc(col + 1, sizeof(char));
  int oxygenGeneratorRating, co2ScrubberRating;

  genOxygenGeneratorRating(report, oxygenGeneratorRatingBin, row, col);
  genCO2ScrubberRating(report, co2ScrubberRatingBin, row, col);
  oxygenGeneratorRating = binaryToInt(oxygenGeneratorRatingBin, col);
  co2ScrubberRating = binaryToInt(co2ScrubberRatingBin, col);

  printf("Oxygen Generator Rating: %d\n", oxygenGeneratorRating);
  printf("CO2 Scrubber Rating: %d\n", co2ScrubberRating);
  printf("Power Consumption: %d\n", oxygenGeneratorRating * co2ScrubberRating);

  free(co2ScrubberRatingBin);
  free(oxygenGeneratorRatingBin);
  for (int i = 0; i < row; i++)
    free(report[i]);
  free(report);
  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
