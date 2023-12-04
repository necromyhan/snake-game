# Snake Game
## About

Simple 2D snake game clone written in C99 using the capabilities of SDL3. Made for the purpose of studying game development.\
\
Used libraries:
- [SDL3](https://github.com/libsdl-org/SDL)
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)


Game is available on two platforms, Linux and Windows.\
Currently only fixed resolution is available.\
\
All game artwork is made by me.\
Font - Kongtext.

### Gameplay screenshot

![Gameplay](https://github.com/necromyhan/snake-game/blob/master/gameplay_screenshot.png)

## Build
### Linux

Required packages for self-build:

- build-essential 
- libfreetype6-dev 
- libsdl2-dev


Install required packages
  ```bash
  sudo apt install build-esseintial libfreetype6-dev libsdl2-dev
  ```
  \
Clone repo form github
  ```bash
  git clone https://github.com/necromyhan/snake-game
  ```
\
Choose project directory
```bash
cd snake-game
```
\
Init and update git submodules
```bash
git submodule init
```
```bash
git submodule update
```
\
Configure CMake
```bash
cmake -B build
```
\
Build project
```bash
cmake --build build
```
\
_Tested on Ubuntu 23.10.1 64-bit with gcc_

### Windows

Same as Linux, but before the cmake step download [FreeType](https://sourceforge.net/projects/freetype/) sources and copy them into ___..\snake-game\sdl\SDL_ttf\external\freetype___ directory.
\
\
_Tested on Windows 10 22H2 64-bit with MSVC_

## Binary

Ready-to-run game builds are available in the [Releases](https://github.com/necromyhan/snake-game/releases/) section.\
For Linux, self-build is recommended.

## Running the Game
### Linux
Launch run_game.sh script in the project directory. Ensure all files remain in their original locations.
```bash
cd snake-game
bash ./run_game.sh
```
### Windows
Execute snake_game.exe. Ensure all files remain in their original locations.

### Controls
- Arrow Keys: Navigate the snake or menu options.
- Enter Key: Select menu items.


## Contributing

Contributions are welcome! If you have ideas for improvements or bug fixes, feel free to fork the repository and submit pull requests.
