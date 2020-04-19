#include "textLine.h"
#include <stdlib.h>
#define INIT_INDEX_VALUE 0

void initializeCurrentLine(lineInText *currentLine)
{
  currentLine->lineText = NULL;
  currentLine->lineTextLen = INIT_BYTES_NUM;
  currentLine->numOfBytesFromStartToHere = INIT_BYTES_NUM;
  currentLine->isMatchInLineFromA = false;
  currentLine->isMatchInLine = false;
  currentLine->indexOfLine = INIT_INDEX_VALUE;
}

void FillLineStruct(lineInText *currentLine, int lineIndex, int numOfBytes)
{
  currentLine->indexOfLine = lineIndex;
  currentLine->isMatchInLine = false;
  currentLine->isMatchInLineFromA = false;
  currentLine->numOfBytesFromStartToHere = numOfBytes;
}

void freeLine(lineInText **line)
{
  free((*line)->lineText);
  free(*line);
}
