
# DP Project - Connect 4
Team Members: **Ioan BAȘNIC, Sebastian BELEA, Petra HORVATH**
## What is it?
Project

## Technologies
project

## Main Features
```sh
# Something
```

## Authors
* **Ioan BAȘNIC** - [GitHub Profile](https://github.com/IoanBasnic)
* **Sebastian BELEA** - [GitHub Profile](https://github.com/belea-sebastian)
* **Petra HORVATH** - [GitHub Profile](https://github.com/Petrified0110)


## Client to server message 
* **[A-H]** - The columns that the player chose to place the pick on

## Server to client (all are separated by a '-' (dash))
* **[0-6][0-5] (first 2 digits)** - The row and the column of the previously chosen, and computed, move
* **[1-2]** - The player whose move it was
* **[0-?]** - Error code, if there was any
* **[0-4]** - Game status

## Error code meaning
* **0** - No errors
* **1** - Invalid input length (S2C)
* **2** - Invalid character (S2C)
* **3** - Invalid move (S2C)
* **4** - The other player's server is down
* **5** - Not this player's turn
* **6** - Something unexpected went wrong (LEAVE LAST)

## Game status meaning
* **0** - Ongoing game
* **1** - Player 1 won
* **2** - Player 2 won
* **3** - Tie
* **4** - Game finished unexpectedly