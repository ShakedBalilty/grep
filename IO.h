#ifndef IO_H
#define IO_H
#include "commandParser.h"
#include "textLine.h"
#include <stdio.h>
#include <sys/types.h>
// according to notes 5.2,6 we put here this define which is used also in matchInLine
#define MATCHING_LINES_FOR_NEW_BLOCK 2
#define INIT_LINE_SIZE 0

void readLine(FILE **stream, ssize_t *getLineResult, lineInText *currentLine);
void printLineToOutput(lineInText *currentLine, parsedCommandStruct *parsedCommand, int *counterForC, bool endOfText,
                       int *linesAfterMatchCounter, int *prevLinesCounter, int *matchFlag);
FILE *openFile(char *pathOfFile);

#endif  // IO_H
