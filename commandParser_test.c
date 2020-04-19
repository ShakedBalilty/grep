#include "commandParser.h"
#include <assert.h>
#include <string.h>

void testIsUsingFile()
{
  int argc[] = {6, 6, 8}, testExpectedResults[3] = {1, 0, 1};
  char *argv0[] = {"my_grep", "bla", "-n", "-c", "-i", "2013.html"},
       *argv1[] = {"my_grep", "-x", "-v", "-A", "2", "yoU"},
       *argv2[] = {"my_grep", "-b", "-i", "blue", "-A", "1", "-v", "colors.txt"};
  assert(testExpectedResults[0] == isUsingFile(argc[0], argv0));
  assert(testExpectedResults[1] == isUsingFile(argc[1], argv1));
  assert(testExpectedResults[2] == isUsingFile(argc[2], argv2));
}

void testCreateAndFillCommand()
{
  parsedCommandStruct *parsedCommand = NULL;
  int argc[] = {6, 6, 9};
  char *argv0[] = {"my_grep", "bla", "-n", "-c", "-i", "2013.html"},
       *argv1[] = {"my_grep", "-x", "-v", "-A", "2", "yoU"},
       *argv2[] = {"my_grep", "-b", "-i", ".(blue|white)\{[a-c]M", "-A", "1", "-v", "-E", "colors.txt"};
  createAndFillCommand(argc[0], argv0, &parsedCommand);
  assert(parsedCommand->n);
  assert(parsedCommand->c);
  assert(parsedCommand->i);
  assert(!parsedCommand->x);
  assert(!parsedCommand->v);
  assert(!(strcmp(parsedCommand->expressionToSearch, "bla")));
  createAndFillCommand(argc[1], argv1, &parsedCommand);
  assert(parsedCommand->x);
  assert(parsedCommand->v);
  assert(parsedCommand->a);
  assert(parsedCommand->aNum == 2);
  assert(!parsedCommand->i);
  assert(!(strcmp(parsedCommand->expressionToSearch, "yoU")));
  createAndFillCommand(argc[2], argv2, &parsedCommand);
  assert(parsedCommand->a);
  assert(parsedCommand->i);
  assert(parsedCommand->b);
  assert(parsedCommand->aNum == 1);
  assert(parsedCommand->v);
  assert(parsedCommand->E);
  assert(!(strcmp(parsedCommand->expressionToSearch, ".(blue|white)\{[a-c]M")));
  assert(parsedCommand->arrayOfExpressionParts[0].partClassifier == POINT);
  assert(parsedCommand->arrayOfExpressionParts[1].partClassifier == ROUND_BRACKETS);
  assert(!strcmp(parsedCommand->arrayOfExpressionParts[1].expressionPartInfo.str1OrStr2, "blue|white"));
  assert(parsedCommand->arrayOfExpressionParts[2].partClassifier == REGULAR);
  assert(parsedCommand->arrayOfExpressionParts[2].expressionPartInfo.regularChar == '{');
  assert(parsedCommand->arrayOfExpressionParts[3].partClassifier == BRACKETS);
  assert(!strcmp(parsedCommand->arrayOfExpressionParts[3].expressionPartInfo.xy, "ac"));
  assert(parsedCommand->arrayOfExpressionParts[4].partClassifier == REGULAR);
  assert(parsedCommand->arrayOfExpressionParts[4].expressionPartInfo.regularChar == 'M');
  freeParsedCommandStruct(parsedCommand);
}

void testFreeParsedCommandStruct()
{
  parsedCommandStruct *parsedCommand = NULL;
  int testExpectedResults[3] = {1, 0, 1};
  char *argv0[] = {"my_grep", "bla", "-n", "-c", "-i", "2013.html"},
       *argv1[] = {"my_grep", "-x", "-v", "-A", "2", "yoU"},
       *argv2[] = {"my_grep", "-b", "-i", "blue", "-A", "1", "-v", "colors.txt"};
  createAndFillCommand(6, argv0, &parsedCommand);
  freeParsedCommandStruct(parsedCommand);
  createAndFillCommand(6, argv1, &parsedCommand);
  freeParsedCommandStruct(parsedCommand);
  createAndFillCommand(8, argv2, &parsedCommand);
  freeParsedCommandStruct(parsedCommand);
}

void main()
{
  testIsUsingFile();
  testCreateAndFillCommand();
  testFreeParsedCommandStruct();
}
