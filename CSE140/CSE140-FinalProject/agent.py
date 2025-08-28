# from pacai.util import reflection
import random
from pacai.agents.capture.capture import CaptureAgent
from enum import Enum

class TerminalState(Enum):
    REACHED_DEPTH = 1
    TEAM_DIED = 2
    ENEMY_DIED = 3
    WE_WON = 4
    WE_LOST = 5

class GoatAgent(CaptureAgent):
    def __init__(self, index, **kwargs):
        super().__init__(index, **kwargs)
        self.score = 0
        self.actions = []

    def registerInitialState(self, gameState):
        """
        This method handles the initial setup of the agent and populates useful fields,
        such as the team the agent is on and the `pacai.core.distanceCalculator.Distancer`.

        IMPORTANT: If this method runs for more than 15 seconds, your agent will time out.
        """

        super().registerInitialState(gameState)

        # Your initialization code goes here, if you need any.
        self.score = self.getScore(gameState)
        self.opponentPositions = [
            gameState.getAgentPosition(o) for o in self.getOpponents(gameState)
            if gameState.getAgentPosition(o) is not None
        ]
        self.agentPos = gameState.getAgentPosition(self.index)
        self.mazeDistances = [
            self.getMazeDistance(o, self.agentPos) for o in self.opponentPositions
        ]
        self.minMazeDist = float('inf')
        for i, mazeDist in enumerate(self.mazeDistances):
            self.minMazeDist = min(self.minMazeDist, mazeDist)
            self.minMazeIndex = i
        self.food = self.getFood(gameState)

    def chooseAction(self, gameState):
        """
        Randomly pick an action.
        """
        self.score = self.getScore(gameState)
        self.opponentPositions = [
            gameState.getAgentPosition(o) for o in self.getOpponents(gameState)
            if gameState.getAgentPosition(o) is not None
        ]
        self.agentPos = gameState.getAgentPosition(self.index)
        self.mazeDistances = [
            self.getMazeDistance(o, self.agentPos) for o in self.opponentPositions
        ]
        self.minMazeDist = float('inf')
        for i, mazeDist in enumerate(self.mazeDistances):
            if mazeDist < self.minMazeDist:
                self.minMazeDist = mazeDist
                self.minMazeIndex = i

        teammate = self.getTeam(gameState)
        for t in teammate:
            if t != self.index:
                teammate = t
                break
        self.teammatePos = gameState.getAgentPosition(teammate)

        # Sort food list by x-coordinate (left to right)
        foodList = self.getFood(gameState).asList()
        sortedFoodList = sorted(foodList, key=lambda f: f[1])  # Sort by X-coordinate

        # Divide food into left and right halves
        halfway = len(sortedFoodList) // 2
        leftHalf = sortedFoodList[:halfway]
        rightHalf = sortedFoodList[halfway:]

        # Get positions of both agents
        agentPos = gameState.getAgentPosition(self.index)
        teammatePos = gameState.getAgentPosition(teammate)

        # Compute distances of both agents to representative food points in each half
        if leftHalf:
            leftReference = leftHalf[0]  # Leftmost food
            myDistLeft = self.getMazeDistance(agentPos, leftReference)
            teammateDistLeft = self.getMazeDistance(teammatePos, leftReference)

        # if rightHalf:
        #     rightReference = rightHalf[-1]  # Rightmost food
        #     myDistRight = self.getMazeDistance(agentPos, rightReference)
        #     teammateDistRight = self.getMazeDistance(teammatePos, rightReference)

        # print("myDistLeft", myDistLeft)
        # print("teammateDistLeft", teammateDistLeft)
        # print("myDistRight", myDistRight)
        # print("teammateDistRight", teammateDistRight)
        # Assign food lists dynamically
        if myDistLeft <= teammateDistLeft:
            # If I am closer to the left, I take the left half
            # print("Agent", self.index, "took the left")
            self.foodList = leftHalf
        else:
            # Otherwise, I take the right half
            # print("Agent", self.index, "took the right")
            self.foodList = rightHalf
        # print("time left", gameState.getTimeleft())
        # print("num food", len(self.getFood(gameState).asList()))
        if (gameState.isOnRedSide(self.teammatePos) and self.red
           and gameState.isOnBlueSide(self.agentPos)):
            self.foodList = self.getFood(gameState).asList()
        if (gameState.isOnBlueSide(self.teammatePos) and not self.red
           and gameState.isOnRedSide(self.agentPos)):
            self.foodList = self.getFood(gameState).asList()
        actions = gameState.getLegalActions(self.index)
        actions.remove("Stop")
        offense = ((self.red and gameState.isOnBlueSide(self.agentPos))
                   or (not self.red and gameState.isOnRedSide(self.agentPos)))
        multiplier = {"South": 1, "North": 1, "West": 1, "East": 1}

        opponentPositions = [
            gameState.getAgentPosition(o) for o in self.getOpponents(gameState)
        ]
        # Corrected defender check: Am I a ghost or a Pacman?
        if self.red:
            intruders = [opp for opp in opponentPositions if gameState.isOnRedSide(opp)]
        else:
            intruders = [opp for opp in opponentPositions if gameState.isOnBlueSide(opp)]

        sortedIntruderList = sorted(intruders, key=lambda f: f[1])  # Sort by X-coordinate
        self.intruders = []
        # Divide food into left and right halves
        if len(sortedIntruderList) >= 1:
            intruder = sortedIntruderList[0]
            myDistToIntruder = self.getMazeDistance(agentPos, intruder)
            teamDistToIntruder = self.getMazeDistance(teammatePos, intruder)
            addedSecondIntruder = False
            xDistToIntruder = (intruder[0] - agentPos[0])
            yDistToIntruder = (intruder[1] - agentPos[1])
            xtDistToIntruder = (intruder[0] - teammatePos[0])
            ytDistToIntruder = (intruder[1] - teammatePos[1])
            if (not self.same_parity(xDistToIntruder, xtDistToIntruder)
                    or not self.same_parity(yDistToIntruder, ytDistToIntruder)):
                self.intruders.append(intruder)
            else:
                if myDistToIntruder < teamDistToIntruder:
                    self.intruders.append(intruder)
                elif len(sortedIntruderList) == 2:
                    self.intruders.append(sortedIntruderList[1])
                    addedSecondIntruder = True
            
            if not addedSecondIntruder and len(sortedIntruderList) == 2:
                intruder = sortedIntruderList[1]
                xDistToIntruder = (intruder[0] - agentPos[0])
                yDistToIntruder = (intruder[1] - agentPos[1])
                xtDistToIntruder = (intruder[0] - teammatePos[0])
                ytDistToIntruder = (intruder[1] - teammatePos[1])
                if (not self.same_parity(xDistToIntruder, xtDistToIntruder)
                        or not self.same_parity(yDistToIntruder, ytDistToIntruder)):
                    self.intruders.append(intruder)
            
        if offense and len(self.actions) >= 6:
            # Check for 2-step oscillation (A-B-A), Stop Action B
            if (self.actions[-1] == self.actions[-3]
                    and self.actions[-2] == self.actions[-4]
                    and self.actions[-2] != self.actions[-1]):
                if self.actions[-2] in actions and len(actions) > 1:
                    # print("action 1", self.actions[-1])
                    # print("action 3", self.actions[-3])
                    # print("removing action 2:", self.actions[-2])
                    actions.remove(self.actions[-2])
            
            # Check for 3-step oscillation (A-B-C-A-B-C)
            if (self.actions[-1] == self.actions[-4]
                    and self.actions[-2] == self.actions[-5]
                    and self.actions[-3] == self.actions[-6]
                    and (self.actions[-1] != self.actions[-2]
                    or self.actions[-1] != self.actions[-3]
                    or self.actions[-2] != self.actions[-3])):
                if len(actions) > 1:
                    # if self.actions[-1] == self.actions[-2] and self.actions[-3] in actions:
                    #     actions.remove(self.actions[-3])
                    # elif self.actions[-2] == self.actions[-3] and self.actions[-1] in actions:
                    #     actions.remove(self.actions[-1])
                    # elif self.actions[-1] == self.actions[-3] and self.actions[-2] in actions:
                    #     actions.remove(self.actions[-2])
                    # else:
                    if self.actions[-3] in actions:
                        actions.remove(self.actions[-3])
        bestActions = self.Minimax(gameState, 0, actions, multiplier)
        chosenAction = random.choice(bestActions)
        self.actions.append(chosenAction)
      
        return chosenAction
        actions = gameState.getLegalActions(self.index)
        return random.choice(actions)
    
    def same_parity(self, a, b):
        return (a >= 0 and b >= 0) or (a < 0 and b < 0)

    def Minimax(self, state, depth, legalActions, multiplier):
        """
        Returns the best action(s) for Pacman using the Minimax algorithm.
        """
        best_actions = []
        best_value = -float('inf')
        team = self.getTeam(state)
        opp = self.getOpponents(state)
        for action in legalActions:
            if action == "Stop":
                continue
            successor = state.generateSuccessor(self.index, action)
            if team[-1] == self.index:
                value = self.Minvalue(successor, depth, opp[0], -float('inf'), float('inf'))
            else:
                value = self.Maxvalue(successor, depth, self.index + 2, -float('inf'), float('inf'))
            value *= multiplier[action]
            if value > best_value:
                best_value = value
                best_action = [action]
            elif abs(value - best_value) < 0.0001:
                best_actions.append(action)
        return best_action
    
    def Maxvalue(self, state, depth, agentId, alpha, beta):
        pos = state.getAgentPosition(self.index)
        initPos = state.getInitialAgentPosition(self.index)
        team = self.getTeam(state)
        opp = self.getOpponents(state)
        oppPos1 = state.getAgentPosition(opp[0])
        oppPos2 = state.getAgentPosition(opp[1])
        oppInitPos1 = state.getInitialAgentPosition(opp[0])
        oppInitPos2 = state.getInitialAgentPosition(opp[1])
        weDied = (
            pos[0] == initPos[0]
            and pos[1] == initPos[1]
        )
        enemy1Died = (
            oppPos1[0] == oppInitPos1[0]
            and oppPos1[1] == oppInitPos1[1]
        )
        enemy2Died = (
            oppPos2[0] == oppInitPos2[0]
            and oppPos2[1] == oppInitPos2[1]
        )

        if depth == 1 or weDied or enemy1Died or enemy2Died or state.isOver():
            if depth == 1:
                return self.betterEvaluationFunction(state, TerminalState.REACHED_DEPTH)
            elif weDied:
                return self.betterEvaluationFunction(state, TerminalState.TEAM_DIED)
            elif enemy1Died or enemy2Died:
                return self.betterEvaluationFunction(state, TerminalState.ENEMY_DIED)
            elif state.isWin():
                return self.betterEvaluationFunction(state, TerminalState.WE_WON)
            elif state.isLose():
                return self.betterEvaluationFunction(state, TerminalState.WE_LOST)

        v = -float('inf')
        for action in state.getLegalActions(agentId):
            if action == "Stop":
                continue
            successor = state.generateSuccessor(agentId, action)
            if team[-1] == agentId:
                v = max(v, self.Minvalue(successor, depth + 1, opp[0], alpha, beta))
            else:
                v = max(v, self.Maxvalue(successor, depth, agentId + 2, alpha, beta))
            if v >= beta:
                return v
            alpha = max(alpha, v)
        return v

    def Minvalue(self, state, depth, agentId, alpha, beta):
        pos = state.getAgentPosition(self.index)
        initPos = state.getInitialAgentPosition(self.index)
        team = self.getTeam(state)
        opp = self.getOpponents(state)
        oppPos1 = state.getAgentPosition(opp[0])
        oppPos2 = state.getAgentPosition(opp[1])
        oppInitPos1 = state.getInitialAgentPosition(opp[0])
        oppInitPos2 = state.getInitialAgentPosition(opp[1])
        weDied = (
            pos[0] == initPos[0]
            and pos[1] == initPos[1]
        )  # Did we die?
        enemy1Died = (
            oppPos1[0] == oppInitPos1[0]
            and oppPos1[1] == oppInitPos1[1]
        )  # Did the enemy die?
        enemy2Died = (
            oppPos2[0] == oppInitPos2[0]
            and oppPos2[1] == oppInitPos2[1]
        )  # Did the enemy die?

        if weDied or enemy1Died or enemy2Died or state.isOver():
            if weDied:
                return self.betterEvaluationFunction(state, TerminalState.TEAM_DIED)
            elif enemy1Died or enemy2Died:
                return self.betterEvaluationFunction(state, TerminalState.ENEMY_DIED)
            elif state.isWin():
                return self.betterEvaluationFunction(state, TerminalState.WE_WON)
            elif state.isLose():
                return self.betterEvaluationFunction(state, TerminalState.WE_LOST)

        v = float('inf')
        for action in state.getLegalActions(agentId):
            if action == "Stop":
                continue
            successor = state.generateSuccessor(agentId, action)
            if agentId == opp[-1]:
                v = min(v, self.Maxvalue(successor, depth, team[0], alpha, beta))
            else:
                v = min(v, self.Minvalue(successor, depth, agentId + 2, alpha, beta))
            if v <= alpha:
                return v
            beta = min(beta, v)
        return v
    
    def betterEvaluationFunction(self, gameState, terminalType):
        """
        Improved evaluation function that separates offensive and defensive behavior.
        Ensures that a defender does not incorrectly run away when near enemy Pacmans.
        """
        capsules = self.getCapsules(gameState)
        teammate = self.getTeam(gameState)
        for t in teammate:
            if t != self.index:
                teammate = t
                break
        teammatePos = gameState.getAgentPosition(teammate)
        agentPos = gameState.getAgentPosition(self.index)
        # Corrected defender check: Am I a ghost or a Pacman?
        if self.red:
            amIDefender = gameState.isOnRedSide(agentPos)
        else:
            amIDefender = gameState.isOnBlueSide(agentPos)

        score = self.getScore(gameState)
        score += self.evalTerminal(gameState, teammatePos, agentPos, terminalType, self.intruders)
        score += self.evalActions(score, capsules)

        defendingFoodList = self.getFoodYouAreDefending(gameState).asList()
        defendingCapsules = self.getCapsulesYouAreDefending(gameState)
    
        # 🛡️ **Defensive Mode (Ghost)**
        if amIDefender or len(defendingFoodList) < 5:
            score += self.evalDefenderMode(
                gameState,
                agentPos,
                self.intruders,
                self.foodList,
                defendingFoodList,
                defendingCapsules
            )
        # 🏃 **Offensive Mode (Pacman)**
        else:
            score += self.evalOffensiveMode(
                agentPos,
                gameState,
                capsules,
                self.foodList
            )
    
        return score

    def evalTerminal(self, gameState, teammatePos, agentPos, terminalType, intruders):
        score = 0
        if terminalType == TerminalState.ENEMY_DIED:
            score += 1000
        elif terminalType == TerminalState.TEAM_DIED:
            bothPlayersOnBlue = (
                gameState.isOnBlueSide(teammatePos)
                and gameState.isOnBlueSide(agentPos)
                and self.red
            )
            bothPlayersOnRed = (
                gameState.isOnRedSide(teammatePos)
                and gameState.isOnRedSide(agentPos)
                and not self.red
            )
            # if (((gameState.isOnRedSide(teammatePos) and self.red)
            # or (gameState.isOnBlueSide(teammatePos) and not self.red))
            # and intruders):
            if (bothPlayersOnBlue or bothPlayersOnRed) and intruders:
                score -= 10
            else:
                score -= 1000
        elif terminalType == TerminalState.WE_WON:
            score += float('inf')
        elif terminalType == TerminalState.WE_LOST:
            score -= float('inf')
        return score
    
    '''
    Did we eat food or a capsule?
    '''
    def evalActions(self, score, capsules):
        # If our score is higher, that means we ate food.
        if score > self.score:
            score += 100
        # If our score is lower, that means we lost food.
        elif score < self.score:
            score -= 20
        if self.red:
            prevCapsules = (
                self.getPreviousObservation().getBlueCapsules()
                if self.getPreviousObservation() else capsules
            )
        else:
            prevCapsules = (
                self.getPreviousObservation().getRedCapsules()
                if self.getPreviousObservation() else capsules
            )
        if len(capsules) < len(prevCapsules):
            score += 500
        # If the enemy's capsules have decreased, that means we ate it.
        if len(capsules) < len(prevCapsules):
            score += 500
        return score

    def evalDefenderMode(self, gameState, agentPos, intruders, foodList, defFoodList, defCapsules):
        score = 0
        # Basic intruder chasing logic
        if intruders:
            closestIntruderDist = (
                min(self.getMazeDistance(agentPos, intruder)
                for intruder in intruders)
            )
            score += 100 / (closestIntruderDist + 1)  # Strong reward for chasing Pacman
        
        # Additional defense: prefer staying near food to guard it
        if foodList:
            closestFoodDist = min(self.getMazeDistance(agentPos, food) for food in foodList)
            if intruders:
                score += 1 / (closestFoodDist + 1)  # Slight bonus for positioning near food
            else:
                score += 10 / (closestFoodDist + 1)
        
        # Find closest food to each intruder
        if intruders and defFoodList:
            for intruder in intruders:
                # Find the food closest to this intruder
                closestDefendFoodDist = float('inf')
                closestDefendFood = None
                for food in defFoodList:
                    dist = self.getMazeDistance(intruder, food)
                    if dist < closestDefendFoodDist:
                        closestDefendFood = food
                        closestDefendFoodDist = dist
                
                if closestDefendFood:
                    # Calculate path metrics
                    intruderToAgentDist = self.getMazeDistance(intruder, agentPos)
                    agentToFoodDist = self.getMazeDistance(agentPos, closestDefendFood)
                    intruderToFoodDist = self.getMazeDistance(intruder, closestDefendFood)
                    
                    # Calculate deviation from ideal path
                    pathDeviation = (intruderToAgentDist + agentToFoodDist) - intruderToFoodDist
                    
                    # Create a graduated scoring system that:
                    # 1. Gives maximum reward when directly on path (deviation < 2)
                    # 2. Decreases reward as deviation increases
                    # 3. Still gives some reward even with larger deviations
                    if pathDeviation < 2:
                        interceptScore = 200 / (intruderToAgentDist + 1)
                    elif pathDeviation < 5:
                        interceptScore = 150 / (intruderToAgentDist + 1)
                    elif pathDeviation < 10:
                        interceptScore = 100 / (intruderToAgentDist + 1)
                    else:
                        # Base score to always encourage moving toward the intercept path
                        interceptScore = 50 / (pathDeviation + intruderToAgentDist + 1)
                    
                    score += interceptScore

        # Defend capsules - prioritize positioning between intruders and capsules
        if intruders and defCapsules:
            for intruder in intruders:
                # Find the capsule closest to this intruder
                closestCapsuleDist = float('inf')
                closestCapsule = None
                for capsule in defCapsules:
                    dist = self.getMazeDistance(intruder, capsule)
                    if dist < closestCapsuleDist:
                        closestCapsule = capsule
                        closestCapsuleDist = dist
                
                if closestCapsule:
                    # Calculate path metrics
                    intruderToAgentDist = self.getMazeDistance(intruder, agentPos)
                    agentToCapsuleDist = self.getMazeDistance(agentPos, closestCapsule)
                    intruderToCapsuleDist = self.getMazeDistance(intruder, closestCapsule)
                    
                    # Calculate deviation from ideal path
                    pathDeviation = ((intruderToAgentDist + agentToCapsuleDist)
                    - intruderToCapsuleDist)
                    
                    # Create a graduated scoring system for capsules (with higher values)
                    if pathDeviation < 2:
                        interceptScore = 300 / (intruderToAgentDist + 1)
                    elif pathDeviation < 5:
                        interceptScore = 250 / (intruderToAgentDist + 1)
                    elif pathDeviation < 10:
                        interceptScore = 200 / (intruderToAgentDist + 1)
                    else:
                        # Base score to always encourage moving toward the intercept path
                        interceptScore = 100 / (pathDeviation + intruderToAgentDist + 1)
                    
                    score += interceptScore
                    
                    # Direct capsule guarding - higher priority than food
                    capsuleGuardScore = (len(defFoodList)
                    / (self.getMazeDistance(agentPos, closestCapsule) + 1))
                    score += capsuleGuardScore
        
        # Extra priority for last few food dots
        if len(defFoodList) <= 4 and defFoodList and self.getScore(gameState) > 0:
            closestDefendDist = min(
                self.getMazeDistance(agentPos, food)
                for food in defFoodList
            )
            score += 50 / (closestDefendDist + 1)  # Higher weight for last few dots
            
        return score
    
    def calculateGhostMetrics(self, agentPos, gameState):
        ghosts = []
        closestGhostDist = float('inf')
        closestGhostIndex = -1
        if self.red:
            ghosts = []
            closestGhostDist = float('inf')
            closestGhostIndex = -1
            for opp in self.getOpponents(gameState):
                pos = gameState.getAgentPosition(opp)
                if not gameState.isOnRedSide(pos):
                    ghosts.append(pos)
                    dist = self.getMazeDistance(pos, agentPos)
                    if dist < closestGhostDist:
                        closestGhostDist = dist
                        closestGhostIndex = opp
        else:
            ghosts = []
            closestGhostDist = float('inf')
            closestGhostIndex = -1
            for opp in self.getOpponents(gameState):
                pos = gameState.getAgentPosition(opp)
                if not gameState.isOnBlueSide(pos):
                    ghosts.append(pos)
                    dist = self.getMazeDistance(pos, agentPos)
                    if dist < closestGhostDist:
                        closestGhostDist = dist
                        closestGhostIndex = opp
            
        if gameState.getAgentState(closestGhostIndex).isScared():
            scared = True
        else:
            scared = False
        return ghosts, closestGhostDist, scared

    def evalOffensiveMode(self, agentPos, gameState, capsules, foodList):
        score = 0
        ghosts, closestGhostDist, scared = self.calculateGhostMetrics(agentPos, gameState)
        
        # Ghost avoidance/scared mechanics
        if ghosts:
            if closestGhostDist < 3:
                if not scared:
                    # Avoid ghosts more strongly at very close distances
                    score -= (4 - closestGhostDist)
                else:
                    # Chase scared ghosts with higher reward when closer
                    score += 1000 / (closestGhostDist + 0.1)
            else:
                if not scared:
                    # More nuanced avoidance at medium distances
                    if closestGhostDist < 5:
                        score -= 50 / (closestGhostDist)
                    else:
                        score -= 5 / (closestGhostDist)
                else:
                    # Chase scared ghosts
                    score += 30 / (closestGhostDist + 0.5)
        
        # Food collection strategy
        if foodList:
            # Find closest food
            closestFoodDist = min(self.getMazeDistance(agentPos, food) for food in foodList)
            
            # Calculate density of food - prefer areas with multiple food items
            foodDensity = sum(1 for food in foodList
                            if self.getMazeDistance(agentPos, food) < 8)
            
            # Calculate food value based on remaining food (more valuable as fewer remain)
            foodValue = 20 / (closestFoodDist + 1) * (15 / (len(foodList) + 1))
            
            # Add density bonus
            foodValue += foodDensity * 5
            
            if not scared and ghosts and closestGhostDist < 5:
                # Reduce food value if ghosts are nearby and not scared
                foodValue *= (closestGhostDist / 5)
            
            score += foodValue
        
        # Capsule strategy - fixed the duplicate capsule code
        if capsules:
            closestCapsuleDist = min(self.getMazeDistance(agentPos, cap) for cap in capsules)
            
            # Prioritize capsules when ghosts are near
            if ghosts and closestGhostDist < 6 and not scared:
                # Higher priority when ghosts are closer
                capsuleValue = 300 / (closestCapsuleDist + 1)
                # Even higher if ghost is very close
                if closestGhostDist < 3:
                    capsuleValue *= 2
                score += capsuleValue
            else:
                # Normal capsule value
                score += 50 / (closestCapsuleDist + 1)
        
                # Add punishment for staying in the same area and reward for moving away
        circlingPenalty = 0
        if len(self.observationHistory) >= 10:  # Need enough observations to detect patterns
            # Get recent positions from observation history
            recentPositions = []
            lookback = min(8, len(self.observationHistory))
            
            for i in range(lookback):
                if i < len(self.observationHistory):
                    pos = self.observationHistory[-(i + 1)].getAgentPosition(self.index)
                    # Only consider positions in enemy territory
                    if self.red:
                        if gameState.isOnBlueSide(pos):
                            recentPositions.append(pos)
                    else:
                        if gameState.isOnRedSide(pos):
                            recentPositions.append(pos)
            
            # Only proceed if we have enough enemy-territory positions to analyze
            if len(recentPositions) > 5:
                # Check if we're circling in a small area
                uniquePositions = set(recentPositions)
                
                # Check if we've visited any position more than once
                positionCounts = {}
                for pos in recentPositions:
                    if pos in positionCounts:
                        positionCounts[pos] += 1
                    else:
                        positionCounts[pos] = 1
                
                # Count how many positions we've visited multiple times
                repeatedPositions = sum(1 for pos, count in positionCounts.items() if count > 1)
                
                # Check if all positions are within a small area (Manhattan distance of 4)
                smallArea = True
                positions = list(uniquePositions)
                if len(uniquePositions) >= 2:
                    for i in range(len(positions)):
                        for j in range(i + 1, len(positions)):
                            x1, y1 = positions[i]
                            x2, y2 = positions[j]
                            manhattanDist = abs(x1 - x2) + abs(y1 - y2)
                            if manhattanDist > 4:
                                smallArea = False
                                break
                        if not smallArea:
                            break
                
                # If positions form a small area,
                # check if current position is moving away or staying near
                if smallArea and len(uniquePositions) < lookback:
                    # Calculate distance from current position to all recent positions
                    distancesToRecent = []
                    for pos in recentPositions:
                        dist = self.getMazeDistance(agentPos, pos)
                        distancesToRecent.append(dist)
                    
                    # Average distance to recent positions
                    # avgDistance = sum(distancesToRecent) / len(distancesToRecent)
                    
                    # Minimum distance to any recent position
                    minDistance = min(distancesToRecent) if distancesToRecent else 0
                    
                    # Check if we're moving away from recent positions or staying in the area
                    if minDistance < 3:  # Still close to recent positions
                        # Calculate how restricted the movement is
                        movementRatio = len(uniquePositions) / lookback
                        
                        # More penalty for more repetition (lower unique/total ratio)
                        circlingPenalty = -50 * (1 - movementRatio) * (1 + repeatedPositions / 2)
                        
                        # Make exceptions for productive behavior or being trapped
                        if len(self.observationHistory) >= 3:
                            currentScore = gameState.getScore()
                            prevScore = self.observationHistory[-3].getScore()
                            
                            # If score improved, don't penalize
                            if currentScore > prevScore:
                                circlingPenalty = 0
                        
                        # Make exception when ghosts are close (agent might be trapped)
                        if ghosts and closestGhostDist < 3:
                            circlingPenalty = 0
                    else:
                        # Reward for moving away from the circling area
                        # Higher reward for moving further away
                        circlingPenalty = 20 + (5 * (minDistance - 2))
                
                # Print for debugging
                # print("agentPos", agentPos)
                # print("recentPositions", recentPositions)
                # print("circlingPenalty", circlingPenalty)
            
            score += circlingPenalty
        return score
         
def createTeam(firstIndex, secondIndex, isRed,
        first = 'pacai.agents.capture.dummy.DummyAgent',
        second = 'pacai.agents.capture.dummy.DummyAgent'):
    """
    This function should return a list of two agents that will form the capture team,
    initialized using firstIndex and secondIndex as their agent indexed.
    isRed is True if the red team is being created,
    and will be False if the blue team is being created.
    """
    # firstAgent = reflection.qualifiedImport(first)
    # secondAgent = reflection.qualifiedImport(second)
    firstAgent = GoatAgent(firstIndex)
    secondAgent = GoatAgent(secondIndex)

    return [
        firstAgent,
        secondAgent
    ]
    # return [
    #     firstAgent(firstIndex),
    #     secondAgent(secondIndex),
    # ]