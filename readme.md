# Snake Game
Simple SDL3 game project written in C99.

## Build
### Linux

```bash
git clone https://github.com/necromyhan/Snake-Game
```

```bash
cd Snake-Game
```

```bash
git submodule init
```

```bash
git submodule update
```

```bash
cmake -B build
```

```bash
cmake --build build
```

### Windows

Same as Linux, but before cmake build download [FreeType](https://sourceforge.net/projects/freetype/) sources and copy them into ___..\Snake-Game\sdl\SDL_ttf\external\freetype directory___.

## Controls

Snake movement and menu navigation with arrows. Enter to choose menu item.
