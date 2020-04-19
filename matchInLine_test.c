#include "matchInLine.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int testReportLineMatchRec()
{
  lineInText *currentLine = (lineInText *)malloc(sizeof(lineInText));
  parsedCommandStruct *currentCommand = (parsedCommandStruct *)malloc(sizeof(parsedCommandStruct));
  int iTruthTable[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
  int vTruthTable[] = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};
  int xTruthTable[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};
  int aTruthTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
  int isMatchExpectedResults[] = {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0}, isMatchRealResults[16] = {0};
  int isMatchInLineFromAExpectedResults[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1};
  int isMatchInLineFromARealResults[16] = {0}, partIndex = 0;
  char lineText[6] = "YOu\n", expressionToSearch[6] = "You", concatinatedCurr[6] = "",
       origExpressionToSearch[6] = "You";
  currentCommand->arrayOfExpressionParts = malloc(4 * sizeof(partInExpression));
  currentCommand->arrayOfExpressionParts[0].expressionPartInfo.regularChar = 'Y';
  currentCommand->arrayOfExpressionParts[0].partClassifier = REGULAR;
  currentCommand->arrayOfExpressionParts[1].expressionPartInfo.regularChar = 'o';
  currentCommand->arrayOfExpressionParts[1].partClassifier = REGULAR;
  currentCommand->arrayOfExpressionParts[2].expressionPartInfo.regularChar = 'u';
  currentCommand->arrayOfExpressionParts[2].partClassifier = REGULAR;
  currentCommand->arrayOfExpressionParts[3].expressionPartInfo.regularChar = '\0';
  currentCommand->arrayOfExpressionParts[3].partClassifier = REGULAR;
  currentCommand->lengthOfOrigExpression = 3;
  int currentIndex = 0, sizeOfTruthTable = 16, linesAfterMatchCounter = 0, prevLinesCounter = 0, matchFlag = 0;
  currentLine->lineText = lineText;
  currentLine->indexOfLine = 2;
  currentLine->numOfBytesFromStartToHere = 12;
  currentCommand->expressionToSearch = expressionToSearch;
  currentCommand->concatenatedExpression = concatinatedCurr;
  currentCommand->origExpressionToSearch = origExpressionToSearch;
  currentCommand->numOfExpressionParts = 3;
  currentCommand->aNum = 2;
  currentCommand->c = false;
  currentCommand->n = false;
  currentCommand->b = false;
  for (currentIndex; currentIndex < sizeOfTruthTable; currentIndex++) {
    strcpy(concatinatedCurr, "");
    currentLine->isMatchInLine = false;
    currentLine->isMatchInLineFromA = false;
    currentCommand->i = iTruthTable[currentIndex];
    currentCommand->v = vTruthTable[currentIndex];
    currentCommand->x = xTruthTable[currentIndex];
    currentCommand->a = aTruthTable[currentIndex];
    reportLineMatchRec(partIndex, currentLine, currentCommand, &linesAfterMatchCounter, &prevLinesCounter, &matchFlag);
    if (currentLine->isMatchInLine) {
      isMatchRealResults[currentIndex] = 1;
    }
    if (currentLine->isMatchInLineFromA) {
      isMatchInLineFromARealResults[currentIndex] = 1;
    }
    assert(isMatchRealResults[currentIndex] == isMatchExpectedResults[currentIndex]);
    assert(isMatchInLineFromARealResults[currentIndex] == isMatchInLineFromAExpectedResults[currentIndex]);
  }
  free(currentLine);
  free(currentCommand->arrayOfExpressionParts);
  free(currentCommand);
  return 0;
}

int main() { testReportLineMatchRec(); }
