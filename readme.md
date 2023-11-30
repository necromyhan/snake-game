# Snake Game
Simple SDL3 game project written in C99.

## Features

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
_Tested on Ubuntu 23.10.1_

### Windows

Same as Linux, but before cmake build download [FreeType](https://sourceforge.net/projects/freetype/) sources and copy them into ___..\Snake-Game\sdl\SDL_ttf\external\freetype___ directory.
\
\
_Tested on Windows 10 22H2_

## Binary

You can download archives with game from Releases section.\
For Linux, self-build is recommended.

## Controls

Snake movement and menu navigation with arrows. Enter to choose menu item.
