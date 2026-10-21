# ROUGE 🎮

A simple but faithful adaptation of the popular genre-defining video game [**ROUGE**](https://en.wikipedia.org/wiki/Rogue_(video_game)).
Made for the course project of Fundamentals of Programming at Sharif University of Technology, Autumn 2024.
The project runs under the C language's ncurses library in linux. 

- Installing the Required Dependencies

  Please note that this game is intended for Debian-based Linux distributions.
  Open a bash terminal and run the following commands:
  ```bash
  sudo apt update
  sudo apt install libncursesw5-dev libsdl2-dev libsdl2-mixer-dev
  ```

- Running the Game

  To run the game, you can either use the provided executable file `game.out` or compile the source code yourself.
  
  You can compile the code with the following command: 
  ```bash
  gcc Rogue-v2.0.0.c -o game.out -lncursesw -lSDL2 -lSDL2_mixer
  ```
  Run the game's executable with this command:
  ```bash
  ./game.out
  ```

- Controls

  Press 'E' in menus to accept items. Press 'E' in game for the food menu. Press 'P' for the spells menu. Press 'E' for the weapons menu. 
  Movement can be done in eight directions (including diagonal directions). Movement is with the numpad:
                up
              7 8 9
         left 4 5 6 right
              1 2 3
               down
  The '5' key doesn't isn't assigned to movement.

P.S. The game has working music. However, the music I used is copyrighted. Therefore, I cannot include them in the public release. 
The game will still function as intended.
