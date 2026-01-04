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

#define CATALOG


#include "pcrlib.h"
#include <conio.h>

char	ch,str[80];	// scratch space

/*========================================================================*/

inputtype playermode[3] = {0,keyboard,joystick1};

boolean		keydown[128];

int JoyXlow [3], JoyXhigh [3], JoyYlow [3], JoyYhigh [3];

int MouseSensitivity;

char key[8],keyB1,keyB2;

void interrupt (*oldint9) ()=NULL;

char	demobuffer[5000];
char	*demoptr;
int	democount;
int	lastdemoval;		// so demo can be RLE compressed
enum demoenum indemo;

/*=======================================================================*/

/*
=======================
=
= SetupKBD
= Clears the keydown array and installs the INT 9 ISR if it isn't allready
= hooked up.
=
=======================
*/
//void sub_0_256A(void)
void SetupKBD ()
{
 void far *vect = getvect (9);
 int i;

 for (i=0;i<128;i++)			/* clear our key down table */
   keydown[i]= false;

 poke (0x40,0x1c,peek(0x40,0x1a));	/* clear the bios key buffer */

 if ( &Int9ISR != vect ) 		/* is our handler allready set up? */
 {
   oldint9 = vect;
   setvect (9,Int9ISR);
 }
}


/*
=========================
=
= Int9ISR
= Called for every keypress.  Keeps track of which keys are down, and passes
= the key on to DOS after clearing the dos buffer (max 1 char in buffer).
=
=========================
*/
//void interrupt sub_0_25D8(void)
void interrupt Int9ISR ()
{
 int key = inportb (0x60);		/* get the key pressed */

 if (key>127)
   keydown [key-128] = false;		/* break scan code */
 else
 {
   keydown [key] = true;		/* make scan code */
   poke (0x40,0x1c,peek(0x40,0x1a));	/* clear the bios key buffer */
 }
asm {
   push ax
   push	bx
   push	cx
   push	dx
   push	si
   push	di
   push	bp
 }
 oldint9 ();				/* give it to DOS */
asm {
   pop	bp
   pop  di
   pop	si
   pop	dx
   pop	cx
   pop	bx
   pop	ax
 }
 outport (0x20,0x20);			/* tell the int manager we got it */
}



/*
===========================
=
= ShutdownKBD
= Sets the int 9 vector back to oldint 9
=
===========================
*/
//void sub_0_2653(void)
void ShutdownKBD ()
{
 if (oldint9 != NULL)
   setvect (9,oldint9);
}


/*
===========================
=
= ControlKBD
=
===========================
*/

void sub_0_2673(void)
{
asm {
db 83h, 0ECh, 06h, 56h, 57h, 33h, 0FFh, 33h, 0F6h, 0A0h
db 60h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 83h, 0BFh, 9Ah, 0AEh, 00h, 74h, 03h, 0BEh, 0FFh
db 0FFh, 0A0h, 62h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 83h, 0BFh, 9Ah, 0AEh, 00h, 74h, 03h
db 0BFh, 01h, 00h, 0A0h, 64h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 83h, 0BFh, 9Ah, 0AEh, 00h
db 74h, 03h, 0BEh, 01h, 00h, 0A0h, 66h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 83h, 0BFh, 9Ah
db 0AEh, 00h, 74h, 03h, 0BFh, 0FFh, 0FFh, 0A0h, 61h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 83h
db 0BFh, 9Ah, 0AEh, 00h, 74h, 06h, 0BEh, 0FFh, 0FFh, 0BFh, 01h, 00h, 0A0h, 67h, 0AEh, 98h
db 0D1h, 0E0h, 8Bh, 0D8h, 83h, 0BFh, 9Ah, 0AEh, 00h, 74h, 06h, 0BEh, 0FFh, 0FFh, 0BFh, 0FFh
db 0FFh, 0A0h, 63h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 83h, 0BFh, 9Ah, 0AEh, 00h, 74h, 06h
db 0BEh, 01h, 00h, 0BFh, 01h, 00h, 0A0h, 65h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 83h, 0BFh
db 9Ah, 0AEh, 00h, 74h, 06h, 0BEh, 01h, 00h, 0BFh, 0FFh, 0FFh, 8Bh, 0C6h, 0BAh, 03h, 00h
db 0F7h, 0EAh, 03h, 0C7h, 2Dh, 0FCh, 0FFh, 8Bh, 0D8h, 83h, 0FBh, 08h, 77h, 46h, 0D1h, 0E3h
db 2Eh, 0FFh, 0A7h, 0AEh, 27h, 0C7h, 46h, 0FAh, 07h, 00h, 0EBh, 38h, 0C7h, 46h, 0FAh, 00h
db 00h, 0EBh, 31h, 0C7h, 46h, 0FAh, 01h, 00h, 0EBh, 2Ah, 0C7h, 46h, 0FAh, 06h, 00h, 0EBh
db 23h, 0C7h, 46h, 0FAh, 08h, 00h, 0EBh, 1Ch, 0C7h, 46h, 0FAh, 02h, 00h, 0EBh, 15h, 0C7h
db 46h, 0FAh, 05h, 00h, 0EBh, 0Eh, 0C7h, 46h, 0FAh, 04h, 00h, 0EBh, 07h, 0C7h, 46h, 0FAh
db 03h, 00h, 0EBh, 00h, 0A0h, 04h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 8Bh, 87h, 9Ah, 0AEh
db 89h, 46h, 0FCh, 0A0h, 05h, 0AEh, 98h, 0D1h, 0E0h, 8Bh, 0D8h, 8Bh, 87h, 9Ah, 0AEh, 89h
db 46h, 0FEh, 0FFh, 76h, 06h, 0FFh, 76h, 04h, 8Dh, 46h, 0FAh, 16h, 50h, 0B9h, 06h, 00h
db 0E8h, 0FFh, 4Dh, 8Bh, 46h, 04h, 0EBh, 00h, 5Fh, 5Eh, 8Bh, 0E5h

db 5Dh, 0C3h
//}
//}


dw 2735h
dw 273Ch
dw 2743h
dw 274Ah
dw 2751h
dw 2758h
dw 275Fh
dw 2766h
dw 276Dh



/*
============================
=
= ControlMouse
=
============================
*/

//void sub_0_27C0(void)
//{
//asm {
db 55h, 8Bh, 0ECh

db 83h, 0ECh, 0Ah, 56h, 57h, 0C7h, 46h, 0FEh, 00h, 00h, 0C7h, 46h, 0FCh
db 00h, 00h, 0B8h, 03h, 00h, 0CDh, 33h, 8Bh, 0F9h, 8Bh, 0F2h, 8Bh, 0C3h, 25h, 01h, 00h
db 89h, 46h, 0F8h, 8Bh, 0C3h, 25h, 02h, 00h, 0D1h, 0E8h, 89h, 46h, 0FAh, 8Bh, 0C7h, 05h
db 0C0h, 0FEh, 0BBh, 02h, 00h, 99h, 0F7h, 0FBh, 3Bh, 06h, 9Ah, 0AFh, 7Eh, 12h, 0C7h, 46h
db 0FEh, 01h, 00h, 0A1h, 9Ah, 0AFh, 0D1h, 0E0h, 8Bh, 0D7h, 2Bh, 0D0h, 8Bh, 0FAh, 0EBh, 25h
db 8Bh, 0C7h, 05h, 0C0h, 0FEh, 0BBh, 02h, 00h, 99h, 0F7h, 0FBh, 8Bh, 16h, 9Ah, 0AFh, 0F7h
db 0DAh, 3Bh, 0C2h, 7Dh, 10h, 0C7h, 46h, 0FEh, 0FFh, 0FFh, 0A1h, 9Ah, 0AFh, 0D1h, 0E0h, 8Bh
db 0D7h, 03h, 0D0h, 8Bh, 0FAh, 8Bh, 0C6h, 05h, 9Ch, 0FFh, 3Bh, 06h, 9Ah, 0AFh, 7Eh, 0Fh
db 0C7h, 46h, 0FCh, 01h, 00h, 8Bh, 0C6h, 2Bh, 06h, 9Ah, 0AFh, 8Bh, 0F0h, 0EBh, 1Ch, 8Bh
db 0C6h, 05h, 9Ch, 0FFh, 8Bh, 16h, 9Ah, 0AFh, 0F7h, 0DAh, 3Bh, 0C2h, 7Dh, 0Dh, 0C7h, 46h
db 0FCh, 0FFh, 0FFh, 8Bh, 0C6h, 03h, 06h, 9Ah, 0AFh, 8Bh, 0F0h, 0B8h, 04h, 00h, 8Bh, 0CFh
db 8Bh, 0D6h, 0CDh, 33h, 8Bh, 46h, 0FCh, 0BAh, 03h, 00h, 0F7h, 0EAh, 03h, 46h, 0FEh, 2Dh
db 0FCh, 0FFh, 8Bh, 0D8h, 83h, 0FBh, 08h, 77h, 46h, 0D1h, 0E3h, 2Eh, 0FFh, 0A7h, 0EBh, 28h
db 0C7h, 46h, 0F6h, 07h, 00h, 0EBh, 38h, 0C7h, 46h, 0F6h, 00h, 00h, 0EBh, 31h, 0C7h, 46h
db 0F6h, 01h, 00h, 0EBh, 2Ah, 0C7h, 46h, 0F6h, 06h, 00h, 0EBh, 23h, 0C7h, 46h, 0F6h, 08h
db 00h, 0EBh, 1Ch, 0C7h, 46h, 0F6h, 02h, 00h, 0EBh, 15h, 0C7h, 46h, 0F6h, 05h, 00h, 0EBh
db 0Eh, 0C7h, 46h, 0F6h, 04h, 00h, 0EBh, 07h, 0C7h, 46h, 0F6h, 03h, 00h, 0EBh, 00h, 0FFh
db 76h, 06h, 0FFh, 76h, 04h, 8Dh, 46h, 0F6h, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 0C2h, 4Ch
db 8Bh, 46h, 04h, 0EBh, 00h, 5Fh, 5Eh, 8Bh, 0E5h

db 5Dh, 0C3h
//}
//}


dw 2890h
dw 2897h
dw 289Eh
dw 28A5h
dw 28ACh
dw 28B3h
dw 28BAh
dw 28C1h
dw 28C8h


/*
===============================
=
= ReadJoystick
= Just return the resistance count of the joystick
=
===============================
*/

//void sub_0_28FD(void)
//{
//asm {
db 55h, 8Bh, 0ECh

db 83h, 0ECh, 0Ah, 56h, 57h, 8Bh, 7Eh, 06h, 8Bh, 76h, 08h, 83h, 7Eh, 04h, 01h, 75h
db 0Ch, 0C7h, 46h, 0F8h, 01h, 00h, 0C7h, 46h, 0F6h, 02h, 00h, 0EBh, 0Ah, 0C7h, 46h, 0F8h
db 04h, 00h, 0C7h, 46h, 0F6h, 08h, 00h, 0C7h, 05h, 00h, 00h, 0C7h, 04h, 00h, 00h, 0B8h
db 01h, 02h, 50h, 0E8h, 46h, 41h, 59h, 50h, 0B8h, 01h, 02h, 50h, 0E8h, 7Dh, 45h, 59h
db 59h, 0FAh, 0B8h, 01h, 02h, 50h, 0E8h, 33h, 41h, 59h, 0B4h, 00h, 89h, 46h, 0FEh, 8Bh
db 46h, 0F8h, 85h, 46h, 0FEh, 74h, 05h, 0B8h, 01h, 00h, 0EBh, 02h, 33h, 0C0h, 89h, 46h
db 0FCh, 8Bh, 46h, 0F6h, 85h, 46h, 0FEh, 74h, 05h, 0B8h, 01h, 00h, 0EBh, 02h, 33h, 0C0h
db 89h, 46h, 0FAh, 8Bh, 46h, 0FCh, 01h, 05h, 8Bh, 46h, 0FAh, 01h, 04h, 8Bh, 46h, 0FCh
db 03h, 46h, 0FAh, 74h, 0Ch, 81h, 3Dh, 0F4h, 01h, 7Dh, 06h, 81h, 3Ch, 0F4h, 01h, 7Ch
db 0B1h, 0FBh, 5Fh, 5Eh, 8Bh, 0E5h
}
}


/*
=============================
=
= ControlJoystick (joy# = 1 / 2)
=
=============================
*/

void sub_0_2998(void)
{
asm {
db 83h, 0ECh, 0Eh, 56h, 57h
db 8Bh, 76h, 08h, 0C7h, 46h, 0FEh, 00h, 00h, 0C7h, 46h, 0FCh, 00h, 00h, 0C7h, 46h, 0FAh
db 00h, 00h, 0C7h, 46h, 0F8h, 00h, 00h, 8Dh, 46h, 0FCh, 50h, 8Dh, 46h, 0FEh, 50h, 56h
db 0E8h, 3Ah, 0FFh, 83h, 0C4h, 06h, 81h, 7Eh, 0FEh, 0F4h, 01h, 7Eh, 05h, 0B8h, 01h, 00h
db 0EBh, 02h, 33h, 0C0h, 50h, 81h, 7Eh, 0FCh, 0F4h, 01h, 7Eh, 05h, 0B8h, 01h, 00h, 0EBh
db 02h, 33h, 0C0h, 5Ah, 0Bh, 0D0h, 74h, 18h, 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 87h, 0EAh, 0ADh
db 40h, 89h, 46h, 0FEh, 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 87h, 0F6h, 0ADh, 40h, 89h, 46h, 0FCh
db 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 87h, 0F0h, 0ADh, 3Bh, 46h, 0FEh, 7Dh, 07h, 0C7h, 46h, 0FAh
db 01h, 00h, 0EBh, 12h, 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 87h, 0EAh, 0ADh, 3Bh, 46h, 0FEh, 7Eh
db 05h, 0C7h, 46h, 0FAh, 0FFh, 0FFh, 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 87h, 0FCh, 0ADh, 3Bh, 46h
db 0FCh, 7Dh, 07h, 0C7h, 46h, 0F8h, 01h, 00h, 0EBh, 12h, 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 87h
db 0F6h, 0ADh, 3Bh, 46h, 0FCh, 7Eh, 05h, 0C7h, 46h, 0F8h, 0FFh, 0FFh, 8Bh, 46h, 0F8h, 0BAh
db 03h, 00h, 0F7h, 0EAh, 03h, 46h, 0FAh, 2Dh, 0FCh, 0FFh, 8Bh, 0D8h, 83h, 0FBh, 08h, 77h
db 46h, 0D1h, 0E3h, 2Eh, 0FFh, 0A7h, 16h, 2Bh, 0C7h, 46h, 0F2h, 07h, 00h, 0EBh, 38h, 0C7h
db 46h, 0F2h, 00h, 00h, 0EBh, 31h, 0C7h, 46h, 0F2h, 01h, 00h, 0EBh, 2Ah, 0C7h, 46h, 0F2h
db 06h, 00h, 0EBh, 23h, 0C7h, 46h, 0F2h, 08h, 00h, 0EBh, 1Ch, 0C7h, 46h, 0F2h, 02h, 00h
db 0EBh, 15h, 0C7h, 46h, 0F2h, 05h, 00h, 0EBh, 0Eh, 0C7h, 46h, 0F2h, 04h, 00h, 0EBh, 07h
db 0C7h, 46h, 0F2h, 03h, 00h, 0EBh, 00h, 0B8h, 01h, 02h, 50h, 0E8h, 0CEh, 3Fh, 59h, 0B4h
db 00h, 8Bh, 0F8h, 83h, 0FEh, 01h, 75h, 22h, 0F7h, 0C7h, 10h, 00h, 75h, 05h, 0B8h, 01h
db 00h, 0EBh, 02h, 33h, 0C0h, 89h, 46h, 0F4h, 0F7h, 0C7h, 20h, 00h, 75h, 05h, 0B8h, 01h
db 00h, 0EBh, 02h, 33h, 0C0h, 89h, 46h, 0F6h, 0EBh, 20h, 0F7h, 0C7h, 40h, 00h, 75h, 05h
db 0B8h, 01h, 00h, 0EBh, 02h, 33h, 0C0h, 89h, 46h, 0F4h, 0F7h, 0C7h, 80h, 00h, 75h, 05h
db 0B8h, 01h, 00h, 0EBh, 02h, 33h, 0C0h, 89h, 46h, 0F6h, 0FFh, 76h, 06h, 0FFh, 76h, 04h
db 8Dh, 46h, 0F2h, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 97h, 4Ah, 8Bh, 46h, 04h, 0EBh, 00h
db 5Fh, 5Eh, 8Bh, 0E5h

db 5Dh, 0C3h
//}
//}


dw 2A68h
dw 2A6Fh
dw 2A76h
dw 2A7Dh
dw 2A84h
dw 2A8Bh
dw 2A92h
dw 2A99h
dw 2AA0h


/*
=============================
=
= ControlPlayer
=
= Expects a 1 or a 2
=
=============================
*/

//void sub_0_2B28(void)
//{
//asm {
db 55h, 8Bh, 0ECh

db 83h, 0ECh, 20h, 83h, 3Eh
db 5Ch, 0C3h, 00h, 74h, 0Ah, 83h, 3Eh, 5Ch, 0C3h, 02h, 74h, 03h, 0E9h, 0BCh, 00h, 8Bh
db 5Eh, 08h, 0D1h, 0E3h, 8Bh, 9Fh, 04h, 0Ch, 83h, 0FBh, 03h, 76h, 03h, 0E9h, 81h, 00h
db 0D1h, 0E3h, 2Eh, 0FFh, 0A7h, 42h, 2Ch, 8Dh, 46h, 0FAh, 16h, 50h, 16h, 8Dh, 46h, 0F2h
db 50h, 0E8h, 0Fh, 0FBh, 59h, 59h, 8Dh, 46h, 0F2h, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 31h
db 4Ah, 0EBh, 5Eh, 8Dh, 46h, 0FAh, 16h, 50h, 16h, 8Dh, 46h, 0ECh, 50h, 0E8h, 40h, 0FCh
db 59h, 59h, 8Dh, 46h, 0ECh, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 15h, 4Ah, 0EBh, 42h, 8Dh
db 46h, 0FAh, 16h, 50h, 0B8h, 01h, 00h, 50h, 16h, 8Dh, 46h, 0E6h, 50h, 0E8h, 0F8h, 0FDh
db 83h, 0C4h, 06h, 8Dh, 46h, 0E6h, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 0F4h, 49h, 0EBh, 21h
db 8Dh, 46h, 0FAh, 16h, 50h, 0B8h, 02h, 00h, 50h, 16h, 8Dh, 46h, 0E0h, 50h, 0E8h, 0D7h
db 0FDh, 83h, 0C4h, 06h, 8Dh, 46h, 0E0h, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 0D3h, 49h, 0EBh
db 00h, 83h, 3Eh, 5Ch, 0C3h, 02h, 75h, 21h, 8Bh, 46h, 0FAh, 0D1h, 0E0h, 0D1h, 0E0h, 8Bh
db 56h, 0FEh, 0D1h, 0E2h, 0Bh, 0C2h, 0Bh, 46h, 0FCh, 89h, 46h, 0F8h, 8Bh, 1Eh, 94h, 0AEh
db 8Ah, 46h, 0F8h, 88h, 07h, 0FFh, 06h, 94h, 0AEh, 0EBh, 2Dh, 8Bh, 1Eh, 94h, 0AEh, 8Ah
db 07h, 98h, 89h, 46h, 0F8h, 0FFh, 06h, 94h, 0AEh, 8Bh, 46h, 0F8h, 25h, 01h, 00h, 89h
db 46h, 0FCh, 8Bh, 46h, 0F8h, 25h, 01h, 00h, 89h, 46h, 0FEh, 8Bh, 46h, 0F8h, 25h, 3Ch
db 00h, 0D1h, 0F8h, 0D1h, 0F8h, 89h, 46h, 0FAh, 0FFh, 76h, 06h, 0FFh, 76h, 04h, 8Dh, 46h
db 0FAh, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 69h, 49h, 8Bh, 46h, 04h, 0EBh, 00h, 8Bh, 0E5h

db 5Dh, 0C3h
//}
//}


dw 2B57h
dw 2B73h
dw 2B8Fh
dw 2BB0h


////////////////////////
//
// RecordDemo
// Clears the demo buffer and starts capturing events
//
////////////////////////

//void sub_0_2C4A(void)
//{
//asm {
db 55h, 8Bh, 0ECh

db 0A0h, 6Eh, 0AEh
db 0A2h, 9Eh, 0AFh, 0C7h, 06h, 94h, 0AEh, 9Fh, 0AFh, 0C7h, 06h, 5Ch, 0C3h, 02h, 00h
}
}


////////////////////////
//
// LoadDemo / SaveDemo
// Loads a demo from disk or
// saves the accumulated demo command string to disk
//
////////////////////////

void sub_0_2C61(void)
{
asm {
db 83h, 0ECh, 06h, 0B8h, 48h, 0Ch, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h
db 8Fh, 43h, 59h, 59h, 0B8h, 0Ah, 00h, 50h, 8Dh, 46h, 0FAh, 50h, 0FFh, 76h, 04h, 0E8h
db 0E1h, 3Fh, 83h, 0C4h, 06h, 8Dh, 46h, 0FAh, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 38h, 43h
db 59h, 59h, 0B8h, 4Dh, 0Ch, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 2Bh, 43h, 59h, 59h, 0FFh
db 36h, 46h, 0Ch, 0B8h, 10h, 0AEh, 50h, 0E8h, 1Eh, 43h, 59h, 59h, 0B8h, 9Eh, 0AFh, 1Eh
db 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 1Fh, 01h, 83h, 0C4h, 06h, 0A0h, 9Eh, 0AFh, 98h, 0A3h
db 6Eh, 0AEh, 0C7h, 06h, 94h, 0AEh, 9Fh, 0AFh, 0C7h, 06h, 5Ch, 0C3h, 01h, 00h, 8Bh, 0E5h
}
}


void sub_0_2CD2(void)
{
asm {
db 83h, 0ECh, 06h, 0B8h, 4Fh, 0Ch, 50h, 0B8h, 10h, 0AEh, 50h
db 0E8h, 1Eh, 43h, 59h, 59h, 0B8h, 0Ah, 00h, 50h, 8Dh, 46h, 0FAh, 50h, 0FFh, 76h, 04h
db 0E8h, 70h, 3Fh, 83h, 0C4h, 06h, 8Dh, 46h, 0FAh, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 0C7h
db 42h, 59h, 59h, 0B8h, 54h, 0Ch, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 0BAh, 42h, 59h, 59h
db 0FFh, 36h, 46h, 0Ch, 0B8h, 10h, 0AEh, 50h, 0E8h, 0ADh, 42h, 59h, 59h, 0A1h, 94h, 0AEh
db 2Dh, 9Eh, 0AFh, 99h, 52h, 50h, 0B8h, 9Eh, 0AFh, 1Eh, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h
db 52h, 01h, 83h, 0C4h, 0Ah, 0C7h, 06h, 5Ch, 0C3h, 00h, 00h, 8Bh, 0E5h
}
}


////////////////////////
//
// StartDemo
//
////////////////////////

/*=========================================================================*/


/*
** Miscellanious library routines
*/


///////////////////////////////
//
// clearkeys
// Clears out the bios buffer and zeros out the keydown array
//
///////////////////////////////
//void sub_0_2D3F(void)
void clearkeys (void)
{
  int i;
  while (bioskey (1))
    bioskey(0);

  for (i=0;i<128;i++)
    keydown [i]=0;
}

/*
===========================================
=
= Allocate a block aligned on a paragraph
=
===========================================
*/

void far *lastparalloc;	// global variable of the EXACT (not paralign)
				// last block, so it can be freed right
//void sub_0_2D70(void)
void huge *paralloc (long size)
{
 void huge *temp;
 word seg,ofs;
/* allocate a block with extra space */
 lastparalloc = (void far*)temp = farmalloc (size+15);
 if (temp == NULL)
 //
 // not enough memory!
 //
 {
   setscreenmode (text);
	_quit ("Out of memory!");
 }

 ofs=FP_OFF(temp);
 if (ofs!=0)			/* set offset to 0 and bump segment */
 {
  seg=FP_SEG(temp);
  seg++;
  ofs=0;
  temp=MK_FP (seg,ofs);
 }
 return (void huge *) temp;
}

//==========================================================================

/*
==============================================
=
= Load a *LARGE* file into a FAR buffer!
= by John Romero (C) 1990 PCRcade
=
==============================================
*/
//void sub_0_2DD7(void)
unsigned long LoadFile(char *filename,char huge *buffer)
{
 unsigned int handle,flength1=0,flength2=0,buf1,buf2,foff1,foff2;

 buf1=FP_OFF(buffer);
 buf2=FP_SEG(buffer);

asm		mov	WORD PTR foff1,0  	// file offset = 0 (start)
asm		mov	WORD PTR foff2,0

asm		mov	dx,filename
asm		mov	ax,3d00h		// OPEN w/handle (read only)
asm		int	21h
asm		jc	out

asm		mov	handle,ax
asm		mov	bx,ax
asm		xor	cx,cx
asm		xor	dx,dx
asm		mov	ax,4202h
asm		int	21h			// SEEK (find file length)
asm		jc	out

asm		mov	flength1,ax
asm		mov	flength2,dx

asm		mov	cx,flength2
asm		inc	cx			// <- at least once!

L_1:

asm		push	cx

asm		mov	cx,foff2
asm		mov	dx,foff1
asm		mov	ax,4200h
asm		int	21h			// SEEK from start

asm		push	ds
asm		mov	bx,handle
asm		mov	cx,-1
asm		mov	dx,buf1
asm		mov	ax,buf2
asm		mov	ds,ax
asm		mov	ah,3fh			// READ w/handle
asm		int	21h
asm		pop	ds

asm		pop	cx
asm		jc	out
asm		cmp	ax,-1
asm		jne	out

asm		push	cx			// need to read the last byte
asm		push	ds			// into the segment! IMPORTANT!
asm		mov	bx,handle
asm		mov	cx,1
asm		mov	dx,buf1
asm		add	dx,-1
asm		mov	ax,buf2
asm		mov	ds,ax
asm		mov	ah,3fh
asm		int	21h
asm		pop	ds
asm		pop	cx

asm		add	buf2,1000h
asm		inc	WORD PTR foff2
asm		loop	L_1

out:

asm		mov	bx,handle		// CLOSE w/handle
asm		mov	ah,3eh
asm		int	21h

return (flength2*0x10000+flength1);

}

//===========================================================================

/*
==============================================
=
= Save a *LARGE* file far a FAR buffer!
= by John Romero (C) 1990 PCRcade
=
==============================================
*/
//void sub_0_2E84(void)
void SaveFile(char *filename,char huge *buffer, long size)
{
 unsigned int handle,buf1,buf2,foff1,foff2;

 buf1=FP_OFF(buffer);
 buf2=FP_SEG(buffer);

asm		mov	WORD PTR foff1,0  		// file offset = 0 (start)
asm		mov	WORD PTR foff2,0

asm		mov	dx,filename
asm		mov	ax,3c00h		// CREATE w/handle (read only)
asm		xor	cx,cx
asm		int	21h
asm		jc	out

asm		mov	handle,ax
asm		cmp	word ptr size+2,0	// larger than 1 segment?
asm		je	L2

L1:

asm		push	ds
asm		mov	bx,handle
asm		mov	cx,8000h
asm		mov	dx,buf1
asm		mov	ax,buf2
asm		mov	ds,ax
asm		mov	ah,40h			// WRITE w/handle
asm		int	21h
asm		pop	ds

asm		add	buf2,800h		// bump ptr up 1/2 segment
asm		sub	WORD PTR size,8000h		// done yet?
asm		sbb	WORD PTR size+2,0
asm		cmp	WORD PTR size+2,0
asm		ja	L1
asm		cmp	WORD PTR size,8000h
asm		jae	L1

L2:

asm		push	ds
asm		mov	bx,handle
asm		mov	cx,WORD PTR size
asm		mov	dx,buf1
asm		mov	ax,buf2
asm		mov	ds,ax
asm		mov	ah,40h			// WRITE w/handle
asm		int	21h
asm		pop	ds
asm		jmp	out

out:

asm		mov	bx,handle		// CLOSE w/handle
asm		mov	ah,3eh
asm		int	21h

}

//==========================================================================


/*
====================================
=
= bloadin
= Paraligns just enough space and bloads in the
= specified file, returning a pointer to the start
=
====================================
*/

void sub_0_2F05(void)
{
asm {
db 83h, 0ECh, 08h, 56h, 0B8h, 00h, 80h, 50h
db 0FFh, 76h, 04h, 0E8h, 21h, 3Eh, 59h, 59h, 8Bh, 0F0h, 3Dh, 0FFh, 0FFh, 74h, 3Ah, 56h
db 0E8h, 0EFh, 3Ah, 59h, 89h, 46h, 0FCh, 89h, 56h, 0FEh, 0FFh, 76h, 0FEh, 0FFh, 76h, 0FCh
db 0E8h, 3Dh, 0FEh, 59h, 59h, 89h, 46h, 0F8h, 89h, 56h, 0FAh, 56h, 0E8h, 0F5h, 30h, 59h
db 0FFh, 76h, 0FAh, 0FFh, 76h, 0F8h, 0FFh, 76h, 04h, 0E8h, 8Bh, 0FEh, 83h, 0C4h, 06h, 8Bh
db 56h, 0FAh, 8Bh, 46h, 0F8h, 0EBh, 08h, 0EBh, 06h, 33h, 0D2h, 33h, 0C0h, 0EBh, 00h, 5Eh
db 8Bh, 0E5h
}
}


void sub_0_2F64(void)
{
asm {
db 83h, 0ECh, 14h, 56h, 0B8h, 00h, 80h, 50h, 0FFh
db 76h, 04h, 0E8h, 0C2h, 3Dh, 59h, 59h, 8Bh, 0F0h, 3Dh, 0FFh, 0FFh, 75h, 03h, 0E9h, 0A3h
db 00h, 56h, 0E8h, 8Dh, 3Ah, 59h, 89h, 46h, 0FCh, 89h, 56h, 0FEh, 0B8h, 04h, 00h, 50h
db 8Dh, 46h, 0F8h, 50h, 56h, 0E8h, 30h, 3Fh, 83h, 0C4h, 06h, 56h, 0E8h, 95h, 30h, 59h
db 0FFh, 76h, 0FAh, 0FFh, 76h, 0F8h, 0E8h, 0C7h, 0FDh, 59h, 59h, 89h, 46h, 0ECh, 89h, 56h
db 0EEh, 0A1h, 66h, 0C3h, 8Bh, 16h, 64h, 0C3h, 89h, 56h, 0F4h, 89h, 46h, 0F6h, 0FFh, 76h
db 0FEh, 0FFh, 76h, 0FCh, 0E8h, 0A9h, 0FDh, 59h, 59h, 89h, 46h, 0F0h, 89h, 56h, 0F2h, 0FFh
db 76h, 0F2h, 0FFh, 76h, 0F0h, 0FFh, 76h, 04h, 0E8h, 0FCh, 0FDh, 83h, 0C4h, 06h, 0FFh, 76h
db 0FAh, 0FFh, 76h, 0F8h, 0FFh, 76h, 0EEh, 0FFh, 76h, 0ECh, 8Bh, 56h, 0F2h, 8Bh, 46h, 0F0h
db 33h, 0C9h, 0BBh, 04h, 00h, 0E8h, 4Ah, 45h, 52h, 50h, 0E8h, 05h, 2Dh, 83h, 0C4h, 0Ch
db 0FFh, 36h, 66h, 0C3h, 0FFh, 36h, 64h, 0C3h, 0E8h, 0C3h, 41h, 59h, 59h, 8Bh, 46h, 0F6h
db 8Bh, 56h, 0F4h, 89h, 16h, 64h, 0C3h, 0A3h, 66h, 0C3h, 8Bh, 56h, 0EEh, 8Bh, 46h, 0ECh
db 0EBh, 08h, 0EBh, 06h, 33h, 0D2h, 33h, 0C0h, 0EBh, 00h, 5Eh, 8Bh, 0E5h
}
}


/*==================================================================================*/

/*
** Graphic routines
*/

grtype grmode;
cardtype _videocard;

void huge *charptr;		// 8*8 tileset
void huge *tileptr;		// 16*16 tileset
void huge *picptr;		// any size picture set
void huge *spriteptr;		// any size masked and hit rect sprites
void huge *egaspriteptr[4];	// spriteptr for each ega plane's data

unsigned crtcaddr;

int sx,sy,leftedge;

/*
========================
=
= setscreenmode
= Call BIOS to set TEXT / CGAgr / EGAgr / VGAgr
=
========================
*/
//void sub_0_302F(void)
void setscreenmode (grtype mode)
{
asm {
db 83h, 0ECh, 04h, 0C7h, 46h, 0FCh, 8Eh, 94h, 8Ch, 5Eh, 0FEh, 8Bh, 5Eh, 04h
db 83h, 0FBh, 03h, 77h, 4Eh, 0D1h, 0E3h, 2Eh, 0FFh, 0A7h, 9Dh, 30h, 0B8h, 03h, 00h, 0CDh
db 10h, 0C7h, 06h, 0EAh, 10h, 00h, 0B8h, 0EBh, 3Ah, 0B8h, 04h, 00h, 0CDh, 10h, 0C7h, 06h
db 0EAh, 10h, 00h, 0B8h, 0EBh, 2Dh, 0B8h, 0Dh, 00h, 0CDh, 10h, 0C7h, 06h, 0EAh, 10h, 00h
db 0A0h, 0E8h, 7Bh, 1Ch, 0E8h, 19h, 0D2h, 0EBh, 1Ah, 0B8h, 13h, 00h, 0CDh, 10h, 0C7h, 06h
db 0EAh, 10h, 00h, 0A0h, 0C4h, 56h, 0FCh, 33h, 0DBh, 0B9h, 00h, 01h, 0B8h, 12h, 10h, 0CDh
db 10h, 0EBh, 00h, 0C7h, 06h, 60h, 0C3h, 0D4h, 03h, 8Bh, 0E5h

db 5Dh, 0C3h
//}
//}


dw 304Ch
dw 3059h
dw 3066h
dw 3079h


/*
========================
=
= egasplitscreen
=
========================
*/

//void sub_0_30A5(void)
//{
//asm {
db 55h, 8Bh, 0ECh

db 56h, 8Bh, 76h, 04h, 0E8h, 0A7h, 24h, 83h
db 3Eh, 0E8h, 0ADh, 05h, 75h, 09h, 0BAh, 02h, 00h, 8Bh, 0C6h, 0F7h, 0EAh, 8Bh, 0F0h, 0B0h
db 18h, 50h, 0FFh, 36h, 60h, 0C3h, 0E8h, 0F3h, 3Dh, 59h, 59h, 8Bh, 0C6h, 0BBh, 00h, 01h
db 99h, 0F7h, 0FBh, 52h, 0A1h, 60h, 0C3h, 40h, 50h, 0E8h, 0E0h, 3Dh, 59h, 59h, 0B0h, 07h
db 50h, 0FFh, 36h, 60h, 0C3h, 0E8h, 0D4h, 3Dh, 59h, 59h, 8Bh, 0C6h, 0BBh, 00h, 01h, 99h
db 0F7h, 0FBh, 0B1h, 04h, 0D2h, 0E0h, 0FEh, 0C0h, 50h, 0A1h, 60h, 0C3h, 40h, 50h, 0E8h, 0BBh
db 3Dh, 59h, 59h, 83h, 3Eh, 0E8h, 0ADh, 05h, 75h, 22h, 0B0h, 09h, 50h, 0FFh, 36h, 60h
db 0C3h, 0E8h, 0A8h, 3Dh, 59h, 59h, 0A1h, 60h, 0C3h, 40h, 50h, 0E8h, 5Eh, 39h, 59h, 24h
db 0BFh, 50h, 0A1h, 60h, 0C3h, 40h, 50h, 0E8h, 92h, 3Dh, 59h, 59h, 5Eh
}
}


/*
========================
=
= crtcstart
=
========================
*/
//void sub_0_312F(void)
void crtcstart (unsigned start)
{
  WaitVBL ();
  outportb (crtcaddr,CRTCSTARTL);
  outportb (crtcaddr+1,start % 256);
  outportb (crtcaddr,CRTCSTARTH);
  outportb (crtcaddr+1,start / 256);
}


int win_xl,win_yl,win_xh,win_yh;

int screencenterx = 19,screencentery = 11;

//////////////////////////
//
// drawwindow
// draws a bordered window and homes the cursor
//
//////////////////////////

void sub_0_3171(void)
{
asm {
db 56h, 57h, 8Bh, 46h, 04h, 0A3h, 8Ch, 0AEh, 8Bh, 46h, 06h, 0A3h
db 90h, 0AEh, 8Bh, 46h, 08h, 0A3h, 8Eh, 0AEh, 8Bh, 46h, 0Ah, 0A3h, 92h, 0AEh, 0B8h, 01h
db 00h, 50h, 0FFh, 76h, 06h, 0FFh, 76h, 04h, 0E8h, 06h, 24h, 83h, 0C4h, 06h, 8Bh, 46h
db 04h, 40h, 8Bh, 0F0h, 0EBh, 0Fh, 0B8h, 02h, 00h, 50h, 0FFh, 76h, 06h, 56h, 0E8h, 0F0h
db 23h, 83h, 0C4h, 06h, 46h, 3Bh, 76h, 08h, 7Ch, 0ECh, 0B8h, 03h, 00h, 50h, 0FFh, 76h
db 06h, 0FFh, 76h, 08h, 0E8h, 0DAh, 23h, 83h, 0C4h, 06h, 8Bh, 46h, 06h, 40h, 8Bh, 0F8h
db 0EBh, 37h, 0B8h, 04h, 00h, 50h, 57h, 0FFh, 76h, 04h, 0E8h, 0C4h, 23h, 83h, 0C4h, 06h
db 8Bh, 46h, 04h, 40h, 8Bh, 0F0h, 0EBh, 0Dh, 0B8h, 20h, 00h, 50h, 57h, 56h, 0E8h, 0B0h
db 23h, 83h, 0C4h, 06h, 46h, 3Bh, 76h, 08h, 7Ch, 0EEh, 0B8h, 05h, 00h, 50h, 57h, 0FFh
db 76h, 08h, 0E8h, 9Ch, 23h, 83h, 0C4h, 06h, 47h, 3Bh, 7Eh, 0Ah, 7Ch, 0C4h, 0B8h, 06h
db 00h, 50h, 0FFh, 76h, 0Ah, 0FFh, 76h, 04h, 0E8h, 86h, 23h, 83h, 0C4h, 06h, 8Bh, 46h
db 04h, 40h, 8Bh, 0F0h, 0EBh, 0Fh, 0B8h, 07h, 00h, 50h, 0FFh, 76h, 0Ah, 56h, 0E8h, 70h
db 23h, 83h, 0C4h, 06h, 46h, 3Bh, 76h, 08h, 7Ch, 0ECh, 0B8h, 08h, 00h, 50h, 0FFh, 76h
db 0Ah, 0FFh, 76h, 08h, 0E8h, 5Ah, 23h, 83h, 0C4h, 06h, 8Bh, 46h, 04h, 40h, 0A3h, 8Ah
db 0AEh, 0A3h, 02h, 0AEh, 8Bh, 46h, 06h, 40h, 0A3h, 68h, 0AEh, 5Fh, 5Eh
}
}

////////////////////////////
//
// erasewindow
// clears out the last window and it's border to spaces
//
///////////////////////////

void sub_0_325F(void)
{
asm {
db 56h, 57h, 8Bh, 76h, 06h, 0EBh, 17h, 8Bh, 7Eh, 04h, 0EBh, 0Ch, 0FFh, 76h
db 0Ch, 56h, 57h, 0E8h, 2Bh, 23h, 83h, 0C4h, 06h, 47h, 3Bh, 7Eh, 08h, 7Eh, 0EFh, 46h
db 3Bh, 76h, 0Ah, 7Eh, 0E4h, 5Fh, 5Eh
}
}


void sub_0_3289(void)
{
asm {
db 0B8h, 20h, 00h, 50h
db 0FFh, 36h, 92h, 0AEh, 0FFh, 36h, 8Eh, 0AEh, 0FFh, 36h, 90h, 0AEh, 0FFh, 36h, 8Ch, 0AEh
db 0E8h, 0BCh, 0FFh, 83h, 0C4h, 0Ah
}
}

/////////////////////////////
//
// centerwindow
// Centers a drawwindow of the given size
//
/////////////////////////////

void sub_0_32A8(void)
{
asm {
db 56h, 57h, 8Bh, 46h, 04h
db 0BBh, 02h, 00h, 99h, 0F7h, 0FBh, 8Bh, 16h, 0Eh, 0Ch, 2Bh, 0D0h, 8Bh, 0FAh, 8Bh, 46h
db 06h, 0BBh, 02h, 00h, 99h, 0F7h, 0FBh, 8Bh, 16h, 10h, 0Ch, 2Bh, 0D0h, 8Bh, 0F2h, 8Bh
db 0C6h, 03h, 46h, 06h, 40h, 50h, 8Bh, 0C7h, 03h, 46h, 04h, 40h, 50h, 56h, 57h, 0E8h
db 8Fh, 0FEh, 83h, 0C4h, 08h, 5Fh, 5Eh
}
}

///////////////////////////////
//
// expwin {h / v}
// Grows the window outward
//
///////////////////////////////
void sub_0_32E9(void)
{
asm {
db 56h, 57h, 8Bh, 7Eh
db 04h, 8Bh, 76h, 06h, 83h, 0FFh, 02h, 7Eh, 26h, 83h, 0FEh, 02h, 7Eh, 13h, 8Bh, 0C6h
db 05h, 0FEh, 0FFh, 50h, 8Bh, 0C7h, 05h, 0FEh, 0FFh, 50h, 0E8h, 0DCh, 0FFh, 59h, 59h, 0EBh
db 0Ch, 56h, 8Bh, 0C7h, 05h, 0FEh, 0FFh, 50h, 0E8h, 23h, 00h, 59h, 59h, 0EBh, 11h, 83h
db 0FEh, 02h, 7Eh, 0Ch, 8Bh, 0C6h, 05h, 0FEh, 0FFh, 50h, 57h, 0E8h, 39h, 00h, 59h, 59h
db 0E8h, 23h, 22h, 56h, 57h, 0E8h, 70h, 0FFh, 59h, 59h, 5Fh, 5Eh
}
}


void sub_0_333E(void)
{
asm {
db 56h, 8Bh, 76h, 04h, 83h, 0FEh, 02h, 7Eh, 0Eh, 0FFh, 76h, 06h, 8Bh, 0C6h, 05h
db 0FEh, 0FFh, 50h, 0E8h, 0E8h, 0FFh, 59h, 59h, 0E8h, 0FBh, 21h, 0FFh, 76h, 06h, 56h, 0E8h
db 46h, 0FFh, 59h, 59h, 5Eh
}
}


void sub_0_3367(void)
{
asm {
db 56h, 8Bh, 76h, 06h, 83h, 0FEh
db 02h, 7Eh, 0Eh, 8Bh, 0C6h, 05h, 0FEh, 0FFh, 50h, 0FFh, 76h, 04h, 0E8h, 0E8h, 0FFh, 59h
db 59h, 0E8h, 0D2h, 21h, 56h, 0FFh, 76h, 04h, 0E8h, 1Dh, 0FFh, 59h, 59h, 5Eh
}
}


/////////////////////////
//
// get
// Flash a cursor at sx,sy and waits for a user bioskey
//
/////////////////////////

void sub_0_3390(void)
{
asm {
db 56h, 57h, 0BEh, 09h, 00h, 0EBh, 21h, 8Bh, 0C6h, 46h, 50h, 0FFh, 36h
db 68h, 0AEh, 0FFh, 36h, 02h, 0AEh, 0E8h, 0F8h, 21h, 83h, 0C4h, 06h, 0E8h, 0A7h, 21h, 0E8h
db 0A4h, 21h, 0E8h, 0A1h, 21h, 0E8h, 9Eh, 21h, 0E8h, 9Bh, 21h, 0B8h, 01h, 00h, 50h, 0E8h
db 85h, 35h, 59h, 8Bh, 0F8h, 0Bh, 0C0h, 75h, 05h, 83h, 0FEh, 0Dh, 7Ch, 0CCh, 0Bh, 0FFh
db 74h, 0C3h, 0B8h, 20h, 00h, 50h, 0FFh, 36h, 68h, 0AEh, 0FFh, 36h, 02h, 0AEh, 0E8h, 0C0h
db 21h, 83h, 0C4h, 06h, 33h, 0C0h, 50h, 0E8h, 5Dh, 35h, 59h, 0EBh, 00h, 5Fh, 5Eh
}
}


/////////////////////////
//
// print
// Prints a string at sx,sy.  No clipping!!!
//
/////////////////////////
//void sub_0_33F1(void)
void print (const char *str)
{
asm {
db 4Ch, 4Ch, 0EBh, 37h, 80h, 7Eh, 0FFh, 0Ah, 75h, 0Ch, 0FFh, 06h
db 68h, 0AEh, 0A1h, 8Ah, 0AEh, 0A3h, 02h, 0AEh, 0EBh, 25h, 80h, 7Eh, 0FFh, 0Dh, 75h, 08h
db 0A1h, 8Ah, 0AEh, 0A3h, 02h, 0AEh, 0EBh, 17h, 8Ah, 46h, 0FFh, 98h, 50h, 0FFh, 36h, 68h
db 0AEh, 0A1h, 02h, 0AEh, 0FFh, 06h, 02h, 0AEh, 50h, 0E8h, 75h, 21h, 83h, 0C4h, 06h, 8Bh
db 5Eh, 04h, 0FFh, 46h, 04h, 8Ah, 07h, 88h, 46h, 0FFh, 0Ah, 0C0h, 75h, 0BAh, 8Bh, 0E5h
}
}


///////////////////////////
//
// printint / printlong
// Converts the value to a string and prints it
//
///////////////////////////
//void sub_0_3442(void)
void printint (int val)
{
  itoa(val,str,10);
  print (str);
}
//void sub_0_3460(void)
void printlong (long val)
{
  ltoa(val,str,10);
  print (str);
}

/*========================================================================*/

int _MouseStatus;

////////////////////////////////////////////////////////////////////
//
// Mouse Routines
//
////////////////////////////////////////////////////////////////////
//void sub_0_3481(void)
int _MouseInit(void)
{
 union REGS regs;
 unsigned char far *vector;

 if ((vector=MK_FP(peek(0,0x33*4+2),peek(0,0x33*4)))==NULL) return 0;

 if (*vector == 207)
   return _MouseStatus = 0;

 _AX=0;
 geninterrupt(0x33);
 return _MouseStatus = 1;
}


//void sub_0_34C6(void)
void _MouseHide(void)
{
 if (!_MouseStatus) return;

 _AX=2;
 geninterrupt(0x33);
}



//void sub_0_34D9(void)
void _MouseShow(void)
{
 if (!_MouseStatus) return;

 _AX=1;
 geninterrupt(0x33);
}


//void sub_0_34EC(void)
int _MouseButton(void)
{
 union REGS regs;

 if (!_MouseStatus) return 0;

 regs.x.ax=3;
 int86(0x33,&regs,&regs);
 return(regs.x.bx);
}



//void sub_0_351D(void)
void _MouseCoords(int *x,int *y)
{
 union REGS regs;

 if (!_MouseStatus) return;

 regs.x.ax=3;
 int86(0x33,&regs,&regs);
 *x=regs.x.cx;
 *y=regs.x.dx;

 *x/=2;
}




////////////////////////////////////////////////////////////////////
//
// Verify a file's existence
//
////////////////////////////////////////////////////////////////////
//void sub_0_3563
long _Verify(char *filename)
{
 int handle;
 long size;

 if ((handle=open(filename,O_BINARY))==-1) return 0;
 size=filelength(handle);
 close(handle);
 return size;
}





////////////////////////////////////////////////////////////////////
//
// print hex byte
//
////////////////////////////////////////////////////////////////////
void sub_0_35A0(void)
{
asm {
db 83h, 0ECh, 14h, 16h, 8Dh, 46h, 0ECh, 50h, 1Eh, 0B8h, 12h, 0Ch, 50h
db 0B9h, 10h, 00h, 0E8h, 0ECh, 3Fh, 16h, 8Dh, 46h, 0FCh, 50h, 1Eh, 0B8h, 22h, 0Ch, 50h
db 0B9h, 02h, 00h, 0E8h, 0DCh, 3Fh, 0C7h, 46h, 0FEh, 00h, 00h, 0EBh, 2Ch, 8Ah, 46h, 04h
db 0B4h, 00h, 0B2h, 01h, 2Ah, 56h, 0FEh, 0D0h, 0E2h, 0D0h, 0E2h, 8Ah, 0CAh, 0D3h, 0F8h, 25h
db 0Fh, 00h, 8Dh, 56h, 0ECh, 03h, 0C2h, 8Bh, 0D8h, 8Ah, 07h, 88h, 46h, 0FCh, 8Dh, 46h
db 0FCh, 50h, 0E8h, 0FCh, 0FDh, 59h, 0FFh, 46h, 0FEh, 83h, 7Eh, 0FEh, 02h, 7Ch, 0CEh, 8Bh
db 0E5h
}
}




////////////////////////////////////////////////////////////////////
//
// print hex
//
////////////////////////////////////////////////////////////////////
void sub_0_3603(void)
{
asm {
db 0B8h, 65h, 0Ch, 50h, 0E8h, 0E4h, 0FDh, 59h, 8Bh, 46h
db 04h, 0B1h, 08h, 0D3h, 0E8h, 50h, 0E8h, 87h, 0FFh, 59h, 8Ah, 46h, 04h, 24h, 0FFh, 50h
db 0E8h, 7Dh, 0FFh, 59h
}
}




////////////////////////////////////////////////////////////////////
//
// print bin
//
////////////////////////////////////////////////////////////////////
void sub_0_3626(void)
{
asm {
db 4Ch, 4Ch, 0B8h, 67h, 0Ch, 50h, 0E8h
db 0BFh, 0FDh, 59h, 0C7h, 46h, 0FEh, 00h, 00h, 0EBh, 24h, 0B1h, 0Fh, 2Ah, 4Eh, 0FEh, 8Bh
db 46h, 04h, 0D3h, 0E8h, 0A9h, 01h, 00h, 74h, 0Ah, 0B8h, 69h, 0Ch, 50h, 0E8h, 0A1h, 0FDh
db 59h, 0EBh, 08h, 0B8h, 6Bh, 0Ch, 50h, 0E8h, 97h, 0FDh, 59h, 0FFh, 46h, 0FEh, 83h, 7Eh
db 0FEh, 10h, 7Ch, 0D6h, 8Bh, 0E5h
}
}




////////////////////////////////////////////////////////////////////
//
// center print
//
////////////////////////////////////////////////////////////////////
//void sub_0_3668(void)
void _printc(char *string)
{
	asm {
		push string
		call strlen
		pop cx
		mov bx, 2
		cwd
		idiv bx
		mov dx, screencenterx
		inc dx
		sub dx, ax
		mov sx, dx
		push string
		call print
		pop cx
	}
}




////////////////////////////////////////////////////////////////////
//
// input unsigned
//
////////////////////////////////////////////////////////////////////
void sub_0_368C(void)
{
asm {
db 83h
db 0ECh, 28h, 56h, 57h, 16h, 8Dh, 46h, 0E8h, 50h, 1Eh, 0B8h, 24h, 0Ch, 50h, 0B9h, 12h
db 00h, 0E8h, 0FEh, 3Eh, 16h, 8Dh, 46h, 0D8h, 50h, 1Eh, 0B8h, 36h, 0Ch, 50h, 0B9h, 10h
db 00h, 0E8h, 0EEh, 3Eh, 0B8h, 11h, 00h, 50h, 8Dh, 46h, 0E8h, 50h, 0E8h, 0F2h, 00h, 59h
db 59h, 80h, 7Eh, 0E8h, 24h, 75h, 69h, 8Dh, 46h, 0E8h, 50h, 0E8h, 57h, 39h, 59h, 05h
db 0FEh, 0FFh, 89h, 46h, 0FAh, 83h, 7Eh, 0FAh, 00h, 7Dh, 05h, 33h, 0C0h, 0E9h, 0CBh, 00h
db 33h, 0F6h, 0C7h, 46h, 0FCh, 00h, 00h, 0EBh, 3Dh, 8Bh, 5Eh, 0FCh, 43h, 8Dh, 46h, 0E8h
db 03h, 0D8h, 8Ah, 07h, 98h, 50h, 0E8h, 96h, 30h, 59h, 88h, 46h, 0FFh, 33h, 0FFh, 0EBh
db 1Dh, 8Ah, 43h, 0D8h, 3Ah, 46h, 0FFh, 75h, 14h, 8Ah, 46h, 0FAh, 2Ah, 46h, 0FCh, 0D0h
db 0E0h, 0D0h, 0E0h, 8Bh, 0D7h, 8Ah, 0C8h, 0D3h, 0E2h, 0Bh, 0F2h, 0EBh, 06h, 47h, 83h, 0FFh
db 10h, 72h, 0DEh, 0FFh, 46h, 0FCh, 8Bh, 46h, 0FCh, 3Bh, 46h, 0FAh, 76h, 0BBh, 0EBh, 77h
db 80h, 7Eh, 0E8h, 25h, 75h, 67h, 8Dh, 46h, 0E8h, 50h, 0E8h, 0E8h, 38h, 59h, 05h, 0FEh
db 0FFh, 89h, 46h, 0FAh, 83h, 7Eh, 0FAh, 00h, 7Dh, 04h, 33h, 0C0h, 0EBh, 5Dh, 33h, 0F6h
db 0C7h, 46h, 0FCh, 00h, 00h, 0EBh, 3Ch, 8Bh, 5Eh, 0FCh, 43h, 8Dh, 46h, 0E8h, 03h, 0D8h
db 80h, 3Fh, 30h, 7Ch, 0Eh, 8Bh, 5Eh, 0FCh, 43h, 8Dh, 46h, 0E8h, 03h, 0D8h, 80h, 3Fh
db 31h, 7Eh, 04h, 33h, 0C0h, 0EBh, 34h, 8Bh, 5Eh, 0FCh, 43h, 8Dh, 46h, 0E8h, 03h, 0D8h
db 8Ah, 07h, 98h, 05h, 0D0h, 0FFh, 8Ah, 4Eh, 0FAh, 2Ah, 4Eh, 0FCh, 0D3h, 0E0h, 0Bh, 0F0h
db 0FFh, 46h, 0FCh, 8Bh, 46h, 0FCh, 3Bh, 46h, 0FAh, 76h, 0BCh, 0EBh, 0Ah, 8Dh, 46h, 0E8h
db 50h, 0E8h, 97h, 31h, 59h, 8Bh, 0F0h, 8Bh, 0C6h, 0EBh, 00h, 5Fh, 5Eh, 8Bh, 0E5h
}
}




////////////////////////////////////////////////////////////////////
//
// line input routine
//
////////////////////////////////////////////////////////////////////
void sub_0_37B1(void)
{
asm {
db 4Ch, 4Ch, 56h, 57h, 33h, 0F6h, 0E8h, 0D3h, 0FBh, 25h, 0FFh, 00h
db 50h, 0E8h, 0CBh, 2Fh, 59h, 88h, 46h, 0FFh, 80h, 7Eh, 0FFh, 7Fh, 74h, 06h, 80h, 7Eh
db 0FFh, 08h, 75h, 1Bh, 0Bh, 0F6h, 7Eh, 17h, 4Eh, 0B8h, 20h, 00h, 50h, 0FFh, 36h, 68h
db 0AEh, 0FFh, 36h, 02h, 0AEh, 0E8h, 0B9h, 1Dh, 83h, 0C4h, 06h, 0FFh, 0Eh, 02h, 0AEh, 80h
db 7Eh, 0FFh, 20h, 7Ch, 2Bh, 80h, 7Eh, 0FFh, 7Ah, 7Fh, 25h, 3Bh, 76h, 06h, 7Dh, 20h
db 8Bh, 5Eh, 04h, 8Ah, 46h, 0FFh, 88h, 00h, 46h, 8Ah, 46h, 0FFh, 98h, 50h, 0FFh, 36h
db 68h, 0AEh, 0A1h, 02h, 0AEh, 0FFh, 06h, 02h, 0AEh, 50h, 0E8h, 84h, 1Dh, 83h, 0C4h, 06h
db 80h, 7Eh, 0FFh, 1Bh, 74h, 06h, 80h, 7Eh, 0FFh, 0Dh, 75h, 8Eh, 8Bh, 0FEh, 0EBh, 07h
db 8Bh, 5Eh, 04h, 0C6h, 01h, 00h, 47h, 3Bh, 7Eh, 06h, 7Ch, 0F4h, 80h, 7Eh, 0FFh, 0Dh
db 75h, 05h, 0B8h, 01h, 00h, 0EBh, 04h, 33h, 0C0h, 0EBh, 00h, 5Fh, 5Eh, 8Bh, 0E5h
}
}

/*========================================================================*/

/*
** Game routines
*/

struct scores scoreswap, highscores[5];

long score;
int level;
int _numlevels, _maxplayers;

char *_extension = "PCR";
boolean	_cgaok, _egaok, _vgaok;

////////////////////////
//
// _loadctrls
// Tries to load the control panel settings
// creates a default if not present
//
////////////////////////

void sub_0_3851(void)
{
asm {
db 56h, 0B8h, 71h, 0Ch, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 0A1h, 37h
db 59h, 59h, 0FFh, 36h, 46h, 0Ch, 0B8h, 10h, 0AEh, 50h, 0E8h, 5Bh, 37h, 59h, 59h, 0B8h
db 80h, 01h, 50h, 0B8h, 01h, 80h, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 0B9h, 34h, 83h, 0C4h
db 06h, 8Bh, 0F0h, 3Dh, 0FFh, 0FFh, 74h, 03h, 0EBh, 78h, 90h, 0C7h, 06h, 9Ch, 0AFh, 03h
db 00h, 0C7h, 06h, 0D2h, 10h, 01h, 00h, 0C7h, 06h, 06h, 0Ch, 00h, 00h, 0C7h, 06h, 08h
db 0Ch, 02h, 00h, 0B8h, 14h, 00h, 0A3h, 0EEh, 0ADh, 0A3h, 0ECh, 0ADh, 0B8h, 3Ch, 00h, 0A3h
db 0F4h, 0ADh, 0A3h, 0F2h, 0ADh, 0B8h, 14h, 00h, 0A3h, 0FAh, 0ADh, 0A3h, 0F8h, 0ADh, 0B8h, 3Ch
db 00h, 0A3h, 00h, 0AEh, 0A3h, 0FEh, 0ADh, 0C7h, 06h, 9Ah, 0AFh, 05h, 00h, 0C6h, 06h, 60h
db 0AEh, 48h, 0C6h, 06h, 61h, 0AEh, 49h, 0C6h, 06h, 62h, 0AEh, 4Dh, 0C6h, 06h, 63h, 0AEh
db 51h, 0C6h, 06h, 64h, 0AEh, 50h, 0C6h, 06h, 65h, 0AEh, 4Fh, 0C6h, 06h, 66h, 0AEh, 4Bh
db 0C6h, 06h, 67h, 0AEh, 47h, 0C6h, 06h, 04h, 0AEh, 1Dh, 0C6h, 06h, 05h, 0AEh, 38h, 0E9h
db 0AAh, 00h, 0B8h, 02h, 00h, 50h, 0B8h, 9Ch, 0AFh, 50h, 56h, 0E8h, 0BAh, 35h, 83h, 0C4h
db 06h, 0B8h, 02h, 00h, 50h, 0B8h, 0D2h, 10h, 50h, 56h, 0E8h, 0ABh, 35h, 83h, 0C4h, 06h
db 0B8h, 06h, 00h, 50h, 0B8h, 04h, 0Ch, 50h, 56h, 0E8h, 9Ch, 35h, 83h, 0C4h, 06h, 0B8h
db 06h, 00h, 50h, 0B8h, 0EAh, 0ADh, 50h, 56h, 0E8h, 8Dh, 35h, 83h, 0C4h, 06h, 0B8h, 06h
db 00h, 50h, 0B8h, 0F6h, 0ADh, 50h, 56h, 0E8h, 7Eh, 35h, 83h, 0C4h, 06h, 0B8h, 06h, 00h
db 50h, 0B8h, 0F0h, 0ADh, 50h, 56h, 0E8h, 6Fh, 35h, 83h, 0C4h, 06h, 0B8h, 06h, 00h, 50h
db 0B8h, 0FCh, 0ADh, 50h, 56h, 0E8h, 60h, 35h, 83h, 0C4h, 06h, 0B8h, 02h, 00h, 50h, 0B8h
db 9Ah, 0AFh, 50h, 56h, 0E8h, 51h, 35h, 83h, 0C4h, 06h, 0B8h, 08h, 00h, 50h, 0B8h, 60h
db 0AEh, 50h, 56h, 0E8h, 42h, 35h, 83h, 0C4h, 06h, 0B8h, 01h, 00h, 50h, 0B8h, 04h, 0AEh
db 50h, 56h, 0E8h, 33h, 35h, 83h, 0C4h, 06h, 0B8h, 01h, 00h, 50h, 0B8h, 05h, 0AEh, 50h
db 56h, 0E8h, 24h, 35h, 83h, 0C4h, 06h, 56h, 0E8h, 89h, 26h, 59h, 5Eh
}
}
//void sub_0_39AF(void)
void _savectrls (void)
{
asm {
db 56h, 0B8h, 7Bh, 0Ch, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 43h, 36h, 59h, 59h
db 0FFh, 36h, 46h, 0Ch, 0B8h, 10h, 0AEh, 50h, 0E8h, 0FDh, 35h, 59h, 59h, 0B8h, 80h, 01h
db 50h, 0B8h, 02h, 03h, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 5Bh, 33h, 83h, 0C4h, 06h, 8Bh
db 0F0h, 3Dh, 0FFh, 0FFh, 75h, 03h, 0E9h, 0AAh, 00h, 0B8h, 02h, 00h, 50h, 0B8h, 9Ch, 0AFh
db 50h, 56h, 0E8h, 0C6h, 2Dh, 83h, 0C4h, 06h, 0B8h, 02h, 00h, 50h, 0B8h, 0D2h, 10h, 50h
db 56h, 0E8h, 0B7h, 2Dh, 83h, 0C4h, 06h, 0B8h, 06h, 00h, 50h, 0B8h, 04h, 0Ch, 50h, 56h
db 0E8h, 0A8h, 2Dh, 83h, 0C4h, 06h, 0B8h, 06h, 00h, 50h, 0B8h, 0EAh, 0ADh, 50h, 56h, 0E8h
db 99h, 2Dh, 83h, 0C4h, 06h, 0B8h, 06h, 00h, 50h, 0B8h, 0F6h, 0ADh, 50h, 56h, 0E8h, 8Ah
db 2Dh, 83h, 0C4h, 06h, 0B8h, 06h, 00h, 50h, 0B8h, 0F0h, 0ADh, 50h, 56h, 0E8h, 7Bh, 2Dh
db 83h, 0C4h, 06h, 0B8h, 06h, 00h, 50h, 0B8h, 0FCh, 0ADh, 50h, 56h, 0E8h, 6Ch, 2Dh, 83h
db 0C4h, 06h, 0B8h, 02h, 00h, 50h, 0B8h, 9Ah, 0AFh, 50h, 56h, 0E8h, 5Dh, 2Dh, 83h, 0C4h
db 06h, 0B8h, 08h, 00h, 50h, 0B8h, 60h, 0AEh, 50h, 56h, 0E8h, 4Eh, 2Dh, 83h, 0C4h, 06h
db 0B8h, 01h, 00h, 50h, 0B8h, 04h, 0AEh, 50h, 56h, 0E8h, 3Fh, 2Dh, 83h, 0C4h, 06h, 0B8h
db 01h, 00h, 50h, 0B8h, 05h, 0AEh, 50h, 56h, 0E8h, 30h, 2Dh, 83h, 0C4h, 06h, 56h, 0E8h
db 0A2h, 25h, 59h, 5Eh
}
}


////////////////////////
//
// loadhighscores
// Tries to load the score file
// creates a default if not present
//
////////////////////////
void sub_0_3A96(void)
{
asm {
db 56h, 0B8h, 85h, 0Ch, 50h, 0B8h, 10h
db 0AEh, 50h, 0E8h, 5Ch, 35h, 59h, 59h, 0FFh, 36h, 46h, 0Ch, 0B8h, 10h, 0AEh, 50h, 0E8h
db 16h, 35h, 59h, 59h, 1Eh, 0B8h, 26h, 0C3h, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 17h, 0F3h
db 83h, 0C4h, 06h, 0Bh, 0C2h, 75h, 42h, 33h, 0F6h, 0EBh, 39h, 8Bh, 0C6h, 0BAh, 0Ah, 00h
db 0F7h, 0EAh, 8Bh, 0D8h, 0C7h, 87h, 26h, 0C3h, 64h, 00h, 0C7h, 87h, 28h, 0C3h, 00h, 00h
db 8Bh, 0C6h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Bh, 0D8h, 0C7h, 87h, 2Ah, 0C3h, 01h, 00h, 0B8h
db 8Dh, 0Ch, 50h, 8Bh, 0C6h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 05h, 2Ch, 0C3h, 50h, 0E8h, 00h
db 35h, 59h, 59h, 46h, 83h, 0FEh, 05h, 7Ch, 0C2h, 5Eh
}
}

//void sub_0_3B0C(void)
void _savehighscores (void)
{
  strcpy (str,"SCORES.");
  strcat (str,_extension);
  SaveFile(str,(char huge *)highscores,sizeof (highscores));
}


////////////////////////
//
// _showhighscores
// Brings up a dialog box with the high score lists and returns immediately
//
////////////////////////
void sub_0_3B41(void)
{
asm {
db 83h, 0ECh, 04h, 56h, 0B8h, 0Fh, 00h, 50h, 0B8h, 11h, 00h, 50h
db 0E8h, 96h, 0F7h, 59h, 59h, 0B8h, 99h, 0Ch, 50h, 0E8h, 95h, 0F8h, 59h, 0B8h, 0ABh, 0Ch
db 50h, 0E8h, 8Dh, 0F8h, 59h, 0B8h, 0BDh, 0Ch, 50h, 0E8h, 85h, 0F8h, 59h, 33h, 0F6h, 0E9h
db 04h, 01h, 0FFh, 06h, 02h, 0AEh, 8Bh, 0C6h, 05h, 31h, 00h, 50h, 0FFh, 36h, 68h, 0AEh
db 0FFh, 36h, 02h, 0AEh, 0E8h, 1Ah, 1Ah, 83h, 0C4h, 06h, 83h, 06h, 02h, 0AEh, 02h, 8Bh
db 0C6h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Bh, 0D8h, 8Bh, 87h, 28h, 0C3h, 8Bh, 97h, 26h, 0C3h
db 89h, 56h, 0FCh, 89h, 46h, 0FEh, 83h, 7Eh, 0FEh, 01h, 7Fh, 0Dh, 7Ch, 07h, 81h, 7Eh
db 0FCh, 0A0h, 86h, 73h, 04h, 0FFh, 06h, 02h, 0AEh, 83h, 7Eh, 0FEh, 00h, 7Fh, 0Dh, 7Ch
db 07h, 81h, 7Eh, 0FCh, 10h, 27h, 73h, 04h, 0FFh, 06h, 02h, 0AEh, 83h, 7Eh, 0FEh, 00h
db 7Fh, 0Dh, 7Ch, 07h, 81h, 7Eh, 0FCh, 0E8h, 03h, 73h, 04h, 0FFh, 06h, 02h, 0AEh, 83h
db 7Eh, 0FEh, 00h, 7Fh, 0Ch, 7Ch, 06h, 83h, 7Eh, 0FCh, 64h, 73h, 04h, 0FFh, 06h, 02h
db 0AEh, 83h, 7Eh, 0FEh, 00h, 7Fh, 0Ch, 7Ch, 06h, 83h, 7Eh, 0FCh, 0Ah, 73h, 04h, 0FFh
db 06h, 02h, 0AEh, 0B8h, 0Ah, 00h, 50h, 0B8h, 10h, 0AEh, 50h, 0FFh, 76h, 0FEh, 0FFh, 76h
db 0FCh, 0E8h, 90h, 30h, 83h, 0C4h, 08h, 0B8h, 10h, 0AEh, 50h, 0E8h, 0D3h, 0F7h, 59h, 0FFh
db 06h, 02h, 0AEh, 8Bh, 0C6h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Bh, 0D8h, 83h, 0BFh, 2Ah, 0C3h
db 0Ah, 7Dh, 04h, 0FFh, 06h, 02h, 0AEh, 0B8h, 0Ah, 00h, 50h, 0B8h, 10h, 0AEh, 50h, 8Bh
db 0C6h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Bh, 0D8h, 0FFh, 0B7h, 2Ah, 0C3h, 0E8h, 14h, 30h, 83h
db 0C4h, 06h, 0B8h, 10h, 0AEh, 50h, 0E8h, 98h, 0F7h, 59h, 0FFh, 06h, 02h, 0AEh, 8Bh, 0C6h
db 0BAh, 0Ah, 00h, 0F7h, 0EAh, 05h, 2Ch, 0C3h, 50h, 0E8h, 85h, 0F7h, 59h, 0B8h, 0CFh, 0Ch
db 50h, 0E8h, 7Dh, 0F7h, 59h, 46h, 83h, 0FEh, 05h, 7Dh, 03h, 0E9h, 0F4h, 0FEh, 5Eh, 8Bh
db 0E5h
}
}


//////////////////////////
//
// _checkhighscore
// Compares score to highscores, and inserts place if needed.
// calls showhighscores in any case
//
//////////////////////////
void sub_0_3C83(void)
{
asm {
db 4Ch, 4Ch, 56h, 57h, 33h, 0FFh, 0E9h, 91h, 00h, 8Bh
db 0C7h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Bh, 0D8h, 8Bh, 87h, 28h, 0C3h, 8Bh, 97h, 26h, 0C3h
db 3Bh, 06h, 0E4h, 0ADh, 7Eh, 03h, 0EBh, 77h, 90h, 7Ch, 06h, 3Bh, 16h, 0E2h, 0ADh, 73h
db 6Eh, 0BEh, 04h, 00h, 0EBh, 26h, 8Bh, 0C6h, 48h, 89h, 46h, 0FEh, 8Bh, 0C6h, 0BAh, 0Ah
db 00h, 0F7h, 0EAh, 05h, 26h, 0C3h, 1Eh, 50h, 8Bh, 46h, 0FEh, 0BAh, 0Ah, 00h, 0F7h, 0EAh
db 05h, 26h, 0C3h, 1Eh, 50h, 0B9h, 0Ah, 00h, 0E8h, 0C7h, 38h, 4Eh, 3Bh, 0FEh, 7Ch, 0D6h
db 8Bh, 0C7h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Bh, 16h, 0E4h, 0ADh, 8Bh, 1Eh, 0E2h, 0ADh, 93h
db 89h, 87h, 26h, 0C3h, 89h, 97h, 28h, 0C3h, 8Bh, 0C7h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Bh
db 16h, 6Eh, 0AEh, 8Bh, 0D8h, 89h, 97h, 2Ah, 0C3h, 0B8h, 0D2h, 0Ch, 50h, 8Bh, 0C7h, 0BAh
db 0Ah, 00h, 0F7h, 0EAh, 05h, 2Ch, 0C3h, 50h, 0E8h, 0E6h, 32h, 59h, 59h, 0EBh, 09h, 47h
db 83h, 0FFh, 05h, 7Dh, 03h, 0E9h, 67h, 0FFh, 0E8h, 16h, 0FEh, 83h, 0FFh, 05h, 7Ch, 03h
db 0E9h, 81h, 00h, 0B8h, 01h, 00h, 50h, 0E8h, 0CCh, 15h, 59h, 0E8h, 01h, 0F0h, 0A1h, 0Eh
db 0Ch, 05h, 06h, 00h, 0A3h, 02h, 0AEh, 8Bh, 0C7h, 0D1h, 0E0h, 8Bh, 16h, 10h, 0Ch, 03h
db 0D0h, 4Ah, 89h, 16h, 68h, 0AEh, 33h, 0F6h, 0E8h, 35h, 0F6h, 89h, 46h, 0FEh, 0A2h, 68h
db 0C3h, 80h, 3Eh, 68h, 0C3h, 20h, 7Ch, 2Eh, 83h, 0FEh, 03h, 7Dh, 29h, 0A0h, 68h, 0C3h
db 98h, 50h, 0FFh, 36h, 68h, 0AEh, 0FFh, 36h, 02h, 0AEh, 0E8h, 24h, 18h, 83h, 0C4h, 06h
db 0FFh, 06h, 02h, 0AEh, 8Bh, 0C7h, 0BAh, 0Ah, 00h, 0F7h, 0EAh, 8Ah, 16h, 68h, 0C3h, 8Bh
db 0D8h, 88h, 90h, 2Ch, 0C3h, 46h, 80h, 3Eh, 68h, 0C3h, 08h, 74h, 07h, 81h, 7Eh, 0FEh
db 00h, 4Bh, 75h, 09h, 0Bh, 0F6h, 7Eh, 05h, 0FFh, 0Eh, 02h, 0AEh, 4Eh, 80h, 3Eh, 68h
db 0C3h, 0Dh, 75h, 0A4h, 5Fh, 5Eh, 8Bh, 0E5h
}
}


////////////////////
//
// _setupgame
//
////////////////////
//void sub_0_3DBA(void)
void _setupgame (void)
{
asm {
db 0E8h, 9Bh, 1Dh
db 0A3h, 0E8h, 0ADh, 0C7h, 06h, 9Ch, 0AFh, 00h, 00h, 83h, 3Eh, 0E8h, 0ADh, 05h, 74h, 0Dh
db 83h, 3Eh, 0E8h, 0ADh, 04h, 74h, 06h, 0C7h, 06h, 5Eh, 0C3h, 00h, 00h, 83h, 3Eh, 0E8h
db 0ADh, 03h, 74h, 0Dh, 83h, 3Eh, 0E8h, 0ADh, 05h, 74h, 06h, 0C7h, 06h, 84h, 0AEh, 00h
db 00h, 0E8h, 5Dh, 0FAh, 83h, 3Eh, 9Ch, 0AFh, 03h, 75h, 0Fh, 83h, 3Eh, 5Eh, 0C3h, 00h
db 74h, 08h, 0C7h, 06h, 9Ch, 0AFh, 03h, 00h, 0EBh, 1Ch, 83h, 3Eh, 9Ch, 0AFh, 02h, 7Ch
db 0Fh, 83h, 3Eh, 84h, 0AEh, 00h, 74h, 08h, 0C7h, 06h, 9Ch, 0AFh, 02h, 00h, 0EBh, 06h
db 0C7h, 06h, 9Ch, 0AFh, 01h, 00h, 0B8h, 0D6h, 0Ch, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 0D0h
db 31h, 59h, 59h, 0FFh, 36h, 46h, 0Ch, 0B8h, 10h, 0AEh, 50h, 0E8h, 8Ah, 31h, 59h, 59h
db 0B8h, 10h, 0AEh, 50h, 0E8h, 0BEh, 0F0h, 59h, 0A3h, 0CEh, 10h, 89h, 16h, 0D0h, 10h, 0E8h
db 3Ah, 14h, 0E8h, 15h, 0E7h, 0B8h, 01h, 00h, 50h, 0E8h, 0C5h, 16h, 59h, 0E8h, 36h, 0FCh
db 0E8h, 0EFh, 0C4h
}
}


////////////////////
//
// _quit
//
////////////////////
//void sub_0_3E65(void)
void _quit (char *error)
{
  setscreenmode (text);
  if (!(*error))
  {
	 _savehighscores ();
	 _savectrls ();
  }
  else
  {
	puts (error);
  }

  ShutdownKBD ();	// shut down the interrupt driven stuff if needed
  ShutdownSound ();

  exit (0);		// quit to DOS
}


