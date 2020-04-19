#include "IO.h"
#include <stdlib.h>
#define EXIT_ERROR -1
#define INIT_LINES_VAL 0

void readLine(FILE **stream, ssize_t *getLineResult, lineInText *currentLine)
{
  ssize_t lineSize = INIT_LINE_SIZE;
  lineSize = getline(&(currentLine->lineText), &(currentLine->lineTextLen), *stream);
  *getLineResult = lineSize;
}

void printForBNArgsOptions(lineInText *currentLine, parsedCommandStruct *parsedCommand,
                           int const *linesAfterMatchCounter, int const *prevLinesCounter, int const *matchFlag)
{
  if (currentLine->isMatchInLine && *matchFlag == MATCHING_LINES_FOR_NEW_BLOCK && *prevLinesCounter == INIT_LINES_VAL &&
      *linesAfterMatchCounter == parsedCommand->aNum) {
    printf("--\n");
  }
  if (!parsedCommand->b && !parsedCommand->n) {
    if (currentLine->isMatchInLine || currentLine->isMatchInLineFromA) {
      printf("%s", currentLine->lineText);
    }
  } else if (parsedCommand->b && !parsedCommand->n) {
    if (currentLine->isMatchInLine || currentLine->isMatchInLineFromA) {
      printf("%d:%s", currentLine->numOfBytesFromStartToHere, currentLine->lineText);
    }
  } else if (!parsedCommand->b && parsedCommand->n) {
    if (currentLine->isMatchInLine) {
      printf("%d:%s", currentLine->indexOfLine, currentLine->lineText);
    } else if (currentLine->isMatchInLineFromA) {
      printf("%d-%s", currentLine->indexOfLine, currentLine->lineText);
    }
  } else {
    if (currentLine->isMatchInLine) {
      printf("%d:%d:%s", currentLine->indexOfLine, currentLine->numOfBytesFromStartToHere, currentLine->lineText);
    } else if (currentLine->isMatchInLineFromA) {
      printf("%d-%d-%s", currentLine->indexOfLine, currentLine->numOfBytesFromStartToHere, currentLine->lineText);
    }
  }
}

void printLineToOutput(lineInText *currentLine, parsedCommandStruct *parsedCommand, int *counterForC, bool endOfText,
                       int *linesAfterMatchCounter, int *prevLinesCounter, int *matchFlag)
{
  if (parsedCommand->c) {
    if (currentLine->isMatchInLine && endOfText == false) {
      *counterForC = *counterForC + 1;
    } else if (endOfText == true) {
      printf("%d\n", *counterForC);
    }
  } else {
    printForBNArgsOptions(currentLine, parsedCommand, linesAfterMatchCounter, prevLinesCounter, matchFlag);
  }
}

FILE *openFile(char *pathOfFile)
{
  FILE *filePtr = NULL;
  filePtr = fopen(pathOfFile, "r");
  if (filePtr == NULL) {
    printf("An error occurred while opening file to read\n");
    exit(EXIT_ERROR);
  }
  return filePtr;
}
