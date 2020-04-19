#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include <stdbool.h>
#define XY_ARRAY_SIZE 2
#define INITIALIZED_VALUE 0
// according to notes 5.2,6 we put here these defines which are used also in matchInLine
#define NULL_TERMINATE_SIZE 1
#define ONE_CHAR_LENGTH 1
#define X_INDEX 0
#define Y_INDEX 1
#define INIT_STR_LENGTH 0

typedef enum { REGULAR, POINT, ROUND_BRACKETS, BRACKETS } partClassifier;

union expressionPartInfo {
  char xy[XY_ARRAY_SIZE];
  char *str1OrStr2;
  char regularChar;
} expressionPartInfo;

typedef struct partInExpression {
  union expressionPartInfo expressionPartInfo;
  partClassifier partClassifier;
} partInExpression;

typedef struct parsedCommandStruct {
  char *expressionToSearch;
  char *origExpressionToSearch;
  char *concatenatedExpression;
  int lengthOfOrigExpression;
  int numOfExpressionParts;
  bool a;
  int aNum;
  bool b;
  bool c;
  bool i;
  bool n;
  bool v;
  bool x;
  bool E;
  struct partInExpression *arrayOfExpressionParts;
} parsedCommandStruct;

void createAndFillCommand(int argc, char *argv[], parsedCommandStruct **parsedCommand);
void freeParsedCommandStruct(parsedCommandStruct *parsedCommand);
bool isUsingFile(int argc, char *argv[]);
void initializeConcatinateExp(parsedCommandStruct *parsedCommand);

#endif  // COMMANDPARSER_H
