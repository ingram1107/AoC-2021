#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  FILE *input = fopen("day1.txt", "r");
  int *d, row = 0;
  char ch;

  if (!input) {
    fprintf(stderr, "error: Cannot open the file\n");
    return 1;
  }

  while((ch = fgetc(input)) != EOF)
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

  for (int i = 1; i < row; i++) {
    if (d[i] > d[i-1]) {
      c++;
    }
  }

  printf("%d\n", c);

  free(d);
  fclose(input);
  return 0;
}
