#Jorge Farinacci
#125008668
#CSCE 420
#Due: June 3, 2019
#hw1pr1.py

import sys

#program varibales
totalNumColumns = 0
totalNumRows = 0
matrixFields = []
current_row_indicator = 0
current_column_indicator = 0
#reading user arguments
lineCount = 0

#read in all user inputs
for line in sys.stdin:
    line = line.strip()
    if lineCount == 0:
        totalNumRows = int(line)
    elif lineCount == 1:
        totalNumColumns = int(line)
    else:
        fields = line.split(" ")
        if len(fields) != totalNumColumns and len(fields) != (totalNumRows * totalNumColumns):
            print("invalid number of fields on line {} field count provided was {}".format(lineCount,len(fields) ))
            sys.exit()
        for field in fields:
            matrixFields.append(int(field))
        if len(matrixFields) == (totalNumRows * totalNumColumns):
            break
    lineCount += 1
def getLocation():
    global current_row_indicator
    global current_column_indicator
    global totalNumColumns
    return current_row_indicator * totalNumColumns + current_column_indicator
def performCleanOp(isClean):
    if isClean == 0:
        return 'NoOp'
    elif isClean == 1:
       return 'suck'
def performMoveOp():
    global current_row_indicator
    global current_column_indicator
    global totalNumColumns
    moveop = "noOp"
    if current_row_indicator% 2 == 0:
        if current_column_indicator != totalNumColumns - 1:
            moveop = 'right'
            current_column_indicator+= 1
        else:
            moveop = 'down'
            current_row_indicator += 1
    else:
        if current_column_indicator != 0:
            moveop = 'left'
            current_column_indicator-= 1
        else:
            moveop = 'down'
            current_row_indicator += 1
    return moveop
#with no mention of obstacles the program assumes only 2 states clean and dirty as well as the ability to move around feely within a rectangular grid (meaning no obstacles will obstruct its path).
locationsVisited = 0
visitedMap = [[0]*totalNumRows*totalNumColumns]
clean = 0
performanceMeasure = 0
while locationsVisited != len(matrixFields) :
    if performCleanOp(int (matrixFields [getLocation()])) == 'suck':
        matrixFields [getLocation()] = 0
        clean += 1
    else:
        if visitedMap[current_row_indicator][current_column_indicator] == 0:
            visitedMap[current_row_indicator][current_column_indicator] = 1
            locationsVisited  += 1
        performMoveOp()
    performanceMeasure += clean

print(performanceMeasure)
