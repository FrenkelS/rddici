/* Reconstructed Dangerous Dave in Copyright Infringement Source Code
 * Copyright (C) 2026 Frenkel Smeijers
 *
 * Heavily modified by K1n9_Duk3 to produce a 100% identical EXE (2026-09-22)
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

#include "BSSCHEAT.H"

#include "pcrlib.h"
#include "NGRABDD2.H"
#include "SOUNDS.H"

#define NUMDEMOS 1

typedef enum {ingame,intitle,inend,inscores} statetype;

typedef enum {nothing,playerobj} classtype;

typedef struct {
	long      x;             /* +00 */
	long      y;             /* +04 */
	int       unk8;          /* +08 */
	int       walkframe;     /* +0A */
	int       unkC;          /* +0C */
	int       walktick;      /* +0E */
	int       walkticks;     /* +10 */
	int       xmove;         /* +12 */
	int       ymove;         /* +14 */
	classtype obclass;       /* +16 */
	int       shapenum;      /* +18 */
	int       unk1A;         /* +1A */
	int       left;          /* +1C */ // hit rectangle
	int       top;           /* +1E */
	int       right;         /* +20 */
	int       bottom;        /* +22 */
	long      unk24;         /* +24 */
	boolean   active;        /* +28 */
	int       word_789_9500; /* +2A */
	void      (*contact)();  /* +2C */
	int       unk2E;         /* +2E */
	void      (*think)();    /* +30 */
} objtype;


typedef struct {
	unsigned int screenindex;   /* +00 */
	int          px;            /* +02 */
	int          py;            /* +04 */
	int          imagewidth;    /* +06 */
	int          imageheight;   /* +08 */
	void far    *shapeptr;      /* +0A */
	void far    *maskptr;       /* +0E */
	int          next;          /* +12 */
	char         unk14[12];     /* +14 */
} drawtype;

// sprite names taken from EGAPICS.DD2
typedef enum
{
	DAVER1SPR,
	DAVER2SPR,
	DAVER3SPR,
	DAVER4SPR,
	DAVESPR,
	DAVEL1SPR,
	DAVEL2SPR,
	DAVEL3SPR,
	DAVEL4SPR,
	DAVEJRSPR,
	DAVEJLSPR,
	DAVEC1SPR,
	DAVEC2SPR,
	DAVEC3SPR,
	DAVEC4SPR,
} spritenames;

/*=================*/
/*		   */
/* typed constants */
/*     		   */
/*=================*/


int tile_block[100] =
{
0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
1,1,1,0,0,0,0,1,1,1,1,1,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


int tile_behavior[100] =
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,1,1,1, // Coin
0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


int animtable[100] =
{
0,1,2,3,4,5,6,7,8,9,10,11,12,
14,15,16,13, // ? Block
17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,
61,62,63,60, // Coin
65,66,67,64, // Brick Block
68,69,70,71,72,73,74,75,76,77,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


int tile_xfactor[100] =
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


/*==================*/
/*		    */
/* global variables */
/*		    */
/*==================*/

int basegravity;
int basejumpmove;
char mapbuffer[24032];	// 24000 bytes for map planes + 32 bytes for map header
// (not sure how big this really is, but it must be at least 9184 bytes!)

int extrahbls;
int tmp_spritenum;
int jumpspeed;
int spritecost;
boolean faceleft;
drawtype spritelist[128];
int basewalkspeed;
int basejumpspeed;
boolean jumpheld;
ControlStruct lastctrl;
int tmp_baseshape;
int mindrawcalls[12];
int playernum;
int spritesshown;
boolean jumping;
drawtype *spritelistptr;
int refreshcount;
int firstsprite;

int davesleft;
boolean leveldone;

int VGAPAL;				// just to make pcrlib happy

boolean exitdemo;
statetype gamestate;

ControlStruct ctrl;


int coverlist[600];
int coverlistindex;


#define PORTTILESWIDE 21
#define PORTTILESHIGH 14
#define BIGPORTSIZE (PORTTILESHIGH*PORTTILESWIDE)

int oldtiles[BIGPORTSIZE], oldtiles2[BIGPORTSIZE];


unsigned int EGApage;
unsigned int *mapplane[4];		// points into map
int mapbwide,mapwwide,mapwidthextra;
long originx, originy;
long originxmin, originymin;
long originxmax, originymax;

// These variables store the current object's position in pixel units.
int p_right;
int p_bottom;
int px, py;
int p_left;
int p_top;

LevelDef *levelheader;

long lastextra;


#define MAXOBJECTS	128
objtype objlist[MAXOBJECTS],*newob,*hit,*obj;
int numobj;


char unused_buffer[2000];	// byte_789_1558

char _dummy_[6];

/****************************************************************************/

//////////////////////////////////
//
// function prototypes
//
//////////////////////////////////

void extern RF_ForceRefresh (void);
void virtualscreen (void);
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

void drawscreen(void)
{
	RF_ForceRefresh();
	if (gamestate == ingame || gamestate == inscores)
	{
		RF_Refresh();
		virtualscreen();
		RF_Refresh();
	}

	if (gamestate == intitle)
		drawpic (0,0,TITLEPIC);

	if (gamestate == inend)
		drawpic (0,0,15);

	if (gamestate == inscores)
		_showhighscores();
}


void virtualscreen(void)
{
	if (grmode == CGAgr)
	{
		// This is for smooth vertical scrolling in CGA. Must be from an early
		// incarnation of what would become "Slordax - The Unknown Enemy".
		// This limits the number of pixel rows displayed on the screen to 176
		// (instead of 200), which allows the system to pan the screen up and down
		// by adjusting the CRTC start address. CGA cards have barely enough video
		// memory for a single 4-color screen at 320x200 pixels, so reducing the
		// number of lines actually displayed is the only way to make room for the
		// extra data necessary for panning up and down.
		if (_videocard == EGAcard)
		{
			outportb(0x3d4, 0x12);	// CRTC vertical display end
			outportb(0x3d5, 0xb0);	// 0xB0 == 176 pixels
		}
		else if (_videocard == VGAcard)
		{
			outportb(0x3d4, 0x12);	// CRTC vertical display end
			outportb(0x3d5, 0x60);	// 0x60 == 176 pixels
			// VGA cards use line-doubling, meaning the default 200 lines are
			// displayed as 400 lines on the CRT screen. The vertical display end
			// must be set to 352 for 176 pixels, which is 0x160 in hex. The
			// highest bit is stored in a separate "overflow" register, which is
			// why writing the value 0x60 will suffice. The overflow bit doesn't
			//	change when switching from 400 (== 0x190) to 352 (== 0x160).
		}
		else
		{
			outportb(0x3d4, 0x06);	// CRTC vertical total
			outportb(0x3d5, 0x58);	// 0x58 == 176 pixels
		}
	}
	if (grmode == EGAgr)
	{
		outportb(crtcaddr, 0x13);	// CRTC row offset register (logical screen line width)
		outportb(crtcaddr + 1, 0x18);	// 0x18 == 0x30 bytes == 384 pixels
		_AH = 0x10;
		_AL = 1;
		_BH = 5;
		geninterrupt(0x10);	// set border color to 5 (magenta)
	}
}


void clearscreen(void)
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

void loadgrfiles(void)
{
	if (grmode == CGAgr)
		installgrfile ("CGAPICS.DD2",0);
	else
	{
		installgrfile ("EGAPICS.DD2",0);
		EGApage ^= 1;
	}
}


void _setupgrmode(void)
{
	setscreenmode(grmode);
	virtualscreen();
}


void repaintscreen(void)
{
	_setupgrmode();
	drawscreen();
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
	if (key==0)
		return;

	switch (key)
	{
		case 0x3b:			// F1
			clearkeys ();
			help ();
			break;
		case 0x3c:			// F2
			clearkeys ();
			controlpanel ();
			virtualscreen ();
			break;
		case 0x3d:			// F3
			clearkeys ();
			expwin (18,1);
			print ("RESET GAME (Y/N)?");
			ch=toupper(get());
			if (ch=='Y')
			{
				objlist[0].obclass = nothing;
				davesleft = 1;
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

	drawscreen ();
}


void help (void)
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

void dotitlepage (void)
{
	int i;
	setscreenmode (grmode);

	if (grmode == EGAgr)
		virtualscreen();

	drawpic (0,0,TITLEPIC);

	gamestate=intitle;
	for (i=0;i<300;i++)
	{
		WaitVBL ();
		indemo = notdemo;
		ctrl = ControlPlayer (1);
		if (ctrl.button1 || ctrl.button2 || keydown[0x39])	// SPACEBAR
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

void doendpage (void)
{
	int i;
	setscreenmode (grmode);
	gamestate = inend;
	sx = 0;
	sy = 0;
	print ("Dave 2 title screen");

	for (i=0;i<300;i++)
	{
		WaitVBL ();
		indemo = notdemo;
		ctrl = ControlPlayer (1);
		if (ctrl.button1 || ctrl.button2 || keydown[0x39])	// SPACEBAR
		{
			exitdemo = true;
			break;
		}
		indemo = demoplay;
		if (bioskey (1))
			dofkeys ();
	}

	clearkeys ();
	sx = 20;
	sy = 24;
	get ();
	indemo = demoplay;
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

void dodemo (void)
{
	int i;

	exitdemo = false;
	indemo = notdemo;

	ctrl = ControlPlayer (1);
	if (ctrl.button1 || ctrl.button2 || keydown[0x39])	// SPACEBAR
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
		for (i=0;i<300;i++)
		{
			WaitVBL ();
			indemo = notdemo;
			ctrl = ControlPlayer (1);
			if (ctrl.button1 || ctrl.button2 || keydown[0x39])	// SPACEBAR
			{
				exitdemo = true;
				break;
			}
			indemo = demoplay;
			if (bioskey (1))
				dofkeys ();
		}

	}

	level = 1;
	leveldone = true;
	indemo = notdemo;
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

void gameover (void)
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

	for (i=0;i<300;i++)
	{
		WaitVBL ();
		ctrl = ControlPlayer (1);
		if (ctrl.button1 || ctrl.button2 || keydown[0x39])	// SPACEBAR
			break;
		if (bioskey (1))
			dofkeys ();
	}
}


void RF_Refresh(void)
{
	int i;
	void VidRefresh(void);

	VidRefresh();

	refreshcount++;

	if (EGApage)
	{
		for (i = 0; i < coverlistindex; i++)
		{
			oldtiles[coverlist[i]] = -1;
		}
	}
	else
	{
		for (i = 0; i < coverlistindex; i++)
		{
			oldtiles2[coverlist[i]] = -1;
		}
	}

	if (grmode == EGAgr)
	{
		EGApage ^= 1;
	}
}


boolean RF_PlaceSprite(void)
{
	unsigned int screenindex;
	int i;
	int prev;
	int tx;
	int ty;
	int tx_min;
	int ty_min;
	int tx_max;
	int ty_max;

	py -= originy / 256;
	px -= originx / 256;

	if (grmode == EGAgr)
	{
		py += (originy / 256) % 16;
		px += (originx / 256) % 16;

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

		screenindex = (ty_max * PORTTILESWIDE + tx_max) * 2;
	}

	for (ty = ty_min; ty <= ty_max; ty++)
	{
		for (tx = tx_min; tx <= tx_max; tx++)
		{
			coverlist[coverlistindex] = ty * PORTTILESWIDE + tx;
			coverlistindex++;
		}
	}

	spritesshown++;
	spritelistptr++;

	spritelistptr->screenindex   = screenindex;
	spritelistptr->px            = px;
	spritelistptr->py            = py;
	spritelistptr->imagewidth    = image.width;
	spritelistptr->imageheight   = image.height;
	spritelistptr->shapeptr      = image.shapeptr;
	spritelistptr->maskptr       = image.maskptr;
	spritelistptr->next          = 0;

	obj->left   = p_left;
	obj->right  = p_right;
	obj->top    = p_top;
	obj->bottom = p_bottom;

	if (spritelist[firstsprite].screenindex >= screenindex)
	{
		spritelist[spritesshown].next = firstsprite;
		firstsprite = spritesshown;
	}
	else
	{
		i = firstsprite;

		while (spritelist[i].screenindex < screenindex)
		{
			prev = i;
			i = spritelist[i].next;
		}

		spritelist[prev        ].next = spritesshown;
		spritelist[spritesshown].next = i;
	}

	return true;
}


void BadThink(void)
{
	_quit("badTHINK!");
}


void FindFreeObj (void)
{
	int i = 1;
	newob = &objlist[1];

	while (newob->obclass != nothing && i < numobj)
	{
		i++;
		newob++;
	}

	if (i >= numobj)
		numobj++;

	newob->left = newob->top = newob->bottom = newob->right = 0;

	newob->think = BadThink;
}


void GetHitbox(void)
{
	px = obj->x / 256;
	py = obj->y / 256;

	if (grmode == CGAgr)
		tmp_spritenum = tmp_baseshape * 2 + ((px / 2) % 2);
	else
	{
		tmp_spritenum = tmp_baseshape * 4 + ((px / 2) % 4);
		px &= ~6;
	}

	obj->shapenum = tmp_spritenum;

	image = spritetable[tmp_spritenum];

	p_left   = px + image.xl;
	p_right  = px + image.xh;
	p_top    = py + image.yl;
	p_bottom = py + image.yh;
}


boolean ObjectsCollide(void)
{
	if (hit->left > p_right)
		return false;

	if (hit->top > p_bottom)
		return false;

	if (hit->right < p_left)
		return false;

	if (hit->bottom < p_top)
		return false;

	return true;
}


int GetTile(int pixx, int pixy)
{
	return mapplane[0][(pixy / 16) * mapwwide + (pixx / 16)];
}


boolean ObjBlocked(void)
{
	int tx1;
	int ty1;
	int tx2;
	int ty2;
	int tx;
	int ty;

	tx1 = p_left   / 16;
	ty1 = p_top    / 16;
	tx2 = p_right  / 16;
	ty2 = p_bottom / 16;

	for (ty = ty1; ty <= ty2; ty++)
	{
		for (tx = tx1; tx <= tx2; tx++)
		{
			if (tile_block[mapplane[0][ty * mapwwide + tx]])
				return true;
		}
	}

	return false;
}


void MoveObj(void)
{
	int xmove;
	int ymove;
	boolean blocked;
	boolean blocked2;

	xmove = obj->xmove;
	ymove = obj->ymove;

	GetHitbox();

	if (xmove < 0)
		xmove = (xmove - 255) / 256;
	else if (xmove > 0)
		xmove = (xmove + 255) / 256;

	if (ymove < 0)
		ymove = (ymove - 255) / 256;
	else if (ymove > 0)
		ymove = (ymove + 255) / 256;

	p_top    += ymove;
	p_bottom += ymove;
	p_left  += xmove;
	p_right += xmove;

	blocked = ObjBlocked();

	p_top    -= ymove;
	p_bottom -= ymove;
	p_left  -= xmove;
	p_right -= xmove;

	if (!blocked)
		return;

	if (xmove < 0)
	{
		p_left  += xmove;
		p_right += xmove;

		blocked = ObjBlocked();

		p_left  -= xmove;
		p_right -= xmove;

		if (blocked == true)
			obj->xmove = (-p_left % 16) << 8;
	}
	else if (xmove > 0)
	{
		p_left  += xmove;
		p_right += xmove;

		blocked = ObjBlocked();

		p_left  -= xmove;
		p_right -= xmove;

		if (blocked == true)
			obj->xmove = (15 - (p_right % 16)) << 8;
	}

	if (ymove < 0)
	{
		p_top    += ymove;
		p_bottom += ymove;

		blocked2 = ObjBlocked();

		p_top    -= ymove;
		p_bottom -= ymove;

		if (blocked2 == true)
			obj->ymove = (-p_top % 16) << 8;
	}
	else if (ymove > 0)
	{
		p_top    += ymove;
		p_bottom += ymove;

		blocked2 = ObjBlocked();

		p_top    -= ymove;
		p_bottom -= ymove;

		if (blocked2 == true)
			obj->ymove = (15 - (p_bottom % 16)) << 8;
	}

	if (!blocked && !blocked2)
		obj->xmove = obj->ymove = 0;
}


boolean MoveIsBlocked(int xmove, int ymove)
{
	obj->x += xmove;
	obj->y += ymove;

	GetHitbox();

	obj->x -= xmove;
	obj->y -= ymove;

	if (p_bottom - originy / 4 < 16  && obj->ymove <= 0)
		return true;

	if (p_bottom - originy / 4 > 172 && obj->ymove >= 0)
		return true;

	if (p_left < 6 || p_right > 303)
		return true;

	return ObjBlocked();
}


void ControlDave(void)
{
	int i;
	int multiplier;
	int tile_left;
	int tile_right;
	int xfactor;

	ControlStruct c;

	int xmove = 0;
	int ymove = 0;

	if (obj->x < 0x00001000)
		obj->x = 0x00001000;

	if ((long)(levelheader->width - 1) << 8 << 4 < obj->x)
		obj->x = (long)(levelheader->width - 1) << 8 << 4;

	if ((long)levelheader->height << 8 << 4 < obj->y)
	{
		obj->obclass = nothing;
		PlaySound(PLUMMETSND);
		WaitEndSound();
		return;
	}

	c = ControlPlayer (playernum + 1);
	if (c.button1)
	{
		if (!jumping && !jumpheld)
		{
			PlaySound(JUMPSND);
			ymove = -basejumpmove;
			jumping = true;
			jumpheld = true;
			jumpspeed = basejumpspeed;
		}

		if (jumpheld && jumpspeed > 0)
		{
			ymove -= jumpspeed;
			jumpspeed -= 7;
		}
	}
	else
	{
		jumpspeed = 0;
		if (!jumping)
			jumpheld = false;
	}

	if (c.button2)
		multiplier = 3;
	else
		multiplier = 2;

	lastctrl = c;

	switch (c.dir)
	{
		case northeast:
		case east:
		case southeast:
			xmove = basewalkspeed * multiplier;
			faceleft = false;
			break;

		case southwest:
		case west:
		case northwest:
			xmove = -basewalkspeed * multiplier;
			faceleft = true;
			break;
	}

	ymove += basegravity;

	if (jumping)
	{
		if (faceleft)
			tmp_baseshape = DAVEJLSPR;
		else
			tmp_baseshape = DAVEJRSPR;
	}
	else
	{
		if (obj->walktick-- == 0)
		{
			obj->walktick = obj->walkticks;

			if (++obj->walkframe == 4)
				obj->walkframe = 0;
		}

		if (xmove == 0)
		{
			obj->walkframe = 1;
			obj->walktick = obj->walkticks;
		}

		if (faceleft)
			tmp_baseshape = obj->walkframe + DAVEL1SPR;
		else
			tmp_baseshape = obj->walkframe + DAVER1SPR;
	}

	GetHitbox();

	tile_left  = GetTile(p_left,  p_bottom + 1);
	tile_right = GetTile(p_right, p_bottom + 1);

	if (tile_xfactor[tile_left])
	{
		if (jumpspeed == 0 && tile_block[tile_left])
			jumping = false;

		xfactor = tile_xfactor[tile_left];
	}

	if (tile_xfactor[tile_right])
	{
		if (jumpspeed == 0 && tile_block[tile_right])
			jumping = false;

		xfactor = tile_xfactor[tile_right];
	}

	obj->xmove += xmove;
	obj->ymove += ymove;

	obj->xmove = (long)obj->xmove * xfactor / 256;
	obj->ymove = obj->ymove * 9 / 10;

	ymove = obj->ymove;

	MoveObj();

	if (obj->ymove == 0 && ymove < 0)
		PlaySound(BUMPSND);

	if (obj->ymove != 0)
		jumping = true;

	if (obj->ymove >= 0)
		jumpspeed = 0;

	xmove = obj->xmove;
	ymove = obj->ymove;

	obj->x += xmove;
	obj->y += ymove;

	if (xmove > 0 && obj->x - originx > 0xB400L)
	{
		originx += xmove;
		if (originx > originxmax)
			originx = originxmax;
	}
	else if (xmove < 0 && obj->x - originx < 0x8C00L)
	{
		originx += xmove;
		if (originx < originxmin)
			originx = originxmin;
	}

	if (ymove > 0 && obj->y - originy > 0xA000L)
	{
		originy += ymove;
		if (originy > originymax)
			originy = originymax;
	}
	else if (ymove < 0 && obj->y - originy < 0x2800L)
	{
		originy += ymove;
		if (originy < originymin)
			originy = originymin;
	}

	GetHitbox();
	RF_PlaceSprite();

	{
		int tx;
		int ty;

		int tx1 = p_left   / 16;
		int ty1 = p_top    / 16;
		int tx2 = p_right  / 16;
		int ty2 = p_bottom / 16;

		for (ty = ty1; ty <= ty2; ty++)
		{
			for (tx = tx1; tx <= tx2; tx++)
			{
				int tilenum = mapplane[0][ty * mapwwide + tx];

				switch (tile_behavior[tilenum])
				{
					case 1:
						PlaySound(GRABCOINSND);
						AddScore(100);
						mapplane[0][ty * mapwwide + tx] = 0;
						break;
				}
			}
		}
	}

	for (hit = &objlist[1], i = 1; i < numobj; i++, hit++)
	{
		if (hit->active && ObjectsCollide())
			hit->contact();
	}
}


#define EXTRASCORE 10000

void AddScore(int toadd)
{
	score += toadd;
	if (score - EXTRASCORE >= lastextra)
	{
		lastextra += EXTRASCORE;
		davesleft++;
	}
}


void levelloop(void)
{
	RF_ForceRefresh();
	do
	{
		spritesshown = coverlistindex = firstsprite = 0;
		spritelistptr = &spritelist[0];
		WaitVBL();
		obj = &objlist[0];
		ControlDave();
		RF_Refresh();
		dofkeys();

		switch (indemo)
		{
		case notdemo:
			if (keydown[0x2e] && keydown[0x14] && keydown[0x39])	// C-T-SPACEBAR
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
			if (objlist[0].obclass == nothing || (keydown[0x42] && keydown[0x20]))	// F8-D
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
			indemo = notdemo;
			ctrl = ControlPlayer (1);
			if (ctrl.button1 || ctrl.button2 || keydown[0x39])	// SPACEBAR
			{
				indemo = demoplay;
				exitdemo = true;
				return;
			}
			indemo = demoplay;
			break;
		}
	} while (!leveldone && objlist[0].obclass != nothing);
}


/*======================================*/
/*				      */
/* playloop                             */
/* all the action is directed from here */
/*				      */
/*======================================*/

void playloop(void)
{
	char st[6];
	int i;

	EGApage = 0;
	davesleft = 4;
	lastextra = 0;
	score = 0;
	leveldone = true;

	RF_ForceRefresh();

	refreshcount = 0;

	do
	{
		if (leveldone)
		{
			spritelist[0].screenindex = 0xffff;
			spritelistptr = &spritelist[0];
			spritesshown = 0;
			coverlistindex = 0;
			firstsprite = 0;

			strcpy(str, "LEVEL0");
			itoa(level, st, 10);
			strcat(str, st);
			strcat(str, ".");
			strcat(str, _extension);

			LoadFile(str, (char *)levelheader);

			for (i = 0; i < levelheader->planes; i++) {
				mapplane[i] = (unsigned int *)((char *)levelheader + i * levelheader->planesize + 32);
			}

			numobj = 1;
			mapwwide = ((LevelDef *)levelheader)->width;
			mapbwide = mapwwide * 2;
			mapwidthextra = mapbwide + -(2 * PORTTILESWIDE);
			originxmin = 0;
			originymin = 0;

			originxmax = ((long)(levelheader->width  + -(PORTTILESWIDE - 1))) << 12;
			originymax = ((long)(levelheader->height + -(PORTTILESHIGH - 1))) << 12;

			originy = originymax;
			originx = 0;
			basegravity = 160;
			basewalkspeed  =  80;
			basejumpspeed = 300;
			basejumpmove = 750;
			RF_Refresh();
			virtualscreen();
			RF_Refresh();
			leveldone = false;
		}

		if (indemo != notdemo)
		{
			initrndt(false);
		}
		else
		{
			initrndt(true);
			originy = originymax;
			originx = 0;
			RF_ForceRefresh();
			RF_Refresh();
			RF_Refresh();
			expwin(14, 9);
			print("\n SCORE:");
			printlong(score);
			print("\n\n WORLD:");
			printint(level);
			print("\n\n DAVES LEFT:");
			printint(davesleft);
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

		objlist[0].y = originy + 0x9600;
		objlist[0].x = 0x00009600;
		objlist[0].ymove = 0;
		objlist[0].xmove = 0;
		objlist[0].obclass = playerobj;
		objlist[0].walkframe = 2;
		objlist[0].walkticks = objlist[0].walktick = 5;
		objlist[0].active = false;
		objlist[0].word_789_9500 = 0;	// is set here but never used
		objlist[0].think = ControlDave;

		lastctrl.button1 = 0;
		lastctrl.button1 = 0;

		jumpspeed = 0;
		jumping = true;
		jumpheld = false;
		faceleft = false;
		originy = originymax;
		originx = 0;

		levelloop();

		if (indemo != notdemo)
			return;

		if (!leveldone)
		{
			davesleft--;
		}
		else
		{
			level++;
			if (level > _numlevels)
			{
				davesleft = 0;
				gamestate = inend;
			}
		}
	} while (davesleft != 0);
}

/***************************************************************************/
/***************************************************************************/

/*=========================*/
/*			   */
/* m a i n   p r o g r a m */
/*			   */
/*=========================*/

void main (void)
{
	_numlevels = 1;
	_maxplayers = 1;

	_cgaok = false;
	_egaok = true;
	_vgaok = false;

	_extension = "DD2";

	_setupgame();

	screencenterx = 25;
	extrahbls = 32;	// not used for EGA refresh

	_setupgrmode();

	levelheader = (LevelDef *)mapbuffer;

	while (1)			// go until quit () is called
	{
		dodemo();
		WaitEndSound();
		playloop();
		if (gamestate == inend)
		{
			doendpage ();		// finished all levels
		}
		gameover();
	}
}

