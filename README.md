## Overview
This project converts PlayStation 1 game images into native executables by translating the PS1 MIPS machine code into C source code that can be compiled on any platform. Input must be a `.bin` file with an accompanying `.cue` file in the same directory.

## Requirements
- MSYS2 (open the UCRT64 shell)
- CMake
- A cloned copy of this repository
- The target PS1 image: `game.bin` plus `game.cue`

## Quick build instructions
1. Open MSYS2 UCRT64.
2. In the terminal, go to the cloned repository directory.
3. Run:
```
mkdir build && cd build
cmake ..
make
```
NOTE: If you're using Ninja with Cmake. Run `ninja` instead of `make`

## Usage
To use `ps1recomp.exe` with a .bin file. Simply run:
```
./ps1recomp game.bin
```
Replace `game.bin` with the actual path/name of your `.bin` file.

If the output is successful. You should get
```
Successfully recompiled to recompiled_game.c
```
After that. Type "cd .." and then type the following command
```
gcc runner.c build/recompiled_game.c -o ps1_game_native
```
Again. Replace `game.bin` with the actual path/name of your `.bin` file.

Finally. Simply run `./ps1_game_native`. You should then see a wild spam of each instruction being run in the terminal.

## Notes
- Ensure the `.bin` has a matching `.cue` file; the tool relies on the `.cue` metadata.
- The recompiler is still incomplete. So the generated C code of the game may contain a lot of missing opcodes. (As we have already tried it with Crash Bandicoot)
- The runtime is still unfinished.
- After running `./ps1_game_native` the game will continue to execute every instruction with no end. And the program itself will remain stuck on PC: `0x80011A18`. It is a bug that will fixed in future updates.
- The project itself is still experimental and a lot of features still need to be added.
