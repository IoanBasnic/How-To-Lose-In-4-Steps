
# DP Project - How To Lose In 4 Steps (Connect 4)
Team Members: **Ioan BAȘNIC, Sebastian BELEA, Petra HORVATH**
## What is it?
How To Lose In 4 Steps is the implementation of the classic game, Connect 4. In a nuttshell, our program is nothing more than a small TCP application that uses various tehnologies like BSD sockets. 

## Architecture
As an architecture, the application is comprised of two major components, the `client` side and the `server` side. 
* **Client** We can regard the `client` as the frontend of the application. It deals with all the interactions that the app might have with the client, and displays a helpful graphic representation of the Connect 4 board that we are all very used to. In addition, it gives the client suggestive hints of what steps he/she is supposed to take and whether the input received from him/her was erroneous or not. Finally, it sends the server the next step that the client decided to take.

* **Server** If the `client` is the frontend, then, it's only natural that the `server` would be regarded as the backend. Besides connecting to and handling the turn of the 2 clients, it is responsible for doing any algorithmic computations and send confirmations, updates or game statuses to both client processes.

## Technologies
BSD sockets

## Main Features
```
* Playing a two player game of Connect 4 in 2 different terminals.
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