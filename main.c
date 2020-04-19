#include "IO.h"
#include "commandParser.h"
#include "matchInLine.h"
#include "textLine.h"
#include <stdlib.h>
#define FIRST_LINE_INDEX 1
#define INIT_MATCH_FLAG 0
#define FIRST_PART_INDEX 0
#define ARGV_FILE_STEP 1
#define END_OF_INPUT 1
#define INIT_C_COUNTER 0
#define INIT_LINES_COUNTER 0

void receiveAndExecute(parsedCommandStruct *parsedCommand, FILE **stream)
{
  ssize_t lineSize = INIT_LINE_SIZE;
  int matchFlag = INIT_MATCH_FLAG;
  lineInText *currentLine = NULL;
  int lineIndex = FIRST_LINE_INDEX, counterForC = INIT_C_COUNTER, linesAfterMatchCounter = INIT_LINES_COUNTER,
      sumOfBytes = INIT_BYTES_NUM, prevLinesCounter = INIT_LINES_COUNTER;
  currentLine = malloc(sizeof *currentLine);
  initializeCurrentLine(currentLine);
  while (true) {
    readLine(stream, &lineSize, currentLine);
    FillLineStruct(currentLine, lineIndex, sumOfBytes);
    sumOfBytes = (int)lineSize + sumOfBytes;
    lineIndex++;
    initializeConcatinateExp(parsedCommand);
    if (lineSize < END_OF_INPUT) {
      break;
    }
    reportLineMatchRec(FIRST_PART_INDEX, currentLine, parsedCommand, &linesAfterMatchCounter, &prevLinesCounter,
                       &matchFlag);
    printLineToOutput(currentLine, parsedCommand, &counterForC, false, &linesAfterMatchCounter, &prevLinesCounter,
                      &matchFlag);
  }
  printLineToOutput(currentLine, parsedCommand, &counterForC, true, NULL, NULL, NULL);
  freeLine(&currentLine);
}

int main(int argc, char *argv[])
{
  parsedCommandStruct *parsedCommand = NULL;
  FILE *filePtr = NULL;
  bool useFile = false;
  useFile = isUsingFile(argc, argv);
  createAndFillCommand(argc, argv, &parsedCommand);
  if (useFile) {
    filePtr = openFile(argv[argc - ARGV_FILE_STEP]);
    receiveAndExecute(parsedCommand, &filePtr);
    fclose(filePtr);
  } else {
    receiveAndExecute(parsedCommand, &stdin);
  }
  freeParsedCommandStruct(parsedCommand);
  free(parsedCommand);
  return 0;
}