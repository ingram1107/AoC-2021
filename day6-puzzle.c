#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1000

void populateLaternfish(long **laternfishes, int days)
{
  for (int i = 0; i < days; i++) {
    long *newLaternfishes = calloc(9, sizeof(long));
    for (int j = 0; j < 9; j++)
      if (j == 0) {
        newLaternfishes[6] += (*laternfishes)[j];
        newLaternfishes[8] += (*laternfishes)[j];
      } else 
        newLaternfishes[j-1] += (*laternfishes)[j];
    long *tmp = *laternfishes;
    *laternfishes = newLaternfishes;
    free(tmp);
  }
}

int main(void)
{
  FILE *input = fopen("day6.txt", "r");
  if (!input) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }

  char *buf = malloc(sizeof(char) * BUFSIZE);
  if (!fgets(buf, BUFSIZE, input)) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }


  long *laternfishes = calloc(9, sizeof(long));
  for (char *bufReader = buf; *bufReader != '\n';) {
    if (*bufReader == ',') {
      bufReader++;
      continue;
    }
    int num = strtol(bufReader, &bufReader, 10); 
    laternfishes[num]++;
  }

  populateLaternfish(&laternfishes, 256);
  long numOfLanternfishes = 0;
  for (int i = 0; i < 9; i++)
    numOfLanternfishes += laternfishes[i];
  printf("Number of laternfishes: %ld\n", numOfLanternfishes);

  free(laternfishes);
  free(buf);
  fclose(input);
  return EXIT_SUCCESS;
}
