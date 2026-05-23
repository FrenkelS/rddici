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
//void sub_0_3E97(void)
void calibratejoy (int joynum)
{
  int stage,dx,dy,xl,yl,xh,yh;
  ControlStruct ctr;

  expwin (24,9);

  print(" Joystick Configuration\n\r");
  print(" ----------------------\n\r");
  print("Hold the joystick in the\n\r");
  print("upper left\n\r");
  print("corner and hit fire:");
  stage=15;
  do				// wait for a button press
  {
    drawchar (sx,sy,stage);
    WaitVBL ();
    WaitVBL ();
    WaitVBL ();
    if (++stage==23)
      stage=15;
    ReadJoystick (joynum,&xl,&yl);
    ctr = ControlJoystick(joynum);
    if (keydown[1])
      goto done;
  } while (ctr.button1!= 1);
  drawchar (sx,sy,' ');
  do                  		// wait for the button release
  {
    ctr = ControlJoystick(joynum);
  } while (ctr.button1);
  WaitVBL ();
  WaitVBL ();			// so the button can't bounce

  print("\n\n\rHold the joystick in the\n\r");
  print("lower right\n\r");
  print("corner and hit fire:");
  do				// wait for a button press
  {
    drawchar (sx,sy,stage);
    WaitVBL ();
    WaitVBL ();
    WaitVBL ();
    if (++stage==23)
      stage=15;
    ReadJoystick (joynum,&xh,&yh);
    ctr = ControlJoystick(joynum);
    if (keydown[1])
      goto done;
  } while (ctr.button1!= 1);
  drawchar (sx,sy,' ');
  do                  		// wait for the button release
  {
    ctr = ControlJoystick(joynum);
  } while (ctr.button1);

  //
  // figure out good boundaries
  //

  dx=(xh-xl) / 4;
  dy=(yh-yl) / 4;
  JoyXlow[joynum]=xl+dx;
  JoyXhigh[joynum]=xh-dx;
  JoyYlow[joynum]=yl+dy;
  JoyYhigh[joynum]=yh-dy;

done:
  clearkeys ();
  erasewindow ();
}


////////////////////////////
//
// calibratemouse
//
////////////////////////////
//void sub_0_407E(void)
void calibratemouse (void)
{
  char ch;

  expwin (24,5);
  print ("  Mouse Configuration   \n\r");
  print ("  -------------------   \n\r");
  print ("Choose the sensitivity  \n\r");
  print ("of the mouse, 1 being   \n\r");
  print ("slow, 9 being fast:");
  do
  {  ch=get() % 256;
    if (ch==27)
      ch='5';
  } while (ch<'1' || ch>'9');
  MouseSensitivity = 15-(ch-'0');
  erasewindow ();
  _AX=0;
  geninterrupt (0x33);		// initialize the mouse
  _AX = 4;
  _CX=320;
  _DX=100;
  geninterrupt (0x33);		// set mouse status


}

/////////////////////////////
//
// print a representation of the scan code key
//
////////////////////////////
//void sub_0_4100(void)
void printscan (int sc)
{
 char static chartable[128] =
 {'?','?','1','2','3','4','5','6','7','8','9','0','-','+','?','?',
  'Q','W','E','R','T','Y','U','I','O','P','[',']','|','?','A','S',
  'D','F','G','H','J','K','L',';','"','?','?','?','Z','X','C','V',
  'B','N','M',',','.','/','?','?','?','?','?','?','?','?','?','?',
  '?','?','?','?','?','?','?','?', 15,'?','-', 21,'5', 17,'+','?',
   19,'?','?','?','?','?','?','?','?','?','?','?','?','?','?','?',
  '?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?',
  '?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?'};

 sc = sc & 0x7f;

 if (sc==1)
   print ("ESC");
 else if (sc==0xe)
   print ("BKSP");
 else if (sc==0xf)
   print ("TAB");
 else if (sc==0x1d)
   print ("CTRL");
 else if (sc==0x2A)
   print ("LSHIFT");
 else if (sc==0x39)
   print ("SPACE");
 else if (sc==0x3A)
   print ("CAPSLK");
 else if (sc>=0x3b && sc<=0x44)
 {
   char str[3];
   print ("F");
   itoa (sc-0x3a,str,10);
   print (str);
 }
 else if (sc==0x57)
   print ("F11");
 else if (sc==0x59)
   print ("F12");
 else if (sc==0x46)
   print ("SCRLLK");
 else if (sc==0x1c)
   print ("ENTER");
 else if (sc==0x36)
   print ("RSHIFT");
 else if (sc==0x37)
   print ("PRTSC");
 else if (sc==0x38)
   print ("ALT");
 else if (sc==0x47)
   print ("HOME");
 else if (sc==0x49)
   print ("PGUP");
 else if (sc==0x4f)
   print ("END");
 else if (sc==0x51)
   print ("PGDN");
 else if (sc==0x52)
   print ("INS");
 else if (sc==0x53)
   print ("DEL");
 else if (sc==0x45)
   print ("NUMLK");
 else
   drawchar (sx++,sy,chartable[sc]);
}

/////////////////////////////
//
// calibratekeys
//
////////////////////////////
//void sub_0_42A8(void)
void calibratekeys (void)
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
//void sub_0_4461(void)
void getconfig (void)
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
//void sub_0_4547(void)
void drawpanel (void)
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
//void sub_0_47FC(void)
void controlpanel (void)
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

//void sub_0_4AD6(void)
void installgrfile (char *filename, int unpack,void huge *inmem)
{
  int i;
  unsigned long a,b,c,d;
  typedef pictype ptype[NUMPICS];
  typedef spritetype stype[NUMSPRITES];

  typedef struct {void huge *charptr;
		  void huge *tileptr;
		  void huge *picptr;
		  void huge *spriteptr;
		  ptype huge *pictableptr;
		  stype huge *spritetableptr;
		  void huge *plane[4];
		  int numchars,numtiles,numpics,numsprites;
		 } picfiletype;

  picfiletype huge *picfile;

  stype huge *(spriteinfile);
  ptype huge *(picinfile);


	{
	 if ( (long)lastgrpic )
	   farfree ((void far *)lastgrpic); // so new graphics modes will free it up

	 if (unpack)
	   picfile = (picfiletype huge *) bloadinLZW (filename);
	 else
	   picfile = (picfiletype huge *) bloadin (filename);

	 lastgrpic = (void far *) lastparalloc;
	}

  numchars = picfile->numchars;
  numtiles = picfile->numtiles;
  numpics = picfile->numpics;
  numsprites = picfile->numsprites;


  if (grmode==EGAgr)		// EGA is special because of bit plane grief!
  {
    charptr = MK_FP(0xa900,0);
    tileptr = MK_FP(0xa900+FP_SEG(picfile->tileptr)-FP_SEG(picfile->charptr),0);
    picptr = MK_FP(0xa900+FP_SEG(picfile->picptr)-FP_SEG(picfile->charptr),0);
    spriteptr = MK_FP(0xa900+FP_SEG(picfile->spriteptr)-FP_SEG(picfile->charptr),0);

    for (i=0;i<4;i++)
    {
      egaplane[i] = FP_SEG(picfile->plane[i])+FP_SEG(picfile);
      egaspriteptr[i] = MK_FP(FP_SEG(picfile)+FP_SEG(picfile->plane[i]) +
	FP_SEG(picfile->spriteptr) - FP_SEG(picfile->charptr),0);
    }
  }
  else
  {
    charptr = MK_FP(FP_SEG(picfile)+FP_SEG(picfile->charptr),0);
    tileptr = MK_FP(FP_SEG(picfile)+FP_SEG(picfile->tileptr),0);
    picptr = MK_FP(FP_SEG(picfile)+FP_SEG(picfile->picptr),0);
    spriteptr = MK_FP(FP_SEG(picfile)+FP_SEG(picfile->spriteptr),0);
  }
  //
  // copy tables into data segment
  //
  picinfile = MK_FP(FP_SEG(picfile->pictableptr)+FP_SEG(picfile)
    ,FP_OFF(picfile->pictableptr)+FP_OFF(picfile));
  spriteinfile = MK_FP(FP_SEG(picfile->spritetableptr)+FP_SEG(picfile)
    ,FP_OFF(picfile->spritetableptr)+FP_OFF(picfile));
  for (i=0; i<NUMPICS; i++)
    pictable[i] = (*picinfile)[i];
  for (i=0; i<NUMSPRITES; i++)
    spritetable[i] = (*spriteinfile)[i];

}




/*=========================================================================*/

//////////////////////////
//
// moveega
// moves the standard stuff into EGA memory
// needs to be called after each setgrmode to ega to refill memory
//
//////////////////////////
//void sub_0_4CEF(void)
void moveega (void)
{
  int plane;

  for (plane=0;plane<4;plane++)
  {
    outportb (SCindex,SCmapmask);
    outportb (SCindex+1,1<<plane);	// write plane #

    movedata (egaplane[plane],0,0xa900,0,0xffff-0xa900);
  }
  outportb (SCindex,SCmapmask);		// read map select
  outportb (SCindex+1,15);	// all planes
}

