#include "matchInLine.h"
#include "IO.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define ASCII_HIGHEST_CHAR 255
#define ASCII_LOWEST_CHAR 1
#define BACKSLASH_N_LENGTH 1
#define A_MATCH_ENDED 0
#define RECURSE_STEP 1
#define END_OF_STRING 0

// based on
// https://stackoverflow.com/questions/27303062/strstr-function-like-that-ignores-upper-or-lower-case
char *insensitiveStrstr(const char *haystack, const char *needle)
{
  do {
    const char *haystackPtr = haystack;
    const char *needlePtr = needle;
    while (tolower((unsigned char)*needlePtr) == tolower((unsigned char)*haystackPtr) && *needlePtr) {
      haystackPtr++;
      needlePtr++;
    }
    if (*needlePtr == END_OF_STRING) {
      return (char *)haystack;
    }
  } while (*haystack++);
  return NULL;
}

void reportLineMatch(lineInText *currentLineToCheck, parsedCommandStruct parsedCommand, int *linesAfterMatchCounterPtr,
                     int *prevLinesCounter, int *matchFlag)
{
  if (parsedCommand.i && !parsedCommand.x) {
    if (insensitiveStrstr(currentLineToCheck->lineText, parsedCommand.expressionToSearch) != NULL) {
      currentLineToCheck->isMatchInLine = true;
    }
  } else if (parsedCommand.i && parsedCommand.x) {
    if ((insensitiveStrstr(currentLineToCheck->lineText, parsedCommand.expressionToSearch) != NULL) &&
        (strlen(currentLineToCheck->lineText) - BACKSLASH_N_LENGTH == strlen(parsedCommand.expressionToSearch))) {
      currentLineToCheck->isMatchInLine = true;
    }
  } else if (parsedCommand.x) {
    if (!(strncmp(currentLineToCheck->lineText, parsedCommand.expressionToSearch,
                  strlen(currentLineToCheck->lineText) - BACKSLASH_N_LENGTH)) &&
        (strlen(currentLineToCheck->lineText) - BACKSLASH_N_LENGTH == strlen(parsedCommand.expressionToSearch))) {
      currentLineToCheck->isMatchInLine = true;
    }
  } else {
    if (strstr(currentLineToCheck->lineText, parsedCommand.expressionToSearch) != NULL) {
      currentLineToCheck->isMatchInLine = true;
    }
  }
  if (parsedCommand.v) {
    currentLineToCheck->isMatchInLine = !(currentLineToCheck->isMatchInLine);
  }
  if (parsedCommand.a && currentLineToCheck->isMatchInLine) {
    if (*matchFlag < MATCHING_LINES_FOR_NEW_BLOCK) {
      (*matchFlag)++;
    }
    *prevLinesCounter = *linesAfterMatchCounterPtr;
    *linesAfterMatchCounterPtr = parsedCommand.aNum;
  }
  if (*linesAfterMatchCounterPtr > A_MATCH_ENDED && !currentLineToCheck->isMatchInLine) {
    *prevLinesCounter = *linesAfterMatchCounterPtr;
    (*linesAfterMatchCounterPtr)--;
    currentLineToCheck->isMatchInLineFromA = true;
  }
}

void updateConExpWithRegChr(parsedCommandStruct *parsedCommand, int *partIndex)
{
  strncat(parsedCommand->concatenatedExpression,
          &parsedCommand->arrayOfExpressionParts[*partIndex].expressionPartInfo.regularChar, ONE_CHAR_LENGTH);
  (*partIndex)++;
}

void updateConExpWithChr(int j, parsedCommandStruct *parsedCommand, char **endOfString)
{
  char currentChar;
  currentChar = j;
  *endOfString = parsedCommand->concatenatedExpression + strlen(parsedCommand->concatenatedExpression);
  strncat(parsedCommand->concatenatedExpression, &currentChar, ONE_CHAR_LENGTH);
}

void updateConExpWithStr1(parsedCommandStruct *parsedCommand, char **endOfString, int partIndex, char **orPtr,
                          int *str1size, char **orPtrcopy)
{
  char *copyStr1OrStr2 = malloc(parsedCommand->lengthOfOrigExpression + NULL_TERMINATE_SIZE);
  strcpy(copyStr1OrStr2, parsedCommand->arrayOfExpressionParts[partIndex].expressionPartInfo.str1OrStr2);
  *endOfString = parsedCommand->concatenatedExpression + strlen(parsedCommand->concatenatedExpression);
  *orPtr = strstr(parsedCommand->arrayOfExpressionParts[partIndex].expressionPartInfo.str1OrStr2, "|");
  *orPtrcopy = (*orPtr) + ONE_CHAR_LENGTH;
  **orPtr = '\0';
  *str1size = strlen(parsedCommand->arrayOfExpressionParts[partIndex].expressionPartInfo.str1OrStr2);
  strcat(parsedCommand->concatenatedExpression,
         parsedCommand->arrayOfExpressionParts[partIndex].expressionPartInfo.str1OrStr2);
  strcpy(parsedCommand->arrayOfExpressionParts[partIndex].expressionPartInfo.str1OrStr2, copyStr1OrStr2);
  free(copyStr1OrStr2);
}

void updateConExpWithStr2(parsedCommandStruct *parsedCommand, char **endOfString, char **orPtrcopy)
{
  int str2size = INIT_STR_LENGTH;
  **endOfString = '\0';
  str2size = strlen(*orPtrcopy);
  strncat(parsedCommand->concatenatedExpression, *orPtrcopy, str2size);
}

void reportLineMatchRec(int partIndex, lineInText *currentLineToCheck, parsedCommandStruct *parsedCommand,
                        int *linesAfterMatchCounterPtr, int *prevLinesCounter, int *matchFlag)
{
  int j = 0;
  char *orPtr = NULL;
  int str1size = INIT_STR_LENGTH;
  char *endOfString = NULL;
  char *orPtrcopy = NULL;
  while (partIndex < parsedCommand->numOfExpressionParts) {
    if (parsedCommand->arrayOfExpressionParts[partIndex].partClassifier == REGULAR) {
      updateConExpWithRegChr(parsedCommand, &partIndex);
    } else if (parsedCommand->arrayOfExpressionParts[partIndex].partClassifier == POINT) {
      for (j = ASCII_LOWEST_CHAR; j < ASCII_HIGHEST_CHAR + ONE_CHAR_LENGTH; j++) {
        updateConExpWithChr(j, parsedCommand, &endOfString);
        reportLineMatchRec(partIndex + RECURSE_STEP, currentLineToCheck, parsedCommand, linesAfterMatchCounterPtr,
                           prevLinesCounter, matchFlag);
        *endOfString = '\0';
      }
      return;
    } else if (parsedCommand->arrayOfExpressionParts[partIndex].partClassifier == BRACKETS) {
      for (j = parsedCommand->arrayOfExpressionParts[partIndex].expressionPartInfo.xy[X_INDEX];
           j < parsedCommand->arrayOfExpressionParts[partIndex].expressionPartInfo.xy[Y_INDEX] + ONE_CHAR_LENGTH; j++) {
        updateConExpWithChr(j, parsedCommand, &endOfString);
        reportLineMatchRec(partIndex + RECURSE_STEP, currentLineToCheck, parsedCommand, linesAfterMatchCounterPtr,
                           prevLinesCounter, matchFlag);
        *endOfString = '\0';
      }
      return;
    } else if (parsedCommand->arrayOfExpressionParts[partIndex].partClassifier == ROUND_BRACKETS) {
      updateConExpWithStr1(parsedCommand, &endOfString, partIndex, &orPtr, &str1size, &orPtrcopy);
      reportLineMatchRec(partIndex + RECURSE_STEP, currentLineToCheck, parsedCommand, linesAfterMatchCounterPtr,
                         prevLinesCounter, matchFlag);
      updateConExpWithStr2(parsedCommand, &endOfString, &orPtrcopy);
      reportLineMatchRec(partIndex + RECURSE_STEP, currentLineToCheck, parsedCommand, linesAfterMatchCounterPtr,
                         prevLinesCounter, matchFlag);
      return;
    }
  }
  if (partIndex == parsedCommand->numOfExpressionParts) {
    strcpy(parsedCommand->expressionToSearch, parsedCommand->concatenatedExpression);
    reportLineMatch(currentLineToCheck, *parsedCommand, linesAfterMatchCounterPtr, prevLinesCounter, matchFlag);
    return;
  }
}