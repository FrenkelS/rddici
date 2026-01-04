/* Reconstructed Dangerous Dave in Copyright Infringement Source Code
 * Copyright (C) 2026 Frenkel Smeijers
 *
 * The code in this file is primarily based on:
 * The Catacomb Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/////////////////////////////////
//
// The PC-Arcade control panel
//
// Takes global variables as input
//
// videocard		CGAcard,EGAcard,VGAcard
//
// Modifies global variables relating to graphics, sound, and controls:
//
// grmode		CGAgr,EGAgr,VGAgr
// soundmode		nosound,spkr,adlib
// player1mode		keyboard,mouse,joystick1,joystick2
// player2mode
//
// JoyXlow[2], JoyXhigh[2], JoyYlow[2], JoyYhigh[2]
// MouseSensitivity, key[8], keyB1, keyB2
//
//
// The calling game must have the following functions:
//
// loadgrfiles		loads the files needed for the new mode
// repaintscreen	Clears the screen and totally redoes everything,
//			including hardware tricks and EGA latched pic data
//
//
// To call, just have a line which checks for F2 being pressed:
//
// if (keydown[0x3c])
//   controlpanel ();
//
// This should allso be called right after the titlepage!
//
/////////////////////////////////

#include "PCRLIB.H"

int rowy[4] = {4,9,14,19};
int collumnx[4] = {14,20,26,32};
int spotok[4][5];

int row,collumn;
grtype oldgrmode, newgrmode;
soundtype oldsoundmode, newsoundmode;
inputtype oldplayermode[3], newplayermode[3];

int joy1ok,joy2ok,mouseok;

////////////////////
//
// prototypes
//
////////////////////

void calibratejoy (int joynum);
void calibratemouse (void);
void printscan (int sc);
void calibratekeys (void);
void drawpanel (void);
void drawpanel (void);
void getconfig (void);

void controlpanel (void);

//=========================================================================

////////////////
//
// calibratejoy
// Brings up a dialog and has the user calibrate
// either joystick1 or joystick2
//
////////////////

void sub_0_3E97(void)
{
asm {
db 83h, 0ECh, 2Ah, 56h, 57h, 8Bh
db 7Eh, 04h, 0B8h, 09h, 00h, 50h, 0B8h, 18h, 00h, 50h, 0E8h, 3Ch, 0F4h, 59h, 59h, 0B8h
db 6Eh, 0Dh, 50h, 0E8h, 3Bh, 0F5h, 59h, 0B8h, 88h, 0Dh, 50h, 0E8h, 33h, 0F5h, 59h, 0B8h
db 0A2h, 0Dh, 50h, 0E8h, 2Bh, 0F5h, 59h, 0B8h, 0BDh, 0Dh, 50h, 0E8h, 23h, 0F5h, 59h, 0B8h
db 0CAh, 0Dh, 50h, 0E8h, 1Bh, 0F5h, 59h, 0BEh, 0Fh, 00h, 56h, 0FFh, 36h, 68h, 0AEh, 0FFh
db 36h, 02h, 0AEh, 0E8h, 0BBh, 16h, 83h, 0C4h, 06h, 0E8h, 6Ah, 16h, 0E8h, 67h, 16h, 0E8h
db 64h, 16h, 46h, 8Bh, 0C6h, 3Dh, 17h, 00h, 75h, 03h, 0BEh, 0Fh, 00h, 8Dh, 46h, 0F8h
db 50h, 8Dh, 46h, 0FAh, 50h, 57h, 0E8h, 0F4h, 0E9h, 83h, 0C4h, 06h, 8Dh, 46h, 0EEh, 16h
db 50h, 57h, 16h, 8Dh, 46h, 0E8h, 50h, 0E8h, 7Eh, 0EAh, 83h, 0C4h, 06h, 8Dh, 46h, 0E8h
db 16h, 50h, 0B9h, 06h, 00h, 0E8h, 7Ah, 36h, 83h, 3Eh, 9Ch, 0AEh, 00h, 74h, 03h, 0E9h
db 40h, 01h, 83h, 7Eh, 0F0h, 01h, 75h, 0A2h, 0B8h, 20h, 00h, 50h, 0FFh, 36h, 68h, 0AEh
db 0FFh, 36h, 02h, 0AEh, 0E8h, 5Ah, 16h, 83h, 0C4h, 06h, 8Dh, 46h, 0EEh, 16h, 50h, 57h
db 16h, 8Dh, 46h, 0E2h, 50h, 0E8h, 40h, 0EAh, 83h, 0C4h, 06h, 8Dh, 46h, 0E2h, 16h, 50h
db 0B9h, 06h, 00h, 0E8h, 3Ch, 36h, 83h, 7Eh, 0F0h, 00h, 75h, 0DEh, 0E8h, 0E7h, 15h, 0E8h
db 0E4h, 15h, 0B8h, 0DFh, 0Dh, 50h, 0E8h, 78h, 0F4h, 59h, 0B8h, 0FDh, 0Dh, 50h, 0E8h, 70h
db 0F4h, 59h, 0B8h, 0Bh, 0Eh, 50h, 0E8h, 68h, 0F4h, 59h, 56h, 0FFh, 36h, 68h, 0AEh, 0FFh
db 36h, 02h, 0AEh, 0E8h, 0Bh, 16h, 83h, 0C4h, 06h, 0E8h, 0BAh, 15h, 0E8h, 0B7h, 15h, 0E8h
db 0B4h, 15h, 46h, 8Bh, 0C6h, 3Dh, 17h, 00h, 75h, 03h, 0BEh, 0Fh, 00h, 8Dh, 46h, 0F4h
db 50h, 8Dh, 46h, 0F6h, 50h, 57h, 0E8h, 44h, 0E9h, 83h, 0C4h, 06h, 8Dh, 46h, 0EEh, 16h
db 50h, 57h, 16h, 8Dh, 46h, 0DCh, 50h, 0E8h, 0CEh, 0E9h, 83h, 0C4h, 06h, 8Dh, 46h, 0DCh
db 16h, 50h, 0B9h, 06h, 00h, 0E8h, 0CAh, 35h, 83h, 3Eh, 9Ch, 0AEh, 00h, 74h, 03h, 0E9h
db 90h, 00h, 83h, 7Eh, 0F0h, 01h, 75h, 0A2h, 0B8h, 20h, 00h, 50h, 0FFh, 36h, 68h, 0AEh
db 0FFh, 36h, 02h, 0AEh, 0E8h, 0AAh, 15h, 83h, 0C4h, 06h, 8Dh, 46h, 0EEh, 16h, 50h, 57h
db 16h, 8Dh, 46h, 0D6h, 50h, 0E8h, 90h, 0E9h, 83h, 0C4h, 06h, 8Dh, 46h, 0D6h, 16h, 50h
db 0B9h, 06h, 00h, 0E8h, 8Ch, 35h, 83h, 7Eh, 0F0h, 00h, 75h, 0DEh, 8Bh, 46h, 0F6h, 2Bh
db 46h, 0FAh, 0BBh, 04h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0FEh, 8Bh, 46h, 0F4h, 2Bh, 46h
db 0F8h, 0BBh, 04h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0FCh, 8Bh, 0DFh, 0D1h, 0E3h, 8Bh, 46h
db 0FAh, 03h, 46h, 0FEh, 89h, 87h, 0EAh, 0ADh, 8Bh, 0DFh, 0D1h, 0E3h, 8Bh, 46h, 0F6h, 2Bh
db 46h, 0FEh, 89h, 87h, 0F0h, 0ADh, 8Bh, 0DFh, 0D1h, 0E3h, 8Bh, 46h, 0F8h, 03h, 46h, 0FCh
db 89h, 87h, 0F6h, 0ADh, 8Bh, 0DFh, 0D1h, 0E3h, 8Bh, 46h, 0F4h, 2Bh, 46h, 0FCh, 89h, 87h
db 0FCh, 0ADh, 0E8h, 0CAh, 0ECh, 0E8h, 11h, 0F2h, 5Fh, 5Eh, 8Bh, 0E5h
}
}


////////////////////////////
//
// calibratemouse
//
////////////////////////////
void sub_0_407E(void)
{
asm {
db 4Ch, 4Ch, 0B8h, 05h, 00h, 50h, 0B8h, 18h, 00h, 50h, 0E8h, 5Bh, 0F2h, 59h, 59h
db 0B8h, 20h, 0Eh, 50h, 0E8h, 5Ah, 0F3h, 59h, 0B8h, 3Bh, 0Eh, 50h, 0E8h, 52h, 0F3h, 59h
db 0B8h, 56h, 0Eh, 50h, 0E8h, 4Ah, 0F3h, 59h, 0B8h, 71h, 0Eh, 50h, 0E8h, 42h, 0F3h, 59h
db 0B8h, 8Ch, 0Eh, 50h, 0E8h, 3Ah, 0F3h, 59h, 0E8h, 0D5h, 0F2h, 0BBh, 00h, 01h, 99h, 0F7h
db 0FBh, 88h, 56h, 0FFh, 80h, 7Eh, 0FFh, 1Bh, 75h, 04h, 0C6h, 46h, 0FFh, 35h, 80h, 7Eh
db 0FFh, 31h, 7Ch, 0E4h, 80h, 7Eh, 0FFh, 39h, 7Fh, 0DEh, 8Ah, 46h, 0FFh, 98h, 05h, 0D0h
db 0FFh, 0BAh, 0Fh, 00h, 2Bh, 0D0h, 89h, 16h, 9Ah, 0AFh, 0E8h, 9Ch, 0F1h, 33h, 0C0h, 0CDh
db 33h, 0B8h, 04h, 00h, 0B9h, 40h, 01h, 0BAh, 64h, 00h, 0CDh, 33h, 8Bh, 0E5h
}
}

/////////////////////////////
//
// print a representation of the scan code key
//
////////////////////////////
void sub_0_4100(void)
{
asm {
db 83h, 0ECh, 04h, 56h, 8Bh, 76h, 04h, 8Bh, 0C6h, 25h, 7Fh, 00h, 8Bh
db 0F0h, 83h, 0FEh, 01h, 75h, 0Bh, 0B8h, 0A0h, 0Eh, 50h, 0E8h, 0D4h, 0F2h, 59h, 0E9h, 82h
db 01h, 83h, 0FEh, 0Eh, 75h, 0Bh, 0B8h, 0A4h, 0Eh, 50h, 0E8h, 0C4h, 0F2h, 59h, 0E9h, 72h
db 01h, 83h, 0FEh, 0Fh, 75h, 0Bh, 0B8h, 0A9h, 0Eh, 50h, 0E8h, 0B4h, 0F2h, 59h, 0E9h, 62h
db 01h, 83h, 0FEh, 1Dh, 75h, 0Bh, 0B8h, 0ADh, 0Eh, 50h, 0E8h, 0A4h, 0F2h, 59h, 0E9h, 52h
db 01h, 83h, 0FEh, 2Ah, 75h, 0Bh, 0B8h, 0B2h, 0Eh, 50h, 0E8h, 94h, 0F2h, 59h, 0E9h, 42h
db 01h, 83h, 0FEh, 39h, 75h, 0Bh, 0B8h, 0B9h, 0Eh, 50h, 0E8h, 84h, 0F2h, 59h, 0E9h, 32h
db 01h, 83h, 0FEh, 3Ah, 75h, 0Bh, 0B8h, 0BFh, 0Eh, 50h, 0E8h, 74h, 0F2h, 59h, 0E9h, 22h
db 01h, 83h, 0FEh, 3Bh, 7Ch, 2Ch, 83h, 0FEh, 44h, 7Fh, 27h, 0B8h, 0C6h, 0Eh, 50h, 0E8h
db 5Fh, 0F2h, 59h, 0B8h, 0Ah, 00h, 50h, 8Dh, 46h, 0FCh, 50h, 8Bh, 0C6h, 05h, 0C6h, 0FFh
db 50h, 0E8h, 0BFh, 2Ah, 83h, 0C4h, 06h, 8Dh, 46h, 0FCh, 50h, 0E8h, 43h, 0F2h, 59h, 0E9h
db 0F1h, 00h, 83h, 0FEh, 57h, 75h, 0Bh, 0B8h, 0C8h, 0Eh, 50h, 0E8h, 33h, 0F2h, 59h, 0E9h
db 0E1h, 00h, 83h, 0FEh, 59h, 75h, 0Bh, 0B8h, 0CCh, 0Eh, 50h, 0E8h, 23h, 0F2h, 59h, 0E9h
db 0D1h, 00h, 83h, 0FEh, 46h, 75h, 0Bh, 0B8h, 0D0h, 0Eh, 50h, 0E8h, 13h, 0F2h, 59h, 0E9h
db 0C1h, 00h, 83h, 0FEh, 1Ch, 75h, 0Bh, 0B8h, 0D7h, 0Eh, 50h, 0E8h, 03h, 0F2h, 59h, 0E9h
db 0B1h, 00h, 83h, 0FEh, 36h, 75h, 0Bh, 0B8h, 0DDh, 0Eh, 50h, 0E8h, 0F3h, 0F1h, 59h, 0E9h
db 0A1h, 00h, 83h, 0FEh, 37h, 75h, 0Bh, 0B8h, 0E4h, 0Eh, 50h, 0E8h, 0E3h, 0F1h, 59h, 0E9h
db 91h, 00h, 83h, 0FEh, 38h, 75h, 0Bh, 0B8h, 0EAh, 0Eh, 50h, 0E8h, 0D3h, 0F1h, 59h, 0E9h
db 81h, 00h, 83h, 0FEh, 47h, 75h, 0Ah, 0B8h, 0EEh, 0Eh, 50h, 0E8h, 0C3h, 0F1h, 59h, 0EBh
db 72h, 83h, 0FEh, 49h, 75h, 0Ah, 0B8h, 0F3h, 0Eh, 50h, 0E8h, 0B4h, 0F1h, 59h, 0EBh, 63h
db 83h, 0FEh, 4Fh, 75h, 0Ah, 0B8h, 0F8h, 0Eh, 50h, 0E8h, 0A5h, 0F1h, 59h, 0EBh, 54h, 83h
db 0FEh, 51h, 75h, 0Ah, 0B8h, 0FCh, 0Eh, 50h, 0E8h, 96h, 0F1h, 59h, 0EBh, 45h, 83h, 0FEh
db 52h, 75h, 0Ah, 0B8h, 01h, 0Fh, 50h, 0E8h, 87h, 0F1h, 59h, 0EBh, 36h, 83h, 0FEh, 53h
db 75h, 0Ah, 0B8h, 05h, 0Fh, 50h, 0E8h, 78h, 0F1h, 59h, 0EBh, 27h, 83h, 0FEh, 45h, 75h
db 0Ah, 0B8h, 09h, 0Fh, 50h, 0E8h, 69h, 0F1h, 59h, 0EBh, 18h, 8Ah, 84h, 0EEh, 0Ch, 98h
db 50h, 0FFh, 36h, 68h, 0AEh, 0A1h, 02h, 0AEh, 0FFh, 06h, 02h, 0AEh, 50h, 0E8h, 01h, 13h
db 83h, 0C4h, 06h, 5Eh, 8Bh, 0E5h
}
}

/////////////////////////////
//
// calibratekeys
//
////////////////////////////
void sub_0_42A8(void)
{
asm {
db 83h, 0ECh, 08h, 56h, 57h
db 0B8h, 0Fh, 00h, 50h, 0B8h, 16h, 00h, 50h, 0E8h, 2Eh, 0F0h, 59h, 59h, 0B8h, 0Fh, 0Fh
db 50h, 0E8h, 2Dh, 0F1h, 59h, 0B8h, 28h, 0Fh, 50h, 0E8h, 25h, 0F1h, 59h, 0B8h, 3Fh, 0Fh
db 50h, 0E8h, 1Dh, 0F1h, 59h, 0B8h, 4Eh, 0Fh, 50h, 0E8h, 15h, 0F1h, 59h, 0B8h, 5Dh, 0Fh
db 50h, 0E8h, 0Dh, 0F1h, 59h, 0B8h, 6Ch, 0Fh, 50h, 0E8h, 05h, 0F1h, 59h, 0B8h, 7Bh, 0Fh
db 50h, 0E8h, 0FDh, 0F0h, 59h, 0B8h, 8Ah, 0Fh, 50h, 0E8h, 0F5h, 0F0h, 59h, 0B8h, 99h, 0Fh
db 50h, 0E8h, 0EDh, 0F0h, 59h, 0B8h, 0A8h, 0Fh, 50h, 0E8h, 0E5h, 0F0h, 59h, 0B8h, 0B7h, 0Fh
db 50h, 0E8h, 0DDh, 0F0h, 59h, 0B8h, 0C6h, 0Fh, 50h, 0E8h, 0D5h, 0F0h, 59h, 0B8h, 0D5h, 0Fh
db 50h, 0E8h, 0CDh, 0F0h, 59h, 0A1h, 02h, 0AEh, 89h, 46h, 0FCh, 0A1h, 68h, 0AEh, 89h, 46h
db 0FAh, 33h, 0FFh, 0EBh, 19h, 0C7h, 06h, 02h, 0AEh, 16h, 00h, 8Bh, 0C7h, 05h, 07h, 00h
db 0A3h, 68h, 0AEh, 8Ah, 85h, 60h, 0AEh, 98h, 50h, 0E8h, 0B4h, 0FDh, 59h, 47h, 83h, 0FFh
db 08h, 7Ch, 0E2h, 0C7h, 06h, 02h, 0AEh, 16h, 00h, 0C7h, 06h, 68h, 0AEh, 0Fh, 00h, 0A0h
db 04h, 0AEh, 98h, 50h, 0E8h, 99h, 0FDh, 59h, 0C7h, 06h, 02h, 0AEh, 16h, 00h, 0C7h, 06h
db 68h, 0AEh, 10h, 00h, 0A0h, 05h, 0AEh, 98h, 50h, 0E8h, 84h, 0FDh, 59h, 8Bh, 46h, 0FCh
db 0A3h, 02h, 0AEh, 8Bh, 46h, 0FAh, 0A3h, 68h, 0AEh, 0E8h, 04h, 0F0h, 0BBh, 00h, 01h, 99h
db 0F7h, 0FBh, 88h, 56h, 0FFh, 80h, 7Eh, 0FFh, 30h, 7Ch, 06h, 80h, 7Eh, 0FFh, 39h, 7Eh
db 03h, 0E9h, 0A5h, 00h, 8Ah, 46h, 0FFh, 98h, 05h, 0D0h, 0FFh, 8Bh, 0F0h, 8Ah, 46h, 0FFh
db 98h, 50h, 0FFh, 36h, 68h, 0AEh, 0FFh, 36h, 02h, 0AEh, 0E8h, 0E4h, 11h, 83h, 0C4h, 06h
db 8Ah, 46h, 0FFh, 98h, 05h, 0D0h, 0FFh, 8Bh, 0F0h, 0B8h, 0EDh, 0Fh, 50h, 0E8h, 21h, 0F0h
db 59h, 0E8h, 6Bh, 0E9h, 0C7h, 46h, 0F8h, 0FFh, 0FFh, 0EBh, 0Bh, 83h, 7Eh, 0F8h, 7Fh, 75h
db 05h, 0C7h, 46h, 0F8h, 0FFh, 0FFh, 0FFh, 46h, 0F8h, 8Bh, 5Eh, 0F8h, 0D1h, 0E3h, 83h, 0BFh
db 9Ah, 0AEh, 00h, 74h, 0E6h, 0E8h, 47h, 0E9h, 0B8h, 02h, 10h, 50h, 0E8h, 0F2h, 0EFh, 59h
db 83h, 0FEh, 08h, 7Dh, 07h, 8Ah, 46h, 0F8h, 88h, 84h, 60h, 0AEh, 83h, 0FEh, 08h, 75h
db 06h, 8Ah, 46h, 0F8h, 0A2h, 04h, 0AEh, 83h, 0FEh, 09h, 75h, 06h, 8Ah, 46h, 0F8h, 0A2h
db 05h, 0AEh, 8Bh, 0C6h, 05h, 07h, 00h, 0A3h, 68h, 0AEh, 0C7h, 06h, 02h, 0AEh, 16h, 00h
db 0B8h, 16h, 10h, 50h, 0E8h, 0BAh, 0EFh, 59h, 0C7h, 06h, 02h, 0AEh, 16h, 00h, 0FFh, 76h
db 0F8h, 0E8h, 0BCh, 0FCh, 59h, 0C6h, 46h, 0FFh, 30h, 80h, 7Eh, 0FFh, 30h, 7Ch, 09h, 80h
db 7Eh, 0FFh, 39h, 7Fh, 03h, 0E9h, 25h, 0FFh, 0E8h, 2Eh, 0EEh, 5Fh, 5Eh, 8Bh, 0E5h
}
}


//=========================================================================

////////////////////
//
// getconfig
// Checks video cards, mouse, and joysticks
//
////////////////////

void sub_0_4461(void)
{
asm {
db 83h, 0ECh, 08h, 0C7h, 06h, 7Ah, 0C3h, 01h, 00h, 0A1h, 84h, 0AEh
db 0A3h, 7Ch, 0C3h, 0A1h, 5Eh, 0C3h, 0A3h, 7Eh, 0C3h, 0C7h, 06h, 80h, 0C3h, 00h, 00h, 0C7h
db 06h, 82h, 0C3h, 00h, 00h, 0C7h, 06h, 84h, 0C3h, 01h, 00h, 0C7h, 06h, 86h, 0C3h, 01h
db 00h, 0C7h, 06h, 88h, 0C3h, 00h, 00h, 0C7h, 06h, 8Ah, 0C3h, 00h, 00h, 0C7h, 06h, 8Ch
db 0C3h, 00h, 00h, 0C7h, 06h, 6Ah, 0C3h, 00h, 00h, 0C7h, 06h, 6Ch, 0C3h, 00h, 00h, 8Dh
db 46h, 0FCh, 50h, 8Dh, 46h, 0FEh, 50h, 0B8h, 01h, 00h, 50h, 0E8h, 3Fh, 0E4h, 83h, 0C4h
db 06h, 81h, 7Eh, 0FEh, 0F4h, 01h, 7Dh, 06h, 0C7h, 06h, 6Ah, 0C3h, 01h, 00h, 8Dh, 46h
db 0FCh, 50h, 8Dh, 46h, 0FEh, 50h, 0B8h, 02h, 00h, 50h, 0E8h, 20h, 0E4h, 83h, 0C4h, 06h
db 81h, 7Eh, 0FEh, 0F4h, 01h, 7Dh, 06h, 0C7h, 06h, 6Ah, 0C3h, 02h, 00h, 0C7h, 06h, 70h
db 0C3h, 01h, 00h, 0B8h, 33h, 00h, 50h, 0E8h, 59h, 25h, 59h, 89h, 46h, 0F8h, 89h, 56h
db 0FAh, 8Bh, 46h, 0F8h, 0Bh, 46h, 0FAh, 75h, 08h, 0C7h, 06h, 70h, 0C3h, 00h, 00h, 0EBh
db 14h, 0C4h, 5Eh, 0F8h, 26h, 8Bh, 07h, 25h, 0FFh, 00h, 3Dh, 0CFh, 00h, 75h, 06h, 0C7h
db 06h, 70h, 0C3h, 00h, 00h, 0C7h, 06h, 8Eh, 0C3h, 01h, 00h, 0A1h, 70h, 0C3h, 0A3h, 90h
db 0C3h, 0A1h, 6Ah, 0C3h, 0A3h, 92h, 0C3h, 0A1h, 6Ch, 0C3h, 0A3h, 94h, 0C3h, 0C7h, 06h, 96h
db 0C3h, 00h, 00h, 8Bh, 0E5h
}
}

//=========================================================================


////////////////
//
// drawpanel
// Draws everything inside the control panel window.
// Used to refresh under dialogs.
//
////////////////

void sub_0_4547(void)
{
asm {
db 0C7h, 06h, 8Ah, 0AEh, 01h, 00h
db 0C7h, 06h, 0E8h, 10h, 00h, 00h, 0C7h, 06h, 02h, 0AEh, 08h, 00h, 0C7h, 06h, 68h, 0AEh
db 02h, 00h, 0B8h, 1Fh, 10h, 50h, 0E8h, 88h, 0EEh, 59h, 0E8h, 0F4h, 0FEh, 0A1h, 0DEh, 0Ch
db 40h, 40h, 0A3h, 68h, 0AEh, 0C7h, 06h, 02h, 0AEh, 02h, 00h, 0B8h, 39h, 10h, 50h, 0E8h
db 6Fh, 0EEh, 59h, 33h, 0C0h, 50h, 0A1h, 0DEh, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h
db 0A1h, 0E6h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h, 0AAh, 14h, 83h, 0C4h, 06h
db 83h, 3Eh, 84h, 0AEh, 00h, 74h, 20h, 0B8h, 01h, 00h, 50h, 0A1h, 0DEh, 0Ch, 0D1h, 0E0h
db 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0E8h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h
db 85h, 14h, 83h, 0C4h, 06h, 0EBh, 1Eh, 0B8h, 03h, 00h, 50h, 0A1h, 0DEh, 0Ch, 0D1h, 0E0h
db 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0E8h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h
db 65h, 14h, 83h, 0C4h, 06h, 83h, 3Eh, 5Eh, 0C3h, 00h, 74h, 20h, 0B8h, 02h, 00h, 50h
db 0A1h, 0DEh, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0EAh, 0Ch, 0D1h, 0E0h, 0D1h
db 0E0h, 0D1h, 0E0h, 50h, 0E8h, 40h, 14h, 83h, 0C4h, 06h, 0EBh, 1Eh, 0B8h, 04h, 00h, 50h
db 0A1h, 0DEh, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0EAh, 0Ch, 0D1h, 0E0h, 0D1h
db 0E0h, 0D1h, 0E0h, 50h, 0E8h, 20h, 14h, 83h, 0C4h, 06h, 0A1h, 0E0h, 0Ch, 40h, 40h, 0A3h
db 68h, 0AEh, 0C7h, 06h, 02h, 0AEh, 02h, 00h, 0B8h, 40h, 10h, 50h, 0E8h, 0B2h, 0EDh, 59h
db 0B8h, 05h, 00h, 50h, 0A1h, 0E0h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0E6h
db 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h, 0ECh, 13h, 83h, 0C4h, 06h, 0B8h, 06h
db 00h, 50h, 0A1h, 0E0h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0E8h, 0Ch, 0D1h
db 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h, 0CEh, 13h, 83h, 0C4h, 06h, 0A1h, 0E2h, 0Ch, 40h
db 40h, 0A3h, 68h, 0AEh, 0C7h, 06h, 02h, 0AEh, 02h, 00h, 0B8h, 47h, 10h, 50h, 0E8h, 60h
db 0EDh, 59h, 0B8h, 07h, 00h, 50h, 0A1h, 0E2h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h
db 0A1h, 0E6h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h, 9Ah, 13h, 83h, 0C4h, 06h
db 83h, 3Eh, 70h, 0C3h, 00h, 74h, 20h, 0B8h, 0Ah, 00h, 50h, 0A1h, 0E2h, 0Ch, 0D1h, 0E0h
db 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0E8h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h
db 75h, 13h, 83h, 0C4h, 06h, 0EBh, 1Eh, 0B8h, 0Ch, 00h, 50h, 0A1h, 0E2h, 0Ch, 0D1h, 0E0h
db 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0E8h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h
db 55h, 13h, 83h, 0C4h, 06h, 83h, 3Eh, 6Ah, 0C3h, 00h, 74h, 20h, 0B8h, 08h, 00h, 50h
db 0A1h, 0E2h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0EAh, 0Ch, 0D1h, 0E0h, 0D1h
db 0E0h, 0D1h, 0E0h, 50h, 0E8h, 30h, 13h, 83h, 0C4h, 06h, 0EBh, 1Eh, 0B8h, 0Bh, 00h, 50h
db 0A1h, 0E2h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 0EAh, 0Ch, 0D1h, 0E0h, 0D1h
db 0E0h, 0D1h, 0E0h, 50h, 0E8h, 10h, 13h, 83h, 0C4h, 06h, 83h, 3Eh, 6Ch, 0C3h, 00h, 74h
db 20h, 0B8h, 09h, 00h, 50h, 0A1h, 0E2h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h
db 0ECh, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h, 0EBh, 12h, 83h, 0C4h, 06h, 0EBh
db 1Eh, 0B8h, 0Bh, 00h, 50h, 0A1h, 0E2h, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h
db 0ECh, 0Ch, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0E8h, 0CBh, 12h, 83h, 0C4h, 06h, 0B8h
db 0Fh, 00h, 50h, 0A1h, 0DEh, 0Ch, 05h, 03h, 00h, 50h, 8Bh, 1Eh, 0CAh, 0E3h, 0D1h, 0E3h
db 8Bh, 87h, 0E4h, 0Ch, 40h, 50h, 0E8h, 08h, 0Eh, 83h, 0C4h, 06h, 0B8h, 0Fh, 00h, 50h
db 0A1h, 0E0h, 0Ch, 05h, 03h, 00h, 50h, 8Bh, 1Eh, 0DAh, 0E7h, 0D1h, 0E3h, 8Bh, 87h, 0E6h
db 0Ch, 40h, 50h, 0E8h, 0EBh, 0Dh, 83h, 0C4h, 06h, 0B8h, 0Fh, 00h, 50h, 0A1h, 0E2h, 0Ch
db 05h, 03h, 00h, 50h, 8Bh, 1Eh, 0E0h, 0E7h, 0D1h, 0E3h, 8Bh, 87h, 0E6h, 0Ch, 40h, 50h
db 0E8h, 0CEh, 0Dh, 83h, 0C4h, 06h, 0C7h, 06h, 68h, 0AEh, 15h, 00h, 0C7h, 06h, 02h, 0AEh
db 01h, 00h, 0B8h, 50h, 10h, 50h, 0E8h, 08h, 0ECh, 59h, 0B8h, 79h, 10h, 50h, 0E8h, 00h
db 0ECh, 59h, 0B8h, 0A2h, 10h, 50h, 0E8h, 0F8h, 0EBh, 59h
}
}

//=========================================================================

////////////////
//
// controlpanel
//
////////////////

void sub_0_47FC(void)
{
asm {
db 83h
db 0ECh, 04h, 56h, 57h, 0E8h, 38h, 0E5h, 0E8h, 90h, 0Bh, 0A1h, 9Ch, 0AFh, 0A3h, 0A2h, 0C3h
db 0A3h, 0CAh, 0E3h, 0A1h, 0D2h, 10h, 0A3h, 0C6h, 0C3h, 0A3h, 0DAh, 0E7h, 0A1h, 06h, 0Ch, 0A3h
db 0CEh, 0E3h, 0A3h, 0E0h, 0E7h, 0A1h, 08h, 0Ch, 0A3h, 0D0h, 0E3h, 0A3h, 0E2h, 0E7h, 0A1h, 0Eh
db 0Ch, 89h, 46h, 0FCh, 8Bh, 3Eh, 10h, 0Ch, 0C7h, 06h, 0Eh, 0Ch, 13h, 00h, 0C7h, 06h
db 10h, 0Ch, 0Bh, 00h, 0FFh, 36h, 9Ch, 0AFh, 0E8h, 0E4h, 0E7h, 59h, 0B8h, 18h, 00h, 50h
db 0B8h, 27h, 00h, 50h, 33h, 0C0h, 50h, 33h, 0C0h, 50h, 0E8h, 14h, 0E9h, 83h, 0C4h, 08h
db 0E8h, 0E4h, 0FCh, 0C7h, 06h, 0DCh, 0E7h, 00h, 00h, 0A1h, 9Ch, 0AFh, 48h, 0A3h, 6Eh, 0C3h
db 8Bh, 1Eh, 6Eh, 0C3h, 0D1h, 0E3h, 8Bh, 87h, 0E6h, 0Ch, 40h, 40h, 0A3h, 02h, 0AEh, 8Bh
db 1Eh, 0DCh, 0E7h, 0D1h, 0E3h, 8Bh, 87h, 0DEh, 0Ch, 05h, 03h, 00h, 0A3h, 68h, 0AEh, 0E8h
db 0FEh, 0EAh, 8Bh, 0F0h, 8Bh, 0C6h, 0BBh, 00h, 01h, 99h, 0F7h, 0FBh, 88h, 56h, 0FFh, 8Bh
db 0C6h, 0BBh, 00h, 01h, 99h, 0F7h, 0FBh, 88h, 46h, 0FEh, 80h, 7Eh, 0FEh, 48h, 75h, 0Ch
db 0FFh, 0Eh, 0DCh, 0E7h, 7Dh, 06h, 0C7h, 06h, 0DCh, 0E7h, 02h, 00h, 80h, 7Eh, 0FEh, 50h
db 75h, 12h, 0FFh, 06h, 0DCh, 0E7h, 0A1h, 0DCh, 0E7h, 3Dh, 02h, 00h, 7Eh, 06h, 0C7h, 06h
db 0DCh, 0E7h, 00h, 00h, 0EBh, 04h, 0FFh, 0Eh, 6Eh, 0C3h, 0A1h, 0DCh, 0E7h, 0BAh, 0Ah, 00h
db 0F7h, 0EAh, 8Bh, 16h, 6Eh, 0C3h, 0D1h, 0E2h, 03h, 0C2h, 8Bh, 0D8h, 83h, 0BFh, 7Ah, 0C3h
db 00h, 74h, 0E3h, 80h, 7Eh, 0FEh, 4Bh, 75h, 2Ch, 83h, 3Eh, 6Eh, 0C3h, 00h, 75h, 06h
db 0C7h, 06h, 6Eh, 0C3h, 04h, 00h, 0EBh, 00h, 0A1h, 0DCh, 0E7h, 0BAh, 0Ah, 00h, 0F7h, 0EAh
db 0FFh, 0Eh, 6Eh, 0C3h, 8Bh, 16h, 6Eh, 0C3h, 0D1h, 0E2h, 03h, 0C2h, 8Bh, 0D8h, 83h, 0BFh
db 7Ah, 0C3h, 00h, 74h, 0E3h, 80h, 7Eh, 0FEh, 4Dh, 75h, 33h, 0EBh, 0Dh, 83h, 3Eh, 6Eh
db 0C3h, 04h, 75h, 06h, 0C7h, 06h, 6Eh, 0C3h, 0FFh, 0FFh, 0A1h, 0DCh, 0E7h, 0BAh, 0Ah, 00h
db 0F7h, 0EAh, 0FFh, 06h, 6Eh, 0C3h, 8Bh, 16h, 6Eh, 0C3h, 0D1h, 0E2h, 03h, 0C2h, 8Bh, 0D8h
db 83h, 0BFh, 7Ah, 0C3h, 00h, 74h, 0D6h, 83h, 3Eh, 6Eh, 0C3h, 03h, 7Fh, 0CFh, 80h, 7Eh
db 0FFh, 0Dh, 74h, 03h, 0E9h, 38h, 01h, 0A1h, 0DCh, 0E7h, 0Bh, 0C0h, 74h, 10h, 3Dh, 01h
db 00h, 74h, 75h, 3Dh, 02h, 00h, 75h, 03h, 0E9h, 99h, 00h, 0E9h, 0FDh, 00h, 0A1h, 6Eh
db 0C3h, 40h, 3Bh, 06h, 0CAh, 0E3h, 75h, 03h, 0E9h, 0F0h, 00h, 0B8h, 20h, 00h, 50h, 8Bh
db 1Eh, 0DCh, 0E7h, 0D1h, 0E3h, 8Bh, 87h, 0DEh, 0Ch, 05h, 03h, 00h, 50h, 8Bh, 1Eh, 0CAh
db 0E3h, 0D1h, 0E3h, 8Bh, 87h, 0E4h, 0Ch, 40h, 50h, 0E8h, 0F5h, 0Bh, 83h, 0C4h, 06h, 0A1h
db 6Eh, 0C3h, 40h, 0A3h, 0CAh, 0E3h, 0A3h, 9Ch, 0AFh, 0E8h, 96h, 0B9h, 0FFh, 36h, 9Ch, 0AFh
db 0E8h, 6Ch, 0E6h, 59h, 83h, 3Eh, 9Ch, 0AFh, 02h, 75h, 03h, 0E8h, 21h, 03h, 0B8h, 18h
db 00h, 50h, 0B8h, 27h, 00h, 50h, 33h, 0C0h, 50h, 33h, 0C0h, 50h, 0E8h, 92h, 0E7h, 83h
db 0C4h, 08h, 0E8h, 62h, 0FBh, 0E9h, 93h, 00h, 0B8h, 20h, 00h, 50h, 8Bh, 1Eh, 0DCh, 0E7h
db 0D1h, 0E3h, 8Bh, 87h, 0DEh, 0Ch, 05h, 03h, 00h, 50h, 8Bh, 1Eh, 0DAh, 0E7h, 0D1h, 0E3h
db 8Bh, 87h, 0E6h, 0Ch, 40h, 50h, 0E8h, 98h, 0Bh, 83h, 0C4h, 06h, 0A1h, 6Eh, 0C3h, 0A3h
db 0DAh, 0E7h, 0EBh, 67h, 0B8h, 20h, 00h, 50h, 8Bh, 1Eh, 0DCh, 0E7h, 0D1h, 0E3h, 8Bh, 87h
db 0DEh, 0Ch, 05h, 03h, 00h, 50h, 8Bh, 1Eh, 0E0h, 0E7h, 0D1h, 0E3h, 8Bh, 87h, 0E6h, 0Ch
db 40h, 50h, 0E8h, 6Ch, 0Bh, 83h, 0C4h, 06h, 0A1h, 6Eh, 0C3h, 0A3h, 0E0h, 0E7h, 83h, 3Eh
db 0E0h, 0E7h, 00h, 75h, 05h, 0E8h, 60h, 0F8h, 0EBh, 2Ch, 83h, 3Eh, 0E0h, 0E7h, 01h, 75h
db 05h, 0E8h, 2Ah, 0F6h, 0EBh, 20h, 83h, 3Eh, 0E0h, 0E7h, 02h, 75h, 0Ah, 0B8h, 01h, 00h
db 50h, 0E8h, 33h, 0F4h, 59h, 0EBh, 0Fh, 83h, 3Eh, 0E0h, 0E7h, 03h, 75h, 08h, 0B8h, 02h
db 00h, 50h, 0E8h, 22h, 0F4h, 59h, 0E8h, 0CEh, 0FAh, 0EBh, 00h, 0B8h, 0Fh, 00h, 50h, 8Bh
db 1Eh, 0DCh, 0E7h, 0D1h, 0E3h, 8Bh, 87h, 0DEh, 0Ch, 05h, 03h, 00h, 50h, 8Bh, 1Eh, 6Eh
db 0C3h, 0D1h, 0E3h, 8Bh, 87h, 0E6h, 0Ch, 40h, 50h, 0E8h, 05h, 0Bh, 83h, 0C4h, 06h, 80h
db 7Eh, 0FFh, 1Bh, 74h, 03h, 0E9h, 0C8h, 0FDh, 0A1h, 0E0h, 0E7h, 0A3h, 06h, 0Ch, 0A1h, 0E2h
db 0E7h, 0A3h, 08h, 0Ch, 0A1h, 0CAh, 0E3h, 0A3h, 9Ch, 0AFh, 8Bh, 46h, 0FCh, 0A3h, 0Eh, 0Ch
db 89h, 3Eh, 10h, 0Ch, 0A1h, 0DAh, 0E7h, 0A3h, 0D2h, 10h, 0E8h, 0C1h, 0B8h, 0E8h, 0F5h, 08h
db 5Fh, 5Eh, 8Bh, 0E5h
}
}


//==========================================================================

/*
===================
=
= installgrfile
=
= Loads a PC-arcade graphic file
= grmode must be set so it knows if it is an EGA planed file
=
= Can be a normal uncompressed file or an RLE'd file
=
===================
*/

unsigned egaplane[4];			// main memory paragraph of plane image
spritetype image, spritetable[NUMSPRITES];	// grfile headers
pictype pictable[NUMPICS];
void far *lastgrpic;

int numchars,numtiles,numpics,numsprites;

void sub_0_4AD6(void)
{
asm {
db 83h, 0ECh, 0Ch, 56h, 0A1h, 0D2h, 0E7h
db 0Bh, 06h, 0D4h, 0E7h, 74h, 0Dh, 0FFh, 36h, 0D4h, 0E7h, 0FFh, 36h, 0D2h, 0E7h, 0E8h, 0DDh
db 26h, 59h, 59h, 83h, 7Eh, 06h, 00h, 74h, 0Fh, 0FFh, 76h, 04h, 0E8h, 65h, 0E4h, 59h
db 89h, 46h, 0FCh, 89h, 56h, 0FEh, 0EBh, 0Dh, 0FFh, 76h, 04h, 0E8h, 0F7h, 0E3h, 59h, 89h
db 46h, 0FCh, 89h, 56h, 0FEh, 0A1h, 66h, 0C3h, 8Bh, 16h, 64h, 0C3h, 89h, 16h, 0D2h, 0E7h
db 0A3h, 0D4h, 0E7h, 0C4h, 5Eh, 0FCh, 26h, 8Bh, 47h, 28h, 0A3h, 0D6h, 0E7h, 0C4h, 5Eh, 0FCh
db 26h, 8Bh, 47h, 2Ah, 0A3h, 0C8h, 0E3h, 0C4h, 5Eh, 0FCh, 26h, 8Bh, 47h, 2Ch, 0A3h, 0C4h
db 0C3h, 0C4h, 5Eh, 0FCh, 26h, 8Bh, 47h, 2Eh, 0A3h, 0D8h, 0E7h, 83h, 3Eh, 9Ch, 0AFh, 02h
db 74h, 03h, 0E9h, 0B0h, 00h, 0C7h, 06h, 6Ah, 0AEh, 00h, 00h, 0C7h, 06h, 6Ch, 0AEh, 00h
db 0A9h, 0C4h, 5Eh, 0FCh, 26h, 8Bh, 47h, 06h, 05h, 00h, 0A9h, 0C4h, 5Eh, 0FCh, 26h, 2Bh
db 47h, 02h, 0C7h, 06h, 96h, 0AEh, 00h, 00h, 0A3h, 98h, 0AEh, 0C4h, 5Eh, 0FCh, 26h, 8Bh
db 47h, 0Ah, 05h, 00h, 0A9h, 0C4h, 5Eh, 0FCh, 26h, 2Bh, 47h, 02h, 0C7h, 06h, 86h, 0AEh
db 00h, 00h, 0A3h, 88h, 0AEh, 0C4h, 5Eh, 0FCh, 26h, 8Bh, 47h, 0Eh, 05h, 00h, 0A9h, 0C4h
db 5Eh, 0FCh, 26h, 2Bh, 47h, 02h, 0C7h, 06h, 80h, 0AEh, 00h, 00h, 0A3h, 82h, 0AEh, 33h
db 0F6h, 0EBh, 4Bh, 8Bh, 0C6h, 0D1h, 0E0h, 0D1h, 0E0h, 0C4h, 5Eh, 0FCh, 03h, 0D8h, 26h, 8Bh
db 47h, 1Ah, 03h, 46h, 0FEh, 8Bh, 0DEh, 0D1h, 0E3h, 89h, 87h, 72h, 0C3h, 8Bh, 0C6h, 0D1h
db 0E0h, 0D1h, 0E0h, 0C4h, 5Eh, 0FCh, 03h, 0D8h, 8Bh, 46h, 0FEh, 26h, 03h, 47h, 1Ah, 0C4h
db 5Eh, 0FCh, 26h, 03h, 47h, 0Eh, 0C4h, 5Eh, 0FCh, 26h, 2Bh, 47h, 02h, 8Bh, 0DEh, 0D1h
db 0E3h, 0D1h, 0E3h, 0C7h, 87h, 70h, 0AEh, 00h, 00h, 89h, 87h, 72h, 0AEh, 46h, 83h, 0FEh
db 04h, 7Ch, 0B0h, 0EBh, 4Ch, 0C4h, 5Eh, 0FCh, 8Bh, 46h, 0FEh, 26h, 03h, 47h, 02h, 0C7h
db 06h, 6Ah, 0AEh, 00h, 00h, 0A3h, 6Ch, 0AEh, 0C4h, 5Eh, 0FCh, 8Bh, 46h, 0FEh, 26h, 03h
db 47h, 06h, 0C7h, 06h, 96h, 0AEh, 00h, 00h, 0A3h, 98h, 0AEh, 0C4h, 5Eh, 0FCh, 8Bh, 46h
db 0FEh, 26h, 03h, 47h, 0Ah, 0C7h, 06h, 86h, 0AEh, 00h, 00h, 0A3h, 88h, 0AEh, 0C4h, 5Eh
db 0FCh, 8Bh, 46h, 0FEh, 26h, 03h, 47h, 0Eh, 0C7h, 06h, 80h, 0AEh, 00h, 00h, 0A3h, 82h
db 0AEh, 0C4h, 5Eh, 0FCh, 26h, 8Bh, 47h, 12h, 03h, 46h, 0FEh, 0C4h, 5Eh, 0FCh, 26h, 8Bh
db 57h, 10h, 03h, 56h, 0FCh, 89h, 56h, 0F4h, 89h, 46h, 0F6h, 0C4h, 5Eh, 0FCh, 26h, 8Bh
db 47h, 16h, 03h, 46h, 0FEh, 0C4h, 5Eh, 0FCh, 26h, 8Bh, 57h, 14h, 03h, 56h, 0FCh, 89h
db 56h, 0F8h, 89h, 46h, 0FAh, 33h, 0F6h, 0EBh, 29h, 8Bh, 0C6h, 0B1h, 04h, 0D3h, 0E0h, 05h
db 0D2h, 0E3h, 1Eh, 50h, 8Bh, 0C6h, 99h, 0B1h, 04h, 0E8h, 85h, 28h, 50h, 52h, 8Bh, 56h
db 0F6h, 8Bh, 46h, 0F4h, 59h, 5Bh, 0E8h, 99h, 28h, 52h, 50h, 0B9h, 10h, 00h, 0E8h, 0F1h
db 28h, 46h, 83h, 0FEh, 40h, 7Ch, 0D2h, 33h, 0F6h, 0EBh, 29h, 8Bh, 0C6h, 0B1h, 05h, 0D3h
db 0E0h, 05h, 0C8h, 0C3h, 1Eh, 50h, 8Bh, 0C6h, 99h, 0B1h, 05h, 0E8h, 53h, 28h, 50h, 52h
db 8Bh, 56h, 0FAh, 8Bh, 46h, 0F8h, 59h, 5Bh, 0E8h, 67h, 28h, 52h, 50h, 0B9h, 20h, 00h
db 0E8h, 0BFh, 28h, 46h, 81h, 0FEh, 00h, 01h, 7Ch, 0D1h, 5Eh, 8Bh, 0E5h
}
}




/*=========================================================================*/

//////////////////////////
//
// moveega
// moves the standard stuff into EGA memory
// needs to be called after each setgrmode to ega to refill memory
//
//////////////////////////
void sub_0_4CEF(void)
{
asm {
db 4Ch, 4Ch, 0C7h, 46h, 0FEh, 00h, 00h, 0EBh, 3Dh, 0B0h, 02h, 50h, 0B8h, 0C4h
db 03h, 50h, 0E8h, 0B7h, 21h, 59h, 59h, 0B0h, 01h, 8Ah, 4Eh, 0FEh, 0D2h, 0E0h, 50h, 0B8h
db 0C5h, 03h, 50h, 0E8h, 0A6h, 21h, 59h, 59h, 0B8h, 0FFh, 56h, 50h, 33h, 0C0h, 50h, 0B8h
db 00h, 0A9h, 50h, 33h, 0C0h, 50h, 8Bh, 5Eh, 0FEh, 0D1h, 0E3h, 0FFh, 0B7h, 72h, 0C3h, 0E8h
db 0B8h, 1Fh, 83h, 0C4h, 0Ah, 0FFh, 46h, 0FEh, 83h, 7Eh, 0FEh, 04h, 7Ch, 0BDh, 0B0h, 02h
db 50h, 0B8h, 0C4h, 03h, 50h, 0E8h, 74h, 21h, 59h, 59h, 0B0h, 0Fh, 50h, 0B8h, 0C5h, 03h
db 50h, 0E8h, 68h, 21h, 59h, 59h, 8Bh, 0E5h
}
}

