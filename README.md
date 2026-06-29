# Chess Game

A terminal chess game written in C for our EECS 22L group project. Two players
take turns on the same machine, type moves in algebraic-ish notation (`e2 e4`),
and the board redraws in the terminal after each move. We split the work across
the team by module so people could work in parallel without stepping on each
other too much.

## About

This was a group project for the course. We worked as a team of eight and split
the work across the modules below so people could build their parts in parallel,
then put it all together.

## How it's put together

We kept everything modular so each piece could be tested on its own and the
headers made it clear what each part was responsible for:

- **`main.c`** — entry point. Starts the UI and hands off to the game loop.
- **`board.c` / `board.h`** — the 8x8 board, piece constants, and get/set helpers.
- **`player.c` / `player.h`** — tracks whose turn it is and switches sides.
- **`io.c` / `io.h`** — reads a line like `e2 e4` and turns it into a `Move`.
- **`move_validator.c` / `move_validator.h`** — the big one. Decides whether a
  move is legal for each piece, keeps you from moving into check, and checks
  whether a square is under attack.
- **`check.c` / `check.h`** — finds the king, decides if it's in check, and
  does the checkmate search.
- **`game.c` / `game.h`** — the main loop: draw, ask for a move, validate, apply,
  switch turns, announce check/checkmate.
- **`ui/ui.h` + `ui/ascii_ui.c`** — the terminal rendering. `ui.h` is an abstract
  interface so we could swap in a different front end later (there's an empty
  `sdl_ui.c` placeholder we never got to).

`config.h` holds a couple of shared constants.

## How to play

Run it and you'll see the board with files `a`–`h` across the top and ranks
`1`–`8` down the side. White is uppercase, black is lowercase, `.` is an empty
square. On your turn, type the from-square and the to-square separated by a
space:

```
Player W, enter your move: e2 e4
```

The game tells you when someone is in check, and it ends when it detects
checkmate.

## What works

- Full starting position and ASCII board that redraws each turn
- Legal movement for all six piece types
- Turn alternation between White and Black
- Captures
- Blocking illegal moves, including moves that would leave your own king in check
- Check detection and announcement
- Checkmate detection that ends the game

## What we didn't get to

We focused on getting the core rules solid and ran out of time for the extras:

- Castling
- En passant
- Pawn promotion
- Stalemate / draw detection
- Saving and loading a game

Input also has to be in exactly the `e2 e4` format — anything else gets
rejected and asks you to try again.

## AI tool use

Some of us used GitHub Copilot for autocomplete and to scaffold repetitive
things like the per-piece `switch` cases. We reviewed and rewrote the logic
ourselves, and all the design decisions were the team's.

## What we took away from it

Mostly that splitting a real C codebase across eight people only works if the
module boundaries and headers are clear from the start and once those settled
down, people could work independently and the pieces actually fit together.
Getting check and checkmate right (simulate the move, test for check, undo the
move) was the trickiest part and where we spent the most time.
