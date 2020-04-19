#include "IO.h"
#include "commandParser.h"
#include "textLine.h"
#include <assert.h>
#include <stdlib.h>
// this module is tested by reading from IO_test_in file and comparing its prints to IO_test_out file
// using script file named runIO_test.sh that is running from makefile with command "make IO_test"

FILE *testOpenFile(char *pathOfFile)
{
  FILE *filePtr = openFile(pathOfFile);
  assert(filePtr != NULL);
  return filePtr;
};

void testReadLine(FILE **stream, ssize_t *getLineResult, lineInText *currentLine)
{
  readLine(stream, getLineResult, currentLine);
  printf("%s", currentLine->lineText);
};

void testPrintLineToOutput(lineInText *currentLine, parsedCommandStruct *parsedCommand)
{
  int bTruthTable[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
  int nTruthTable[] = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};
  int cTruthTable[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};
  int aTruthTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
  int currentIndex = 0, sizeOfTruthTable = 12, linesAfterMatchCounter = 0, prevLinesCounter = 0, matchFlag = 0,
      counterForC = 0;
  currentLine->lineText = "YOu\n";
  currentLine->indexOfLine = 2;
  currentLine->isMatchInLine = true;
  currentLine->isMatchInLineFromA = false;
  currentLine->numOfBytesFromStartToHere = 12;
  parsedCommand->aNum = 2;
  for (currentIndex; currentIndex < sizeOfTruthTable; currentIndex++) {
    parsedCommand->c = cTruthTable[currentIndex];
    parsedCommand->n = nTruthTable[currentIndex];
    parsedCommand->b = bTruthTable[currentIndex];
    if (currentIndex > 9 && currentIndex < 13) {
      parsedCommand->a = true;
      currentLine->isMatchInLineFromA = aTruthTable[currentIndex];
      currentLine->isMatchInLine = false;
    }
    if (parsedCommand->c) {
      printLineToOutput(currentLine, parsedCommand, &counterForC, false, &linesAfterMatchCounter, &prevLinesCounter,
                        &matchFlag);
      printLineToOutput(currentLine, parsedCommand, &counterForC, true, &linesAfterMatchCounter, &prevLinesCounter,
                        &matchFlag);
    } else {
      printLineToOutput(currentLine, parsedCommand, &counterForC, false, &linesAfterMatchCounter, &prevLinesCounter,
                        &matchFlag);
    }
  }
}

int main()
{
  lineInText *currentLine = (lineInText *)malloc(sizeof(lineInText));
  int counterForC = INITIALIZED_VALUE;
  ssize_t lineSize = INITIALIZED_VALUE;
  parsedCommandStruct *parsedCommand = (parsedCommandStruct *)malloc(sizeof(parsedCommandStruct));
  FILE *filePtr = testOpenFile("IO_test_in");
  testReadLine(&filePtr, &lineSize, currentLine);
  testPrintLineToOutput(currentLine, parsedCommand);
  fclose(filePtr);
  free(currentLine);
  free(parsedCommand);
}
