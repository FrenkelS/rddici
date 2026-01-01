/* Reconstructed Dangerous Dave in Copyright Infringement Source Code
 * Copyright (C) 2025 Frenkel Smeijers
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

/*
** catacomb II -- the c translation...
*/

#include "pcrlib.h"
#include "NGRABCA2.H"
#include "SOUNDS.H"

#define NUMDEMOS 1

#define maxpics 2047
#define numtiles 24*24   /*number of tiles displayed on screen*/
#define numlevels 30
#define maxobj 200           /*maximum possible active objects*/
#define solidwall 129
#define blankfloor 128
#define leftoff 11
#define topoff 11
#define tile2s 256          /*tile number where the 2*2 pictures start*/
#define tile3s tile2s+67*4
#define tile4s tile3s+35*9
#define tile5s tile4s+19*16
#define lasttile tile5s+19*25

typedef enum {playercmd,gargcmd,dragoncmd,ramstraight,ramdiag,straight,idle,
    fade,explode,gunthinke,gunthinks} thinktype;

typedef enum {benign,monster,pshot,mshot,nukeshot} tagtype;

typedef enum {nothing,player,goblin,skeleton,ogre,gargoyle,dragon,turbogre,
    wallhit,shot,bigshot,rock,dead1,dead2,dead3,dead4,dead5,dead6,teleporter,
    torch,secretgate,gune,guns,lastclass} classtype;

typedef enum {ingame,intitle,inscores} statetype;


typedef struct {
  boolean active;	/*if false, the object has not seen the player yet*/
  classtype  class;
  byte  x,y,		/*location of upper left corner in world*/
    stage,		/*animation frame being drawn*/
    delay;		/*number of frames to pause without doing anything*/
  dirtype  dir;		/*direction facing*/
  char hp;		/*hit points*/
  byte oldx,oldy;	/*position where it was last drawn*/
  int oldtile;		/*origin tile when last drawn*/
  char filler[1];	/*pad to 16 bytes*/
   } activeobj;

typedef struct {	/*holds a copy of activeobj, and its class info*/
  boolean  active;	/*if false, the object has not seen the player yet*/
  classtype  class;
  byte  x,y,		/*location of upper left corner in world*/
    stage,		/*animation frame being drawn*/
    delay;		/*number of frames to pause without doing anything*/
  dirtype  dir;		/*direction facing*/
  char hp;		/*hit points*/
  byte oldx,oldy;		/*position where it was last drawn*/
  int oldtile;		/*origin tile when last drawn*/
  char filler[1];	/*pad to 16 bytes*/

  byte think;
  byte contact;
  byte solid;
  word  firstchar;
  byte  size;
  byte  stages;
  byte  dirmask;
  word  speed;
  byte  hitpoints;
  byte  damage;
  word  points;
  char filler2[2];	/*pad to 32 bytes*/
  } objdesc;


/*=================*/
/*		   */
/* typed constants */
/*     		   */
/*=================*/
  char altmeters[14][14] = {
 {127,127,127,127,127,127,127,127,127,127,127,127,127},
 {23,127,127,127,127,127,127,127,127,127,127,127,127},
 {23,25,127,127,127,127,127,127,127,127,127,127,127},
 {23,24,25,127,127,127,127,127,127,127,127,127,127},
 {23,24,24,25,127,127,127,127,127,127,127,127,127},
 {23,24,24,24,25,127,127,127,127,127,127,127,127},
 {23,24,24,24,24,25,127,127,127,127,127,127,127},
 {23,24,24,24,24,24,25,127,127,127,127,127,127},
 {23,24,24,24,24,24,24,25,127,127,127,127,127},
 {23,24,24,24,24,24,24,24,25,127,127,127,127},
 {23,24,24,24,24,24,24,24,24,25,127,127,127},
 {23,24,24,24,24,24,24,24,24,24,25,127,127},
 {23,24,24,24,24,24,24,24,24,24,24,25,127},
 {23,24,24,24,24,24,24,24,24,24,24,24,25} };

  char meters[14][14] = {
 {127,127,127,127,127,127,127,127,127,127,127,127,127},
 {26,127,127,127,127,127,127,127,127,127,127,127,127},
 {26,28,127,127,127,127,127,127,127,127,127,127,127},
 {26,27,28,127,127,127,127,127,127,127,127,127,127},
 {26,27,27,28,127,127,127,127,127,127,127,127,127},
 {26,27,27,27,28,127,127,127,127,127,127,127,127},
 {26,27,27,27,27,28,127,127,127,127,127,127,127},
 {26,27,27,27,27,27,28,127,127,127,127,127,127},
 {26,27,27,27,27,27,27,28,127,127,127,127,127},
 {26,27,27,27,27,27,27,27,28,127,127,127,127},
 {26,27,27,27,27,27,27,27,27,28,127,127,127},
 {26,27,27,27,27,27,27,27,27,27,28,127,127},
 {26,27,27,27,27,27,27,27,27,27,27,28,127},
 {26,27,27,27,27,27,27,27,27,27,27,27,28} };

 dirtype opposite[9] =
    {south,west,north,east,southwest,northwest,northeast,southeast,nodir};


/*==================*/
/*		    */
/* global variables */
/*		    */
/*==================*/
  enum {quited,killed,reseted,victorious} gamexit; /*determines what to do after playloop*/

  int oldtiles [numtiles];		/*tile displayed last refresh*/
  int background[87][86];		/*base map*/
  int view[87][86];			/*base map with objects drawn in*/
  int originx, originy;			/*current world location of ul corn*/
  byte priority [maxpics+1];		/*tile draw overlap priorities*/

  int items[6],saveitems[6];
  int shotpower;			/*0-13 characters in power meter*/
  int side;	                        /*which side shots come from*/
  int boltsleft;			/*number of shots left in a bolt*/

  activeobj o[maxobj+1],saveo[1];	/*everything that moves is here*/
  objdesc obj , altobj;			/*total info about objecton and alt*/
  int altnum;				/*o[#] of altobj*/
  int numobj,objecton;			/*number of objects in o now*/

  struct {
    byte think;			/*some of these sizes are for the*/
    byte contact;			/*convenience of the assembly routines*/
    byte solid;
    word firstchar;
    byte size;
    byte stages;
    byte dirmask;
    word speed;
    byte hitpoints;
    byte damage;
    word points;
    byte filler[2];
  } objdef [lastclass];


  int i,j,k,x,y,z;
  boolean playdone, leveldone;

  boolean tempb;
  char far *tempp;

  int chkx,chky,chkspot;		/*spot being checked by walk*/

  word frameon;
  char far *grmem;
  classtype clvar;

  int VGAPAL;				// just to make pcrlib happy

  boolean exitdemo,resetgame;
  statetype gamestate;

  ControlStruct ctrl;

  char far *pics, far *picsexact;

  unsigned EGADATASTART;

  long savescore;

//NOLAN ADDED
	boolean GODMODE = false;


/****************************************************************************/

//////////////////////////////////
//
// function prototypes
//
//////////////////////////////////

void extern drawobj (void);
void extern eraseobj (void);
void extern doall (void);
void extern egamove (void);
void extern cgarefresh (void);
void extern egarefresh (void);
void dofkeys (void);



/*==============================*/
/*			        */
/* xxxrefresh                   */
/* refresh the changed areas of */
/* the tiles map in the various */
/* graphics modes.              */
/*			        */
/*==============================*/

char demowin [5][16] = {
  {14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16},
  {17,' ','-','-','-',' ','D','E','M','O',' ','-','-','-',' ',18},
  {17,'S','P','A','C','E',' ','T','O',' ','S','T','A','R','T',18},
  {17,'F','1',' ','T','O',' ','G','E','T',' ','H','E','L','P',18},
  {19,20,20,20,20,20,20,20,20,20,20,20,20,20,20,21} };

void sub_0_239(void)
{
asm {
db 0E8h, 15h, 1Bh, 83h
db 3Eh, 0C2h, 94h, 00h, 74h, 07h, 83h, 3Eh, 0C2h, 94h, 03h, 75h, 09h, 0E8h, 63h, 05h
db 0E8h, 3Dh, 00h, 0E8h, 5Dh, 05h, 83h, 3Eh, 0C2h, 94h, 01h, 75h, 10h, 0B8h, 0Dh, 00h
db 50h, 33h, 0C0h, 50h, 33h, 0C0h, 50h, 0E8h, 0DDh, 57h, 83h, 0C4h, 06h, 83h, 3Eh, 0C2h
db 94h, 02h, 75h, 10h, 0B8h, 0Fh, 00h, 50h, 33h, 0C0h, 50h, 33h, 0C0h, 50h, 0E8h, 0C6h
db 57h, 83h, 0C4h, 06h, 83h, 3Eh, 0C2h, 94h, 03h, 75h, 03h, 0E8h, 0B3h, 38h
}
}


void sub_0_290(void)
{
asm {
db 83h, 3Eh, 9Ch, 0AFh, 01h, 75h, 5Ah, 83h, 3Eh, 0E8h, 0ADh, 03h, 75h
db 1Ah, 0B0h, 12h, 50h, 0B8h, 0D4h, 03h, 50h, 0E8h, 11h, 6Ch, 59h, 59h, 0B0h, 0B0h, 50h
db 0B8h, 0D5h, 03h, 50h, 0E8h, 05h, 6Ch, 59h, 59h, 0EBh, 39h, 83h, 3Eh, 0E8h, 0ADh, 05h
db 75h, 1Ah, 0B0h, 12h, 50h, 0B8h, 0D4h, 03h, 50h, 0E8h, 0F0h, 6Bh, 59h, 59h, 0B0h, 60h
db 50h, 0B8h, 0D5h, 03h, 50h, 0E8h, 0E4h, 6Bh, 59h, 59h, 0EBh, 18h, 0B0h, 06h, 50h, 0B8h
db 0D4h, 03h, 50h, 0E8h, 0D6h, 6Bh, 59h, 59h, 0B0h, 58h, 50h, 0B8h, 0D5h, 03h, 50h, 0E8h
db 0CAh, 6Bh, 59h, 59h, 83h, 3Eh, 9Ch, 0AFh, 02h, 75h, 21h, 0B0h, 13h, 50h, 0FFh, 36h
db 60h, 0C3h, 0E8h, 0B7h, 6Bh, 59h, 59h, 0B0h, 18h, 50h, 0A1h, 60h, 0C3h, 40h, 50h, 0E8h
db 0AAh, 6Bh, 59h, 59h, 0B4h, 10h, 0B0h, 01h, 0B7h, 05h, 0CDh, 10h
}
}


void sub_0_31E(void)
{
asm {
db 0C7h, 06h, 0E8h, 10h, 0FFh, 0FFh, 0B8h, 20h, 00h, 50h, 0B8h, 18h, 00h, 50h, 0B8h
db 27h, 00h, 50h, 33h, 0C0h, 50h, 33h, 0C0h, 50h, 0E8h, 23h, 2Fh, 83h, 0C4h, 0Ah, 0C7h
db 06h, 0E8h, 10h, 00h, 00h, 33h, 0C0h, 0A3h, 8Ah, 0AEh, 0A3h, 68h, 0AEh, 0A3h, 02h, 0AEh
}
}


void sub_0_352(void)
{
asm {
db 83h, 3Eh, 9Ch, 0AFh, 01h, 75h, 0Eh, 33h, 0C0h, 50h, 0B8h
db 0C8h, 03h, 50h, 0E8h, 70h, 47h, 59h, 59h, 0EBh, 12h, 33h, 0C0h, 50h, 0B8h, 0D4h, 03h
db 50h, 0E8h, 62h, 47h, 59h, 59h, 81h, 36h, 54h, 15h, 01h, 00h
}
}


void sub_0_37E(void)
{
asm {
db 0FFh, 36h, 9Ch, 0AFh, 0E8h, 0A7h, 2Ch, 59h, 0E8h, 04h, 0FFh
}
}


void sub_0_38E(void)
{
asm {
db 0E8h, 0EAh, 0FFh, 0E8h, 0A2h, 0FEh
}
}


void sub_0_399(void)
{
asm {
db 56h, 57h, 0B8h, 01h
db 00h, 50h, 0E8h, 0A2h, 65h, 59h, 0BBh, 00h, 01h, 99h, 0F7h, 0FBh, 8Bh, 0F0h, 0Bh, 0F6h
db 75h, 03h, 0E9h, 0BBh, 00h, 8Bh, 0FEh, 0B9h, 06h, 00h, 0BBh, 74h, 04h, 2Eh, 39h, 3Fh
db 74h, 07h, 43h, 43h, 0E2h, 0F7h, 0E9h, 0A2h, 00h, 2Eh, 0FFh, 67h, 0Ch, 0E8h, 6Fh, 29h
db 0E8h, 0B9h, 00h, 0E9h, 97h, 00h, 0E8h, 66h, 29h, 0E8h, 20h, 44h, 0E8h, 0B1h, 0FEh, 0E9h
db 8Bh, 00h, 0E8h, 5Ah, 29h, 0B8h, 01h, 00h, 50h, 0B8h, 12h, 00h, 50h, 0E8h, 0F9h, 2Eh
db 59h, 59h, 0B8h, 0E0h, 03h, 50h, 0E8h, 0F8h, 2Fh, 59h, 0E8h, 93h, 2Fh, 50h, 0E8h, 8Eh
db 63h, 59h, 0A2h, 68h, 0C3h, 80h, 3Eh, 68h, 0C3h, 59h, 75h, 0Ch, 0C7h, 06h, 0ECh, 94h
db 00h, 00h, 0C7h, 06h, 84h, 94h, 01h, 00h, 0EBh, 53h, 0E8h, 22h, 29h, 0B8h, 01h, 00h
db 50h, 0B8h, 07h, 00h, 50h, 0E8h, 0C1h, 2Eh, 59h, 59h, 0B8h, 0F2h, 03h, 50h, 0E8h, 0C0h
db 2Fh, 59h, 0E8h, 5Bh, 2Fh, 0EBh, 36h, 0E8h, 05h, 29h, 0B8h, 01h, 00h, 50h, 0B8h, 0Ch
db 00h, 50h, 0E8h, 0A4h, 2Eh, 59h, 59h, 0B8h, 0F9h, 03h, 50h, 0E8h, 0A3h, 2Fh, 59h, 0E8h
db 3Eh, 2Fh, 50h, 0E8h, 39h, 63h, 59h, 0A2h, 68h, 0C3h, 80h, 3Eh, 68h, 0C3h, 59h, 75h
db 08h, 0B8h, 05h, 04h, 50h, 0E8h, 0FDh, 39h, 59h, 0EBh, 02h, 0EBh, 03h, 0E8h, 0C9h, 0FDh
db 5Fh, 5Eh

db 5Dh, 0C3h
//}
//}


db 01h, 00h
db 3Bh, 00h
db 3Ch, 00h
db 3Dh, 00h
db 43h, 00h
db 44h, 00h
db 37h, 04h
db 0CDh, 03h
db 0D6h, 03h
db 0E2h, 03h
db 1Ah, 04h
db 37h, 04h


//void sub_0_48C(void)
//{
//asm {
db 55h, 8Bh, 0ECh

db 0B8h
db 15h, 00h, 50h, 0B8h, 24h, 00h, 50h, 0E8h, 4Fh, 2Eh, 59h, 59h, 0B8h, 06h, 04h, 50h
db 0E8h, 4Eh, 2Fh, 59h, 0E8h, 0E9h, 2Eh
}
}


void sub_0_4A9(void)
{
asm {
db 83h, 0ECh, 06h, 56h
db 0FFh, 36h, 9Ch, 0AFh, 0E8h, 78h, 2Bh, 59h, 83h, 3Eh, 9Ch, 0AFh, 02h, 75h, 03h, 0E8h
db 0CEh, 0FDh, 0B8h, 0Dh, 00h, 50h, 33h, 0C0h, 50h, 33h, 0C0h, 50h, 0E8h, 78h, 55h, 83h
db 0C4h, 06h, 0C7h, 06h, 0C2h, 94h, 01h, 00h, 33h, 0F6h, 0EBh, 5Bh, 0E8h, 77h, 50h, 0C7h
db 06h, 5Ch, 0C3h, 00h, 00h, 0B8h, 3Ah, 1Dh, 1Eh, 50h, 0B8h, 01h, 00h, 50h, 16h, 8Dh
db 46h, 0FAh, 50h, 0E8h, 32h, 26h, 83h, 0C4h, 06h, 8Dh, 46h, 0FAh, 16h, 50h, 0B9h, 06h
db 00h, 0E8h, 9Eh, 70h, 83h, 3Eh, 3Ch, 1Dh, 00h, 75h, 0Eh, 83h, 3Eh, 3Eh, 1Dh, 00h
db 75h, 07h, 83h, 3Eh, 0Ch, 0AFh, 00h, 74h, 08h, 0C7h, 06h, 1Eh, 82h, 01h, 00h, 0EBh
db 1Ch, 0C7h, 06h, 5Ch, 0C3h, 01h, 00h, 0B8h, 01h, 00h, 50h, 0E8h, 19h, 64h, 59h, 0Bh
db 0C0h, 74h, 03h, 0E8h, 63h, 0FEh, 46h, 81h, 0FEh, 2Ch, 01h, 7Ch, 9Fh, 0C7h, 06h, 0C2h
db 94h, 00h, 00h, 5Eh, 8Bh, 0E5h
}
}


void sub_0_548(void)
{
asm {
db 83h, 0ECh, 06h, 56h, 0FFh
db 36h, 9Ch, 0AFh, 0E8h, 0D9h, 2Ah, 59h, 0C7h, 06h, 0C2h, 94h, 02h, 00h, 0C7h, 06h, 02h
db 0AEh, 00h, 00h, 0C7h, 06h, 68h, 0AEh, 00h, 00h, 0B8h, 18h, 04h, 50h, 0E8h, 81h, 2Eh
db 59h, 33h, 0F6h, 0EBh, 5Bh, 0E8h, 0DEh, 4Fh, 0C7h, 06h, 5Ch, 0C3h, 00h, 00h, 0B8h, 3Ah
db 1Dh, 1Eh, 50h, 0B8h, 01h, 00h, 50h, 16h, 8Dh, 46h, 0FAh, 50h, 0E8h, 99h, 25h, 83h
db 0C4h, 06h, 8Dh, 46h, 0FAh, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 05h, 70h, 83h, 3Eh, 3Ch
db 1Dh, 00h, 75h, 0Eh, 83h, 3Eh, 3Eh, 1Dh, 00h, 75h, 07h, 83h, 3Eh, 0Ch, 0AFh, 00h
db 74h, 08h, 0C7h, 06h, 1Eh, 82h, 01h, 00h, 0EBh, 1Ch, 0C7h, 06h, 5Ch, 0C3h, 01h, 00h
db 0B8h, 01h, 00h, 50h, 0E8h, 80h, 63h, 59h, 0Bh, 0C0h, 74h, 03h, 0E8h, 0CAh, 0FDh, 46h
db 81h, 0FEh, 2Ch, 01h, 7Ch, 9Fh, 0E8h, 66h, 27h, 0C7h, 06h, 02h, 0AEh, 14h, 00h, 0C7h
db 06h, 68h, 0AEh, 18h, 00h, 0E8h, 0A8h, 2Dh, 0C7h, 06h, 5Ch, 0C3h, 01h, 00h, 0C7h, 06h
db 0C2h, 94h, 00h, 00h, 5Eh, 8Bh, 0E5h
}
}


void sub_0_5F9(void)
{
asm {
db 83h, 0ECh, 0Ch, 56h
db 0C7h, 06h, 1Eh, 82h, 00h, 00h, 0C7h, 06h, 5Ch, 0C3h, 00h, 00h, 0B8h, 3Ah, 1Dh, 1Eh
db 50h, 0B8h, 01h, 00h, 50h, 16h, 8Dh, 46h, 0FAh, 50h, 0E8h, 0Bh, 25h, 83h, 0C4h, 06h
db 8Dh, 46h, 0FAh, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 77h, 6Fh, 83h, 3Eh, 3Ch, 1Dh, 00h
db 75h, 0Eh, 83h, 3Eh, 3Eh, 1Dh, 00h, 75h, 07h, 83h, 3Eh, 0Ch, 0AFh, 00h, 74h, 06h
db 0C7h, 06h, 1Eh, 82h, 01h, 00h, 0B8h, 01h, 00h, 50h, 0E8h, 0FAh, 62h, 59h, 0Bh, 0C0h
db 74h, 03h, 0E8h, 44h, 0FDh, 0E9h, 0A5h, 00h, 0E8h, 4Eh, 0FEh, 83h, 3Eh, 1Eh, 82h, 00h
db 74h, 03h, 0E9h, 0A2h, 00h, 33h, 0C0h, 0BAh, 0FFh, 7Fh, 50h, 52h, 0E8h, 0B1h, 5Fh, 99h
db 52h, 50h, 0E8h, 0FEh, 6Dh, 40h, 8Bh, 0F0h, 56h, 0E8h, 0E5h, 25h, 59h, 0C7h, 06h, 38h
db 1Dh, 01h, 00h, 0E8h, 0A6h, 0Fh, 83h, 3Eh, 1Eh, 82h, 00h, 74h, 02h, 0EBh, 78h, 0C7h
db 06h, 0C2h, 94h, 03h, 00h, 0E8h, 0A9h, 34h, 33h, 0F6h, 0EBh, 5Bh, 0E8h, 0B7h, 4Eh, 0C7h
db 06h, 5Ch, 0C3h, 00h, 00h, 0B8h, 3Ah, 1Dh, 1Eh, 50h, 0B8h, 01h, 00h, 50h, 16h, 8Dh
db 46h, 0F4h, 50h, 0E8h, 72h, 24h, 83h, 0C4h, 06h, 8Dh, 46h, 0F4h, 16h, 50h, 0B9h, 06h
db 00h, 0E8h, 0DEh, 6Eh, 83h, 3Eh, 3Ch, 1Dh, 00h, 75h, 0Eh, 83h, 3Eh, 3Eh, 1Dh, 00h
db 75h, 07h, 83h, 3Eh, 0Ch, 0AFh, 00h, 74h, 08h, 0C7h, 06h, 1Eh, 82h, 01h, 00h, 0EBh
db 1Ch, 0C7h, 06h, 5Ch, 0C3h, 01h, 00h, 0B8h, 01h, 00h, 50h, 0E8h, 59h, 62h, 59h, 0Bh
db 0C0h, 74h, 03h, 0E8h, 0A3h, 0FCh, 46h, 81h, 0FEh, 2Ch, 01h, 7Ch, 9Fh, 83h, 3Eh, 1Eh
db 82h, 00h, 75h, 03h, 0E9h, 51h, 0FFh, 0C7h, 06h, 6Eh, 0AEh, 01h, 00h, 0C7h, 06h, 38h
db 1Dh, 01h, 00h, 0C7h, 06h, 5Ch, 0C3h, 00h, 00h, 5Eh, 8Bh, 0E5h
}
}


void sub_0_71E(void)
{
asm {
db 83h, 0ECh, 06h, 56h, 0B8h, 04h, 00h, 50h, 0B8h, 0Bh, 00h, 50h, 0E8h, 0B9h, 2Bh
db 59h, 59h, 0B8h, 2Ch, 04h, 50h, 0E8h, 0B8h, 2Ch, 59h, 0B8h, 07h, 00h, 50h, 0E8h, 0C5h
db 4Bh, 59h, 0E8h, 0B2h, 4Ch, 33h, 0F6h, 0EBh, 04h, 0E8h, 0Ah, 4Eh, 46h, 83h, 0FEh, 78h
db 7Ch, 0F7h, 0C7h, 06h, 0C2h, 94h, 03h, 00h, 0E8h, 28h, 35h, 33h, 0F6h, 0EBh, 49h, 0E8h
db 0F4h, 4Dh, 0B8h, 3Ah, 1Dh, 1Eh, 50h, 0B8h, 01h, 00h, 50h, 16h, 8Dh, 46h, 0FAh, 50h
db 0E8h, 0B5h, 23h, 83h, 0C4h, 06h, 8Dh, 46h, 0FAh, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 21h
db 6Eh, 83h, 3Eh, 3Ch, 1Dh, 00h, 75h, 0Eh, 83h, 3Eh, 3Eh, 1Dh, 00h, 75h, 07h, 83h
db 3Eh, 0Ch, 0AFh, 00h, 74h, 02h, 0EBh, 16h, 0B8h, 01h, 00h, 50h, 0E8h, 0A8h, 61h, 59h
db 0Bh, 0C0h, 74h, 03h, 0E8h, 0F2h, 0FBh, 46h, 81h, 0FEh, 2Ch, 01h, 7Ch, 0B1h, 5Eh, 8Bh
db 0E5h
}
}


void sub_0_7B3(void)
{
asm {
db 56h, 0E8h, 8Ch, 16h, 0FFh, 06h, 0CAh, 94h, 83h, 3Eh
db 54h, 15h, 00h, 74h, 1Dh, 33h, 0F6h, 0EBh, 11h, 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 9Fh, 20h
db 7Bh, 0D1h, 0E3h, 0C7h, 87h, 0D2h, 7Fh, 0FFh, 0FFh, 46h, 3Bh, 36h, 9Eh, 94h, 7Ch, 0E9h
db 0EBh, 1Bh, 33h, 0F6h, 0EBh, 11h, 8Bh, 0DEh, 0D1h, 0E3h, 8Bh, 9Fh, 20h, 7Bh, 0D1h, 0E3h
db 0C7h, 87h, 2Ah, 82h, 0FFh, 0FFh, 46h, 3Bh, 36h, 9Eh, 94h, 7Ch, 0E9h, 83h, 3Eh, 9Ch
db 0AFh, 02h, 75h, 06h, 81h, 36h, 54h, 15h, 01h, 00h, 5Eh
}
}


void sub_0_80D(void)
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


void sub_0_AA7(void)
{
asm {
db 0B8h, 3Eh, 04h, 50h, 0E8h, 0B4h
db 33h, 59h
}
}


void sub_0_AB4(void)
{
asm {
db 56h, 0BEh, 01h, 00h, 0C7h, 06h, 24h, 82h, 08h
db 95h, 0EBh, 06h, 46h, 83h, 06h, 24h, 82h, 32h, 8Bh, 1Eh, 24h, 82h, 83h, 7Fh, 16h
db 00h, 74h, 06h, 3Bh, 36h, 22h, 82h, 7Ch, 0EAh, 3Bh, 36h, 22h, 82h, 7Ch, 04h, 0FFh
db 06h, 22h, 82h, 8Bh, 1Eh, 24h, 82h, 33h, 0C0h, 89h, 47h, 20h, 8Bh, 1Eh, 24h, 82h
db 89h, 47h, 22h, 8Bh, 1Eh, 24h, 82h, 89h, 47h, 1Eh, 8Bh, 1Eh, 24h, 82h, 89h, 47h
db 1Ch, 8Bh, 1Eh, 24h, 82h, 0C7h, 47h, 30h, 0A7h, 0Ah, 5Eh
}
}


void sub_0_B0D(void)
{
asm {
db 33h, 0C0h, 0BAh, 00h, 01h, 50h, 52h, 8Bh, 1Eh, 28h, 82h, 0FFh, 77h, 02h, 0FFh, 37h
db 0E8h, 50h, 69h, 0A3h, 2Ch, 1Dh, 33h, 0C0h, 0BAh, 00h, 01h, 50h, 52h, 8Bh, 1Eh, 28h
db 82h, 0FFh, 77h, 06h, 0FFh, 77h, 04h, 0E8h, 39h, 69h, 0A3h, 2Eh, 1Dh, 83h, 3Eh, 9Ch
db 0AFh, 01h, 75h, 1Dh, 0A1h, 0A0h, 94h, 0D1h, 0E0h, 50h, 0A1h, 2Ch, 1Dh, 0BBh, 02h, 00h
db 99h, 0F7h, 0FBh, 0BBh, 02h, 00h, 99h, 0F7h, 0FBh, 58h, 03h, 0C2h, 0A3h, 26h, 82h, 0EBh
db 23h, 0A1h, 0A0h, 94h, 0D1h, 0E0h, 0D1h, 0E0h, 50h, 0A1h, 2Ch, 1Dh, 0BBh, 02h, 00h, 99h
db 0F7h, 0FBh, 0BBh, 04h, 00h, 99h, 0F7h, 0FBh, 58h, 03h, 0C2h, 0A3h, 26h, 82h, 81h, 26h
db 2Ch, 1Dh, 0F9h, 0FFh, 8Bh, 1Eh, 28h, 82h, 0A1h, 26h, 82h, 89h, 47h, 18h, 0B8h, 0A4h
db 0C3h, 1Eh, 50h, 0A1h, 26h, 82h, 0B1h, 05h, 0D3h, 0E0h, 05h, 0C8h, 0C3h, 1Eh, 50h, 0B9h
db 20h, 00h, 0E8h, 0FDh, 69h, 0A1h, 2Ch, 1Dh, 03h, 06h, 0B0h, 0C3h, 0A3h, 34h, 1Dh, 0A1h
db 2Ch, 1Dh, 03h, 06h, 0B4h, 0C3h, 0A3h, 28h, 1Dh, 0A1h, 2Eh, 1Dh, 03h, 06h, 0B2h, 0C3h
db 0A3h, 36h, 1Dh, 0A1h, 2Eh, 1Dh, 03h, 06h, 0B6h, 0C3h, 0A3h, 2Ah, 1Dh
}
}


void sub_0_BCF(void)
{
asm {
db 8Bh, 1Eh, 0D0h, 7Fh, 8Bh, 47h, 1Ch, 3Bh, 06h, 28h, 1Dh, 7Eh, 04h, 33h
db 0C0h, 0EBh, 38h, 8Bh, 1Eh, 0D0h, 7Fh, 8Bh, 47h, 1Eh, 3Bh, 06h, 2Ah, 1Dh, 7Eh, 04h
db 33h, 0C0h, 0EBh, 27h, 8Bh, 1Eh, 0D0h, 7Fh, 8Bh, 47h, 20h, 3Bh, 06h, 34h, 1Dh, 7Dh
db 04h, 33h, 0C0h, 0EBh, 16h, 8Bh, 1Eh, 0D0h, 7Fh, 8Bh, 47h, 22h, 3Bh, 06h, 36h, 1Dh
db 7Dh, 04h, 33h, 0C0h, 0EBh, 05h, 0B8h, 01h, 00h, 0EBh, 00h
}
}


void sub_0_C1D(void)
{
asm {
db 8Bh, 46h, 06h, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 0F7h, 2Eh, 7Eh, 94h, 50h, 8Bh, 46h
db 04h, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 5Ah, 03h, 0D0h, 0D1h, 0E2h, 8Bh, 1Eh, 86h, 94h
db 03h, 0DAh, 8Bh, 07h, 0EBh, 00h
}
}


void sub_0_C48(void)
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


void sub_0_CBE(void)
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


void sub_0_E52(void)
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
asm {
db 83h, 0ECh, 24h, 56h
db 57h, 33h, 0FFh, 33h, 0F6h, 8Bh, 1Eh, 28h, 82h, 83h, 7Fh, 02h, 00h, 7Fh, 15h, 7Ch
db 06h, 81h, 3Fh, 00h, 10h, 73h, 0Dh, 8Bh, 1Eh, 28h, 82h, 0C7h, 07h, 00h, 10h, 0C7h
db 47h, 02h, 00h, 00h, 8Bh, 1Eh, 0C8h, 94h, 8Bh, 07h, 48h, 99h, 0B1h, 08h, 0E8h, 0D0h
db 65h, 0B1h, 04h, 0E8h, 0CBh, 65h, 8Bh, 1Eh, 28h, 82h, 3Bh, 57h, 02h, 7Fh, 21h, 7Ch
db 04h, 3Bh, 07h, 73h, 1Bh, 8Bh, 1Eh, 0C8h, 94h, 8Bh, 07h, 48h, 99h, 0B1h, 08h, 0E8h
db 0AFh, 65h, 0B1h, 04h, 0E8h, 0AAh, 65h, 8Bh, 1Eh, 28h, 82h, 89h, 07h, 89h, 57h, 02h
db 8Bh, 1Eh, 0C8h, 94h, 8Bh, 47h, 02h, 99h, 0B1h, 08h, 0E8h, 94h, 65h, 0B1h, 04h, 0E8h
db 8Fh, 65h, 8Bh, 1Eh, 28h, 82h, 3Bh, 57h, 06h, 7Fh, 1Eh, 7Ch, 05h, 3Bh, 47h, 04h
db 73h, 17h, 8Bh, 1Eh, 28h, 82h, 0C7h, 47h, 16h, 00h, 00h, 0B8h, 05h, 00h, 50h, 0E8h
db 54h, 43h, 59h, 0E8h, 41h, 44h, 0E9h, 0C4h, 04h, 8Dh, 46h, 0F0h, 16h, 50h, 0A1h, 0BAh
db 94h, 40h, 50h, 16h, 8Dh, 46h, 0EAh, 50h, 0E8h, 5Dh, 1Bh, 83h, 0C4h, 06h, 8Dh, 46h
db 0EAh, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 0C9h, 65h, 83h, 7Eh, 0F2h, 00h, 74h, 48h, 83h
db 3Eh, 0C4h, 94h, 00h, 75h, 28h, 83h, 3Eh, 90h, 94h, 00h, 75h, 21h, 0B8h, 04h, 00h
db 50h, 0E8h, 12h, 43h, 59h, 0A1h, 32h, 1Dh, 0F7h, 0D8h, 8Bh, 0F0h, 0C7h, 06h, 0C4h, 94h
db 01h, 00h, 0C7h, 06h, 90h, 94h, 01h, 00h, 0A1h, 82h, 94h, 0A3h, 76h, 84h, 83h, 3Eh
db 90h, 94h, 00h, 74h, 10h, 83h, 3Eh, 76h, 84h, 00h, 7Eh, 09h, 2Bh, 36h, 76h, 84h
db 83h, 2Eh, 76h, 84h, 07h, 0EBh, 13h, 0C7h, 06h, 76h, 84h, 00h, 00h, 83h, 3Eh, 0C4h
db 94h, 00h, 75h, 06h, 0C7h, 06h, 90h, 94h, 00h, 00h, 83h, 7Eh, 0F4h, 00h, 74h, 07h
db 0C7h, 46h, 0FCh, 03h, 00h, 0EBh, 05h, 0C7h, 46h, 0FCh, 02h, 00h, 0B8h, 92h, 94h, 1Eh
db 50h, 8Dh, 46h, 0F0h, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 46h, 65h, 8Bh, 5Eh, 0F0h, 4Bh
db 83h, 0FBh, 06h, 77h, 29h, 0D1h, 0E3h, 2Eh, 0FFh, 0A7h, 83h, 14h, 0A1h, 80h, 94h, 0F7h
db 6Eh, 0FCh, 8Bh, 0F8h, 0C7h, 06h, 7Ah, 84h, 00h, 00h, 0EBh, 12h, 0A1h, 80h, 94h, 0F7h
db 0D8h, 0F7h, 6Eh, 0FCh, 8Bh, 0F8h, 0C7h, 06h, 7Ah, 84h, 01h, 00h, 0EBh, 00h, 03h, 36h
db 30h, 1Dh, 83h, 3Eh, 0C4h, 94h, 00h, 74h, 17h, 83h, 3Eh, 7Ah, 84h, 00h, 74h, 08h
db 0C7h, 06h, 0A0h, 94h, 0Ah, 00h, 0EBh, 06h, 0C7h, 06h, 0A0h, 94h, 09h, 00h, 0EBh, 6Fh
db 8Bh, 1Eh, 28h, 82h, 8Bh, 47h, 0Eh, 0FFh, 4Fh, 0Eh, 0Bh, 0C0h, 75h, 26h, 8Bh, 1Eh
db 28h, 82h, 8Bh, 47h, 10h, 8Bh, 1Eh, 28h, 82h, 89h, 47h, 0Eh, 8Bh, 1Eh, 28h, 82h
db 0FFh, 47h, 0Ah, 8Bh, 47h, 0Ah, 3Dh, 04h, 00h, 75h, 09h, 8Bh, 1Eh, 28h, 82h, 0C7h
db 47h, 0Ah, 00h, 00h, 0Bh, 0FFh, 75h, 17h, 8Bh, 1Eh, 28h, 82h, 0C7h, 47h, 0Ah, 01h
db 00h, 8Bh, 1Eh, 28h, 82h, 8Bh, 47h, 10h, 8Bh, 1Eh, 28h, 82h, 89h, 47h, 0Eh, 83h
db 3Eh, 7Ah, 84h, 00h, 74h, 0Fh, 8Bh, 1Eh, 28h, 82h, 8Bh, 47h, 0Ah, 05h, 05h, 00h
db 0A3h, 0A0h, 94h, 0EBh, 0Ah, 8Bh, 1Eh, 28h, 82h, 8Bh, 47h, 0Ah, 0A3h, 0A0h, 94h, 0E8h
db 0EBh, 0F9h, 0A1h, 2Ah, 1Dh, 40h, 50h, 0FFh, 36h, 34h, 1Dh, 0E8h, 0EFh, 0FAh, 59h, 59h
db 89h, 46h, 0FAh, 0A1h, 2Ah, 1Dh, 40h, 50h, 0FFh, 36h, 28h, 1Dh, 0E8h, 0DEh, 0FAh, 59h
db 59h, 89h, 46h, 0F8h, 8Bh, 5Eh, 0FAh, 0D1h, 0E3h, 83h, 0BFh, 00h, 03h, 00h, 74h, 25h
db 83h, 3Eh, 76h, 84h, 00h, 75h, 12h, 8Bh, 5Eh, 0FAh, 0D1h, 0E3h, 83h, 0BFh, 0A8h, 00h
db 00h, 74h, 06h, 0C7h, 06h, 0C4h, 94h, 00h, 00h, 8Bh, 5Eh, 0FAh, 0D1h, 0E3h, 8Bh, 87h
db 00h, 03h, 89h, 46h, 0F6h, 8Bh, 5Eh, 0F8h, 0D1h, 0E3h, 83h, 0BFh, 00h, 03h, 00h, 74h
db 25h, 83h, 3Eh, 76h, 84h, 00h, 75h, 12h, 8Bh, 5Eh, 0F8h, 0D1h, 0E3h, 83h, 0BFh, 0A8h
db 00h, 00h, 74h, 06h, 0C7h, 06h, 0C4h, 94h, 00h, 00h, 8Bh, 5Eh, 0F8h, 0D1h, 0E3h, 8Bh
db 87h, 00h, 03h, 89h, 46h, 0F6h, 8Bh, 1Eh, 28h, 82h, 01h, 7Fh, 12h, 8Bh, 1Eh, 28h
db 82h, 01h, 77h, 14h, 33h, 0C0h, 0BAh, 00h, 01h, 50h, 52h, 8Bh, 1Eh, 28h, 82h, 8Bh
db 47h, 12h, 99h, 50h, 8Bh, 46h, 0F6h, 52h, 99h, 59h, 5Bh, 0E8h, 75h, 66h, 52h, 50h
db 0E8h, 0A0h, 62h, 8Bh, 1Eh, 28h, 82h, 89h, 47h, 12h, 8Bh, 1Eh, 28h, 82h, 8Bh, 47h
db 14h, 0BAh, 09h, 00h, 0F7h, 0EAh, 0BBh, 0Ah, 00h, 99h, 0F7h, 0FBh, 8Bh, 1Eh, 28h, 82h
db 89h, 47h, 14h, 8Bh, 1Eh, 28h, 82h, 8Bh, 77h, 14h, 0E8h, 0C1h, 0FAh, 8Bh, 1Eh, 28h
db 82h, 83h, 7Fh, 14h, 00h, 75h, 0Ch, 0Bh, 0F6h, 7Dh, 08h, 0B8h, 03h, 00h, 50h, 0E8h
db 0F4h, 40h, 59h, 8Bh, 1Eh, 28h, 82h, 83h, 7Fh, 14h, 00h, 74h, 06h, 0C7h, 06h, 0C4h
db 94h, 01h, 00h, 8Bh, 1Eh, 28h, 82h, 83h, 7Fh, 14h, 00h, 7Ch, 06h, 0C7h, 06h, 76h
db 84h, 00h, 00h, 8Bh, 1Eh, 28h, 82h, 8Bh, 7Fh, 12h, 8Bh, 1Eh, 28h, 82h, 8Bh, 77h
db 14h, 8Bh, 0C7h, 99h, 8Bh, 1Eh, 28h, 82h, 01h, 07h, 11h, 57h, 02h, 8Bh, 0C6h, 99h
db 8Bh, 1Eh, 28h, 82h, 01h, 47h, 04h, 11h, 57h, 06h, 0Bh, 0FFh, 7Eh, 4Dh, 8Bh, 1Eh
db 28h, 82h, 8Bh, 47h, 02h, 8Bh, 17h, 2Bh, 16h, 0CCh, 94h, 1Bh, 06h, 0CEh, 94h, 0Bh
db 0C0h, 7Ch, 38h, 7Fh, 06h, 81h, 0FAh, 00h, 0B4h, 76h, 30h, 8Bh, 0C7h, 99h, 01h, 06h
db 0CCh, 94h, 11h, 16h, 0CEh, 94h, 0A1h, 0CEh, 94h, 8Bh, 16h, 0CCh, 94h, 3Bh, 06h, 0D8h
db 0ADh, 7Ch, 16h, 7Fh, 06h, 3Bh, 16h, 0D6h, 0ADh, 76h, 0Eh, 0A1h, 0D8h, 0ADh, 8Bh, 16h
db 0D6h, 0ADh, 89h, 16h, 0CCh, 94h, 0A3h, 0CEh, 94h, 0EBh, 4Fh, 0Bh, 0FFh, 7Dh, 4Bh, 8Bh
db 1Eh, 28h, 82h, 8Bh, 47h, 02h, 8Bh, 17h, 2Bh, 16h, 0CCh, 94h, 1Bh, 06h, 0CEh, 94h
db 0Bh, 0C0h, 7Fh, 36h, 7Ch, 06h, 81h, 0FAh, 00h, 8Ch, 73h, 2Eh, 8Bh, 0C7h, 99h, 01h
db 06h, 0CCh, 94h, 11h, 16h, 0CEh, 94h, 0A1h, 0CEh, 94h, 8Bh, 16h, 0CCh, 94h, 3Bh, 06h
db 4Eh, 15h, 7Fh, 16h, 7Ch, 06h, 3Bh, 16h, 4Ch, 15h, 73h, 0Eh, 0A1h, 4Eh, 15h, 8Bh
db 16h, 4Ch, 15h, 89h, 16h, 0CCh, 94h, 0A3h, 0CEh, 94h, 0Bh, 0F6h, 7Eh, 4Eh, 8Bh, 1Eh
db 28h, 82h, 8Bh, 47h, 06h, 8Bh, 57h, 04h, 2Bh, 16h, 0D0h, 94h, 1Bh, 06h, 0D2h, 94h
db 0Bh, 0C0h, 7Ch, 38h, 7Fh, 06h, 81h, 0FAh, 00h, 0A0h, 76h, 30h, 8Bh, 0C6h, 99h, 01h
db 06h, 0D0h, 94h, 11h, 16h, 0D2h, 94h, 0A1h, 0D2h, 94h, 8Bh, 16h, 0D0h, 94h, 3Bh, 06h
db 0DCh, 0ADh, 7Ch, 16h, 7Fh, 06h, 3Bh, 16h, 0DAh, 0ADh, 76h, 0Eh, 0A1h, 0DCh, 0ADh, 8Bh
db 16h, 0DAh, 0ADh, 89h, 16h, 0D0h, 94h, 0A3h, 0D2h, 94h, 0EBh, 50h, 0Bh, 0F6h, 7Dh, 4Ch
db 8Bh, 1Eh, 28h, 82h, 8Bh, 47h, 06h, 8Bh, 57h, 04h, 2Bh, 16h, 0D0h, 94h, 1Bh, 06h
db 0D2h, 94h, 0Bh, 0C0h, 7Fh, 36h, 7Ch, 06h, 81h, 0FAh, 00h, 28h, 73h, 2Eh, 8Bh, 0C6h
db 99h, 01h, 06h, 0D0h, 94h, 11h, 16h, 0D2h, 94h, 0A1h, 0D2h, 94h, 8Bh, 16h, 0D0h, 94h
db 3Bh, 06h, 52h, 15h, 7Fh, 16h, 7Ch, 06h, 3Bh, 16h, 50h, 15h, 73h, 0Eh, 0A1h, 52h
db 15h, 8Bh, 16h, 50h, 15h, 89h, 16h, 0D0h, 94h, 0A3h, 0D2h, 94h, 0E8h, 6Eh, 0F7h, 0E8h
db 6Bh, 0F4h, 0A1h, 34h, 1Dh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0E4h, 0A1h, 36h
db 1Dh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 89h, 46h, 0E2h, 0A1h, 28h, 1Dh, 0BBh, 10h, 00h
db 99h, 0F7h, 0FBh, 89h, 46h, 0E0h, 0A1h, 2Ah, 1Dh, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 89h
db 46h, 0DEh, 8Bh, 46h, 0E2h, 89h, 46h, 0E6h, 0EBh, 65h, 8Bh, 46h, 0E4h, 89h, 46h, 0E8h
db 0EBh, 52h, 8Bh, 46h, 0E6h, 0F7h, 2Eh, 7Eh, 94h, 03h, 46h, 0E8h, 0D1h, 0E0h, 8Bh, 1Eh
db 86h, 94h, 03h, 0D8h, 8Bh, 07h, 89h, 46h, 0DCh, 8Bh, 5Eh, 0DCh, 0D1h, 0E3h, 8Bh, 87h
db 70h, 01h, 3Dh, 01h, 00h, 74h, 02h, 0EBh, 28h, 0B8h, 06h, 00h, 50h, 0E8h, 0F6h, 3Eh
db 59h, 0B8h, 64h, 00h, 50h, 0E8h, 79h, 00h, 59h, 8Bh, 46h, 0E6h, 0F7h, 2Eh, 7Eh, 94h
db 03h, 46h, 0E8h, 0D1h, 0E0h, 8Bh, 1Eh, 86h, 94h, 03h, 0D8h, 0C7h, 07h, 00h, 00h, 0EBh
db 00h, 0FFh, 46h, 0E8h, 8Bh, 46h, 0E8h, 3Bh, 46h, 0E0h, 7Eh, 0A6h, 0FFh, 46h, 0E6h, 8Bh
db 46h, 0E6h, 3Bh, 46h, 0DEh, 7Eh, 93h, 0C7h, 06h, 0D0h, 7Fh, 08h, 95h, 0C7h, 46h, 0FEh
db 01h, 00h, 0EBh, 20h, 8Bh, 1Eh, 0D0h, 7Fh, 83h, 7Fh, 28h, 00h, 74h, 0Eh, 0E8h, 6Eh
db 0F7h, 0Bh, 0C0h, 74h, 07h, 8Bh, 1Eh, 0D0h, 7Fh, 0FFh, 57h, 2Ch, 0FFh, 46h, 0FEh, 83h
db 06h, 0D0h, 7Fh, 32h, 8Bh, 46h, 0FEh, 3Bh, 06h, 22h, 82h, 7Ch, 0D7h, 5Fh, 5Eh, 8Bh
db 0E5h

db 5Dh, 0C3h
//}
//}


db 6Ch, 10h
db 6Ch, 10h
db 6Ch, 10h
db 8Eh, 10h
db 7Ch, 10h
db 7Ch, 10h
db 7Ch, 10h


//void sub_0_1491(void)
//{
//asm {
db 55h, 8Bh, 0ECh

db 8Bh, 46h, 04h, 99h, 01h, 06h, 0E2h, 0ADh, 11h, 16h, 0E4h, 0ADh
db 0A1h, 0E4h, 0ADh, 8Bh, 16h, 0E2h, 0ADh, 81h, 0C2h, 0F0h, 0D8h, 15h, 0FFh, 0FFh, 3Bh, 06h
db 0C0h, 94h, 7Ch, 17h, 75h, 06h, 3Bh, 16h, 0BEh, 94h, 72h, 0Fh, 81h, 06h, 0BEh, 94h
db 10h, 27h, 83h, 16h, 0C0h, 94h, 00h, 0FFh, 06h, 84h, 94h
}
}


void sub_0_14CD(void)
{
asm {
db 83h, 0ECh, 06h, 0E8h, 7Eh, 08h, 33h, 0C0h, 0A3h, 0D4h, 94h, 0A3h, 9Eh, 94h, 0A3h, 0BCh
db 94h, 0C7h, 06h, 0C6h, 94h, 7Ch, 84h, 0E8h, 6Ch, 40h, 0C7h, 06h, 28h, 82h, 0D6h, 94h
db 0E8h, 26h, 0FAh, 0E8h, 0BDh, 0F2h, 0E8h, 0A0h, 0EEh, 0A1h, 5Ch, 0C3h, 0Bh, 0C0h, 74h, 10h
db 3Dh, 01h, 00h, 75h, 03h, 0E9h, 0BFh, 00h, 3Dh, 02h, 00h, 74h, 60h, 0E9h, 07h, 01h
db 83h, 3Eh, 0F6h, 0AEh, 00h, 74h, 53h, 83h, 3Eh, 0C2h, 0AEh, 00h, 74h, 4Ch, 83h, 3Eh
db 0Ch, 0AFh, 00h, 74h, 45h, 0E8h, 17h, 18h, 0B8h, 01h, 00h, 50h, 0B8h, 0Fh, 00h, 50h
db 0E8h, 75h, 1Dh, 59h, 59h, 0B8h, 48h, 04h, 50h, 0E8h, 0B5h, 1Eh, 59h, 0E8h, 50h, 1Eh
db 0A2h, 68h, 0C3h, 80h, 3Eh, 68h, 0C3h, 30h, 7Eh, 17h, 80h, 3Eh, 68h, 0C3h, 39h, 7Fh
db 10h, 0A0h, 68h, 0C3h, 98h, 05h, 0CFh, 0FFh, 0A3h, 6Eh, 0AEh, 0C7h, 06h, 38h, 1Dh, 01h
db 00h, 0E8h, 0F0h, 07h, 0E8h, 4Ch, 0F2h, 0E8h, 49h, 0F2h, 0E9h, 0AAh, 00h, 83h, 3Eh, 0ECh
db 94h, 00h, 74h, 0Eh, 83h, 3Eh, 1Eh, 0AFh, 00h, 74h, 4Ah, 83h, 3Eh, 0DAh, 0AEh, 00h
db 74h, 43h, 0E8h, 0BAh, 17h, 0B8h, 01h, 00h, 50h, 0B8h, 0Fh, 00h, 50h, 0E8h, 18h, 1Dh
db 59h, 59h, 0B8h, 57h, 04h, 50h, 0E8h, 58h, 1Eh, 59h, 0E8h, 0F3h, 1Dh, 0A2h, 68h, 0C3h
db 80h, 3Eh, 68h, 0C3h, 30h, 7Eh, 07h, 80h, 3Eh, 68h, 0C3h, 39h, 7Fh, 00h, 0A0h, 68h
db 0C3h, 98h, 05h, 0D0h, 0FFh, 50h, 0E8h, 19h, 17h, 59h, 0E8h, 97h, 07h, 0E8h, 0F3h, 0F1h
db 0E8h, 0F0h, 0F1h, 0EBh, 63h, 0EBh, 50h, 0C7h, 06h, 5Ch, 0C3h, 00h, 00h, 0B8h, 3Ah, 1Dh
db 1Eh, 50h, 0B8h, 01h, 00h, 50h, 16h, 8Dh, 46h, 0FAh, 50h, 0E8h, 4Ah, 15h, 83h, 0C4h
db 06h, 8Dh, 46h, 0FAh, 16h, 50h, 0B9h, 06h, 00h, 0E8h, 0B6h, 5Fh, 83h, 3Eh, 3Ch, 1Dh
db 00h, 75h, 0Eh, 83h, 3Eh, 3Eh, 1Dh, 00h, 75h, 07h, 83h, 3Eh, 0Ch, 0AFh, 00h, 74h
db 0Eh, 0C7h, 06h, 5Ch, 0C3h, 01h, 00h, 0C7h, 06h, 1Eh, 82h, 01h, 00h, 0EBh, 19h, 0C7h
db 06h, 5Ch, 0C3h, 01h, 00h, 0EBh, 00h, 83h, 3Eh, 38h, 1Dh, 00h, 75h, 0Ah, 83h, 3Eh
db 0ECh, 94h, 00h, 74h, 03h, 0E9h, 0AEh, 0FEh, 8Bh, 0E5h
}
}


void sub_0_162C(void)
{
asm {
db 83h
db 0ECh, 06h, 56h, 0C7h, 06h, 54h, 15h, 00h, 00h, 0C7h, 06h, 84h, 94h, 04h, 00h, 0C7h
db 06h, 0BEh, 94h, 00h, 00h, 0C7h, 06h, 0C0h, 94h, 00h, 00h, 0C7h, 06h, 0E2h, 0ADh, 00h
db 00h, 0C7h, 06h, 0E4h, 0ADh, 00h, 00h, 0C7h, 06h, 38h, 1Dh, 01h, 00h, 0E8h, 0F4h, 06h
db 0C7h, 06h, 0CAh, 94h, 00h, 00h, 83h, 3Eh, 38h, 1Dh, 00h, 75h, 03h, 0E9h, 41h, 01h
db 0C7h, 06h, 7Ch, 84h, 0FFh, 0FFh, 0C7h, 06h, 0C6h, 94h, 7Ch, 84h, 0C7h, 06h, 0BCh, 94h
db 00h, 00h, 0C7h, 06h, 9Eh, 94h, 00h, 00h, 0C7h, 06h, 0D4h, 94h, 00h, 00h, 0B8h, 66h
db 04h, 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 68h, 59h, 59h, 59h, 0B8h, 0Ah, 00h, 50h, 8Dh
db 46h, 0FAh, 50h, 0FFh, 36h, 6Eh, 0AEh, 0E8h, 0B9h, 55h, 83h, 0C4h, 06h, 8Dh, 46h, 0FAh
db 50h, 0B8h, 10h, 0AEh, 50h, 0E8h, 10h, 59h, 59h, 59h, 0B8h, 6Dh, 04h, 50h, 0B8h, 10h
db 0AEh, 50h, 0E8h, 03h, 59h, 59h, 59h, 0FFh, 36h, 46h, 0Ch, 0B8h, 10h, 0AEh, 50h, 0E8h
db 0F6h, 58h, 59h, 59h, 1Eh, 0FFh, 36h, 0C8h, 94h, 0B8h, 10h, 0AEh, 50h, 0E8h, 0F7h, 16h
db 83h, 0C4h, 06h, 33h, 0F6h, 0EBh, 1Bh, 8Bh, 1Eh, 0C8h, 94h, 8Bh, 47h, 0Eh, 0F7h, 0EEh
db 8Bh, 16h, 0C8h, 94h, 03h, 0D0h, 83h, 0C2h, 20h, 8Bh, 0DEh, 0D1h, 0E3h, 89h, 97h, 86h
db 94h, 46h, 8Bh, 1Eh, 0C8h, 94h, 39h, 77h, 04h, 7Fh, 0DCh, 0C7h, 06h, 22h, 82h, 01h
db 00h, 8Bh, 1Eh, 0C8h, 94h, 8Bh, 07h, 0A3h, 7Eh, 94h, 0A1h, 7Eh, 94h, 0D1h, 0E0h, 0A3h
db 7Ch, 94h, 0A1h, 7Ch, 94h, 05h, 0D6h, 0FFh, 0A3h, 56h, 15h, 0C7h, 06h, 4Ch, 15h, 00h
db 00h, 0C7h, 06h, 4Eh, 15h, 00h, 00h, 0C7h, 06h, 50h, 15h, 00h, 00h, 0C7h, 06h, 52h
db 15h, 00h, 00h, 8Bh, 1Eh, 0C8h, 94h, 8Bh, 07h, 05h, 0ECh, 0FFh, 99h, 0B1h, 0Ch, 0E8h
db 0CFh, 5Dh, 0A3h, 0D6h, 0ADh, 89h, 16h, 0D8h, 0ADh, 8Bh, 1Eh, 0C8h, 94h, 8Bh, 47h, 02h
db 05h, 0F3h, 0FFh, 99h, 0B1h, 0Ch, 0E8h, 0B8h, 5Dh, 0A3h, 0DAh, 0ADh, 89h, 16h, 0DCh, 0ADh
db 0A1h, 0DCh, 0ADh, 8Bh, 16h, 0DAh, 0ADh, 89h, 16h, 0D0h, 94h, 0A3h, 0D2h, 94h, 0C7h, 06h
db 0CCh, 94h, 00h, 00h, 0C7h, 06h, 0CEh, 94h, 00h, 00h, 0C7h, 06h, 30h, 1Dh, 0A0h, 00h
db 0C7h, 06h, 80h, 94h, 50h, 00h, 0C7h, 06h, 82h, 94h, 2Ch, 01h, 0C7h, 06h, 32h, 1Dh
db 0EEh, 02h, 0E8h, 0Eh, 0F0h, 0E8h, 0E8h, 0EAh, 0E8h, 08h, 0F0h, 0C7h, 06h, 38h, 1Dh, 00h
db 00h, 83h, 3Eh, 5Ch, 0C3h, 00h, 74h, 0Ah, 33h, 0C0h, 50h, 0E8h, 63h, 3Dh, 59h, 0E9h
db 0BEh, 00h, 0B8h, 01h, 00h, 50h, 0E8h, 58h, 3Dh, 59h, 0A1h, 0DCh, 0ADh, 8Bh, 16h, 0DAh
db 0ADh, 89h, 16h, 0D0h, 94h, 0A3h, 0D2h, 94h, 0C7h, 06h, 0CCh, 94h, 00h, 00h, 0C7h, 06h
db 0CEh, 94h, 00h, 00h, 0E8h, 6Dh, 05h, 0E8h, 0C9h, 0EFh, 0E8h, 0C6h, 0EFh, 0B8h, 09h, 00h
db 50h, 0B8h, 0Eh, 00h, 50h, 0E8h, 0F1h, 1Ah, 59h, 59h, 0B8h, 6Fh, 04h, 50h, 0E8h, 0F0h
db 1Bh, 59h, 0FFh, 36h, 0E4h, 0ADh, 0FFh, 36h, 0E2h, 0ADh, 0E8h, 53h, 1Ch, 59h, 59h, 0B8h
db 78h, 04h, 50h, 0E8h, 0DBh, 1Bh, 59h, 0FFh, 36h, 6Eh, 0AEh, 0E8h, 24h, 1Ch, 59h, 0B8h
db 82h, 04h, 50h, 0E8h, 0CBh, 1Bh, 59h, 0FFh, 36h, 84h, 94h, 0E8h, 14h, 1Ch, 59h, 0B8h
db 02h, 00h, 50h, 0E8h, 0D0h, 3Ah, 59h, 0E8h, 0BDh, 3Bh, 83h, 3Eh, 1Ch, 0AFh, 00h, 74h
db 3Fh, 83h, 3Eh, 0DAh, 0AEh, 00h, 74h, 38h, 0E8h, 09h, 05h, 0E8h, 65h, 0EFh, 0E8h, 62h
db 0EFh, 0E8h, 0EBh, 14h, 0B8h, 01h, 00h, 50h, 0B8h, 0Ch, 00h, 50h, 0E8h, 49h, 1Ah, 59h
db 59h, 0B8h, 91h, 04h, 50h, 0E8h, 89h, 1Bh, 59h, 0E8h, 24h, 1Bh, 0A2h, 68h, 0C3h, 80h
db 3Eh, 68h, 0C3h, 0Dh, 75h, 0F3h, 33h, 0C0h, 50h, 0E8h, 0A5h, 3Ch, 59h, 0E8h, 0CAh, 13h
db 0A1h, 0D2h, 94h, 8Bh, 16h, 0D0h, 94h, 81h, 0C2h, 00h, 96h, 15h, 00h, 00h, 89h, 16h
db 0DAh, 94h, 0A3h, 0DCh, 94h, 0C7h, 06h, 0D6h, 94h, 00h, 96h, 0C7h, 06h, 0D8h, 94h, 00h
db 00h, 0C7h, 06h, 0EAh, 94h, 00h, 00h, 0C7h, 06h, 0E8h, 94h, 00h, 00h, 0C7h, 06h, 0ECh
db 94h, 01h, 00h, 0C7h, 06h, 0E0h, 94h, 02h, 00h, 0B8h, 05h, 00h, 0A3h, 0E4h, 94h, 0A3h
db 0E6h, 94h, 0C7h, 06h, 0FEh, 94h, 00h, 00h, 0C7h, 06h, 00h, 95h, 00h, 00h, 0C7h, 06h
db 06h, 95h, 19h, 0Fh, 0C7h, 06h, 94h, 94h, 00h, 00h, 0C7h, 06h, 94h, 94h, 00h, 00h
db 0C7h, 06h, 76h, 84h, 00h, 00h, 0C7h, 06h, 0C4h, 94h, 01h, 00h, 0C7h, 06h, 90h, 94h
db 00h, 00h, 0C7h, 06h, 7Ah, 84h, 00h, 00h, 0A1h, 0DCh, 0ADh, 8Bh, 16h, 0DAh, 0ADh, 89h
db 16h, 0D0h, 94h, 0A3h, 0D2h, 94h, 0C7h, 06h, 0CCh, 94h, 00h, 00h, 0C7h, 06h, 0CEh, 94h
db 00h, 00h, 0E8h, 0B8h, 0FBh, 83h, 3Eh, 5Ch, 0C3h, 00h, 74h, 02h, 0EBh, 30h, 83h, 3Eh
db 38h, 1Dh, 00h, 75h, 06h, 0FFh, 0Eh, 84h, 94h, 0EBh, 19h, 0FFh, 06h, 6Eh, 0AEh, 0A1h
db 6Eh, 0AEh, 3Bh, 06h, 0E0h, 0ADh, 7Eh, 0Ch, 0C7h, 06h, 84h, 94h, 00h, 00h, 0C7h, 06h
db 0C2h, 94h, 02h, 00h, 83h, 3Eh, 84h, 94h, 00h, 74h, 03h, 0E9h, 18h, 0FDh, 5Eh, 8Bh
db 0E5h
}
}


//void sub_0_1953(void)
void main(void)
{
asm {
db 0C7h, 06h, 0E0h, 0ADh, 01h, 00h, 0C7h, 06h, 62h, 0C3h
db 01h, 00h, 0C7h, 06h, 0E6h, 0ADh, 00h, 00h, 0C7h, 06h, 84h, 0AEh, 01h, 00h, 0C7h, 06h
db 5Eh, 0C3h, 00h, 00h, 0C7h, 06h, 46h, 0Ch, 9Dh, 04h, 0E8h, 3Dh, 24h, 0C7h, 06h, 0Eh
db 0Ch, 19h, 00h, 0C7h, 06h, 20h, 82h, 20h, 00h, 0E8h, 0F2h, 0E9h, 0C7h, 06h, 0C8h, 94h
db 40h, 1Dh, 0E8h, 64h, 0ECh, 0E8h, 5Fh, 3Ah, 0E8h, 91h, 0FCh, 83h, 3Eh, 0C2h, 94h, 02h
db 75h, 03h, 0E8h, 0A3h, 0EBh, 0E8h, 76h, 0EDh, 0EBh, 0E8h
}
}


void refresh (void)
{
  int x,y,basex,basey;
  word underwin [5][16];

  basex=originx+4;
  basey=originy+17;
  if (indemo)
  {
    for (y=0; y<=4; y++)
      for (x=0; x<=15; x++)
	{
	  underwin[y][x]=view[y+basey][x+basex];
	  view[y+basey][x+basex]=demowin[y][x];
	};
  }

  WaitVBL ();
  if (grmode==CGAgr)
    cgarefresh();
  else
    egarefresh();

  if (indemo)
  {
    for (y=0; y<=4; y++)
      for (x=0; x<=15; x++)
	view[y+basey][x+basex]=underwin[y][x];
  }

  WaitVBL ();
}


void simplerefresh(void)
{
  WaitVBL ();
  if (grmode==CGAgr)
    cgarefresh();
  else
    egarefresh();

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

void loadgrfiles ()
{
  int i;

  if (grmode==CGAgr)
  {
    if (picsexact != NULL)
      farfree (picsexact);
    pics= (char far *)bloadin("CGACHARS.CA2");
    picsexact = lastparalloc;
    installgrfile ("CGAPICS.CA2",0,0);
    setscreenmode (grmode);
  }
  else
  {
    EGADATASTART = 0xA800;
    installgrfile ("EGAPICS.CA2",0,0);
    setscreenmode (grmode);
    moveega ();
    pics= (char far *)bloadin("EGACHARS.CA2");
    EGAmove ();
    farfree (lastparalloc);		// chars are allready in EGA mem
  }

}


/*======================================*/
/*				        */
/* restore                              */
/* redraws every tile on the tiled area */
/* by setting oldtiles to -1.  used to  */
/* erase any temporary windows.         */
/*				        */
/*======================================*/

void clearold (void)
{
  memset (&oldtiles,0xff,sizeof(oldtiles)); /*clear all oldtiles*/
};


void restore (void)
{
  clearold ();
  simplerefresh ();
};



/*      */
/* help */
/*      */
boolean wantmore (void)
{
  sx=2;
  sy=20;
  print ("(space for more/esc)");
  sx=12;
  sy=21;
  ch = get ();
  if (ch==27)
    return false;

  return true;
};


/*	   */
/* charpic */
/*	   */
void charpic(int x,int y, classtype c, dirtype dir, int stage)
{
  int xx,yy,size,tilenum;

  size=objdef[c].size;
  tilenum=objdef[c].firstchar+size*size
    * ((dir & objdef[c].dirmask)*objdef[c].stages+stage);

  for (yy=y;yy<=y+size-1;yy++)
    for (xx=x;xx<=x+size-1;xx++)
	drawchar (xx,yy,tilenum++);
};

void help (void)
{
  int x,y;
  char far *oldcharptr;
#define OLDSET oldcharptr = charptr;charptr = MK_FP(0xa400,0);
#define NEWSET charptr = oldcharptr;

  centerwindow (20,20);
  print ("  C A T A C O M B   \n");
  print ("   - - - - - - -    \n");
  print (" by John Carmack    \n");
  print ("                    \n");
  print ("\n");
  print ("f1 = help           \n");
  print ("f2 = control panel  \n");
  print ("f3 = game reset     \n");
  print ("f4 = save game      \n");
  print ("f5 = load saved game\n");
  print ("f9 = pause          \n");
  print ("f10 / ESC = quit    \n");
  print ("\n");
  print ("hit fire at the demo\n");
  print ("to begin playing.   \n");
  if (!wantmore())
    return;

  centerwindow (20,20);
  print ("\nKeyboard controls:  \n\n");
  print ("move    : arrows    \n");
  print ("button1 : ctrl      \n");
  print ("button2 : alt       \n");
  print ("\nTo switch to mouse \n");
  print ("or joystick control,\n");
  print ("hit f2             \n");

  if (!wantmore())
    return;

  centerwindow (20,20);
  print ("Button 1 / ctrl key:\n");
  print ("Builds shot power.  \n");
  print ("If the shot power   \n");
  print ("meter is full when  \n");
  print ("the button is       \n");
  print ("released, a super   \n");
  print ("shot will be        \n");
  print ("launched.           \n");
  print ("\n");

  OLDSET;

  for (y=11; y<=18; y++)
    for (x=3; x<=20; x++)
      drawchar (x,y,128);

  charpic (4,14,player,east,2);
  charpic (19,15,shot,east,1);
  charpic (17,14,shot,east,0);
  charpic (15,15,shot,east,1);
  charpic (8,14,bigshot,east,0);

  NEWSET;

  if (!wantmore())
    return;

  centerwindow (20,20);
  print ("Button 2 / alt key:\n");
  print ("Allows you to move  \n");
  print ("without changing the\n");
  print ("direction you are   \n");
  print ("facing.  Good for   \n");
  print ("searching walls and \n");
  print ("fighting retreats.  \n");

  OLDSET;

  for (y=11; y<=18; y++)
    for (x=3; x<=20; x++)
      if (y==15)
	drawchar (x,y,129);
      else if (y==16)
	drawchar (x,y,131);
      else
	drawchar (x,y,128);
  charpic (6,13,player,south,2);
  sx=6;
  sy=15;
  print ("\35\35\36\36\37\37");

  NEWSET;

  if (!wantmore())
    return;

  centerwindow (20,20);
  print ("\"P\" or \"space\" will \n");
  print ("take a healing      \n");
  print ("potion if you have  \n");
  print ("one.  This restores \n");
  print ("the body meter to   \n");
  print ("full strength.  Keep\n");
  print ("a sharp eye on the  \n");
  print ("meter, because when \n");
  print ("it runs out, you are\n");
  print ("dead!               \n\n");
  print ("\"B\" will cast a bolt\n");
  print ("spell if you have   \n");
  print ("any.  You can mow   \n");
  print ("down a lot of       \n");
  print ("monsters with a bit \n");
  print ("of skill.           \n");

  if (!wantmore())
    return;

  centerwindow (20,20);
  print ("\"N\" or \"enter\" will \n");
  print ("cast a nuke spell.  \n");
  print ("This usually wipes  \n");
  print ("out all the monsters\n");
  print ("near you.  Consider \n");
  print ("it a panic button   \n");
  print ("when you are being  \n");
  print ("mobbed by monsters! \n\n");

  OLDSET;

  print ("               \200\200\200\n");
  print ("POTIONS:       \200\242\200\n");
  print ("               \200\200\200\n");
  print ("SCROLLS:       \200\243\200\n");
  print (" (BOLTS/NUKES) \200\200\200\n");
  print ("TREASURE:      \200\247\200\n");
  print (" (POINTS)      \200\200\200\n");
  print ("               \200\200\200\n");

  NEWSET;

  wantmore();

};

/*       */
/* reset */
/*       */
void reset(void)
{
  centerwindow (18,1);
  print ("reset game (y/n)?");
  ch= get ();
  if (ch=='y')
    {
      gamexit=killed;
      playdone=true;
    };
};



/*=========================================================================*/


/*==============================*/
/*			        */
/* loadlevel / savelevel        */
/* loads map level into memory, */
/* and sets everything up.      */
/*			        */
/*==============================*/

void loadlevel(void)
{
  int i;

  classtype tokens[256-230]  =
    {player,teleporter,goblin,skeleton,ogre,gargoyle,dragon,turbogre,
     guns,gune,secretgate,nothing,nothing,nothing,nothing,nothing,
     nothing,nothing,nothing,nothing,nothing,nothing,nothing,nothing,
     nothing,nothing};

  char filename[64],st[64];
  int x,y,xx,yy,recs, btile;
  char sm[4096],rle[4096];

  strcpy (filename,"level");
  itoa (level,st,10);
  strcat (filename,st);
  strcat (filename,".CA2");

  LoadFile (filename,rle);
  RLEExpand(&rle[4],&sm,4096);

  numobj=0;
  o[0].x=13;          /*just defaults if no player token is found*/
  o[0].y=13;
  o[0].stage=0;
  o[0].delay=0;
  o[0].dir=east;
  o[0].oldx=0;
  o[0].oldy=0;
  o[0].oldtile=-1;


  for (yy=0; yy<64; yy++)
    for (xx=0; xx<64; xx++)
      {
	btile=sm[yy*64+xx];
	if (btile<230)
	  background[yy+topoff][xx+leftoff]=btile;
	else
	  {

/*hit a monster token*/
	    background[yy+topoff][xx+leftoff]=blankfloor;
	    if (tokens[btile-230]==player)

/*the player token determines where you start in level*/

	      {
		o[0].x=xx+topoff;
		o[0].y=yy+leftoff;
	      }
            else

/*monster tokens add to the object list*/

	      {
		numobj++;
		o[numobj].active=false;
		o[numobj].class=tokens[btile-230];
		o[numobj].x=xx+leftoff;
		o[numobj].y=yy+topoff;
		o[numobj].stage=0;
		o[numobj].delay=0;
		o[numobj].dir=(dirtype)(rndt()/64);  /*random 0-3*/
		o[numobj].hp=objdef[o[numobj].class].hitpoints;
		o[numobj].oldx=x;
		o[numobj].oldy=y;
		o[numobj].oldtile=-1;
	      };

	    };

	  };



  originx = o[0].x-11;
  originy = o[0].y-11;

  shotpower=0;
  for (y=topoff-1; y<65+topoff; y++)
    for (x=leftoff-1; x<64+leftoff; x++)
      view[y][x]=background[y][x];

  sx=33;                  /*print the new level number on the right window*/
  sy=1;
  printint (level);
  print (" ");          /*in case it went from double to single digit*/
  restore();



  for (i=0;i<6;i++)
    saveitems[i] = items[i];
  savescore = score;
  saveo[0] = o[0];
};


/*==========================================================================*/


#include "cat_play.c"


/*
==============
=
= drawside
=
==============
*/
void drawside ()
{
  int i;

  for (sx=0;sx<40;sx++)
    drawchar (sx,24,0);

  for (sy=0;sy<24;sy++)
    drawchar (39,sy,0);

  drawwindow (24,0,38,23);  /*draw the right side window*/
  print ("  level\n\nscore:\n\ntop  :\n\nk:\np:\nb:\nn:\n\n");
  print (" shot power\n\n\n    body\n\n\n");
  printhighscore();
  printbody();
  printshotpower();
  printscore();
  sx=33;                  /*print the new level number on the right window*/
  sy=1;
  printint (level);

  drawpic (25*8,17*8,SIDEPIC);

  for (i=1; i<=items[1] && i<11; i++)
    drawchar (26+i,7,31);  /*key icon*/

  for (i=1; i<=items[2] && i<11; i++)
    drawchar (26+i,8,29);  /*potion icon*/

  for (i=1; i<=items[3] && i<11; i++)
    drawchar (26+i,9,30);  /*scroll icon*/

  for (i=1; i<=items[5] && i<11; i++)
    drawchar (26+i,10,30);  /*scroll icon*/
}


/*
=============
=
= repaintscreen
=
=============
*/

void repaintscreen ()
{
  switch (gamestate)
  {
    case intitle:
      drawpic (0,0,TITLEPIC);
      break;
    case ingame:
      restore ();
      drawside ();
      printscore ();
      sx=33;                  /*print the new level number on the right window*/
      sy=1;
      printint (level);
      break;
    case inscores:
      restore ();
      drawside ();
      printscore ();
      sx=33;                  /*print the new level number on the right window*/
      sy=1;
      printint (level);
      indemo = demoplay;
      break;
    default:
      sx=sy=10;
      print ("Bad gamestate!");
      clearkeys ();
      get();
  }

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

void dofkeys (void)
{
  int i,handle;
  char st2[10];
  int key=bioskey(1)/256;
  if (key==1)			// make ESC into F10
    key=0x44;
  if (key<0x3b || key>0x44)
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
      break;
    case 0x3d:			// F3
      clearkeys ();
      expwin (18,1);
      print ("RESET GAME (Y/N)?");
      ch=toupper(get());
      if (ch=='Y')
	resetgame = true;
      break;

    case 0x3e:			// F4
      clearkeys ();
      expwin (22,4);
      if (indemo != notdemo)
      {
	print ("Can't save game here!");
	get ();
	break;
      }
      print ("Save as game #(1-9):");
      ch=toupper(get());
      drawchar (sx,sy,ch);
      if (ch<'1' || ch>'9')
	break;
      //
      // save game
      //
      strcpy (str,"GAME0.CA2");
      str[4]=ch;
      if (_Verify(str))
      {
	print ("\nGame exists,\noverwrite (Y/N)?");
	ch=get();
	if (ch!='Y' && ch!='y')
	  break;
	sx=leftedge;
	print ("                    ");
	sy--;
	sx=leftedge;
	print ("                    ");
	sx=leftedge;
	sy--;
      }
      if ((handle = open(str, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE)) == -1)
	return;
      write(handle, &saveitems, sizeof(items));
      write(handle, &savescore, sizeof(score));
      write(handle, &level, sizeof(level));
      write(handle, &saveo[0],sizeof(o[0]));

      close(handle);
      print ("\nGame saved.  Hit F5\n");
      print ("when you wish to\n");
      print ("restart the game.");
      get();
      break;

    case 0x3f:			// F5
      clearkeys ();
      expwin (22,4);
      print ("Load game #(1-9):");
      ch=toupper(get());
      drawchar (sx,sy,ch);
      if (ch<'1' || ch>'9')
	break;
      //
      // load game
      //
      strcpy (str,"GAME0.CA2");
      str[4]=ch;
      if ((handle = open(str, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
      {
	print ("\nGame not found.");
	get ();
	break;
      }
      read(handle, &items, sizeof(items));
      read(handle, &score, sizeof(score));
      read(handle, &level, sizeof(level));
      read(handle, &o[0],sizeof(o[0]));
      close(handle);
      exitdemo = true;
      if (indemo != notdemo)
	playdone = true;
      drawside ();		// draw score, icons, etc
      leveldone = true;
      break;
    case 0x43:			// F9
      clearkeys ();
      expwin (7,1);
      print ("PAUSED");
      get ();
      break;
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

  clearold ();
  clearkeys ();
  repaintscreen ();
}
