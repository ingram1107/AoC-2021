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

int main(void)
{
  FILE *input = fopen("day10.txt", "r");
  if (!input) {
    perror("FileIOError");
    exit(EXIT_FAILURE);
  }

  char *buf = malloc(BUFSIZE * sizeof(char));
  Stack stack = stackInit();
  char openBrace;
  int syntaxErrorScore = 0;
  while (fgets(buf, BUFSIZE, input) != 0) {
    for (char *bufReader = buf; *bufReader != '\n'; bufReader++) {
      if (*bufReader == '(' || *bufReader == '[' ||
          *bufReader == '{' || *bufReader == '<')
        stackPush(*bufReader, stack);
      else if (*bufReader == ')' || *bufReader == ']' ||
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
          switch (*bufReader) {
            case ')':
              syntaxErrorScore += 3;
              break;
            case ']':
              syntaxErrorScore += 57;
              break;
            case '}':
              syntaxErrorScore += 1197;
              break;
            case '>':
              syntaxErrorScore += 25137;
          }
          stackEmptify(stack);
          break;
        }
      }
    }
  }

  printf("Syntax Error Score: %d\n", syntaxErrorScore);

  stackDestroy(stack);
  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
