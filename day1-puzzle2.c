#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  FILE *input = fopen("day1.txt", "r");
  int *d, row = 0;
  char ch;

  if (!input) {
    fprintf(stderr, "error: Cannot open the file");
    return 1;
  }

  while ((ch = fgetc(input)) != EOF)
    if (ch == '\n') row++;
  rewind(input);

  d = calloc(row, sizeof(int));
  if (!d) {
    fprintf(stderr, "error: Not enough memory");
    return 2;
  }

  for (int i = 0; i < row; i++)
    if (fscanf(input, "%d", &d[i]) != 1) {
      fprintf(stderr, "error: Fail to read the file");
      return 3;
    }

  int c = 0;
  int newRow = row-2;
  int *nd = calloc(newRow, sizeof(int));

  for (int i = 0; i < newRow; i++)
    nd[i] = 0;

  for (int i = 0, j = 2; i < newRow; i++, j++)
    for (int k = j; k >= j-2; k--)
      nd[i] += d[k];

  for (int i = 1; i < newRow; i++)
    if (nd[i] > nd[i-1])
      c++;

  printf("%d\n", c);

  free(nd);
  free(d);
  fclose(input);
  return 0;
}
