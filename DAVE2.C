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

/*
** Dangerous Dave in Copyright Infringement
*/

#include "pcrlib.h"
#include "NGRABDD2.H"
#include "SOUNDS.H"

#define NUMDEMOS 1

//#define maxpics 2047
//#define numtiles 24*24   /*number of tiles displayed on screen*/
//#define numlevels 1
//#define maxobj 200           /*maximum possible active objects*/
//#define solidwall 129
//#define blankfloor 128
//#define leftoff 11
//#define topoff 11
//#define tile2s 256          /*tile number where the 2*2 pictures start*/
//#define tile3s tile2s+67*4
//#define tile4s tile3s+35*9
//#define tile5s tile4s+19*16
//#define lasttile tile5s+19*25

//typedef enum {playercmd,gargcmd,dragoncmd,ramstraight,ramdiag,straight,idle,
//    fade,explode,gunthinke,gunthinks} thinktype;

//typedef enum {benign,monster,pshot,mshot,nukeshot} tagtype;

typedef enum {nothing,player,goblin,skeleton,ogre,gargoyle,dragon,turbogre,
    wallhit,shot,bigshot,rock,dead1,dead2,dead3,dead4,dead5,dead6,teleporter,
    torch,secretgate,gune,guns,lastclass} classtype;

typedef enum {ingame,intitle,in_TODO,inscores} statetype;


//typedef struct {
//  boolean active;	/*if false, the object has not seen the player yet*/
//  classtype  class;
//  byte  x,y,		/*location of upper left corner in world*/
//    stage,		/*animation frame being drawn*/
//    delay;		/*number of frames to pause without doing anything*/
//  dirtype  dir;		/*direction facing*/
//  char hp;		/*hit points*/
//  byte oldx,oldy;	/*position where it was last drawn*/
//  int oldtile;		/*origin tile when last drawn*/
//  char filler[1];	/*pad to 16 bytes*/
//   } activeobj;

//typedef struct {	/*holds a copy of activeobj, and its class info*/
//  boolean  active;	/*if false, the object has not seen the player yet*/
//  classtype  class;
//  byte  x,y,		/*location of upper left corner in world*/
//    stage,		/*animation frame being drawn*/
//    delay;		/*number of frames to pause without doing anything*/
//  dirtype  dir;		/*direction facing*/
//  char hp;		/*hit points*/
//  byte oldx,oldy;		/*position where it was last drawn*/
//  int oldtile;		/*origin tile when last drawn*/
//  char filler[1];	/*pad to 16 bytes*/
//
//  byte think;
//  byte contact;
//  byte solid;
//  word  firstchar;
//  byte  size;
//  byte  stages;
//  byte  dirmask;
//  word  speed;
//  byte  hitpoints;
//  byte  damage;
//  word  points;
//  char filler2[2];	/*pad to 32 bytes*/
//  } objdesc;


typedef struct {
  char      unk0[0x16];
  classtype class;
  char      unk18[0x04];
  int       x1;
  int       y1;
  int       x2;
  int       y2;
  char      unk24[0x0C];
  void      (*think)();
} objtype;


typedef struct {
  long      dword_789_94D6; /* +00 */
  long      dword_789_94DA; /* +04 */
  int       word_789_94DE;  /* +08 */
  int       word_789_94E0;  /* +0A */
  int       word_789_94E2;  /* +0C */
  int       word_789_94E4;  /* +0E */
  int       word_789_94E6;  /* +10 */
  int       word_789_94E8;  /* +12 */
  int       word_789_94EA;  /* +14 */
  int       gamexit;        /* +16 */ /* enum {quited,killed,reseted,victorious} */ /*determines what to do after playloop*/
  int       word_789_94EE;  /* +18 */
  int       word_789_94F0;  /* +1A */
  int       word_789_94F2;  /* +1C */
  int       word_789_94F4;  /* +1E */
  int       word_789_94F6;  /* +20 */
  int       word_789_94F8;  /* +22 */
} type94D6;


/*=================*/
/*		   */
/* typed constants */
/*     		   */
/*=================*/


/*==================*/
/*		    */
/* global variables */
/*		    */
/*==================*/

const int word_789_238[78] = {
0,1,2,3,4,5,6,7,8,9,
0x0A,0x0B,0x0C,0x0E,0x0F,
0x10,
0x0D,
0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3D,0x3E,0x3F,0x3C,
0x41,0x42,0x43,0x40,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D
};

int word_789_4A2;
int word_789_4A4;
int word_789_154C;
int word_789_154E;
int word_789_1550;
int word_789_1552;
int word_789_1556;
int word_789_1D28;
int word_789_1D2A;
int word_789_1D2C;
int word_789_1D2E;
int word_789_1D30;
int word_789_1D32;
int word_789_1D34;
int word_789_1D36;
char unk_789_1D40[15890];
int word_789_7B20[600];
objtype *objptr_789_7FD0;
int word_789_8220;
int word_789_8226;
type94D6 *word_789_8228;
int word_789_8476;
int word_789_8478;
boolean bool_789_847A;
int word_789_847C_TODO;
int word_789_848E_TODO;
int word_789_947C;
int word_789_947E;
int word_789_9480;
int word_789_9482;
int word_789_9486[4];
boolean bool_789_9490;
ControlStruct ctrl_789_9492;
int word_789_949E;
int word_789_94A0;
int word_789_94A2[12];
int word_789_94BA;
int word_789_94BC;
boolean bool_789_94C4;
int word_789_94C6_TODO;
char *word_789_94C8;
int word_789_94CA;
long dword_789_94CC;
long dword_789_94D0;
int word_789_94D4;
type94D6 type94D6_789_94D6;
int word_789_94FE;
int word_789_9500;
void (*func_789_9506)();
long dword_789_ADD6;
long dword_789_ADDA;


//  int oldtiles [numtiles];		/*tile displayed last refresh*/
//  int background[87][86];		/*base map*/
//  int view[87][86];			/*base map with objects drawn in*/
//  int originx, originy;			/*current world location of ul corn*/
//  byte priority [maxpics+1];		/*tile draw overlap priorities*/

//  int items[6],saveitems[6];
//  int shotpower;			/*0-13 characters in power meter*/
//  int side;	                        /*which side shots come from*/
//  int boltsleft;			/*number of shots left in a bolt*/

//  activeobj o[maxobj+1],saveo[1];	/*everything that moves is here*/
//  objdesc obj , altobj;			/*total info about objecton and alt*/
//  int altnum;				/*o[#] of altobj*/
//  int numobj,objecton;			/*number of objects in o now*/

//  struct {
//    byte think;			/*some of these sizes are for the*/
//    byte contact;			/*convenience of the assembly routines*/
//    byte solid;
//    word firstchar;
//    byte size;
//    byte stages;
//    byte dirmask;
//    word speed;
//    byte hitpoints;
//    byte damage;
//    word points;
//    byte filler[2];
//  } objdef [lastclass];


//  int i,j,k,x,y,z;
  int lives;
  boolean leveldone;

//  boolean tempb;
//  char far *tempp;

//  int chkx,chky,chkspot;		/*spot being checked by walk*/

//  word frameon;
//  char far *grmem;
//  classtype clvar;

  int VGAPAL;				// just to make pcrlib happy

  boolean exitdemo,resetgame;
  statetype gamestate;

  ControlStruct ctrl;

//  char far *pics, far *picsexact;

//  long savescore;


objtype *new;
int lastobj;

#define MAXOBJECTS	60
objtype objlist[MAXOBJECTS];


#define PORTTILESWIDE 21
#define PORTTILESHIGH 14
#define BIGPORTSIZE (PORTTILESHIGH*PORTTILESWIDE)


int drawoffs1[BIGPORTSIZE], drawoffs0[BIGPORTSIZE];

unsigned int drawpage;

long lastExtraScore;


/****************************************************************************/

//////////////////////////////////
//
// function prototypes
//
//////////////////////////////////

//void extern drawobj (void);
void extern RF_ForceRefresh (void);
//void extern eraseobj (void);
//void extern doall (void);
//void extern egamove (void);
//void extern cgarefresh (void);
//void extern egarefresh (void);
void extern sub_0_1E46 (void);
void sub_0_290 (void);
void RF_Refresh (void);
void dofkeys (void);
void help (void);
void playloop(void);



/*==============================*/
/*			        */
/* xxxrefresh                   */
/* refresh the changed areas of */
/* the tiles map in the various */
/* graphics modes.              */
/*			        */
/*==============================*/

void sub_0_239(void)
{
  RF_ForceRefresh();
  if (gamestate == ingame || gamestate == inscores)
  {
    RF_Refresh();
    sub_0_290();
    RF_Refresh();
  }

  if (gamestate == intitle)
    drawpic (0,0,TITLEPIC);

  if (gamestate == in_TODO)
    drawpic (0,0,15);

  if (gamestate == inscores)
    _showhighscores();
}


void sub_0_290(void)
{
  if (grmode == CGAgr)
  {
    if (_videocard == EGAcard)
    {
      outportb(0x3d4, 0x12);
      outportb(0x3d5, 0xb0);
    }
    else if (_videocard == VGAcard)
    {
      outportb(0x3d4, 0x12);
      outportb(0x3d5, 0x60);
    }
    else
    {
      outportb(0x3d4, 0x06);
      outportb(0x3d5, 0x58);
    }
  }
  if (grmode == EGAgr)
  {
    outportb(crtcaddr, 0x13);
    outportb(crtcaddr + 1, 0x18);
	_AH = 0x10;
    _AL = 1;
    _BH = 5;
    geninterrupt(0x10);
  }
}


void sub_0_31E(void)
{
  xormask = 0xffff;
  bar(0, 0, 39, 24, 32);
  xormask = 0;
  sx = sy = leftedge = 0;
}

/*
===================
=
= loadgrfiles
=
= Loads the tiles and sprites, and sets up the pointers and tables
=
===================
*/

void loadgrfiles () // sub_0_352
{
  if (grmode == CGAgr)
    installgrfile ("CGAPICS.DD2",0);
  else
  {
    installgrfile ("EGAPICS.DD2",0);
    drawpage ^= 1;
    asm nop; // TODO
  }
}


void sub_0_37E(void)
{
  setscreenmode(grmode);
  sub_0_290();
}


void repaintscreen () // sub_0_38E
{
  sub_0_37E();
  sub_0_239();
}


/*
=============
=
= dofkeys
=
= Checks to see if an F-key is being pressed and handles it
=
=============
*/

void dofkeys (void) // sub_0_399
{
  int i,handle;
  char st2[10];
  int key=bioskey(1)/256;
  if (key==0)
    return;

  switch (key)
  {
    case 0x3b:			// F1
      clearkeys ();
      help ();
      break;
    case 0x3c:          	// F2
      clearkeys ();
      controlpanel ();
      sub_0_290 ();
      break;
    case 0x3d:			// F3
      clearkeys ();
      expwin (18,1);
      print ("RESET GAME (Y/N)?");
      ch=toupper(get());
      if (ch=='Y')
      {
        type94D6_789_94D6.gamexit = 0;
        lives = 1;
      }
      break;

    case 0x43:			// F9
      clearkeys ();
      expwin (7,1);
      print ("PAUSED");
      get ();
      break;
	case 0x01:			// ESC
	case 0x44:			// F10
      clearkeys ();
      expwin (12,1);
      print ("QUIT (Y/N)?");
      ch=toupper(get());
      if (ch=='Y')

	_quit ("");
      break;

    default:
      return;
  }

  sub_0_239 ();
}


void help (void) // sub_0_48C
{
  expwin (36,21);
  print ("dave2 help screen");
  get ();
}



/*=========================================================================*/

/*
=============
=
= dotitlepage
=
=============
*/

void dotitlepage (void) // sub_0_4A9
{
  int i;
  setscreenmode (grmode);

  if (grmode == EGAgr)
    sub_0_290();

  drawpic (0,0,TITLEPIC);

  gamestate=intitle;
  for (i=0;i<300;i++)
  {
    WaitVBL ();
    indemo = notdemo;
    ctrl = ControlPlayer (1);
    if (ctrl.button1 || ctrl.button2 || keydown[0x39])
    {
      exitdemo = true;
      break;
    }
    indemo = demoplay;
    if (bioskey (1))
      dofkeys ();
  }
  gamestate=ingame;
}


/*=========================================================================*/

/*
=============
=
= doendpage
=
=============
*/

void doendpage (void) // sub_0_548
{
  int i;
  setscreenmode (grmode);
  gamestate = in_TODO;
  sx = 0;
  sy = 0;
  print ("Dave 2 title screen");

  for (i=0;i<300;i++)
  {
    WaitVBL ();
    indemo = 0;
    ctrl = ControlPlayer (1);
    if (ctrl.button1 || ctrl.button2 || keydown[0x39])
    {
      exitdemo = true;
      break;
    }
    indemo = 1;
    if (bioskey (1))
      dofkeys ();
  }

  clearkeys ();
  sx = 20;
  sy = 24;
  get ();
  indemo = 1;
  gamestate = ingame;
}


/*=========================================================================*/


/*
=============
=
= dodemo
=
= Shows a random demo
=
=============
*/

void dodemo (void) // sub_0_5F9
{
  int i;

  exitdemo = false;
  indemo = 0;

  ctrl = ControlPlayer (1);
  if (ctrl.button1 || ctrl.button2 || keydown[0x39])
    exitdemo = true;

  if (bioskey (1))
    dofkeys ();

  while (!exitdemo)
  {
    dotitlepage ();

    if (exitdemo)
      break;

    i=random(NUMDEMOS)+1;
    LoadDemo (i);
    leveldone=true;
    playloop ();
    if (exitdemo)
      break;

    gamestate=inscores;
    _showhighscores ();
    for (i=0;i<500;i++)
    {
      WaitVBL ();
      indemo = notdemo;
      ctrl = ControlPlayer (1);
      if (ctrl.button1 || ctrl.button2 || keydown[0x39])
      {
	exitdemo = true;
	break;
      }
      indemo = 1;
      if (bioskey (1))
	dofkeys ();
    }

  }

  level = 1;
  leveldone = true;
  indemo = 0;
}

/*=========================================================================*/

/*
============
=
= gameover
=
= End game, check for high score
=
============
*/

void gameover (void) // sub_0_71E
{
  int i;

  expwin (11,4);
  print ("\n GAME OVER\n     ");
  PlaySound (GAMEOVERSND);
  WaitEndSound ();
  for (i=0;i<120;i++)
    WaitVBL ();
  gamestate=inscores;
  _checkhighscore ();

  for (i=0;i<500;i++)
  {
	 WaitVBL ();
	 ctrl = ControlPlayer (1);
	 if (ctrl.button1 || ctrl.button2 || keydown[0x39])
		break;
	 if (bioskey (1))
		dofkeys ();
  }
}


void RF_Refresh(void) // sub_0_7B3
{
asm    call    sub_0_1E46
asm    inc     word_789_94CA
asm    cmp     drawpage, 0
asm    jz      short loc_0_7E2
asm    xor     si, si
asm    jmp     short loc_0_7DA

loc_0_7C9:

asm    mov     bx, si
asm    shl     bx, 1
asm    mov     bx, word_789_7B20[bx]
asm    shl     bx, 1
asm    mov     drawoffs1[bx], -1
asm    inc     si

loc_0_7DA:

asm    cmp     si, word_789_949E
asm    jl      short loc_0_7C9
asm    jmp     short loc_0_7FD

loc_0_7E2:

asm    xor     si, si
asm    jmp     short loc_0_7F7

loc_0_7E6:

asm    mov     bx, si
asm    shl     bx, 1
asm    mov     bx, word_789_7B20[bx]
asm    shl     bx, 1
asm    mov     drawoffs0[bx], -1
asm    inc     si

loc_0_7F7:

asm    cmp     si, word_789_949E
asm    jl      short loc_0_7E6

loc_0_7FD:

asm    cmp     grmode, EGAgr
asm    jnz     short loc_0_80A
asm    xor     drawpage, 1
asm    nop; // TODO

loc_0_80A:
;
}


void sub_0_80D_TODO(void)
{
asm {
db 83h, 0ECh, 0Eh, 56h, 57h, 33h, 0C0h, 0BAh, 00h, 01h, 50h, 52h, 0FFh, 36h, 0D2h, 94h
db 0FFh, 36h, 0D0h, 94h, 0E8h, 4Ch, 6Ch, 8Bh, 16h, 2Eh, 1Dh, 2Bh, 0D0h, 89h, 16h, 2Eh
db 1Dh, 33h, 0C0h, 0BAh, 00h, 01h, 50h, 52h, 0FFh, 36h, 0CEh, 94h, 0FFh, 36h, 0CCh, 94h
db 0E8h, 30h, 6Ch, 8Bh, 16h, 2Ch, 1Dh, 2Bh, 0D0h, 89h, 16h, 2Ch, 1Dh, 83h, 3Eh, 9Ch
db 0AFh, 02h, 74h, 03h, 0E9h, 0Eh, 01h, 33h, 0C0h, 0BAh, 10h, 00h, 50h, 52h, 33h, 0C0h
db 0BAh, 00h, 01h, 50h, 52h, 0FFh, 36h, 0D2h, 94h, 0FFh, 36h, 0D0h, 94h, 0E8h, 03h, 6Ch
db 52h, 50h, 0E8h, 0Dh, 6Ch, 8Bh, 16h, 2Eh, 1Dh, 03h, 0D0h, 89h, 16h, 2Eh, 1Dh, 33h
db 0C0h, 0BAh, 10h, 00h, 50h, 52h, 33h, 0C0h, 0BAh, 00h, 01h, 50h, 52h, 0FFh, 36h, 0CEh
db 94h, 0FFh, 36h, 0CCh, 94h, 0E8h, 0DBh, 6Bh, 52h, 50h, 0E8h, 0E5h, 6Bh, 8Bh, 16h, 2Ch
db 1Dh, 03h, 0D0h, 89h, 16h, 2Ch, 1Dh, 83h, 06h, 2Eh, 1Dh, 20h, 0A1h, 2Ch, 1Dh, 0BBh
db 10h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0F4h, 83h, 7Eh, 0F4h, 00h, 7Dh, 07h, 0C7h, 46h
db 0F4h, 00h, 00h, 0EBh, 0Bh, 83h, 7Eh, 0F4h, 17h, 7Eh, 05h, 33h, 0C0h, 0E9h, 0D1h, 01h
db 0A1h, 0A4h, 0C3h, 0D1h, 0E0h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 2Ch, 1Dh, 5Ah, 03h, 0C2h
db 48h, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 8Bh, 0F8h, 83h, 0FFh, 14h, 7Eh, 05h, 0BFh, 14h
db 00h, 0EBh, 09h, 0Bh, 0FFh, 7Dh, 05h, 33h, 0C0h, 0E9h, 0A5h, 01h, 0A1h, 2Eh, 1Dh, 05h
db 0E0h, 0FFh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0F2h, 83h, 7Eh, 0F2h, 00h, 7Dh
db 07h, 0C7h, 46h, 0F2h, 00h, 00h, 0EBh, 0Bh, 83h, 7Eh, 0F2h, 0Dh, 7Eh, 05h, 33h, 0C0h
db 0E9h, 7Eh, 01h, 0A1h, 2Eh, 1Dh, 03h, 06h, 0A6h, 0C3h, 05h, 0DFh, 0FFh, 0BBh, 10h, 00h
db 99h, 0F7h, 0FBh, 8Bh, 0F0h, 83h, 0FEh, 0Dh, 7Eh, 05h, 0BEh, 0Dh, 00h, 0EBh, 09h, 0Bh
db 0F6h, 7Dh, 05h, 33h, 0C0h, 0E9h, 59h, 01h, 0A1h, 2Ch, 1Dh, 0D1h, 0F8h, 0D1h, 0F8h, 0D1h
db 0F8h, 05h, 04h, 00h, 0A3h, 2Ch, 1Dh, 8Bh, 0C6h, 0BAh, 15h, 00h, 0F7h, 0EAh, 03h, 0C7h
db 0D1h, 0E0h, 89h, 46h, 0FEh, 8Bh, 46h, 0F2h, 89h, 46h, 0F6h, 0EBh, 2Ch, 8Bh, 46h, 0F4h
db 89h, 46h, 0F8h, 0EBh, 1Ch, 8Bh, 46h, 0F6h, 0BAh, 15h, 00h, 0F7h, 0EAh, 03h, 46h, 0F8h
db 8Bh, 1Eh, 9Eh, 94h, 0D1h, 0E3h, 89h, 87h, 20h, 7Bh, 0FFh, 06h, 9Eh, 94h, 0FFh, 46h
db 0F8h, 39h, 7Eh, 0F8h, 7Eh, 0DFh, 0FFh, 46h, 0F6h, 39h, 76h, 0F6h, 7Eh, 0CFh, 0FFh, 06h
db 0BCh, 94h, 83h, 06h, 0C6h, 94h, 20h, 8Bh, 1Eh, 0C6h, 94h, 8Bh, 46h, 0FEh, 89h, 07h
db 8Bh, 1Eh, 0C6h, 94h, 0A1h, 2Ch, 1Dh, 89h, 47h, 02h, 8Bh, 1Eh, 0C6h, 94h, 0A1h, 2Eh
db 1Dh, 89h, 47h, 04h, 8Bh, 1Eh, 0C6h, 94h, 0A1h, 0A4h, 0C3h, 89h, 47h, 06h, 8Bh, 1Eh
db 0C6h, 94h, 0A1h, 0A6h, 0C3h, 89h, 47h, 08h, 8Bh, 1Eh, 0C6h, 94h, 0A1h, 0AAh, 0C3h, 8Bh
db 16h, 0A8h, 0C3h, 89h, 57h, 0Ah, 89h, 47h, 0Ch, 8Bh, 1Eh, 0C6h, 94h, 0A1h, 0AEh, 0C3h
db 8Bh, 16h, 0ACh, 0C3h, 89h, 57h, 0Eh, 89h, 47h, 10h, 8Bh, 1Eh, 0C6h, 94h, 0C7h, 47h
db 12h, 00h, 00h, 8Bh, 1Eh, 28h, 82h, 0A1h, 34h, 1Dh, 89h, 47h, 1Ch, 8Bh, 1Eh, 28h
db 82h, 0A1h, 28h, 1Dh, 89h, 47h, 20h, 8Bh, 1Eh, 28h, 82h, 0A1h, 36h, 1Dh, 89h, 47h
db 1Eh, 8Bh, 1Eh, 28h, 82h, 0A1h, 2Ah, 1Dh, 89h, 47h, 22h, 8Bh, 1Eh, 0D4h, 94h, 0B1h
db 05h, 0D3h, 0E3h, 8Bh, 87h, 7Ch, 84h, 3Bh, 46h, 0FEh, 72h, 17h, 8Bh, 1Eh, 0BCh, 94h
db 0B1h, 05h, 0D3h, 0E3h, 0A1h, 0D4h, 94h, 89h, 87h, 8Eh, 84h, 0A1h, 0BCh, 94h, 0A3h, 0D4h
db 94h, 0EBh, 49h, 0A1h, 0D4h, 94h, 89h, 46h, 0FCh, 0EBh, 14h, 8Bh, 46h, 0FCh, 89h, 46h
db 0FAh, 8Bh, 5Eh, 0FCh, 0B1h, 05h, 0D3h, 0E3h, 8Bh, 87h, 8Eh, 84h, 89h, 46h, 0FCh, 8Bh
db 5Eh, 0FCh, 0B1h, 05h, 0D3h, 0E3h, 8Bh, 87h, 7Ch, 84h, 3Bh, 46h, 0FEh, 72h, 0DCh, 8Bh
db 5Eh, 0FAh, 0B1h, 05h, 0D3h, 0E3h, 0A1h, 0BCh, 94h, 89h, 87h, 8Eh, 84h, 8Bh, 1Eh, 0BCh
db 94h, 0B1h, 05h, 0D3h, 0E3h, 8Bh, 46h, 0FCh, 89h, 87h, 8Eh, 84h, 0B8h, 01h, 00h, 0EBh
db 00h, 5Fh, 5Eh, 8Bh, 0E5h
}
}


void BadThink(void) // sub_0_AA7
{
  _quit("badTHINK!");
}


void FindFreeObj (void) // sub_0_AB4
{
  int i = 1;
  new = &objlist[1];

  while (new->class != nothing && i < lastobj)
  {
    i++;
    new++;
  }

  if (i >= lastobj)
    lastobj++;

  new->x1 = new->y1 = new->y2 = new->x2 = 0;

  new->think = BadThink;
}


void sub_0_B0D(void)
{
	word_789_1D2C = word_789_8228->dword_789_94D6 / 256;
	word_789_1D2E = word_789_8228->dword_789_94DA / 256;

	if (grmode == CGAgr)
		word_789_8226 = word_789_94A0 * 2 + ((word_789_1D2C / 2) % 2);
	else
	{
		word_789_8226 = word_789_94A0 * 4 + ((word_789_1D2C / 2) % 4);
		word_789_1D2C &= ~6;
		asm nop; // TODO
	}

	word_789_8228->word_789_94EE = word_789_8226;

	image = spritetable[word_789_8226];

	word_789_1D34 = word_789_1D2C + image.xl;
	word_789_1D28 = word_789_1D2C + image.xh;
	word_789_1D36 = word_789_1D2E + image.yl;
	word_789_1D2A = word_789_1D2E + image.yh;
}


boolean sub_0_BCF(void)
{
  if (objptr_789_7FD0->x1 > word_789_1D28)
    return false;

  if (objptr_789_7FD0->y1 > word_789_1D2A)
    return false;

  if (objptr_789_7FD0->x2 < word_789_1D34)
    return false;

  if (objptr_789_7FD0->y2 < word_789_1D36)
    return false;

  return true;
}


int sub_0_C1D(int arg_0, int arg_2)
{
	IMPLEMENT_ME("sub_0_C1D");

asm {
db 0FBh, 0F7h, 2Eh, 7Eh, 94h, 50h, 8Bh, 46h
db 04h, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 5Ah, 03h, 0D0h, 0D1h, 0E2h, 8Bh, 1Eh, 86h, 94h
db 03h, 0DAh, 8Bh, 07h, 0EBh, 00h
}
}


void sub_0_C48_TODO(void)
{
asm {
db 83h, 0ECh, 06h, 56h, 57h
db 0A1h, 34h, 1Dh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0FEh, 0A1h, 36h, 1Dh, 0BBh
db 10h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0FCh, 0A1h, 28h, 1Dh, 0BBh, 10h, 00h, 99h, 0F7h
db 0FBh, 89h, 46h, 0FAh, 0A1h, 2Ah, 1Dh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 8Bh, 0C8h, 8Bh
db 76h, 0FCh, 0EBh, 2Ch, 8Bh, 7Eh, 0FEh, 0EBh, 21h, 8Bh, 0C6h, 0F7h, 2Eh, 7Eh, 94h, 03h
db 0C7h, 0D1h, 0E0h, 8Bh, 1Eh, 86h, 94h, 03h, 0D8h, 8Bh, 1Fh, 0D1h, 0E3h, 83h, 0BFh, 0A8h
db 00h, 00h, 74h, 05h, 0B8h, 01h, 00h, 0EBh, 0Fh, 47h, 3Bh, 7Eh, 0FAh, 7Eh, 0DAh, 46h
db 3Bh, 0F1h, 7Eh, 0D0h, 33h, 0C0h, 0EBh, 00h, 5Fh, 5Eh, 8Bh, 0E5h
}
}


void sub_0_CBE_TODO(void)
{
asm {
db 83h, 0ECh, 04h, 56h, 57h, 8Bh, 1Eh, 28h, 82h, 8Bh, 7Fh, 12h, 8Bh, 1Eh, 28h
db 82h, 8Bh, 77h, 14h, 0E8h, 36h, 0FEh, 0Bh, 0FFh, 7Dh, 0Fh, 8Bh, 0C7h, 05h, 01h, 0FFh
db 0BBh, 00h, 01h, 99h, 0F7h, 0FBh, 8Bh, 0F8h, 0EBh, 11h, 0Bh, 0FFh, 7Eh, 0Dh, 8Bh, 0C7h
db 05h, 0FFh, 00h, 0BBh, 00h, 01h, 99h, 0F7h, 0FBh, 8Bh, 0F8h, 0Bh, 0F6h, 7Dh, 0Fh, 8Bh
db 0C6h, 05h, 01h, 0FFh, 0BBh, 00h, 01h, 99h, 0F7h, 0FBh, 8Bh, 0F0h, 0EBh, 11h, 0Bh, 0F6h
db 7Eh, 0Dh, 8Bh, 0C6h, 05h, 0FFh, 00h, 0BBh, 00h, 01h, 99h, 0F7h, 0FBh, 8Bh, 0F0h, 01h
db 36h, 36h, 1Dh, 01h, 36h, 2Ah, 1Dh, 01h, 3Eh, 34h, 1Dh, 01h, 3Eh, 28h, 1Dh, 0E8h
db 16h, 0FFh, 89h, 46h, 0FEh, 29h, 36h, 36h, 1Dh, 29h, 36h, 2Ah, 1Dh, 29h, 3Eh, 34h
db 1Dh, 29h, 3Eh, 28h, 1Dh, 83h, 7Eh, 0FEh, 00h, 75h, 03h, 0E9h, 0FEh, 00h, 0Bh, 0FFh
db 7Dh, 34h, 01h, 3Eh, 34h, 1Dh, 01h, 3Eh, 28h, 1Dh, 0E8h, 0EBh, 0FEh, 89h, 46h, 0FEh
db 29h, 3Eh, 34h, 1Dh, 29h, 3Eh, 28h, 1Dh, 83h, 7Eh, 0FEh, 01h, 75h, 16h, 0A1h, 34h
db 1Dh, 0F7h, 0D8h, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 0B1h, 08h, 0D3h, 0E2h, 8Bh, 1Eh, 28h
db 82h, 89h, 57h, 12h, 0EBh, 39h, 0Bh, 0FFh, 7Eh, 35h, 01h, 3Eh, 34h, 1Dh, 01h, 3Eh
db 28h, 1Dh, 0E8h, 0B3h, 0FEh, 89h, 46h, 0FEh, 29h, 3Eh, 34h, 1Dh, 29h, 3Eh, 28h, 1Dh
db 83h, 7Eh, 0FEh, 01h, 75h, 19h, 0A1h, 28h, 1Dh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 0B8h
db 0Fh, 00h, 2Bh, 0C2h, 0B1h, 08h, 0D3h, 0E0h, 8Bh, 1Eh, 28h, 82h, 89h, 47h, 12h, 0Bh
db 0F6h, 7Dh, 34h, 01h, 36h, 36h, 1Dh, 01h, 36h, 2Ah, 1Dh, 0E8h, 7Ah, 0FEh, 89h, 46h
db 0FCh, 29h, 36h, 36h, 1Dh, 29h, 36h, 2Ah, 1Dh, 83h, 7Eh, 0FCh, 01h, 75h, 16h, 0A1h
db 36h, 1Dh, 0F7h, 0D8h, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 0B1h, 08h, 0D3h, 0E2h, 8Bh, 1Eh
db 28h, 82h, 89h, 57h, 14h, 0EBh, 39h, 0Bh, 0F6h, 7Eh, 35h, 01h, 36h, 36h, 1Dh, 01h
db 36h, 2Ah, 1Dh, 0E8h, 42h, 0FEh, 89h, 46h, 0FCh, 29h, 36h, 36h, 1Dh, 29h, 36h, 2Ah
db 1Dh, 83h, 7Eh, 0FCh, 01h, 75h, 19h, 0A1h, 2Ah, 1Dh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh
db 0B8h, 0Fh, 00h, 2Bh, 0C2h, 0B1h, 08h, 0D3h, 0E0h, 8Bh, 1Eh, 28h, 82h, 89h, 47h, 14h
db 83h, 7Eh, 0FEh, 00h, 75h, 16h, 83h, 7Eh, 0FCh, 00h, 75h, 10h, 8Bh, 1Eh, 28h, 82h
db 33h, 0C0h, 89h, 47h, 14h, 8Bh, 1Eh, 28h, 82h, 89h, 47h, 12h, 5Fh, 5Eh, 8Bh, 0E5h
}
}


void sub_0_E52_TODO(int arg_0, int arg_2)
{
asm {
db 8Bh, 46h, 04h, 99h, 8Bh, 1Eh, 28h, 82h, 01h, 07h, 11h
db 57h, 02h, 8Bh, 46h, 06h, 99h, 8Bh, 1Eh, 28h, 82h, 01h, 47h, 04h, 11h, 57h, 06h
db 0E8h, 9Ah, 0FCh, 8Bh, 46h, 04h, 99h, 8Bh, 1Eh, 28h, 82h, 29h, 07h, 19h, 57h, 02h
db 8Bh, 46h, 06h, 99h, 8Bh, 1Eh, 28h, 82h, 29h, 47h, 04h, 19h, 57h, 06h, 33h, 0C0h
db 0BAh, 04h, 00h, 50h, 52h, 0FFh, 36h, 0D2h, 94h, 0FFh, 36h, 0D0h, 94h, 0E8h, 0D3h, 65h
db 50h, 0A1h, 2Ah, 1Dh, 52h, 99h, 5Bh, 59h, 2Bh, 0C1h, 1Bh, 0D3h, 0Bh, 0D2h, 7Fh, 16h
db 7Ch, 05h, 3Dh, 10h, 00h, 73h, 0Fh, 8Bh, 1Eh, 28h, 82h, 83h, 7Fh, 14h, 00h, 7Fh
db 05h, 0B8h, 01h, 00h, 0EBh, 51h, 33h, 0C0h, 0BAh, 04h, 00h, 50h, 52h, 0FFh, 36h, 0D2h
db 94h, 0FFh, 36h, 0D0h, 94h, 0E8h, 9Bh, 65h, 50h, 0A1h, 2Ah, 1Dh, 52h, 99h, 5Bh, 59h
db 2Bh, 0C1h, 1Bh, 0D3h, 0Bh, 0D2h, 7Ch, 16h, 7Fh, 05h, 3Dh, 0ACh, 00h, 76h, 0Fh, 8Bh
db 1Eh, 28h, 82h, 83h, 7Fh, 14h, 00h, 7Ch, 05h, 0B8h, 01h, 00h, 0EBh, 19h, 83h, 3Eh
db 34h, 1Dh, 06h, 7Ch, 08h, 81h, 3Eh, 28h, 1Dh, 2Fh, 01h, 7Eh, 05h, 0B8h, 01h, 00h
db 0EBh, 05h, 0E8h, 33h, 0FDh, 0EBh, 00h
}
}


void sub_0_F19(void)
{
	int di, si;
	ControlStruct c;
	int var_8;
	int var_6;
	int var_4;

	di = 0;
	si = 0;

	if (word_789_8228->dword_789_94D6 < 0x00001000)
		word_789_8228->dword_789_94D6 = 0x00001000;

	if ((long)((*(int*)word_789_94C8) - 1) << 8 << 4 < word_789_8228->dword_789_94D6)
		word_789_8228->dword_789_94D6 = (long)((*(int*)word_789_94C8) - 1) << 8 << 4;

	if ((long)(*(int*)(word_789_94C8 + 2)) << 8 << 4 < word_789_8228->dword_789_94DA)
	{
		word_789_8228->gamexit = 0;
		PlaySound(PLUMMETSND);
		WaitEndSound();
		return;
	}

	c = ControlPlayer (word_789_94BA + 1);
	if (c.button2)
	{
		if (!bool_789_94C4 && !bool_789_9490)
		{
			PlaySound(JUMPSND);
			si = -word_789_1D32;
			bool_789_94C4 = true;
			bool_789_9490 = true;
			word_789_8476 = word_789_9482;
		}

		if (bool_789_9490 && word_789_8476 > 0)
		{
			si -= word_789_8476;
			word_789_8476 -= 7;
		}
	}
	else
	{
		word_789_8476 = 0;
		if (!bool_789_94C4)
			bool_789_9490 = false;
	}

	if (c.button1)
		var_4 = 3;
	else
		var_4 = 2;

	ctrl_789_9492 = c;

	switch (c.dir)
	{
		case northeast:
		case east:
		case southeast:
			di = word_789_9480 * var_4;
			bool_789_847A = false;
			break;

		case southwest:
		case west:
		case northwest:
			di = -word_789_9480 * var_4;
			bool_789_847A = true;
			break;
	}

	si += word_789_1D30;

	if (bool_789_94C4)
	{
		if (bool_789_847A)
			word_789_94A0 = 10;
		else
			word_789_94A0 = 9;
	}
	else
	{
		if (word_789_8228->word_789_94E4-- == 0)
		{
			word_789_8228->word_789_94E4 = word_789_8228->word_789_94E6;

			if (++word_789_8228->word_789_94E0 == 4)
				word_789_8228->word_789_94E0 = 0;
		}

		if (di == 0)
		{
			word_789_8228->word_789_94E0 = 1;
			word_789_8228->word_789_94E4 = word_789_8228->word_789_94E6;
		}

		if (bool_789_847A)
			word_789_94A0 = word_789_8228->word_789_94E0 + 5;
		else
			word_789_94A0 = word_789_8228->word_789_94E0;
	}

	sub_0_B0D();

	var_6 = sub_0_C1D(word_789_1D34, word_789_1D2A + 1);
	var_8 = sub_0_C1D(word_789_1D28, word_789_1D2A + 1);

	IMPLEMENT_ME("sub_0_F19");

asm {
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
db 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h, 90h
}
}


#define EXTRASCORE 10000

void AddScore(int toadd) // sub_0_1491
{
	score += toadd;
	if (score - EXTRASCORE >= lastExtraScore)
	{
		lastExtraScore += EXTRASCORE;
		lives++;
	}
}


void sub_0_14CD(void)
{
  RF_ForceRefresh();
  do
  {
    word_789_94BC = word_789_949E = word_789_94D4 = 0;
    word_789_94C6_TODO = 0x847C;
    WaitVBL();
    word_789_8228 = &type94D6_789_94D6;
    sub_0_F19();
    RF_Refresh();
    dofkeys();

    switch (indemo)
    {
      case notdemo:
          if (keydown[0x2e] && keydown[0x14] && keydown[0x39])
          {
            clearkeys();
            centerwindow(15, 1);
            print("Warp to level:");
            ch = get();
            if (ch > '0' && ch <= '9')
            {
              level = ch - '1';
              leveldone = true;
            }
            RF_ForceRefresh();
            RF_Refresh();
            RF_Refresh();
          }
        break;

      case recording:
          if (!type94D6_789_94D6.gamexit || (keydown[0x42] && keydown[0x20]))
          {
            clearkeys();
            centerwindow(15, 1);
            print("SAVE AS DEMO#:");
            ch = get ();
            if (ch<='0' || ch<='9')
            {
            }
            SaveDemo(ch-'0');
            RF_ForceRefresh();
            RF_Refresh();
            RF_Refresh();
            return;
          }
        break;

      case demoplay:
          indemo = 0;
          ctrl = ControlPlayer (1);
          if (ctrl.button1 || ctrl.button2 || keydown[0x39])
          {
            indemo = 1;
            exitdemo = true;
            break;
          }
          indemo = 1;
        break;
    }

  } while (!leveldone && type94D6_789_94D6.gamexit);
}


/*======================================*/
/*				      */
/* playloop                             */
/* all the action is directed from here */
/*				      */
/*======================================*/

void playloop(void) // sub_0_162C
{
	char st[6];
	int plane;

	drawpage = 0;
	lives = 4;
	lastExtraScore = 0;
	score = 0;
	leveldone = true;

	RF_ForceRefresh();

	word_789_94CA = 0;

	do
	{
		if (leveldone)
		{
			word_789_847C_TODO = -1;
			word_789_94C6_TODO = 0x847C;
			word_789_94BC = 0;
			word_789_949E = 0;
			word_789_94D4 = 0;

			strcpy(str, "LEVEL0");
			itoa(level, st, 10);
			strcat(str, st);
			strcat(str, ".");
			strcat(str, _extension);

			LoadFile(str, word_789_94C8);

			for (plane = 0; (*(int*)(word_789_94C8 + 4)) > plane; plane++) {
				word_789_9486[plane] = word_789_94C8 + (*(int*)(word_789_94C8 + 14)) * plane + 0x20;
			}

			lastobj = 1;
			word_789_947E = *(int*)word_789_94C8;
			word_789_947C = word_789_947E << 1;
			word_789_1556 = word_789_947C + -42;
			word_789_154C = 0;
			word_789_154E = 0;
			word_789_1550 = 0;
			word_789_1552 = 0;

			dword_789_ADD6 = ((long)((*(int*)(word_789_94C8 + 0)) + -20)) << 12;
			dword_789_ADDA = ((long)((*(int*)(word_789_94C8 + 2)) + -13)) << 12;

			dword_789_94D0 = dword_789_ADDA;
			dword_789_94CC = 0;
			word_789_1D30 = 160;
			word_789_9480 = 80;
			word_789_9482 = 300;
			word_789_1D32 = 750;
			RF_Refresh();
			sub_0_290();
			RF_Refresh();
			leveldone = false;
		}

		if (indemo != 0)
		{
			initrndt(false);
		}
		else
		{
			initrndt(true);
			dword_789_94D0 = dword_789_ADDA;
			dword_789_94CC = 0;
			RF_ForceRefresh();
			RF_Refresh();
			RF_Refresh();
			expwin(14, 9);
			print("\n SCORE:");
			printlong(score);
			print("\n\n WORLD:");
			printint(level);
			print("\n\n DAVES LEFT:");
			printint(lives);
			PlaySound(STARTSOUNDSND);
			WaitEndSound();
			if (keydown[0x41] && keydown[0x20]) // 'D+F7' to record a demo
			{
				RF_ForceRefresh();
				RF_Refresh();
				RF_Refresh();
				clearkeys();
				centerwindow(12, 1);
				print("RECORD DEMO");
				do
				{
					ch=get ();
				} while (ch!=13);

				initrndt(false);
				RecordDemo();
			}
		}

		type94D6_789_94D6.dword_789_94DA = dword_789_94D0 + 0x9600;
		type94D6_789_94D6.dword_789_94D6 = 0x00009600;
		type94D6_789_94D6.word_789_94EA = 0;
		type94D6_789_94D6.word_789_94E8 = 0;
		type94D6_789_94D6.gamexit = 1;
		type94D6_789_94D6.word_789_94E0 = 2;
		type94D6_789_94D6.word_789_94E6 = type94D6_789_94D6.word_789_94E4 = 5;
		word_789_94FE = 0;
		word_789_9500 = 0;
		func_789_9506 = sub_0_F19;
		ctrl_789_9492.button1 = 0;
		ctrl_789_9492.button1 = 0;
		word_789_8476 = 0;
		bool_789_94C4 = true;
		bool_789_9490 = false;
		bool_789_847A = false;
		dword_789_94D0 = dword_789_ADDA;
		dword_789_94CC = 0;
		sub_0_14CD();
		if (indemo != 0)
			return;

		if (!leveldone)
		{
			lives--;
		}
		else
		{
			level++;
			if (level > _numlevels)
			{
				lives = 0;
				gamestate = in_TODO;
			}
		}
	} while (lives != 0);
}

/***************************************************************************/
/***************************************************************************/

/*=========================*/
/*			   */
/* m a i n   p r o g r a m */
/*			   */
/*=========================*/

void main (void) // sub_0_1953
{
	_numlevels = 1;
	_maxplayers = 1;

	_cgaok = false;
	_egaok = true;
	_vgaok = false;

	_extension = "DD2";

	_setupgame();

	screencenterx = 25;
	word_789_8220 = 32;

	sub_0_37E();

	word_789_94C8 = unk_789_1D40;

	while (1)			// go until quit () is called
	{
		dodemo();
		WaitEndSound();
		playloop();
		if (gamestate == in_TODO)
		{
			doendpage ();		// finished all levels
		}
		gameover();
	}
}

