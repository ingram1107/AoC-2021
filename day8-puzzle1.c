#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define BUFSIZE 100

int main(void)
{
  FILE *input = fopen("day8.txt", "r");
  if (!input) {
    perror("FileIOError: ");
    exit(EXIT_FAILURE);
  }

  char *buf = malloc(BUFSIZE * sizeof(char));
  int numOfEasyDigits = 0;
  while (fgets(buf, BUFSIZE, input) != 0) {
    int numOfAlpha = 0;
    bool isOutputValues = false, endOfLine = false;
    for (char *bufReader = buf; !endOfLine ; bufReader++) {
      if (*bufReader == '\n') endOfLine = true;
      if (*bufReader == '|') {
        isOutputValues = true;
        bufReader++;
      }
      if (isOutputValues && isspace(*bufReader)) {
        if (numOfAlpha == 2 || numOfAlpha == 3 || numOfAlpha == 4 || numOfAlpha == 7)
          numOfEasyDigits++;
        numOfAlpha = 0;
      } else if (isOutputValues && isalpha(*bufReader)) numOfAlpha++;
    }
  }

  printf("Number of easy digits: %d\n", numOfEasyDigits);

  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
