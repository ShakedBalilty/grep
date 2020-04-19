#ifndef MATCHINLINE_H
#define MATCHINLINE_H
#include "commandParser.h"
#include "textLine.h"

void reportLineMatchRec(int partIndex, lineInText *currentLineToCheck, parsedCommandStruct *parsedCommand,
                        int *linesAfterMatchCounterPtr, int *prevLinesCounter, int *matchFlag);

#endif  // MATCHINLINE_H
