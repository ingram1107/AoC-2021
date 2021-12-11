#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFSIZE 1000

struct Node {
  char element;
  struct Node *next;
};

typedef struct Node *PtrToNode;
typedef PtrToNode Stack;

bool stackIsEmpty(Stack s)
{
  return s->next == NULL;
}

void stackPush(char element, Stack s)
{
  PtrToNode tmp = malloc(sizeof(struct Node));
  if (!tmp)
    fprintf(stderr, "MemoryError: not enough memory\n");
  else {
    tmp->element = element;
    tmp->next = s->next;
    s->next = tmp;
  }
}

char stackTop(Stack s)
{
  if (!stackIsEmpty(s)) return s->next->element;
  else {
    fprintf(stderr, "StackError: stack is empty\n");
    return 0;
  }
}

void stackPop(Stack s)
{
  PtrToNode top;
  if (stackIsEmpty(s)) 
    fprintf(stderr, "StackError: stack is empty\n");
  else {
    top = s->next;
    s->next = s->next->next;
    free(top);
  }
}

void stackEmptify(Stack s)
{
  if (!s)
    fprintf(stderr, "StackError: stack is uninitialise\n");
  else
    while (!stackIsEmpty(s))
      stackPop(s);
}

Stack stackInit(void)
{
  Stack s = calloc(1, sizeof(struct Node));
  if (!s) {
    fprintf(stderr, "MemoryError: not enough memory\n");
    exit(EXIT_FAILURE);
  }
  stackEmptify(s);
  return s;
}

void stackDestroy(Stack s)
{
  if (!stackIsEmpty(s)) stackEmptify(s);
  free(s);
}

void swap(long *x, long *y)
{
  long tmp = *x;
  *x = *y;
  *y = tmp;
}

int partition(long array[], int first, int last)
{
  long pivot = array[last];
  int i = (first - 1);

  for (int j = first; j <= last-1; j++) {
    if (array[j] <= pivot) {
      i++;
      swap(&array[i], &array[j]);
    }
  }

  i++;
  swap(&array[i], &array[last]);
  return i;
}

void quickSort(long array[], long int first, long int last)
{
  if (first < last) {
    long int pivot = partition(array, first, last);
    quickSort(array, first, pivot-1);
    quickSort(array, pivot+1, last);
  }
}

int main(void)
{
  FILE *input = fopen("day10.txt", "r");
  if (!input) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }

  char *buf = malloc(BUFSIZE * sizeof(char));
  int sortingScoresSize = 0;
  while (fgets(buf, BUFSIZE, input) != 0)
    sortingScoresSize++;
  rewind(input);

  Stack stack = stackInit();
  char openBrace;
  int needSorted = 0;
  long *sortingScores = calloc(sortingScoresSize, sizeof(long));
  long *sortingScore = sortingScores;
  while (fgets(buf, BUFSIZE, input) != 0) {
    bool terminate = false;
    for (char *bufReader = buf; !terminate; bufReader++) {
      if (*bufReader == '\n') {
        openBrace = stackTop(stack);
        terminate = true;
        while (!stackIsEmpty(stack)) {
          *sortingScore *= 5;
          switch (openBrace) {
            case '(':
              *sortingScore += 1;
              break;
            case '[':
              *sortingScore += 2;
              break;
            case '{':
              *sortingScore += 3;
              break;
            case '<':
              *sortingScore += 4;
              break;
            default:
              goto cleanup;
          }
          stackPop(stack);
          openBrace = stackTop(stack);
          if (stackIsEmpty(stack)) {
            sortingScore++;
            needSorted++;
            goto cleanup;
          }
        }
      } else if (*bufReader == '(' || *bufReader == '[' ||
          *bufReader == '{' || *bufReader == '<') {
        stackPush(*bufReader, stack);
      } else if (*bufReader == ')' || *bufReader == ']' ||
          *bufReader == '}' || *bufReader == '>') {
        openBrace = stackTop(stack);
        if (openBrace == 0)
          break;
        else if (openBrace == '(' && *bufReader == ')' ||
            openBrace == '[' && *bufReader == ']' ||
            openBrace == '{' && *bufReader == '}' ||
            openBrace == '<' && *bufReader == '>')
          stackPop(stack);
        else {
cleanup:
          stackEmptify(stack);
          break;
        }
      }
    }
  }

  quickSort(sortingScores, 0, needSorted-1);
  printf("\nMiddle Score: %ld\n", sortingScores[needSorted/2]);

  free(sortingScores);
  stackDestroy(stack);
  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
