
import copy
class EightPuzzle():
    def __init__(self,startState,goalState):
        start =  [x.strip() for x in startState.split(",")]
        goal = [x.strip() for x in goalState.split(",")]
        start = list(map(int,start))
        goal = list(map(int,goal))
        self.currState = start
        self.goalState = goal

    def displayBoard(self):
        boardStr = ''
        for i in range (0,9):
                boardStr += str(self.currState[i]) + ''
                if i % 3 == 0:
                    boardStr += '\n'
        print(boardStr)

    def getBlankIndex(self):
        for i in range (0,9):
            if self.currState[i] == 0:
                return i

    def goalReached(self):
        if self.currState == self.goalState:
            return True
        else:
            return False
    def getNeighbors(self):
        neighbors = []
        zeroIndex = self.getBlankIndex()
        if zeroIndex in [0,1,3,4,6,7]:
            right = copy.deepcopy(self)
            right.currState[zeroIndex], right.currState[zeroIndex + 1] = right.currState[zeroIndex + 1], right.currState[zeroIndex]
            neighbors.append((right,'right'))
        if zeroIndex in [1,2,4,5,7,8]:
            left = copy.deepcopy(self)
            left.currState[zeroIndex], left.currState[zeroIndex - 1] = left.currState[zeroIndex - 1], left.currState[zeroIndex] 
            neighbors.append((left,'left'))
        if zeroIndex in [3,4,5,6,7,8]:
            up = copy.deepcopy(self)
            up.currState[zeroIndex], up.currState[zeroIndex - 3] = up.currState[zeroIndex - 3], up.currState[zeroIndex]
            neighbors.append((up,'up'))
        if zeroIndex in [0,1,2,3,4,5]:
            down = copy.deepcopy(self)
            down.currState[zeroIndex], down.currState[zeroIndex + 3] = down.currState[zeroIndex + 3], down.currState[zeroIndex]
            neighbors.append((down,'down'))
        return neighbors
    def bfs(self):
        queue = []
        visited = []
        current = self
        queue.append(current)
        isPathFound = False
        for node in queue:
            for neighbor in node.getNeighbors():
                if neighbor[0] in visited:
                    continue
                queue.append(neighbor[0])
                if neighbor[0].goalReached():
                    isPathFound = True
                    break
            queue = queue[1:]
            visited.append(node)
            if isPathFound:
                break
        return isPathFound

def main():
    startState = input("Enter 8-puzzle starting state by rows (0 for blank): ")
    goalState = input("Enter 8-puzzle ending state by rows (0 for blank): ")
    game = EightPuzzle(startState,goalState)
    print(game.bfs())
if __name__ == '__main__':
    main()
