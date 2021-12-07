#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 100

typedef struct Position {
  int horizontal;
  int depth;
  int aim;
} Position;

int main(void)
{
  Position *sub = (Position *) malloc(sizeof(Position));
  sub->horizontal = 0;
  sub->depth = 0;
  sub->aim = 0;

  FILE *input = fopen("day2.txt", "r");
  if (!input) {
    perror("FileIOError");
    return EXIT_FAILURE;
  }

  char *buf = malloc((BUFSIZE+1) * sizeof(char));
  int numOfCmds = 0;
  while ((fgets(buf, BUFSIZE, input)) != 0)
    numOfCmds++;
  rewind(input);

  char **cmds = calloc(numOfCmds, sizeof(char *));
  for (int i = 0; i < numOfCmds; i++)
    cmds[i] = malloc(10 * sizeof(char));
  int *vector = malloc((numOfCmds) * sizeof(int));
  for (int i = 0; i < numOfCmds; i++)
    if (fscanf(input, "%s %d\n", cmds[i], &vector[i]) != 2) {
      perror("FileIOError: ");
      exit(EXIT_FAILURE);
    }

  for (int i = 0; i < numOfCmds; i++) {
    if (strcmp(cmds[i], "forward") == 0) {
      sub->horizontal += vector[i];
      sub->depth += sub->aim * vector[i];
    } else if (strcmp(cmds[i], "down") == 0) sub->aim += vector[i];
    else if (strcmp(cmds[i], "up") == 0) sub->aim -= vector[i];
    else fprintf(stderr, "Unknown command");
  }

  printf("Final Position: %d:%d\n", sub->horizontal, sub->depth);
  printf("Final horizontal position * Final depth: %d\n", sub->horizontal * sub->depth);

  for (int i = 0; i < numOfCmds; i++)
    free(cmds[i]);
  free(cmds);
  free(vector);
  free(buf);
  fclose(input);
  free(sub);
}
