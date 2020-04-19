#include "commandParser.h"
#include <stdlib.h>
#include <string.h>
#define EXIT_ERROR -1
#define INITIAL_PARAMS_COUNTER 0
#define ARGV_INDEX_AFTER_PROG_NAME 1
#define FIRST_CHAR_IN_STRING 0
#define PARAMS_COUNT_WITH_FILE 2
#define INITIAL_PARTS_SUM 0
#define BRACKET_STEP 4
#define BACKSLASH_STEP 1
#define LENGTH_TO_X 1
#define LENGTH_TO_Y 3
#define INIT_ARRAY_VAL 0
#define A_STEP 1
#define INIT_A_NUM 0

bool isUsingFile(int argc, char *argv[])
{
  int nonParamArgs = INITIAL_PARAMS_COUNTER, i = ARGV_INDEX_AFTER_PROG_NAME;
  for (i = ARGV_INDEX_AFTER_PROG_NAME; i < argc; i++) {
    if (argv[i][FIRST_CHAR_IN_STRING] == '-') {
      continue;
    } else {
      if (strcmp(&argv[i - A_STEP][FIRST_CHAR_IN_STRING], "-A") == 0) {
        continue;
      } else {
        nonParamArgs += 1;
      }
    }
  }
  return nonParamArgs == PARAMS_COUNT_WITH_FILE;
}

void updateArgumentsOfCommandStruct(int argc, char *argv[], parsedCommandStruct *parsedCommand)
{
  int i;

  for (i = ARGV_INDEX_AFTER_PROG_NAME; i < argc; i++) {
    if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-b")) {
      parsedCommand->b = true;
    } else if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-c")) {
      parsedCommand->c = true;
    } else if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-i")) {
      parsedCommand->i = true;
    } else if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-n")) {
      parsedCommand->n = true;
    } else if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-v")) {
      parsedCommand->v = true;
    } else if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-x")) {
      parsedCommand->x = true;
    } else if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-A")) {
      parsedCommand->a = true;
      parsedCommand->aNum = atoi(argv[i + A_STEP]);
    } else if (!strcmp(&argv[i][FIRST_CHAR_IN_STRING], "-E")) {
      parsedCommand->E = true;
    } else {
      continue;
    }
  }
}

void initializeCommandArgs(parsedCommandStruct **parsedCommand)
{
  (*parsedCommand)->b = false;
  (*parsedCommand)->c = false;
  (*parsedCommand)->i = false;
  (*parsedCommand)->n = false;
  (*parsedCommand)->v = false;
  (*parsedCommand)->x = false;
  (*parsedCommand)->a = false;
  (*parsedCommand)->E = false;
  (*parsedCommand)->aNum = INIT_A_NUM;
  (*parsedCommand)->arrayOfExpressionParts = NULL;
  (*parsedCommand)->lengthOfOrigExpression = INIT_STR_LENGTH;
  (*parsedCommand)->expressionToSearch = NULL;
  (*parsedCommand)->origExpressionToSearch = NULL;
  (*parsedCommand)->concatenatedExpression = NULL;
}

int searchExpressionArgumentIndex(int argc, char *argv[])
{
  int i = ARGV_INDEX_AFTER_PROG_NAME;
  for (i = ARGV_INDEX_AFTER_PROG_NAME; i < argc; i++) {
    if (argv[i][FIRST_CHAR_IN_STRING] != '-' && strcmp(&argv[i - A_STEP][FIRST_CHAR_IN_STRING], "-A") != 0) {
      return i;
    } else {
      continue;
    }
  }
  exit(EXIT_ERROR);
}

int computeNumOfParts(char *expression)
{
  int i = 0, partsSum = INITIAL_PARTS_SUM, expressionLen = strlen(expression);
  char *closingBracket = NULL;
  for (i = 0; i < expressionLen; i++) {
    if (expression[i] == '\\') {
      partsSum++;
      i = i + BACKSLASH_STEP;
    } else if (expression[i] == '[') {
      partsSum++;
      i = i + BRACKET_STEP;
    } else if (expression[i] == '.') {
      partsSum++;
    } else if (expression[i] == '(') {
      partsSum++;
      closingBracket = strstr(expression + i, ")");
      i = closingBracket - expression;
    } else {
      partsSum++;
    }
  }
  return partsSum;
}

void fillRegularChar(parsedCommandStruct *parsedCommand, int indexOfParts, char charToFill)
{
  parsedCommand->arrayOfExpressionParts[indexOfParts].partClassifier = REGULAR;
  parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.regularChar = charToFill;
}

void fillBrackets(parsedCommandStruct *parsedCommand, int indexOfParts, char x, char y)
{
  parsedCommand->arrayOfExpressionParts[indexOfParts].partClassifier = BRACKETS;
  parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.xy[X_INDEX] = x;
  parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.xy[Y_INDEX] = y;
}

int fillRoundBrackets(parsedCommandStruct *parsedCommand, int indexOfParts, char *str1OrStr2)
{
  parsedCommand->arrayOfExpressionParts[indexOfParts].partClassifier = ROUND_BRACKETS;
  char *placeOfClosingBracket = strstr(str1OrStr2, ")");
  int str12Size = placeOfClosingBracket - str1OrStr2 - ONE_CHAR_LENGTH;
  parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.str1OrStr2 = NULL;
  parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.str1OrStr2 =
      malloc(str12Size + NULL_TERMINATE_SIZE);
  memset(parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.str1OrStr2, INIT_ARRAY_VAL,
         str12Size + NULL_TERMINATE_SIZE);
  strncpy(parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.str1OrStr2,
          str1OrStr2 + ONE_CHAR_LENGTH, str12Size);
  return str12Size + ONE_CHAR_LENGTH;
}

void buildArrayOfExpressionParts(parsedCommandStruct *parsedCommand)
{
  int numOfParts = computeNumOfParts(parsedCommand->expressionToSearch);
  int i = 0, indexOfParts = 0;
  parsedCommand->arrayOfExpressionParts = NULL;
  parsedCommand->arrayOfExpressionParts = malloc(numOfParts * sizeof(partInExpression));
  memset(parsedCommand->arrayOfExpressionParts, INIT_ARRAY_VAL, numOfParts * sizeof(partInExpression));
  for (i = 0; i < (int)strlen(parsedCommand->expressionToSearch); i++) {
    if (parsedCommand->expressionToSearch[i] == '.') {
      parsedCommand->arrayOfExpressionParts[indexOfParts].partClassifier = POINT;
    } else if (parsedCommand->expressionToSearch[i] == '[') {
      fillBrackets(parsedCommand, indexOfParts, parsedCommand->expressionToSearch[i + LENGTH_TO_X],
                   parsedCommand->expressionToSearch[i + LENGTH_TO_Y]);
      i = i + BRACKET_STEP;
    } else if (parsedCommand->expressionToSearch[i] == '(') {
      parsedCommand->arrayOfExpressionParts[indexOfParts].expressionPartInfo.str1OrStr2 = NULL;
      i = i + fillRoundBrackets(parsedCommand, indexOfParts, parsedCommand->expressionToSearch + i);
    } else if (parsedCommand->expressionToSearch[i] == '\\') {
      fillRegularChar(parsedCommand, indexOfParts, parsedCommand->expressionToSearch[i + ONE_CHAR_LENGTH]);
      i = i + BACKSLASH_STEP;
    } else {
      fillRegularChar(parsedCommand, indexOfParts, parsedCommand->expressionToSearch[i]);
    }
    indexOfParts++;
  }
}

void createAndFillCommand(int argc, char *argv[], parsedCommandStruct **parsedCommand)
{
  int indexOfExpressionArg = INITIALIZED_VALUE;
  indexOfExpressionArg = searchExpressionArgumentIndex(argc, argv);
  *parsedCommand = (parsedCommandStruct *)malloc(sizeof(struct parsedCommandStruct));
  initializeCommandArgs(parsedCommand);
  (*parsedCommand)->expressionToSearch = (char *)malloc(strlen(argv[indexOfExpressionArg]) + NULL_TERMINATE_SIZE);
  strcpy((*parsedCommand)->expressionToSearch, argv[indexOfExpressionArg]);
  (*parsedCommand)->origExpressionToSearch = (char *)malloc(strlen(argv[indexOfExpressionArg]) + NULL_TERMINATE_SIZE);
  strcpy((*parsedCommand)->origExpressionToSearch, argv[indexOfExpressionArg]);
  (*parsedCommand)->lengthOfOrigExpression = strlen((*parsedCommand)->expressionToSearch);
  (*parsedCommand)->numOfExpressionParts = computeNumOfParts((*parsedCommand)->expressionToSearch);
  updateArgumentsOfCommandStruct(argc, argv, *parsedCommand);
  buildArrayOfExpressionParts(*parsedCommand);
  (*parsedCommand)->concatenatedExpression = (char *)malloc(strlen(argv[indexOfExpressionArg]) + NULL_TERMINATE_SIZE);
  strcpy((*parsedCommand)->concatenatedExpression, "");
}

void initializeConcatinateExp(parsedCommandStruct *parsedCommand) { strcpy(parsedCommand->concatenatedExpression, ""); }

void freeParsedCommandStruct(parsedCommandStruct *parsedCommand)
{
  int i = 0;
  free(parsedCommand->expressionToSearch);
  free(parsedCommand->origExpressionToSearch);
  free(parsedCommand->concatenatedExpression);
  for (i = 0; i < parsedCommand->numOfExpressionParts; i++) {
    if (parsedCommand->arrayOfExpressionParts[i].partClassifier == ROUND_BRACKETS) {
      free(parsedCommand->arrayOfExpressionParts[i].expressionPartInfo.str1OrStr2);
    }
  }
  free(parsedCommand->arrayOfExpressionParts);
}