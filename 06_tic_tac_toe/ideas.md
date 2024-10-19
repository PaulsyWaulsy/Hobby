## Defensive AI
- Given the current state of the board, try to block the players
    next move if they will win

 1)  X |   |    2)  X |   |    3)  X |   | X 
    ---+---+---    ---+---+---    ---+---+---
       |   |    ->    | O |    ->    | O |   
    ---+---+---    ---+---+---    ---+---+---
       |   |          |   |          |   |   

 4)  X | O | X  5)  X | O | X  6)  X | O | X 
    ---+---+---    ---+---+---    ---+---+---
       | O |    ->    | O |    ->    | O | O 
    ---+---+---    ---+---+---    ---+---+---
       |   |          |   | X        |   | X 

 7)  X | O | X  8)  X | O | X  9)  X | O | X 
    ---+---+---    ---+---+---    ---+---+---
     X | O | O  ->  X | O | O  ->  X | O | O 
    ---+---+---    ---+---+---    ---+---+---
       |   | X      O |   | X      O | X | X 

2. if the middle is available and it wont result in a loss, the AI
    should select the middle to minimise the threat of future loss

4,6,7. Select the position that would block a future loss

## MinMax


