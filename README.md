RDDICI
======

Reconstructed Dangerous Dave in Copyright Infringement Source Code.

The source code of the game that came before it ([Catacomb II](https://github.com/CatacombGames/TheCatacomb)) and the game that came after it ([Commander Keen 1](https://pckf.com/viewtopic.php?t=18248)) are available.
How hard can it be to combine the two to recreate the source code of Dangerous Dave in Copyright Infringement, the tech demo that started id Software?

You need to supply your own copy of Turbo C++ 1.00, Turbo Assembler 2.0, [LZEXE 0.91](https://bellard.org/lzexe) and UNLZEXE 0.8.

Run `MOUNTDIR.BAT` to set up the build environment.
Run `BUILD.BAT` to build `RDDICI.EXE`.
When `LZEXE` asks "Voulez-vous arrêter ? (O/N):", enter "N".

The uncompressed `RDDICI.EXE` should have the same size as the uncompressed `DAVE2.EXE` and the code should be equal, except for the memory addresses of all the variables.
