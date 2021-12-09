#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define BUFSIZE 100

int main(void)
{
  FILE *input = fopen("day8.txt", "r");
  if (!input) {
    perror("FileIOError: ");
    exit(EXIT_FAILURE);
  }

  char *buf = malloc(BUFSIZE * sizeof(char));
  int sum = 0;
  while (fgets(buf, BUFSIZE, input) != 0) {
    int numOfAlpha = 0;
    char *charSequence = malloc(8 * sizeof(char));
    char *charSequenceReader = charSequence;
    unsigned int *numOfChars = calloc(8, sizeof(unsigned int));
    char digit1[3], digit4[5], digit7[4], digit8[8];
    char *keys = malloc(9 * sizeof(char));
    char *decodedChar = calloc(6, sizeof(char));
    char *decodedCharReader = decodedChar;
    bool isOutputValues = false, endOfLine = false;

    for (char *bufReader = buf; !endOfLine ; bufReader++) {
      if (*bufReader == '|') {
        isOutputValues = true;

        for (int i = 0; i < 8; i++) {
          if (numOfChars[i] == 4) {
            keys[4] = 'a' + i;
          } else if (numOfChars[i] == 6) {
            keys[1] = 'a' + i;
          } else if (numOfChars[i] == 9) {
            keys[5] = 'a' + i;
          }
        }

        for (char *checkBuf1 = digit1; *checkBuf1 != '\0'; checkBuf1++) {
          if (*checkBuf1 == keys[5]) continue;
          for (char *checkBuf2 = digit7; *checkBuf2 != '\0'; checkBuf2++) {
            if (*checkBuf1 == *checkBuf2) keys[2] = *checkBuf1;
          }
        }

        for (char *checkBuf1 = digit7; *checkBuf1 != '\0'; checkBuf1++) {
          if (*checkBuf1 != keys[2] && *checkBuf1 != keys[5]) keys[0] = *checkBuf1;
        }

        for (char *checkBuf1 = digit4; *checkBuf1 != '\0'; checkBuf1++) {
          if (numOfChars[*checkBuf1 - 'a'] == 7)
            keys[3] = *checkBuf1;
        }

        for (char *checkBuf1 = digit8; *checkBuf1 != '\0'; checkBuf1++) {
          if (numOfChars[*checkBuf1 - 'a'] == 7 && *checkBuf1 != keys[3])
            keys[6] = *checkBuf1;
        }
        
        keys[7] = '\0';

        bufReader++;
      }

      if (!isOutputValues) {
        if (isspace(*bufReader)) {
          charSequenceReader = charSequence;
          switch (numOfAlpha) {
            case 2:
              strncpy(digit1, charSequenceReader, numOfAlpha);
              digit1[numOfAlpha] = '\0';
              break;
            case 3:
              strncpy(digit7, charSequenceReader, numOfAlpha);
              digit7[numOfAlpha] = '\0';
              break;
            case 4:
              strncpy(digit4, charSequenceReader, numOfAlpha);
              digit4[numOfAlpha] = '\0';
              break;
            case 7:
              strncpy(digit8, charSequenceReader, numOfAlpha);
              digit8[numOfAlpha] = '\0';
          }
          numOfAlpha = 0;
        } else if (isalpha(*bufReader)) {
          *charSequenceReader = *bufReader;
          numOfChars[*charSequenceReader - 'a']++;
          charSequenceReader++;
          numOfAlpha++;
        }
      } else {
        if (isspace(*bufReader) || *bufReader == '\n') {
          char markedChar = ' ';
          bool isNot6, isNot0, isNot9;
          isNot6 = isNot0 = isNot9 = false;
          charSequenceReader = charSequence;
          switch (numOfAlpha) {
            case 2:
              *decodedCharReader = '1';
              decodedCharReader++;
              break;
            case 3:
              *decodedCharReader = '7';
              decodedCharReader++;
              break;
            case 4:
              *decodedCharReader = '4';
              decodedCharReader++;
              break;
            case 5:
              for (int i = 0; i < 5; i++) {
                if (isspace(markedChar) &&
                    (charSequenceReader[i] == keys[1] ||
                     charSequenceReader[i] == keys[2] ||
                     charSequenceReader[i] == keys[4] ||
                     charSequenceReader[i] == keys[5])) {
                  markedChar = charSequenceReader[i];
                } else if (markedChar == keys[1] ||
                           (markedChar == keys[5] && charSequenceReader[i] == keys[1])) {
                  *decodedCharReader = '5';
                } else if ((markedChar == keys[2] && charSequenceReader[i] == keys[4]) ||
                           (markedChar == keys[4] && charSequenceReader[i] == keys[2])) {
                  *decodedCharReader = '2';
                } else if ((markedChar == keys[2] && charSequenceReader[i] == keys[5]) ||
                           (markedChar == keys[5] && charSequenceReader[i] == keys[2])) {
                  *decodedCharReader = '3';
                }
              }
              decodedCharReader++;
              break;
            case 6:
              for (int i = 0; i < 6; i++) {
                if (charSequenceReader[i] == keys[2]) {
                  isNot6 = true;
                } else if (charSequenceReader[i] == keys[3]) {
                  isNot0 = true;
                } else if (charSequenceReader[i] == keys[4]) {
                  isNot9 = true;
                }
              }

              if (isNot6 && isNot0)
                *decodedCharReader = '9';
              else if (isNot6 && isNot9)
                *decodedCharReader = '0';
              else if (isNot0 && isNot9)
                *decodedCharReader = '6';

              decodedCharReader++;
              break;
            case 7:
              *decodedCharReader = '8';
              decodedCharReader++;
          }
          numOfAlpha = 0;

          if (*bufReader == '\n') {
            *decodedCharReader = '\0';
            decodedCharReader = decodedChar;
            sum += strtol(decodedCharReader, &decodedCharReader, 10);
            endOfLine = true;
          }
        } else if (isalpha(*bufReader)) {
          *charSequenceReader = *bufReader;
          charSequenceReader++;
          numOfAlpha++;
        }
      }
    }
    free(decodedChar);
    free(keys);
    free(numOfChars);
    free(charSequence);
  }

  printf("Total sum: %d\n", sum);

  free(buf);
  fclose(input);
  exit(EXIT_SUCCESS);
}
