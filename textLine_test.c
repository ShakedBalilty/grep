#include "textLine.h"
#include <assert.h>
#include <stdlib.h>
#define INITIALIZED_VALUE 0

void testInitializeCurrentLine(lineInText *currentLine)
{
  initializeCurrentLine(currentLine);
  assert(currentLine->lineText == NULL && currentLine->lineTextLen == INITIALIZED_VALUE);
  assert(currentLine->numOfBytesFromStartToHere == INITIALIZED_VALUE);
  assert(!currentLine->isMatchInLineFromA & !currentLine->isMatchInLine);
  assert(currentLine->indexOfLine == INITIALIZED_VALUE);
}

void testFillLineStruct(lineInText *currentLine)
{
  int lineIndex = 1, numOfBytes = 48;
  FillLineStruct(currentLine, lineIndex, numOfBytes);
  assert((!currentLine->isMatchInLine) & (!currentLine->isMatchInLineFromA));
  assert((currentLine->indexOfLine == lineIndex) & (currentLine->numOfBytesFromStartToHere == numOfBytes));
}

void testFreeLine(lineInText **line) { freeLine(line); }

main()
{
  lineInText *currentLine = (lineInText *)malloc(sizeof(lineInText));
  testInitializeCurrentLine(currentLine);
  testFillLineStruct(currentLine);
  testFreeLine(&currentLine);
}
