#Jorge Farinacci
#125008668
#CSCE 420
#Due: June 3, 2019
#hw1pr2.py

import random
room = [[]]
visited = [[]]
currentRow = 0
currentColumn = 0
prevRow = 0
prevColumn = 0
def generateRandomRoom():
    global room
    global visited
    global currentRow
    global currentColumn
    global prevRow
    global prevColumn
    room =[[0 for i in range (1000)] for j in range(1000)]
    visited = [[0 for i in range(1000)] for j in range(1000)]
    currentRow = 0
    currentColumn = 0
    prevRow = 0
    prevColumn = 0
    # Make random cells dirty
    numOfDirtyCells = random.randint(0,1000000)
    for x in range(numOfDirtyCells):
        i = random.randint(0,999)
        j = random.randint(0,999)
        room[i][j] = 1
    #add obstacles to random cells
    numOfObstacles = random.randint(0,1000000)
    for x in range(numOfObstacles):
        i = random.randint(0,999)
        j = random.randint(0,999)
        room[i][j] = -1

def collisionDetection(i, j):
    global room
    if room[i][j]== -1:
        return 'collision detected'
    else:
        return 'No collision'
def cleanOp(i,j):
    global room
    if room[i][j] == 1:
        return 'Suck'
    else:
        return 'Is Clean'
def moveOp(i,j):
    global currentRow
    global prevRow
    global currentColumn
    global prevColumn
    list = ['Up','Down','Left','Right']
    if i == 0:
        list = list[1:]
    elif i == 999:
        list = [x for x in list if x != 'Down']
    if j == 0:
        list = [x for x in list if x != 'Left']
    elif j == 999:
       list = list[:-1]
    direction = random.choice(list)
    prevRow = currentRow
    prevColumn = currentColumn
    if direction == 'Up':
        currentRow -= 1
    elif direction == 'Down':
        currentRow += 1
    elif direction == 'Right':
        currentColumn += 1
    elif direction == 'Left':
        currentColumn -= 1
    return direction
def undoMoveOp():
    global currentRow
    global prevRow
    global currentColumn
    global prevColumn
    direction = 'None'
    if currentColumn < prevColumn:
        direction = 'Right'
    elif currentColumn > prevColumn:
        direction ='Left'
    elif currentRow < prevRow:
        direction = 'Down'
    elif currentRow > prevRow:
        direction = 'Up'
    currentRow = prevRow
    currentColumn = prevColumn
    return direction
def agentProgram():
    generateRandomRoom()
    actionCount = 0
    performanceMeasure = 0
    numCleanTiles = 0
    totalNewCellsVisited = 0
    while actionCount != 2000000:
        if totalNewCellsVisited == 1000000:
            break
        if collisionDetection(currentRow,currentColumn) == "No collision":
            isClean = cleanOp(currentRow,currentColumn)
            if isClean == 'Suck':
               room[currentRow][currentColumn] = 0
            elif isClean == 'Is Clean':
                if visited[currentRow][currentColumn] == 0:
                    totalNewCellsVisited = 1
                    numCleanTiles += 1
                    visited[currentRow][currentColumn] = 1
                moveOp(currentRow,currentColumn)
        elif collisionDetection(currentRow,currentColumn) == "collision detected":
            visited[currentRow][currentColumn] = 1
            undoMoveOp()
        performanceMeasure += numCleanTiles
        actionCount += 1

    return performanceMeasure

def main():
    total = 0
    for x in range(100): total += agentProgram()
    avg = total/100
    print(avg)
if __name__ == '__main__':
    main()
