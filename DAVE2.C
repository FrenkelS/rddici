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
  unsigned int word_789_847C; /* +00 */
  int          px;            /* +02 */
  int          py;            /* +04 */
  int          imagewidth;    /* +06 */
  int          imageheight;   /* +08 */
  void far    *shapeptr;      /* +0A */
  void far    *maskptr;       /* +0E */
  int          word_789_848E; /* +12 */
  char         unk12[12];     /* +14 */
} type847C;


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

long dword_789_154C;
long dword_789_1550;
int word_789_1D28;
int word_789_1D2A;
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
boolean bool_789_847A;
type847C type847C_789_847C[128];
int word_789_9480;
int word_789_9482;
boolean bool_789_9490;
ControlStruct ctrl_789_9492;
int word_789_949E;
int word_789_94A0;
int word_789_94A2[14];
boolean bool_789_94C4;
type847C *word_789_94C6;
int word_789_94CA;
int word_789_94D4;
type94D6 type94D6_789_94D6;
int word_789_94FE;
int word_789_9500;
void (*func_789_9506)();


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


#define PORTTILESWIDE 21
#define PORTTILESHIGH 14
#define BIGPORTSIZE (PORTTILESHIGH*PORTTILESWIDE)

int drawoffs1[BIGPORTSIZE], drawoffs0[BIGPORTSIZE];


unsigned int *mapplane[4];		// points into map
int mapbwide,mapwwide,mapbytesextra;
long originxglobal, originyglobal;
long originxmax, originymax;
unsigned int drawpage;

int px, py;

char *bigbuffer;

long lastExtraScore;


#define MAXOBJECTS	60
objtype objlist[MAXOBJECTS];


int tile_numframes[100] =
{
0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
0,0,
1,1,1,1,1,1,1,1,1,1,1,1,
0,0,0,0,0,0,
1,1,1,
0,0,0,0,
1,1,1,1,1,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


int tile_behavior[100] =
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


int tile_block[100] =
{
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0xF5,0xF5,0xF5,0xF5,
0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,0xD7,
0,0,0,0,0,0,0,0,0
};


/****************************************************************************/

//////////////////////////////////
//
// function prototypes
//
//////////////////////////////////

void extern RF_ForceRefresh (void);
void sub_0_290 (void);
void RF_Refresh (void);
void dofkeys (void);
void help (void);
void AddScore(int toadd);
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


void sub_0_31E_UNUSED(void)
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
	int si;
	void VidRefresh(void);

	VidRefresh();

	word_789_94CA++;

	if (drawpage)
	{
		for (si = 0; si < word_789_949E; si++)
		{
			drawoffs1[word_789_7B20[si]] = -1;
		}
	}
	else
	{
		for (si = 0; si < word_789_949E; si++)
		{
			drawoffs0[word_789_7B20[si]] = -1;
		}
	}

	if (grmode == EGAgr)
	{
		drawpage ^= 1;
		asm nop; // TODO
	}
}


boolean RF_PlaceSprite(void) // sub_0_80D
{
	unsigned int var_2;
	int var_4;
	int var_6;
	int tx;
	int ty;
	int tx_min;
	int ty_min;
	int tx_max;
	int ty_max;

	py -= originyglobal / 256;
	px -= originxglobal / 256;

	if (grmode == EGAgr)
	{
		py += (originyglobal / 256) % 16;
		px += (originxglobal / 256) % 16;

		py += 32;

		tx_min = px / 16;
		if (tx_min < 0)
			tx_min = 0;
		else if (tx_min > 23)
			return false;

		tx_max = (px + image.width * 8 - 1) / 16;
		if (tx_max > PORTTILESWIDE - 1)
			tx_max = PORTTILESWIDE - 1;
		else if (tx_max < 0)
			return false;

		ty_min = (py - 32) / 16;
		if (ty_min < 0)
			ty_min = 0;
		else if (ty_min > PORTTILESHIGH - 1)
			return false;

		ty_max = (py + image.height - 33) / 16;
		if (ty_max > PORTTILESHIGH - 1)
			ty_max = PORTTILESHIGH - 1;
		else if (ty_max < 0)
			return false;

		px = (px >> 3) + 4;

		var_2 = (ty_max * PORTTILESWIDE + tx_max) * 2;
	}

	for (ty = ty_min; ty <= ty_max; ty++)
	{
		for (tx = tx_min; tx <= tx_max; tx++)
		{
			word_789_7B20[word_789_949E] = ty * PORTTILESWIDE + tx;
			word_789_949E++;
		}
	}

	word_789_94A2[13]++;
	word_789_94C6++;

	word_789_94C6->word_789_847C = var_2;
	word_789_94C6->px            = px;
	word_789_94C6->py            = py;
	word_789_94C6->imagewidth    = image.width;
	word_789_94C6->imageheight   = image.height;
	word_789_94C6->shapeptr      = image.shapeptr;
	word_789_94C6->maskptr       = image.maskptr;
	word_789_94C6->word_789_848E = 0;

	word_789_8228->word_789_94F2 = word_789_1D34;
	word_789_8228->word_789_94F6 = word_789_1D28;
	word_789_8228->word_789_94F4 = word_789_1D36;
	word_789_8228->word_789_94F8 = word_789_1D2A;

	if (type847C_789_847C[word_789_94D4].word_789_847C >= var_2)
	{
		type847C_789_847C[word_789_94A2[13]].word_789_848E = word_789_94D4;
		word_789_94D4 = word_789_94A2[13];
	}
	else
	{
		var_4 = word_789_94D4;

		while (type847C_789_847C[var_4].word_789_847C < var_2)
		{
			var_6 = var_4;
			var_4 = type847C_789_847C[var_4].word_789_848E;
		}

		type847C_789_847C[var_6            ].word_789_848E = word_789_94A2[13];
		type847C_789_847C[word_789_94A2[13]].word_789_848E = var_4;
	}

	return true;
}


void BadThink_UNUSED(void) // sub_0_AA7
{
  _quit("badTHINK!");
}


void FindFreeObj_UNUSED (void) // sub_0_AB4
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

  new->think = BadThink_UNUSED;
}


void sub_0_B0D(void)
{
	px = word_789_8228->dword_789_94D6 / 256;
	py = word_789_8228->dword_789_94DA / 256;

	if (grmode == CGAgr)
		word_789_8226 = word_789_94A0 * 2 + ((px / 2) % 2);
	else
	{
		word_789_8226 = word_789_94A0 * 4 + ((px / 2) % 4);
		px &= ~6;
		asm nop; // TODO
	}

	word_789_8228->word_789_94EE = word_789_8226;

	image = spritetable[word_789_8226];

	word_789_1D34 = px + image.xl;
	word_789_1D28 = px + image.xh;
	word_789_1D36 = py + image.yl;
	word_789_1D2A = py + image.yh;
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
	return mapplane[0][(arg_2 / 16) * mapwwide + (arg_0 / 16)];
}


boolean sub_0_C48(void)
{
	int var_2;
	int var_4;
	int var_6;
	int cx;
	int di;
	int si;

	var_2 = word_789_1D34 / 16;
	var_4 = word_789_1D36 / 16;
	var_6 = word_789_1D28 / 16;
	cx = word_789_1D2A / 16;

	for (si = var_4; si <= cx; si++)
	{
		for (di = var_2; di <= var_6; di++)
		{
			if (tile_numframes[mapplane[0][si * mapwwide + di]])
				return true;
		}
	}

	return false;
}


void sub_0_CBE(void)
{
	int di;
	int si;
	boolean var_2;
	boolean var_4;

	di = word_789_8228->word_789_94E8;
	si = word_789_8228->word_789_94EA;

	sub_0_B0D();

	if (di < 0)
		di = (di - 255) / 256;
	else if (di > 0)
		di = (di + 255) / 256;

	if (si < 0)
		si = (si - 255) / 256;
	else if (si > 0)
		si = (si + 255) / 256;

	word_789_1D36 += si;
	word_789_1D2A += si;
	word_789_1D34 += di;
	word_789_1D28 += di;

	var_2 = sub_0_C48();

	word_789_1D36 -= si;
	word_789_1D2A -= si;
	word_789_1D34 -= di;
	word_789_1D28 -= di;

	if (!var_2)
		return;

	if (di < 0)
	{
		word_789_1D34 += di;
		word_789_1D28 += di;

		var_2 = sub_0_C48();

		word_789_1D34 -= di;
		word_789_1D28 -= di;

		if (var_2 == true)
			word_789_8228->word_789_94E8 = (-word_789_1D34 % 16) << 8;
	}
	else if (di > 0)
	{
		word_789_1D34 += di;
		word_789_1D28 += di;

		var_2 = sub_0_C48();

		word_789_1D34 -= di;
		word_789_1D28 -= di;

		if (var_2 == true)
			word_789_8228->word_789_94E8 = (15 - (word_789_1D28 % 16)) << 8;
	}

	if (si < 0)
	{
		word_789_1D36 += si;
		word_789_1D2A += si;

		var_4 = sub_0_C48();

		word_789_1D36 -= si;
		word_789_1D2A -= si;

		if (var_4 == true)
			word_789_8228->word_789_94EA = (-word_789_1D36 % 16) << 8;
	}
	else if (si > 0)
	{
		word_789_1D36 += si;
		word_789_1D2A += si;

		var_4 = sub_0_C48();

		word_789_1D36 -= si;
		word_789_1D2A -= si;

		if (var_4 == true)
			word_789_8228->word_789_94EA = (15 - (word_789_1D2A % 16)) << 8;
	}

	if (!var_2 && !var_4)
		word_789_8228->word_789_94E8 = word_789_8228->word_789_94EA = 0;
}


boolean sub_0_E52_UNUSED(int arg_0, int arg_2)
{
	word_789_8228->dword_789_94D6 += arg_0;
	word_789_8228->dword_789_94DA += arg_2;

	sub_0_B0D();

	word_789_8228->dword_789_94D6 -= arg_0;
	word_789_8228->dword_789_94DA -= arg_2;

	if (word_789_1D2A - originyglobal / 4 < 16)
	{
		if (word_789_8228->word_789_94EA <= 0)
			return true;
	}

	if (word_789_1D2A - originyglobal / 4 > 172)
	{
		if (word_789_8228->word_789_94EA >= 0)
			return true;
	}

	if (word_789_1D34 < 6 || word_789_1D28 > 303)
		return true;

	return sub_0_C48();
}


void sub_0_F19(void)
{
	// TODO order of variables
	int var_2;
	int var_4;
	int var_6;
	int var_8;
	ControlStruct c;
	int var_A;
	int var_18;
	int var_1A;
	int var_1C;
	int var_1E;
	int var_20;
	int var_22;
	int var_24;

	int di = 0;
	int si = 0;

	if (word_789_8228->dword_789_94D6 < 0x00001000)
		word_789_8228->dword_789_94D6 = 0x00001000;

	if ((long)(((LevelDef *)bigbuffer)->width - 1) << 8 << 4 < word_789_8228->dword_789_94D6)
		word_789_8228->dword_789_94D6 = (long)(((LevelDef *)bigbuffer)->width - 1) << 8 << 4;

	if ((long)((LevelDef *)bigbuffer)->height << 8 << 4 < word_789_8228->dword_789_94DA)
	{
		word_789_8228->gamexit = 0;
		PlaySound(PLUMMETSND);
		WaitEndSound();
		return;
	}

	c = ControlPlayer (word_789_94A2[12] + 1);
	if (c.button1)
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

	if (c.button2)
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

	if (tile_block[var_6])
	{
		if (word_789_8476 == 0 && tile_numframes[var_6])
			bool_789_94C4 = false;

		var_A = tile_block[var_6];
	}

	if (tile_block[var_8])
	{
		if (word_789_8476 == 0 && tile_numframes[var_8])
			bool_789_94C4 = false;

		var_A = tile_block[var_8];
	}

	word_789_8228->word_789_94E8 += di;
	word_789_8228->word_789_94EA += si;

	word_789_8228->word_789_94E8 = (long)word_789_8228->word_789_94E8 * var_A / 256;
	word_789_8228->word_789_94EA = word_789_8228->word_789_94EA * 9 / 10;

	si = word_789_8228->word_789_94EA;

	sub_0_CBE();

	if (word_789_8228->word_789_94EA == 0 && si < 0)
		PlaySound(BUMPSND);

	if (word_789_8228->word_789_94EA != 0)
		bool_789_94C4 = true;

	if (word_789_8228->word_789_94EA >= 0)
		word_789_8476 = 0;

	di = word_789_8228->word_789_94E8;
	si = word_789_8228->word_789_94EA;

	word_789_8228->dword_789_94D6 += di;
	word_789_8228->dword_789_94DA += si;

	if (di > 0)
	{
		if (word_789_8228->dword_789_94D6 - originxglobal > 0xB400L)
		{
			originxglobal += di;
			if (originxglobal > originxmax)
				originxglobal = originxmax;
		}
	}
	else if (di < 0)
	{
		if (word_789_8228->dword_789_94D6 - originxglobal < 0x8C00L)
		{
			originxglobal += di;
			if (originxglobal < dword_789_154C)
				originxglobal = dword_789_154C;
		}
	}

	if (si > 0)
	{
		if (word_789_8228->dword_789_94DA - originyglobal > 0xA000L)
		{
			originyglobal += si;
			if (originyglobal > originymax)
				originyglobal = originymax;
		}
	}
	else if (si < 0)
	{
		if (word_789_8228->dword_789_94DA - originyglobal < 0x2800L)
		{
			originyglobal += si;
			if (originyglobal < dword_789_1550)
				originyglobal = dword_789_1550;
		}
	}

	sub_0_B0D();
	RF_PlaceSprite();

	var_1C = word_789_1D34 / 16;
	var_1E = word_789_1D36 / 16;
	var_20 = word_789_1D28 / 16;
	var_22 = word_789_1D2A / 16;

	for (var_1A = var_1E; var_1A <= var_22; var_1A++)
	{
		for (var_18 = var_1C; var_18 <= var_20; var_18++)
		{
			var_24 = mapplane[0][var_1A * mapwwide + var_18];

			switch (tile_behavior[var_24])
			{
				case 1:
					PlaySound(GRABCOINSND);
					AddScore(100);
					mapplane[0][var_1A * mapwwide + var_18] = 0;
					break;
			}
		}
	}

	for (objptr_789_7FD0 = objlist,	var_2 = 1; var_2 < lastobj; var_2++, objptr_789_7FD0++)
	{
		if (objptr_789_7FD0->think != NULL)
		{
			if (sub_0_BCF())
				objptr_789_7FD0->think();
		}
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
    word_789_94A2[13] = word_789_949E = word_789_94D4 = 0;
    word_789_94C6 = &type847C_789_847C[0];
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
	int i;

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
			type847C_789_847C[0].word_789_847C = 0xffff;
			word_789_94C6 = &type847C_789_847C[0];
			word_789_94A2[13] = 0;
			word_789_949E = 0;
			word_789_94D4 = 0;

			strcpy(str, "LEVEL0");
			itoa(level, st, 10);
			strcat(str, st);
			strcat(str, ".");
			strcat(str, _extension);

			LoadFile(str, bigbuffer);

			for (i = 0; i < ((LevelDef *)bigbuffer)->planes; i++) {
				mapplane[i] = (unsigned int *)(bigbuffer + i * ((LevelDef *)bigbuffer)->planesize + 32);
			}

			lastobj = 1;
			mapwwide = ((LevelDef *)bigbuffer)->width;
			mapbwide = mapwwide * 2;
			mapbytesextra = mapbwide + -(2 * PORTTILESWIDE);
			dword_789_154C = 0;
			dword_789_1550 = 0;

			originxmax = ((long)(((LevelDef *)bigbuffer)->width  + -(PORTTILESWIDE - 1))) << 12;
			originymax = ((long)(((LevelDef *)bigbuffer)->height + -(PORTTILESHIGH - 1))) << 12;

			originyglobal = originymax;
			originxglobal = 0;
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
			originyglobal = originymax;
			originxglobal = 0;
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

		type94D6_789_94D6.dword_789_94DA = originyglobal + 0x9600;
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
		originyglobal = originymax;
		originxglobal = 0;
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

	bigbuffer = unk_789_1D40;

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

