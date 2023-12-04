# Snake Game
## Features

Simple Snake game that revives the timeless classic in a modern coding environment. Written in C99 and leveraging the capabilities of [SDL3](https://github.com/libsdl-org/SDL), this game not only offers entertainment but also serves as an reference for those who interested in game development with C and SDL.\
\
The game is available on two platforms, Linux and Windows.\
\
Currently only fixed resolution is available.

## Build
### Linux

| Required packages|
|------------------|
| build-essential  |
| libfreetype6     |
| libfreetype6-dev |
| libsdl2-dev      |

1. Install required packages
```bash
sudo apt install build-esseintial libfreetype6 libfreetype6-dev libsdl2-dev
```
\
2. Clone repo form github
```bash
git clone https://github.com/necromyhan/snake-game
```
\
3.  Choose project directory
```bash
cd snake-game
```
\
4. Init and update git submodules
```bash
git submodule init
```
```bash
git submodule update
```
\
5. Configure CMake
```bash
cmake -B build
```
\
6. Build project
```bash
cmake --build build
```
\
_Tested on Ubuntu 23.10.1 64-bit_

### Windows

Same as Linux, but before cmake build download [FreeType](https://sourceforge.net/projects/freetype/) sources and copy them into ___..\Snake-Game\sdl\SDL_ttf\external\freetype___ directory.
\
\
_Tested on Windows 10 22H2 64-bit_

## Binary

Ready-to-run game builds are available in the [Releases](https://github.com/necromyhan/snake-game/releases/) section.\
For Linux, self-build is recommended.

## Running the Game
### Linux
Install libsdl2 library via apt.\
Launch run_game.sh script in the project directory.
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
