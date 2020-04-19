#ifndef TEXTLINE_H
#define TEXTLINE_H
#include <stdbool.h>
#include <stddef.h>
#define INIT_BYTES_NUM 0

typedef struct lineInText {
  char *lineText;
  size_t lineTextLen;
  int indexOfLine;
  int numOfBytesFromStartToHere;
  bool isMatchInLine;
  bool isMatchInLineFromA;
} lineInText;

void freeLine(lineInText **line);
void FillLineStruct(lineInText *currentLine, int lineIndex, int numOfBytes);
void initializeCurrentLine(lineInText *currentLine);

#endif  // TEXTLINE_H
