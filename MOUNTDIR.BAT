@echo off
cycles 50000

rem *** Mount the Turbo C++ 1.00 directory as drive X: ***
mount x C:\DOS\PROGTAAL\TCPP100

rem *** Mount the Turbo Assembler 2.0 directory as drive W: ***
mount w C:\DOS\PROGTAAL\TASM200

rem *** Make sure TC++ and TASM executables are in the PATH environment: ***
set PATH=X:\BIN;W:\;%PATH%
