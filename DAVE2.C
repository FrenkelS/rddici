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

//#define CATALOG

/*
** catacomb II -- the c translation...
*/

#include "pcrlib.h"
//#include "NGRABCA2.H"
//#include "SOUNDS.H"

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

typedef enum {ingame,intitle,inscores} statetype;


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
//int word_789_A8 = 0; // 7B38
//int word_789_AA = 0;
//int word_789_AC = 0;
//int word_789_AE = 0;
//int word_789_B0 = 0;
//int word_789_B2 = 0;
//int word_789_B4 = 0;
//int word_789_B6 = 0;
//int word_789_B8 = 0;
//int word_789_BA = 0;
//int word_789_BC = 0;
//int word_789_BE = 0;
//int word_789_C0 = 0;
//int word_789_C2 = 1;
//int word_789_C4 = 1;
//int word_789_C6 = 1;
//int word_789_C8 = 1;
//int word_789_CA = 1;
//int word_789_CC = 1;
//int word_789_CD = 1;
//int word_789_D0 = 1;
//int word_789_D2 = 1;
//int word_789_D4 = 1;
//int word_789_D6 = 1;
//int word_789_D8 = 1;
//int word_789_DA = 1;
//int word_789_DC = 1;
//int word_789_DE = 1;
//int word_789_E0 = 1;
//int word_789_E2 = 1;
//int word_789_E4 = 1;
//int word_789_E6 = 1;
//int word_789_E8 = 1;
//int word_789_EA = 1;
//int word_789_EC = 1;
//int word_789_EE = 1;
//int word_789_F0 = 1;
//int word_789_F2 = 0;
//int word_789_F4 = 0;
//int word_789_F6 = 1;
//int word_789_F8 = 1;
//int word_789_FA = 1;
//int word_789_FC = 1;
//int word_789_FE = 1;
//int word_789_100 = 1;
//int word_789_102 = 1;
//int word_789_104 = 1;
//int word_789_106 = 1;
//int word_789_108 = 1;
//int word_789_10A = 1;
//int word_789_10C = 1;
//int word_789_10E = 0;
//int word_789_110 = 0;
//int word_789_112 = 0;
//int word_789_114 = 0;
//int word_789_116 = 0;
//int word_789_118 = 0;
//int word_789_11A = 1;
//int word_789_11C = 1;
//int word_789_11E = 1;
//int word_789_120 = 0;
//int word_789_122 = 0;
//int word_789_124 = 0;
//int word_789_126 = 0;
//int word_789_128 = 1;
//int word_789_12A = 1;
//int word_789_12C = 1;
//int word_789_12E = 1;
//int word_789_130 = 1;
//int word_789_132 = 0;
//int word_789_134 = 0;
//int word_789_136 = 0;
//int word_789_138 = 0;
//int word_789_13A = 0;
//int word_789_13C = 0;
//int word_789_13E = 0;
//int word_789_140 = 0;
//int word_789_142 = 0;
//int word_789_144 = 0;
//int word_789_146 = 0;
//int word_789_148 = 0;
//int word_789_14A = 0;
//int word_789_14C = 0;
//int word_789_14E = 0;
//int word_789_150 = 0;
//int word_789_152 = 0;
//int word_789_154 = 0;
//int word_789_156 = 0;
//int word_789_158 = 0;
//int word_789_15A = 0;
//int word_789_15C = 0;
//int word_789_15E = 0;
//int word_789_160 = 0;
//int word_789_162 = 0;
//int word_789_164 = 0;
//int word_789_166 = 0;
//int word_789_168 = 0;
//int word_789_16A = 0;
//int word_789_16C = 0;
//int word_789_16E = 0;
//int word_789_170 = 0;
//int word_789_172 = 0;
//int word_789_174 = 0;
//int word_789_176 = 0;
//int word_789_178 = 0;
//int word_789_17A = 0;
//int word_789_17C = 0;
//int word_789_17E = 0;
//int word_789_180 = 0;
//int word_789_182 = 0;
//int word_789_184 = 0;
//int word_789_186 = 0;
//int word_789_188 = 0;
//int word_789_18A = 0;
//int word_789_18C = 0;
//int word_789_18E = 0;
//int word_789_190 = 0;
//int word_789_192 = 0;
//int word_789_194 = 0;
//int word_789_196 = 0;
//int word_789_198 = 0;
//int word_789_19A = 0;
//int word_789_19C = 0;
//int word_789_19E = 0;
//int word_789_1A0 = 0;
//int word_789_1A2 = 0;
//int word_789_1A4 = 0;
//int word_789_1A6 = 0;
//int word_789_1A8 = 0;
//int word_789_1AA = 0;
//int word_789_1AC = 0;
//int word_789_1AE = 0;
//int word_789_1B0 = 0;
//int word_789_1B2 = 0;
//int word_789_1B4 = 0;
//int word_789_1B6 = 0;
//int word_789_1B8 = 0;
//int word_789_1BA = 0;
//int word_789_1BC = 0;
//int word_789_1BE = 0;
//int word_789_1C0 = 0;
//int word_789_1C2 = 0;
//int word_789_1C4 = 0;
//int word_789_1C6 = 0;
//int word_789_1C8 = 0;
//int word_789_1CA = 0;
//int word_789_1CC = 0;
//int word_789_1CE = 0;
//int word_789_1D0 = 0;
//int word_789_1D2 = 0;
//int word_789_1D4 = 0;
//int word_789_1D6 = 0;
//int word_789_1D8 = 0;
//int word_789_1DA = 0;
//int word_789_1DC = 0;
//int word_789_1DE = 0;
//int word_789_1E0 = 0;
//int word_789_1E2 = 0;
//int word_789_1E4 = 0;
//int word_789_1E6 = 0;
//int word_789_1E8 = 1;
//int word_789_1EA = 1;
//int word_789_1EC = 1;
//int word_789_1EE = 1;
//int word_789_1F0 = 0;
//int word_789_1F2 = 0;
//int word_789_1F4 = 0;
//int word_789_1F6 = 0;
//int word_789_1F8 = 0;
//int word_789_1FA = 0;
//int word_789_1FC = 0;
//int word_789_1FE = 0;
//int word_789_200 = 0;
//int word_789_202 = 0;
//int word_789_204 = 0;
//int word_789_206 = 0;
//int word_789_208 = 0;
//int word_789_20A = 0;
//int word_789_20C = 0;
//int word_789_20E = 0;
//int word_789_210 = 0;
//int word_789_212 = 0;
//int word_789_214 = 0;
//int word_789_216 = 0;
//int word_789_218 = 0;
//int word_789_21A = 0;
//int word_789_21C = 0;
//int word_789_21E = 0;
//int word_789_220 = 0;
//int word_789_222 = 0;
//int word_789_224 = 0;
//int word_789_226 = 0;
//int word_789_228 = 0;
//int word_789_22A = 0;
//int word_789_22C = 0;
//int word_789_22E = 0;
//int word_789_230 = 0;
//int word_789_232 = 0;
//int word_789_234 = 0;
//int word_789_236 = 0;
//int word_789_238 = 0;
//int word_789_23A = 1;
//int word_789_23C = 2;
//int word_789_23E = 3;
//int word_789_240 = 4;
//int word_789_242 = 5;
//int word_789_244 = 6;
//int word_789_246 = 7;
//int word_789_248 = 8;
//int word_789_24a = 9;
//int word_789_24c = 10;
//int word_789_24e = 11;
//int word_789_250 = 12;
//int word_789_252 = 14;
//int word_789_254 = 15;
//int word_789_256 = 16;
//int word_789_258 = 13;
//int word_789_25a = 17;
//int word_789_25c = 0x0012;
//int word_789_25e = 0x0013;
//int word_789_260 = 0x0014;
//int word_789_262 = 0x0015;
//int word_789_264 = 0x0016;
//int word_789_266 = 0x0017;
//int word_789_268 = 0x0018;
//int word_789_26a = 0x0019;
//int word_789_26c = 0x001a;
//int word_789_26e = 0x001b;
//int word_789_270 = 0x001c;
//int word_789_272 = 0x001d;
//int word_789_274 = 0x001e;
//int word_789_276 = 0x001f;
//int word_789_278 = 0x0020;
//int word_789_27a = 0x0021;
//int word_789_27c = 0x0022;
//int word_789_27e = 0x0023;
//int word_789_280 = 0x0024;
//int word_789_282 = 0x0025;
//int word_789_284 = 0x0026;
//int word_789_286 = 0x0027;
//int word_789_288 = 0x0028;
//int word_789_28a = 0x0029;
//int word_789_28c = 0x002a;
//int word_789_28e = 0x002b;
//int word_789_290 = 0x002c;
//int word_789_292 = 0x002d;
//int word_789_294 = 0x002e;
//int word_789_296 = 0x002f;
//int word_789_298 = 0x0030;
//int word_789_29a = 0x0031;
//int word_789_29c = 0x0032;
//int word_789_29e = 0x0033;
//int word_789_2a0 = 0x0034;
//int word_789_2a2 = 0x0035;
//int word_789_2a4 = 0x0036;
//int word_789_2a6 = 0x0037;
//int word_789_2a8 = 0x0038;
//int word_789_2aa = 0x0039;
//int word_789_2ac = 0x003a;
//int word_789_2ae = 0x003b;
//int word_789_2b0 = 0x003d;
//int word_789_2b2 = 0x003e;
//int word_789_2b4 = 0x003f;
//int word_789_2b6 = 0x003c;
//int word_789_2b8 = 0x0041;
//int word_789_2ba = 0x0042;
//int word_789_2bc = 0x0043;
//int word_789_2be = 0x0040;
//int word_789_2c0 = 0x0044;
//int word_789_2c2 = 0x0045;
//int word_789_2c4 = 0x0046;
//int word_789_2c6 = 0x0047;
//int word_789_2c8 = 0x0048;
//int word_789_2ca = 0x0049;
//int word_789_2cc = 0x004a;
//int word_789_2ce = 0x004b;
//int word_789_2d0 = 0x004c;
//int word_789_2d2 = 0x004d;
//int word_789_2d4 = 0;
//int word_789_2d6 = 0;
//int word_789_2d8 = 0;
//int word_789_2da = 0;
//int word_789_2dc = 0;
//int word_789_2de = 0;
//int word_789_2e0 = 0;
//int word_789_2e2 = 0;
//int word_789_2e4 = 0;
//int word_789_2e6 = 0;
//int word_789_2e8 = 0;
//int word_789_2ea = 0;
//int word_789_2ec = 0;
//int word_789_2ee = 0;
//int word_789_2f0 = 0;
//int word_789_2f2 = 0;
//int word_789_2f4 = 0;
//int word_789_2f6 = 0;
//int word_789_2f8 = 0;
//int word_789_2fa = 0;
//int word_789_2fc = 0;
//int word_789_2fe = 0;
//int word_789_300 = 0x00d7;
//int word_789_302 = 0x00d7;
//int word_789_304 = 0x00d7;
//int word_789_306 = 0x00d7;
//int word_789_308 = 0x00d7;
//int word_789_30a = 0x00d7;
//int word_789_30c = 0x00d7;
//int word_789_30e = 0x00d7;
//int word_789_310 = 0x00d7;
//int word_789_312 = 0x00d7;
//int word_789_314 = 0x00d7;
//int word_789_316 = 0x00d7;
//int word_789_318 = 0x00d7;
//int word_789_31a = 0x00d7;
//int word_789_31c = 0x00d7;
//int word_789_31e = 0x00d7;
//int word_789_320 = 0x00d7;
//int word_789_322 = 0x00d7;
//int word_789_324 = 0x00d7;
//int word_789_326 = 0x00d7;
//int word_789_328 = 0x00d7;
//int word_789_32a = 0x00d7;
//int word_789_32c = 0x00d7;
//int word_789_32e = 0x00d7;
//int word_789_330 = 0x00d7;
//int word_789_332 = 0x00d7;
//int word_789_334 = 0x00d7;
//int word_789_336 = 0x00d7;
//int word_789_338 = 0x00d7;
//int word_789_33a = 0x00d7;
//int word_789_33c = 0x00d7;
//int word_789_33e = 0x00d7;
//int word_789_340 = 0x00d7;
//int word_789_342 = 0x00d7;
//int word_789_344 = 0x00d7;
//int word_789_346 = 0x00d7;
//int word_789_348 = 0x00d7;
//int word_789_34a = 0x00d7;
//int word_789_34c = 0x00d7;
//int word_789_34e = 0x00d7;
//int word_789_350 = 0x00d7;
//int word_789_352 = 0x00d7;
//int word_789_354 = 0x00d7;
//int word_789_356 = 0x00d7;
//int word_789_358 = 0x00d7;
//int word_789_35a = 0x00d7;
//int word_789_35c = 0x00d7;
//int word_789_35e = 0x00d7;
//int word_789_360 = 0x00d7;
//int word_789_362 = 0x00d7;
//int word_789_364 = 0x00d7;
//int word_789_366 = 0x00d7;
//int word_789_368 = 0x00d7;
//int word_789_36a = 0x00d7;
//int word_789_36c = 0x00d7;
//int word_789_36e = 0x00d7;
//int word_789_370 = 0x00d7;
//int word_789_372 = 0x00d7;
//int word_789_374 = 0x00d7;
//int word_789_376 = 0x00d7;
//int word_789_378 = 0x00d7;
//int word_789_37a = 0x00d7;
//int word_789_37c = 0x00d7;
//int word_789_37e = 0x00d7;
//int word_789_380 = 0x00d7;
//int word_789_382 = 0x00d7;
//int word_789_384 = 0x00d7;
//int word_789_386 = 0x00d7;
//int word_789_388 = 0x00d7;
//int word_789_38a = 0x00d7;
//int word_789_38c = 0x00d7;
//int word_789_38e = 0x00d7;
//int word_789_390 = 0x00d7;
//int word_789_392 = 0x00d7;
//int word_789_394 = 0x00d7;
//int word_789_396 = 0x00d7;
//int word_789_398 = 0x00d7;
//int word_789_39a = 0x00f5;
//int word_789_39c = 0x00f5;
//int word_789_39e = 0x00f5;
//int word_789_3a0 = 0x00f5;
//int word_789_3a2 = 0x00d7;
//int word_789_3a4 = 0x00d7;
//int word_789_3a6 = 0x00d7;
//int word_789_3a8 = 0x00d7;
//int word_789_3aa = 0x00d7;
//int word_789_3ac = 0x00d7;
//int word_789_3ae = 0x00d7;
//int word_789_3b0 = 0x00d7;
//int word_789_3b2 = 0x00d7;
//int word_789_3b4 = 0x00d7;
//int word_789_3b6 = 0;
//int word_789_3b8 = 0;
//int word_789_3ba = 0;
//int word_789_3bc = 0;
//int word_789_3be = 0;
//int word_789_3c0 = 0;
//int word_789_3c2 = 0;
//int word_789_3c4 = 0;
//int word_789_3c6 = 0;
//char aCgapics_dd2[] = "CGAPICS.DD2";
//char aEgapics_dd2[] = "EGAPICS.DD2";
//char aResetGameYN[] = "RESET GAME (Y/N)?";
//char aPaused[] = "PAUSED";
//char aQuitYN[] = "QUIT (Y/N)?";
//char unk_789_405 = 0;
//char aDave2HelpScree[] = "dave2 help screen";
//char aDave2TitleScre[] = "Dave 2 title screen";
//char aGameOver[] = "\n GAME OVER\n     ";
//char aBadthink[] = "badTHINK!";
//char aWarpToLevel[] = "Warp to level:";
//char aSaveAsDemo[] = "SAVE AS DEMO#:";
//char aLevel0[] = "LEVEL0";
//char a__1[] = ".";
//char aScore[] = "\n SCORE:";
//char aWorld[] = "\n\n WORLD:";
//char aDavesLeft[] = "\n\n DAVES LEFT:";
//char aRecordDemo[] = "RECORD DEMO";
//char aDd2[] = "DD2";
//char unk_789_4a1 = 0;
//int word_789_4a2 = 0;
//int word_789_4a4 = 0;
//int word_789_4a6 = 0;
//int word_789_4a8 = 0x2193;
//int word_789_4aa = 0x21b5;
//int word_789_4ac = 0x21d9;
//int word_789_4ae = 0x2209;
//int word_789_4b0 = 0x2239;
//int word_789_4b2 = 0x2273;
//int word_789_4b4 = 0x22ad;
//int word_789_4b6 = 0x22f1;
//int word_789_4b8 = 0x2335;
//int word_789_4ba = 0;
//int word_789_4bc = 0x247e;
//int word_789_4be = 0x2498;
//int word_789_4c0 = 0x24b6;
//int word_789_4c2 = 0x24de;
//int word_789_4c4 = 0x2506;
//int word_789_4c6 = 0x2538;
//int word_789_4c8 = 0;
//int word_789_4ca = 0x0040;
//int word_789_4cc = 0x0080;
//int word_789_4ce = 0x00c0;
//int word_789_4d0 = 0x0100;
//int word_789_4d2 = 0x0140;
//int word_789_4d4 = 0x0180;
//int word_789_4d6 = 0x01c0;
//int word_789_4d8 = 0x0200;
//int word_789_4da = 0x0240;
//int word_789_4dc = 0x0280;
//int word_789_4de = 0x02c0;
//int word_789_4e0 = 0x0300;
//int word_789_4e2 = 0x0340;
//int word_789_4e4 = 0x0380;
//int word_789_4e6 = 0x03c0;
//int word_789_4e8 = 0x0400;
//int word_789_4ea = 0x0440;
//int word_789_4ec = 0x0480;
//int word_789_4ee = 0x04c0;
//int word_789_4f0 = 0x0500;
//int word_789_4f2 = 0x0540;
//int word_789_4f4 = 0x0580;
//int word_789_4f6 = 0x05c0;
//int word_789_4f8 = 0x0600;
//int word_789_4fa = 0x0640;
//int word_789_4fc = 0x0680;
//int word_789_4fe = 0x06c0;
//int word_789_500 = 0x0700;
//int word_789_502 = 0x0740;
//int word_789_504 = 0x0780;
//int word_789_506 = 0x07c0;
//int word_789_508 = 0x0800;
//int word_789_50a = 0x0840;
//int word_789_50c = 0x0880;
//int word_789_50e = 0x08c0;
//int word_789_510 = 0x0900;
//int word_789_512 = 0x0940;
//int word_789_514 = 0x0980;
//int word_789_516 = 0x09c0;
//int word_789_518 = 0x0a00;
//int word_789_51a = 0x0a40;
//int word_789_51c = 0x0a80;
//int word_789_51e = 0x0ac0;
//int word_789_520 = 0x0b00;
//int word_789_522 = 0x0b40;
//int word_789_524 = 0x0b80;
//int word_789_526 = 0x0bc0;
//int word_789_528 = 0x0c00;
//int word_789_52a = 0x0c40;
//int word_789_52c = 0x0c80;
//int word_789_52e = 0x0cc0;
//int word_789_530 = 0x0d00;
//int word_789_532 = 0x0d40;
//int word_789_534 = 0x0d80;
//int word_789_536 = 0x0dc0;
//int word_789_538 = 0x0e00;
//int word_789_53a = 0x0e40;
//int word_789_53c = 0x0e80;
//int word_789_53e = 0x0ec0;
//int word_789_540 = 0x0f00;
//int word_789_542 = 0x0f40;
//int word_789_544 = 0x0f80;
//int word_789_546 = 0x0fc0;
//int word_789_548 = 0x1000;
//int word_789_54a = 0x1040;
//int word_789_54c = 0x1080;
//int word_789_54e = 0x10c0;
//int word_789_550 = 0x1100;
//int word_789_552 = 0x1140;
//int word_789_554 = 0x1180;
//int word_789_556 = 0x11c0;
//int word_789_558 = 0x1200;
//int word_789_55a = 0x1240;
//int word_789_55c = 0x1280;
//int word_789_55e = 0x12c0;
//int word_789_560 = 0x1300;
//int word_789_562 = 0x1340;
//int word_789_564 = 0x1380;
//int word_789_566 = 0x13c0;
//int word_789_568 = 0x1400;
//int word_789_56a = 0x1440;
//int word_789_56c = 0x1480;
//int word_789_56e = 0x14c0;
//int word_789_570 = 0x1500;
//int word_789_572 = 0x1540;
//int word_789_574 = 0x1580;
//int word_789_576 = 0x15c0;
//int word_789_578 = 0x1600;
//int word_789_57a = 0x1640;
//int word_789_57c = 0x1680;
//int word_789_57e = 0x16c0;
//int word_789_580 = 0x1700;
//int word_789_582 = 0x1740;
//int word_789_584 = 0x1780;
//int word_789_586 = 0x17c0;
//int word_789_588 = 0x1800;
//int word_789_58a = 0x1840;
//int word_789_58c = 0x1880;
//int word_789_58e = 0x18c0;
//int word_789_590 = 0x1900;
//int word_789_592 = 0x1940;
//int word_789_594 = 0x1980;
//int word_789_596 = 0x19c0;
//int word_789_598 = 0x1a00;
//int word_789_59a = 0x1a40;
//int word_789_59c = 0x1a80;
//int word_789_59e = 0x1ac0;
//int word_789_5a0 = 0x1b00;
//int word_789_5a2 = 0x1b40;
//int word_789_5a4 = 0x1b80;
//int word_789_5a6 = 0x1bc0;
//int word_789_5a8 = 0x1c00;
//int word_789_5aa = 0x1c40;
//int word_789_5ac = 0x1c80;
//int word_789_5ae = 0x1cc0;
//int word_789_5b0 = 0x1d00;
//int word_789_5b2 = 0x1d40;
//int word_789_5b4 = 0x1d80;
//int word_789_5b6 = 0x1dc0;
//int word_789_5b8 = 0x1e00;
//int word_789_5ba = 0x1e40;
//int word_789_5bc = 0x1e80;
//int word_789_5be = 0x1ec0;
//int word_789_5c0 = 0x1f00;
//int word_789_5c2 = 0x1f40;
//int word_789_5c4 = 0x1f80;
//int word_789_5c6 = 0x1fc0;
//int word_789_5c8 = 0x2000;
//int word_789_5ca = 0x2040;
//int word_789_5cc = 0x2080;
//int word_789_5ce = 0x20c0;
//int word_789_5d0 = 0x2100;
//int word_789_5d2 = 0x2140;
//int word_789_5d4 = 0x2180;
//int word_789_5d6 = 0x21c0;
//int word_789_5d8 = 0x2200;
//int word_789_5da = 0x2240;
//int word_789_5dc = 0x2280;
//int word_789_5de = 0x22c0;
//int word_789_5e0 = 0x2300;
//int word_789_5e2 = 0x2340;
//int word_789_5e4 = 0x2380;
//int word_789_5e6 = 0x23c0;
//int word_789_5e8 = 0x2400;
//int word_789_5ea = 0x2440;
//int word_789_5ec = 0x2480;
//int word_789_5ee = 0x24c0;
//int word_789_5f0 = 0x2500;
//int word_789_5f2 = 0x2540;
//int word_789_5f4 = 0x2580;
//int word_789_5f6 = 0x25c0;
//int word_789_5f8 = 0x2600;
//int word_789_5fa = 0x2640;
//int word_789_5fc = 0x2680;
//int word_789_5fe = 0x26c0;
//int word_789_600 = 0x2700;
//int word_789_602 = 0x2740;
//int word_789_604 = 0x2780;
//int word_789_606 = 0x27c0;
//int word_789_608 = 0x2800;
//int word_789_60a = 0x2840;
//int word_789_60c = 0x2880;
//int word_789_60e = 0x28c0;
//int word_789_610 = 0x2900;
//int word_789_612 = 0x2940;
//int word_789_614 = 0x2980;
//int word_789_616 = 0x29c0;
//int word_789_618 = 0x2a00;
//int word_789_61a = 0x2a40;
//int word_789_61c = 0x2a80;
//int word_789_61e = 0x2ac0;
//int word_789_620 = 0x2b00;
//int word_789_622 = 0x2b40;
//int word_789_624 = 0x2b80;
//int word_789_626 = 0x2bc0;
//int word_789_628 = 0x2c00;
//int word_789_62a = 0x2c40;
//int word_789_62c = 0x2c80;
//int word_789_62e = 0x2cc0;
//int word_789_630 = 0x2d00;
//int word_789_632 = 0x2d40;
//int word_789_634 = 0x2d80;
//int word_789_636 = 0x2dc0;
//int word_789_638 = 0x2e00;
//int word_789_63a = 0x2e40;
//int word_789_63c = 0x2e80;
//int word_789_63e = 0x2ec0;
//int word_789_640 = 0x2f00;
//int word_789_642 = 0x2f40;
//int word_789_644 = 0x2f80;
//int word_789_646 = 0x2fc0;
//int word_789_648 = 0x3000;
//int word_789_64a = 0x3040;
//int word_789_64c = 0x3080;
//int word_789_64e = 0x30c0;
//int word_789_650 = 0x3100;
//int word_789_652 = 0x3140;
//int word_789_654 = 0x3180;
//int word_789_656 = 0x31c0;
//int word_789_658 = 0x3200;
//int word_789_65a = 0x3240;
//int word_789_65c = 0x3280;
//int word_789_65e = 0x32c0;
//int word_789_660 = 0x3300;
//int word_789_662 = 0x3340;
//int word_789_664 = 0x3380;
//int word_789_666 = 0x33c0;
//int word_789_668 = 0x3400;
//int word_789_66a = 0x3440;
//int word_789_66c = 0x3480;
//int word_789_66e = 0x34c0;
//int word_789_670 = 0x3500;
//int word_789_672 = 0x3540;
//int word_789_674 = 0x3580;
//int word_789_676 = 0x35c0;
//int word_789_678 = 0x3600;
//int word_789_67a = 0x3640;
//int word_789_67c = 0x3680;
//int word_789_67e = 0x36c0;
//int word_789_680 = 0x3700;
//int word_789_682 = 0x3740;
//int word_789_684 = 0x3780;
//int word_789_686 = 0x37c0;
//int word_789_688 = 0x3800;
//int word_789_68a = 0x3840;
//int word_789_68c = 0x3880;
//int word_789_68e = 0x38c0;
//int word_789_690 = 0x3900;
//int word_789_692 = 0x3940;
//int word_789_694 = 0x3980;
//int word_789_696 = 0x39c0;
//int word_789_698 = 0x3a00;
//int word_789_69a = 0x3a40;
//int word_789_69c = 0x3a80;
//int word_789_69e = 0x3ac0;
//int word_789_6a0 = 0x3b00;
//int word_789_6a2 = 0x3b40;
//int word_789_6a4 = 0x3b80;
//int word_789_6a6 = 0x3bc0;
//int word_789_6a8 = 0x3c00;
//int word_789_6aa = 0x3c40;
//int word_789_6ac = 0x3c80;
//int word_789_6ae = 0x3cc0;
//int word_789_6b0 = 0x3d00;
//int word_789_6b2 = 0x3d40;
//int word_789_6b4 = 0x3d80;
//int word_789_6b6 = 0x3dc0;
//int word_789_6b8 = 0x3e00;
//int word_789_6ba = 0x3e40;
//int word_789_6bc = 0x3e80;
//int word_789_6be = 0x3ec0;
//int word_789_6c0 = 0x3f00;
//int word_789_6c2 = 0x3f40;
//int word_789_6c4 = 0x3f80;
//int word_789_6c6 = 0x3fc0;
//int word_789_6c8 = 0;
//int word_789_6ca = 0x0020;
//int word_789_6cc = 0x0040;
//int word_789_6ce = 0x0060;
//int word_789_6d0 = 0x0080;
//int word_789_6d2 = 0x00a0;
//int word_789_6d4 = 0x00c0;
//int word_789_6d6 = 0x00e0;
//int word_789_6d8 = 0x0100;
//int word_789_6da = 0x0120;
//int word_789_6dc = 0x0140;
//int word_789_6de = 0x0160;
//int word_789_6e0 = 0x0180;
//int word_789_6e2 = 0x01a0;
//int word_789_6e4 = 0x01c0;
//int word_789_6e6 = 0x01e0;
//int word_789_6e8 = 0x0200;
//int word_789_6ea = 0x0220;
//int word_789_6ec = 0x0240;
//int word_789_6ee = 0x0260;
//int word_789_6f0 = 0x0280;
//int word_789_6f2 = 0x02a0;
//int word_789_6f4 = 0x02c0;
//int word_789_6f6 = 0x02e0;
//int word_789_6f8 = 0x0300;
//int word_789_6fa = 0x0320;
//int word_789_6fc = 0x0340;
//int word_789_6fe = 0x0360;
//int word_789_700 = 0x0380;
//int word_789_702 = 0x03a0;
//int word_789_704 = 0x03c0;
//int word_789_706 = 0x03e0;
//int word_789_708 = 0x0400;
//int word_789_70a = 0x0420;
//int word_789_70c = 0x0440;
//int word_789_70e = 0x0460;
//int word_789_710 = 0x0480;
//int word_789_712 = 0x04a0;
//int word_789_714 = 0x04c0;
//int word_789_716 = 0x04e0;
//int word_789_718 = 0x0500;
//int word_789_71a = 0x0520;
//int word_789_71c = 0x0540;
//int word_789_71e = 0x0560;
//int word_789_720 = 0x0580;
//int word_789_722 = 0x05a0;
//int word_789_724 = 0x05c0;
//int word_789_726 = 0x05e0;
//int word_789_728 = 0x0600;
//int word_789_72a = 0x0620;
//int word_789_72c = 0x0640;
//int word_789_72e = 0x0660;
//int word_789_730 = 0x0680;
//int word_789_732 = 0x06a0;
//int word_789_734 = 0x06c0;
//int word_789_736 = 0x06e0;
//int word_789_738 = 0x0700;
//int word_789_73a = 0x0720;
//int word_789_73c = 0x0740;
//int word_789_73e = 0x0760;
//int word_789_740 = 0x0780;
//int word_789_742 = 0x07a0;
//int word_789_744 = 0x07c0;
//int word_789_746 = 0x07e0;
//int word_789_748 = 0x0800;
//int word_789_74a = 0x0820;
//int word_789_74c = 0x0840;
//int word_789_74e = 0x0860;
//int word_789_750 = 0x0880;
//int word_789_752 = 0x08a0;
//int word_789_754 = 0x08c0;
//int word_789_756 = 0x08e0;
//int word_789_758 = 0x0900;
//int word_789_75a = 0x0920;
//int word_789_75c = 0x0940;
//int word_789_75e = 0x0960;
//int word_789_760 = 0x0980;
//int word_789_762 = 0x09a0;
//int word_789_764 = 0x09c0;
//int word_789_766 = 0x09e0;
//int word_789_768 = 0x0a00;
//int word_789_76a = 0x0a20;
//int word_789_76c = 0x0a40;
//int word_789_76e = 0x0a60;
//int word_789_770 = 0x0a80;
//int word_789_772 = 0x0aa0;
//int word_789_774 = 0x0ac0;
//int word_789_776 = 0x0ae0;
//int word_789_778 = 0x0b00;
//int word_789_77a = 0x0b20;
//int word_789_77c = 0x0b40;
//int word_789_77e = 0x0b60;
//int word_789_780 = 0x0b80;
//int word_789_782 = 0x0ba0;
//int word_789_784 = 0x0bc0;
//int word_789_786 = 0x0be0;
//int word_789_788 = 0x0c00;
//int word_789_78a = 0x0c20;
//int word_789_78c = 0x0c40;
//int word_789_78e = 0x0c60;
//int word_789_790 = 0x0c80;
//int word_789_792 = 0x0ca0;
//int word_789_794 = 0x0cc0;
//int word_789_796 = 0x0ce0;
//int word_789_798 = 0x0d00;
//int word_789_79a = 0x0d20;
//int word_789_79c = 0x0d40;
//int word_789_79e = 0x0d60;
//int word_789_7a0 = 0x0d80;
//int word_789_7a2 = 0x0da0;
//int word_789_7a4 = 0x0dc0;
//int word_789_7a6 = 0x0de0;
//int word_789_7a8 = 0x0e00;
//int word_789_7aa = 0x0e20;
//int word_789_7ac = 0x0e40;
//int word_789_7ae = 0x0e60;
//int word_789_7b0 = 0x0e80;
//int word_789_7b2 = 0x0ea0;
//int word_789_7b4 = 0x0ec0;
//int word_789_7b6 = 0x0ee0;
//int word_789_7b8 = 0x0f00;
//int word_789_7ba = 0x0f20;
//int word_789_7bc = 0x0f40;
//int word_789_7be = 0x0f60;
//int word_789_7c0 = 0x0f80;
//int word_789_7c2 = 0x0fa0;
//int word_789_7c4 = 0x0fc0;
//int word_789_7c6 = 0x0fe0;
//int word_789_7c8 = 0;
//int word_789_7ca = 0x0004;
//int word_789_7cc = 0x0008;
//int word_789_7ce = 0x000c;
//int word_789_7d0 = 0x0010;
//int word_789_7d2 = 0x0014;
//int word_789_7d4 = 0x0018;
//int word_789_7d6 = 0x001c;
//int word_789_7d8 = 0x0020;
//int word_789_7da = 0x0024;
//int word_789_7dc = 0x0028;
//int word_789_7de = 0x002c;
//int word_789_7e0 = 0x0030;
//int word_789_7e2 = 0x0034;
//int word_789_7e4 = 0x0038;
//int word_789_7e6 = 0x003c;
//int word_789_7e8 = 0x0040;
//int word_789_7ea = 0x0044;
//int word_789_7ec = 0x0048;
//int word_789_7ee = 0x004c;
//int word_789_7f0 = 0x0280;
//int word_789_7f2 = 0x0284;
//int word_789_7f4 = 0x0288;
//int word_789_7f6 = 0x028c;
//int word_789_7f8 = 0x0290;
//int word_789_7fa = 0x0294;
//int word_789_7fc = 0x0298;
//int word_789_7fe = 0x029c;
//int word_789_800 = 0x02a0;
//int word_789_802 = 0x02a4;
//int word_789_804 = 0x02a8;
//int word_789_806 = 0x02ac;
//int word_789_808 = 0x02b0;
//int word_789_80a = 0x02b4;
//int word_789_80c = 0x02b8;
//int word_789_80e = 0x02bc;
//int word_789_810 = 0x02c0;
//int word_789_812 = 0x02c4;
//int word_789_814 = 0x02c8;
//int word_789_816 = 0x02cc;
//int word_789_818 = 0x0500;
//int word_789_81a = 0x0504;
//int word_789_81c = 0x0508;
//int word_789_81e = 0x050c;
//int word_789_820 = 0x0510;
//int word_789_822 = 0x0514;
//int word_789_824 = 0x0518;
//int word_789_826 = 0x051c;
//int word_789_828 = 0x0520;
//int word_789_82a = 0x0524;
//int word_789_82c = 0x0528;
//int word_789_82e = 0x052c;
//int word_789_830 = 0x0530;
//int word_789_832 = 0x0534;
//int word_789_834 = 0x0538;
//int word_789_836 = 0x053c;
//int word_789_838 = 0x0540;
//int word_789_83a = 0x0544;
//int word_789_83c = 0x0548;
//int word_789_83e = 0x054c;
//int word_789_840 = 0x0780;
//int word_789_842 = 0x0784;
//int word_789_844 = 0x0788;
//int word_789_846 = 0x078c;
//int word_789_848 = 0x0790;
//int word_789_84a = 0x0794;
//int word_789_84c = 0x0798;
//int word_789_84e = 0x079c;
//int word_789_850 = 0x07a0;
//int word_789_852 = 0x07a4;
//int word_789_854 = 0x07a8;
//int word_789_856 = 0x07ac;
//int word_789_858 = 0x07b0;
//int word_789_85a = 0x07b4;
//int word_789_85c = 0x07b8;
//int word_789_85e = 0x07bc;
//int word_789_860 = 0x07c0;
//int word_789_862 = 0x07c4;
//int word_789_864 = 0x07c8;
//int word_789_866 = 0x07cc;
//int word_789_868 = 0x0a00;
//int word_789_86a = 0x0a04;
//int word_789_86c = 0x0a08;
//int word_789_86e = 0x0a0c;
//int word_789_870 = 0x0a10;
//int word_789_872 = 0x0a14;
//int word_789_874 = 0x0a18;
//int word_789_876 = 0x0a1c;
//int word_789_878 = 0x0a20;
//int word_789_87a = 0x0a24;
//int word_789_87c = 0x0a28;
//int word_789_87e = 0x0a2c;
//int word_789_880 = 0x0a30;
//int word_789_882 = 0x0a34;
//int word_789_884 = 0x0a38;
//int word_789_886 = 0x0a3c;
//int word_789_888 = 0x0a40;
//int word_789_88a = 0x0a44;
//int word_789_88c = 0x0a48;
//int word_789_88e = 0x0a4c;
//int word_789_890 = 0x0c80;
//int word_789_892 = 0x0c84;
//int word_789_894 = 0x0c88;
//int word_789_896 = 0x0c8c;
//int word_789_898 = 0x0c90;
//int word_789_89a = 0x0c94;
//int word_789_89c = 0x0c98;
//int word_789_89e = 0x0c9c;
//int word_789_8a0 = 0x0ca0;
//int word_789_8a2 = 0x0ca4;
//int word_789_8a4 = 0x0ca8;
//int word_789_8a6 = 0x0cac;
//int word_789_8a8 = 0x0cb0;
//int word_789_8aa = 0x0cb4;
//int word_789_8ac = 0x0cb8;
//int word_789_8ae = 0x0cbc;
//int word_789_8b0 = 0x0cc0;
//int word_789_8b2 = 0x0cc4;
//int word_789_8b4 = 0x0cc8;
//int word_789_8b6 = 0x0ccc;
//int word_789_8b8 = 0x0f00;
//int word_789_8ba = 0x0f04;
//int word_789_8bc = 0x0f08;
//int word_789_8be = 0x0f0c;
//int word_789_8c0 = 0x0f10;
//int word_789_8c2 = 0x0f14;
//int word_789_8c4 = 0x0f18;
//int word_789_8c6 = 0x0f1c;
//int word_789_8c8 = 0x0f20;
//int word_789_8ca = 0x0f24;
//int word_789_8cc = 0x0f28;
//int word_789_8ce = 0x0f2c;
//int word_789_8d0 = 0x0f30;
//int word_789_8d2 = 0x0f34;
//int word_789_8d4 = 0x0f38;
//int word_789_8d6 = 0x0f3c;
//int word_789_8d8 = 0x0f40;
//int word_789_8da = 0x0f44;
//int word_789_8dc = 0x0f48;
//int word_789_8de = 0x0f4c;
//int word_789_8e0 = 0x1180;
//int word_789_8e2 = 0x1184;
//int word_789_8e4 = 0x1188;
//int word_789_8e6 = 0x118c;
//int word_789_8e8 = 0x1190;
//int word_789_8ea = 0x1194;
//int word_789_8ec = 0x1198;
//int word_789_8ee = 0x119c;
//int word_789_8f0 = 0x11a0;
//int word_789_8f2 = 0x11a4;
//int word_789_8f4 = 0x11a8;
//int word_789_8f6 = 0x11ac;
//int word_789_8f8 = 0x11b0;
//int word_789_8fa = 0x11b4;
//int word_789_8fc = 0x11b8;
//int word_789_8fe = 0x11bc;
//int word_789_900 = 0x11c0;
//int word_789_902 = 0x11c4;
//int word_789_904 = 0x11c8;
//int word_789_906 = 0x11cc;
//int word_789_908 = 0x1400;
//int word_789_90a = 0x1404;
//int word_789_90c = 0x1408;
//int word_789_90e = 0x140c;
//int word_789_910 = 0x1410;
//int word_789_912 = 0x1414;
//int word_789_914 = 0x1418;
//int word_789_916 = 0x141c;
//int word_789_918 = 0x1420;
//int word_789_91a = 0x1424;
//int word_789_91c = 0x1428;
//int word_789_91e = 0x142c;
//int word_789_920 = 0x1430;
//int word_789_922 = 0x1434;
//int word_789_924 = 0x1438;
//int word_789_926 = 0x143c;
//int word_789_928 = 0x1440;
//int word_789_92a = 0x1444;
//int word_789_92c = 0x1448;
//int word_789_92e = 0x144c;
//int word_789_930 = 0x1680;
//int word_789_932 = 0x1684;
//int word_789_934 = 0x1688;
//int word_789_936 = 0x168c;
//int word_789_938 = 0x1690;
//int word_789_93a = 0x1694;
//int word_789_93c = 0x1698;
//int word_789_93e = 0x169c;
//int word_789_940 = 0x16a0;
//int word_789_942 = 0x16a4;
//int word_789_944 = 0x16a8;
//int word_789_946 = 0x16ac;
//int word_789_948 = 0x16b0;
//int word_789_94a = 0x16b4;
//int word_789_94c = 0x16b8;
//int word_789_94e = 0x16bc;
//int word_789_950 = 0x16c0;
//int word_789_952 = 0x16c4;
//int word_789_954 = 0x16c8;
//int word_789_956 = 0x16cc;
//int word_789_958 = 0x1900;
//int word_789_95a = 0x1904;
//int word_789_95c = 0x1908;
//int word_789_95e = 0x190c;
//int word_789_960 = 0x1910;
//int word_789_962 = 0x1914;
//int word_789_964 = 0x1918;
//int word_789_966 = 0x191c;
//int word_789_968 = 0x1920;
//int word_789_96a = 0x1924;
//int word_789_96c = 0x1928;
//int word_789_96e = 0x192c;
//int word_789_970 = 0x1930;
//int word_789_972 = 0x1934;
//int word_789_974 = 0x1938;
//int word_789_976 = 0x193c;
//int word_789_978 = 0x1940;
//int word_789_97a = 0x1944;
//int word_789_97c = 0x1948;
//int word_789_97e = 0x194c;
//int word_789_980 = 0x1b80;
//int word_789_982 = 0x1b84;
//int word_789_984 = 0x1b88;
//int word_789_986 = 0x1b8c;
//int word_789_988 = 0x1b90;
//int word_789_98a = 0x1b94;
//int word_789_98c = 0x1b98;
//int word_789_98e = 0x1b9c;
//int word_789_990 = 0x1ba0;
//int word_789_992 = 0x1ba4;
//int word_789_994 = 0x1ba8;
//int word_789_996 = 0x1bac;
//int word_789_998 = 0x1bb0;
//int word_789_99a = 0x1bb4;
//int word_789_99c = 0x1bb8;
//int word_789_99e = 0x1bbc;
//int word_789_9a0 = 0x1bc0;
//int word_789_9a2 = 0x1bc4;
//int word_789_9a4 = 0x1bc8;
//int word_789_9a6 = 0x1bcc;
//int word_789_9a8 = 0x0604;
//int word_789_9aa = 0x0606;
//int word_789_9ac = 0x0608;
//int word_789_9ae = 0x060a;
//int word_789_9b0 = 0x060c;
//int word_789_9b2 = 0x060e;
//int word_789_9b4 = 0x0610;
//int word_789_9b6 = 0x0612;
//int word_789_9b8 = 0x0614;
//int word_789_9ba = 0x0616;
//int word_789_9bc = 0x0618;
//int word_789_9be = 0x061a;
//int word_789_9c0 = 0x061c;
//int word_789_9c2 = 0x061e;
//int word_789_9c4 = 0x0620;
//int word_789_9c6 = 0x0622;
//int word_789_9c8 = 0x0624;
//int word_789_9ca = 0x0626;
//int word_789_9cc = 0x0628;
//int word_789_9ce = 0x062a;
//int word_789_9d0 = 0x062c;
//int word_789_9d2 = 0x0904;
//int word_789_9d4 = 0x0906;
//int word_789_9d6 = 0x0908;
//int word_789_9d8 = 0x090a;
//int word_789_9da = 0x090c;
//int word_789_9dc = 0x090e;
//int word_789_9de = 0x0910;
//int word_789_9e0 = 0x0912;
//int word_789_9e2 = 0x0914;
//int word_789_9e4 = 0x0916;
//int word_789_9e6 = 0x0918;
//int word_789_9e8 = 0x091a;
//int word_789_9ea = 0x091c;
//int word_789_9ec = 0x091e;
//int word_789_9ee = 0x0920;
//int word_789_9f0 = 0x0922;
//int word_789_9f2 = 0x0924;
//int word_789_9f4 = 0x0926;
//int word_789_9f6 = 0x0928;
//int word_789_9f8 = 0x092a;
//int word_789_9fa = 0x092c;
//int word_789_9fc = 0x0c04;
//int word_789_9fe = 0x0c06;
//int word_789_a00 = 0x0c08;
//int word_789_a02 = 0x0c0a;
//int word_789_a04 = 0x0c0c;
//int word_789_a06 = 0x0c0e;
//int word_789_a08 = 0x0c10;
//int word_789_a0a = 0x0c12;
//int word_789_a0c = 0x0c14;
//int word_789_a0e = 0x0c16;
//int word_789_a10 = 0x0c18;
//int word_789_a12 = 0x0c1a;
//int word_789_a14 = 0x0c1c;
//int word_789_a16 = 0x0c1e;
//int word_789_a18 = 0x0c20;
//int word_789_a1a = 0x0c22;
//int word_789_a1c = 0x0c24;
//int word_789_a1e = 0x0c26;
//int word_789_a20 = 0x0c28;
//int word_789_a22 = 0x0c2a;
//int word_789_a24 = 0x0c2c;
//int word_789_a26 = 0x0f04;
//int word_789_a28 = 0x0f06;
//int word_789_a2a = 0x0f08;
//int word_789_a2c = 0x0f0a;
//int word_789_a2e = 0x0f0c;
//int word_789_a30 = 0x0f0e;
//int word_789_a32 = 0x0f10;
//int word_789_a34 = 0x0f12;
//int word_789_a36 = 0x0f14;
//int word_789_a38 = 0x0f16;
//int word_789_a3a = 0x0f18;
//int word_789_a3c = 0x0f1a;
//int word_789_a3e = 0x0f1c;
//int word_789_a40 = 0x0f1e;
//int word_789_a42 = 0x0f20;
//int word_789_a44 = 0x0f22;
//int word_789_a46 = 0x0f24;
//int word_789_a48 = 0x0f26;
//int word_789_a4a = 0x0f28;
//int word_789_a4c = 0x0f2a;
//int word_789_a4e = 0x0f2c;
//int word_789_a50 = 0x1204;
//int word_789_a52 = 0x1206;
//int word_789_a54 = 0x1208;
//int word_789_a56 = 0x120a;
//int word_789_a58 = 0x120c;
//int word_789_a5a = 0x120e;
//int word_789_a5c = 0x1210;
//int word_789_a5e = 0x1212;
//int word_789_a60 = 0x1214;
//int word_789_a62 = 0x1216;
//int word_789_a64 = 0x1218;
//int word_789_a66 = 0x121a;
//int word_789_a68 = 0x121c;
//int word_789_a6a = 0x121e;
//int word_789_a6c = 0x1220;
//int word_789_a6e = 0x1222;
//int word_789_a70 = 0x1224;
//int word_789_a72 = 0x1226;
//int word_789_a74 = 0x1228;
//int word_789_a76 = 0x122a;
//int word_789_a78 = 0x122c;
//int word_789_a7a = 0x1504;
//int word_789_a7c = 0x1506;
//int word_789_a7e = 0x1508;
//int word_789_a80 = 0x150a;
//int word_789_a82 = 0x150c;
//int word_789_a84 = 0x150e;
//int word_789_a86 = 0x1510;
//int word_789_a88 = 0x1512;
//int word_789_a8a = 0x1514;
//int word_789_a8c = 0x1516;
//int word_789_a8e = 0x1518;
//int word_789_a90 = 0x151a;
//int word_789_a92 = 0x151c;
//int word_789_a94 = 0x151e;
//int word_789_a96 = 0x1520;
//int word_789_a98 = 0x1522;
//int word_789_a9a = 0x1524;
//int word_789_a9c = 0x1526;
//int word_789_a9e = 0x1528;
//int word_789_aa0 = 0x152a;
//int word_789_aa2 = 0x152c;
//int word_789_aa4 = 0x1804;
//int word_789_aa6 = 0x1806;
//int word_789_aa8 = 0x1808;
//int word_789_aaa = 0x180a;
//int word_789_aac = 0x180c;
//int word_789_aae = 0x180e;
//int word_789_ab0 = 0x1810;
//int word_789_ab2 = 0x1812;
//int word_789_ab4 = 0x1814;
//int word_789_ab6 = 0x1816;
//int word_789_ab8 = 0x1818;
//int word_789_aba = 0x181a;
//int word_789_abc = 0x181c;
//int word_789_abe = 0x181e;
//int word_789_ac0 = 0x1820;
//int word_789_ac2 = 0x1822;
//int word_789_ac4 = 0x1824;
//int word_789_ac6 = 0x1826;
//int word_789_ac8 = 0x1828;
//int word_789_aca = 0x182a;
//int word_789_acc = 0x182c;
//int word_789_ace = 0x1b04;
//int word_789_ad0 = 0x1b06;
//int word_789_ad2 = 0x1b08;
//int word_789_ad4 = 0x1b0a;
//int word_789_ad6 = 0x1b0c;
//int word_789_ad8 = 0x1b0e;
//int word_789_ada = 0x1b10;
//int word_789_adc = 0x1b12;
//int word_789_ade = 0x1b14;
//int word_789_ae0 = 0x1b16;
//int word_789_ae2 = 0x1b18;
//int word_789_ae4 = 0x1b1a;
//int word_789_ae6 = 0x1b1c;
//int word_789_ae8 = 0x1b1e;
//int word_789_aea = 0x1b20;
//int word_789_aec = 0x1b22;
//int word_789_aee = 0x1b24;
//int word_789_af0 = 0x1b26;
//int word_789_af2 = 0x1b28;
//int word_789_af4 = 0x1b2a;
//int word_789_af6 = 0x1b2c;
//int word_789_af8 = 0x1e04;
//int word_789_afa = 0x1e06;
//int word_789_afc = 0x1e08;
//int word_789_afe = 0x1e0a;
//int word_789_b00 = 0x1e0c;
//int word_789_b02 = 0x1e0e;
//int word_789_b04 = 0x1e10;
//int word_789_b06 = 0x1e12;
//int word_789_b08 = 0x1e14;
//int word_789_b0a = 0x1e16;
//int word_789_b0c = 0x1e18;
//int word_789_b0e = 0x1e1a;
//int word_789_b10 = 0x1e1c;
//int word_789_b12 = 0x1e1e;
//int word_789_b14 = 0x1e20;
//int word_789_b16 = 0x1e22;
//int word_789_b18 = 0x1e24;
//int word_789_b1a = 0x1e26;
//int word_789_b1c = 0x1e28;
//int word_789_b1e = 0x1e2a;
//int word_789_b20 = 0x1e2c;
//int word_789_b22 = 0x2104;
//int word_789_b24 = 0x2106;
//int word_789_b26 = 0x2108;
//int word_789_b28 = 0x210a;
//int word_789_b2a = 0x210c;
//int word_789_b2c = 0x210e;
//int word_789_b2e = 0x2110;
//int word_789_b30 = 0x2112;
//int word_789_b32 = 0x2114;
//int word_789_b34 = 0x2116;
//int word_789_b36 = 0x2118;
//int word_789_b38 = 0x211a;
//int word_789_b3a = 0x211c;
//int word_789_b3c = 0x211e;
//int word_789_b3e = 0x2120;
//int word_789_b40 = 0x2122;
//int word_789_b42 = 0x2124;
//int word_789_b44 = 0x2126;
//int word_789_b46 = 0x2128;
//int word_789_b48 = 0x212a;
//int word_789_b4a = 0x212c;
//int word_789_b4c = 0x2404;
//int word_789_b4e = 0x2406;
//int word_789_b50 = 0x2408;
//int word_789_b52 = 0x240a;
//int word_789_b54 = 0x240c;
//int word_789_b56 = 0x240e;
//int word_789_b58 = 0x2410;
//int word_789_b5a = 0x2412;
//int word_789_b5c = 0x2414;
//int word_789_b5e = 0x2416;
//int word_789_b60 = 0x2418;
//int word_789_b62 = 0x241a;
//int word_789_b64 = 0x241C;
//int word_789_b66 = 0x241E;
//int word_789_b68 = 0x2420;
//int word_789_b6a = 0x2422;
//int word_789_b6c = 0x2424;
//int word_789_b6e = 0x2426;
//int word_789_b70 = 0x2428;
//int word_789_b72 = 0x242A;
//int word_789_b74 = 0x242C;
//int word_789_b76 = 0x2704;
//int word_789_b78 = 0x2706;
//int word_789_b7a = 0x2708;
//int word_789_b7c = 0x270A;
//int word_789_b7e = 0x270C;
//int word_789_b80 = 0x270E;
//int word_789_b82 = 0x2710;
//int word_789_b84 = 0x2712;
//int word_789_b86 = 0x2714;
//int word_789_b88 = 0x2716;
//int word_789_b8a = 0x2718;
//int word_789_b8c = 0x271A;
//int word_789_b8e = 0x271C;
//int word_789_b90 = 0x271E;
//int word_789_b92 = 0x2720;
//int word_789_b94 = 0x2722;
//int word_789_b96 = 0x2724;
//int word_789_b98 = 0x2726;
//int word_789_b9a = 0x2728;
//int word_789_b9c = 0x272A;
//int word_789_b9e = 0x272C;
//int word_789_ba0 = 0x2A04;
//int word_789_ba2 = 0x2A06;
//int word_789_ba4 = 0x2A08;
//int word_789_ba6 = 0x2A0A;
//int word_789_ba8 = 0x2A0C;
//int word_789_baa = 0x2A0E;
//int word_789_bac = 0x2A10;
//int word_789_bae = 0x2A12;
//int word_789_bb0 = 0x2A14;
//int word_789_bb2 = 0x2A16;
//int word_789_bb4 = 0x2A18;
//int word_789_bb6 = 0x2A1A;
//int word_789_bb8 = 0x2A1C;
//int word_789_bba = 0x2A1E;
//int word_789_bbc = 0x2A20;
//int word_789_bbe = 0x2A22;
//int word_789_bc0 = 0x2A24;
//int word_789_bc2 = 0x2A26;
//int word_789_bc4 = 0x2A28;
//int word_789_bc6 = 0x2A2A;
//int word_789_bc8 = 0x2A2C;
//int word_789_bca = 0x2D04;
//int word_789_bcc = 0x2D06;
//int word_789_bce = 0x2D08;
//int word_789_bd0 = 0x2D0A;
//int word_789_bd2 = 0x2D0C;
//int word_789_bd4 = 0x2D0E;
//int word_789_bd6 = 0x2D10;
//int word_789_bd8 = 0x2D12;
//int word_789_bda = 0x2D14;
//int word_789_bdc = 0x2D16;
//int word_789_bde = 0x2D18;
//int word_789_be0 = 0x2D1A;
//int word_789_be2 = 0x2D1C;
//int word_789_be4 = 0x2D1E;
//int word_789_be6 = 0x2D20;
//int word_789_be8 = 0x2D22;
//int word_789_bea = 0x2D24;
//int word_789_bec = 0x2D26;
//int word_789_bee = 0x2D28;
//int word_789_bf0 = 0x2D2A;
//int word_789_bf2 = 0x2D2C;
//int word_789_bf4 = 0;
//int word_789_bf6 = 0;
//int word_789_bf8 = 0;
//int word_789_bfa = 0;
//int word_789_bfc = 0;
//int word_789_bfe = 0;


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
int word_789_1D30;
int word_789_1D32;
int word_789_1D34;
int word_789_1D36;
char unk_789_1D40[15890];
int word_789_7B20[600];
int *word_789_7FD0;
int word_789_8220;
//int word_789_8228;
int word_789_8476;
int word_789_8478;
int word_789_847A;
int word_789_847C;
int word_789_848E;
int word_789_947C;
int word_789_947E;
int word_789_9480;
int word_789_9482;
int word_789_9486[4];
int word_789_9490;
int word_789_9494;
int word_789_949E;
int word_789_94A2[12];
int word_789_94BC;
int word_789_94C4;
int word_789_94C6;
char *word_789_94C8;
int word_789_94CA;
int word_789_94CC;
int word_789_94CE;
long dword_789_94D0;
int word_789_94D4;
int word_789_94D6;
int word_789_94D8;
long dword_789_94DA;
int word_789_94E0;
int word_789_94E4;
int word_789_94E6;
int word_789_94E8;
int word_789_94EA;
int word_789_94FE;
int word_789_9500;
int word_789_9506;
long dword_789_ADD6;
long dword_789_ADDA;


  enum {quited,killed,reseted,victorious} gamexit; /*determines what to do after playloop*/

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

//  unsigned EGADATASTART;

//  long savescore;


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


objtype *new;
int lastobj;


#define OBJECT_POOL ((objtype *)0x9508)


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
  if (gamestate == 0 || gamestate == 3)
  {
    RF_Refresh();
    sub_0_290();
    RF_Refresh();
  }

  if (gamestate == 1)
    drawpic(0, 0, 13);

  if (gamestate == 2)
    drawpic(0, 0, 15);

  if (gamestate == 3)
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


void sub_0_31E_TODO(void)
{
  xormask = 0xffff;
  bar(0, 0, 39, 24, 32);
  xormask = 0;
  sx = sy = leftedge = 0;
}


//void sub_0_352(void)
void loadgrfiles(void)
{
  if (grmode == CGAgr)
    installgrfile("CGAPICS.DD2", 0);
  else
  {
    installgrfile("EGAPICS.DD2", 0);
    drawpage ^= 1;
    asm nop; // TODO
  }
}


void sub_0_37E(void)
{
  setscreenmode(grmode);
  sub_0_290();
}


//void sub_0_38E(void)
void repaintscreen(void)
{
  sub_0_37E();
  sub_0_239();
}


//void sub_0_399(void)
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
        gamexit = quited;
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


//void sub_0_48C(void)
void help (void)
{
  expwin (36,21);
  print ("dave2 help screen");
  get ();
}


void sub_0_4A9(void)
{
  int i;
  setscreenmode (grmode);

  if (grmode == EGAgr)
    sub_0_290();

  drawpic (0,0,0x0d);

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
void sub_0_548(void)
{
  int i;
  setscreenmode (grmode);
  gamestate = 2;
  sx = 0;
  sy = 0;
  print ("Dave 2 title screen");

  for (i=0;i<300;i++)
  {
    WaitVBL ();
    indemo = 0;
    ctrl = ControlPlayer(1);
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
  gamestate = 0;
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
//void sub_0_5F9(void)
void dodemo (void)
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
    sub_0_4A9 ();

    if (exitdemo)
      break;

    i=random(NUMDEMOS)+1;
    LoadDemo (i);
    leveldone=true;
    playloop ();
    if (exitdemo)
      break;

    gamestate=3;
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
//void sub_0_71E(void)
void gameover (void)
{
  int i;

  expwin (11,4);
  print ("\n GAME OVER\n     ");
  PlaySound (7);
  WaitEndSound ();
  for (i=0;i<120;i++)
    WaitVBL ();
  gamestate=3;
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


//void sub_0_7B3(void)
void RF_Refresh(void)
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


//void sub_0_AA7(void)
void BadThink_TODO(void)
{
  _quit("badTHINK!");
}


//void sub_0_AB4(void)
void FindFreeObj_TODO (void)
{
  int si = 1;
  new = OBJECT_POOL; // TODO

  while (new->class != nothing && si < lastobj)
  {
    si++;
    new++;
  }

  if (si >= lastobj)
    lastobj++;

  new->x1 = new->y1 = new->y2 = new->x2 = 0;

  new->think = BadThink_TODO;
}


void sub_0_B0D_TODO(void)
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


boolean sub_0_BCF_TODO(void)
{
  if (word_789_7FD0[14] > word_789_1D28)
    return false;

  if (word_789_7FD0[15] > word_789_1D2A)
    return false;

  if (word_789_7FD0[16] < word_789_1D34)
    return false;

  if (word_789_7FD0[17] < word_789_1D36)
    return false;

  return true;
}


int sub_0_C1D_TODO(int arg_0, int arg_2)
{
asm {
db 8Bh, 46h, 06h, 0BBh, 10h, 00h, 99h, 0F7h, 0FBh, 0F7h, 2Eh, 7Eh, 94h, 50h, 8Bh, 46h
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


void sub_0_E52_TODO(void)
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


void sub_0_F19_TODO(void)
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
//void AddScore(int toadd)
//{
//asm {
db 55h, 8Bh, 0ECh

db 8Bh, 46h, 04h, 99h, 01h, 06h, 0E2h, 0ADh, 11h, 16h, 0E4h, 0ADh
db 0A1h, 0E4h, 0ADh, 8Bh, 16h, 0E2h, 0ADh, 81h, 0C2h, 0F0h, 0D8h, 15h, 0FFh, 0FFh, 3Bh, 06h
db 0C0h, 94h, 7Ch, 17h, 75h, 06h, 3Bh, 16h, 0BEh, 94h, 72h, 0Fh, 81h, 06h, 0BEh, 94h
db 10h, 27h, 83h, 16h, 0C0h, 94h, 00h, 0FFh, 06h, 84h, 94h
}
}


// FIXME fsm: what does break do in a switch inside a do-loop?
void sub_0_14CD(void)
{
	IMPLEMENT_ME("sub_0_14CD");
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
db 90h, 90h
}
}
//{
//  RF_ForceRefresh();
//  do
//  {
//    word_789_94BC = word_789_949E = word_789_94D4 = 0;
//	word_789_94C6 = 0x847C;
//    WaitVBL();
//    word_789_8228 = 0x94D6;
//    sub_0_F19();
//    RF_Refresh();
//    dofkeys();
//
//switch (indemo)
//{
//  case 0:
//      if (keydown[0x2e] && keydown[0x14] && keydown[0x39])
//      {
//        clearkeys();
//        centerwindow(15, 1);
//        print(aWarpToLevel); // "Warp to level:"
//        ch = get();
//        if (ch > '0' && ch <= '9')
//        {
//          level = ch - '1';
//          leveldone = true;
//        }
//        RF_ForceRefresh();
//        RF_Refresh();
//        RF_Refresh();
//      }
//    break;
//
//  case 1:
//      indemo = 0;
//      ctrl = ControlPlayer (1);
//      if (ctrl.button1 || ctrl.button2 || keydown[0x39])
//      {
//        indemo = 1;
//        exitdemo = true;
//        break;
//      }
//      indemo = 1;
//    break;
//
//  case 2:
//      if (!gamexit || (keydown[0x42] && keydown[0x20]))
//      {
//        clearkeys();
//        centerwindow(15, 1);
//        print(aSaveAsDemo); // "SAVE AS DEMO#:"
//        do
//        {
//          ch = get ();
//        } while (ch<'0' || ch>'9');
//        SaveDemo(ch-'0');
//        RF_ForceRefresh();
//        RF_Refresh();
//        RF_Refresh();
//        return;
//      }
//    break;
//}
//
//
//  } while (!leveldone && gamexit);
//}


/*======================================*/
/*				      */
/* playloop                             */
/* all the action is directed from here */
/*				      */
/*======================================*/
//void sub_0_162C(void)
void playloop(void)
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

loc_0_1666:
	if (!leveldone)
		goto loc_0_17B1;

loc_0_1670:
	word_789_847C = -1;
	word_789_94C6 = 0x847C;
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
	word_789_94CC = 0;
	word_789_94CE = 0;
	word_789_1D30 = 160;
	word_789_9480 = 80;
	word_789_9482 = 300;
	word_789_1D32 = 750;
	RF_Refresh();
	sub_0_290();
	RF_Refresh();
	leveldone = false;

loc_0_17B1:
	if (indemo != 0)
	{
		initrndt(false);
		goto loc_0_1880;
	}

loc_0_17C2:
	initrndt(true);
	dword_789_94D0 = dword_789_ADDA;
	word_789_94CC = 0;
	word_789_94CE = 0;
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
	PlaySound(2);
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

loc_0_1880:
	dword_789_94DA = dword_789_94D0 + 0x9600;
	word_789_94D6 = 0x9600;
	word_789_94D8 = 0;
	word_789_94EA = 0;
	word_789_94E8 = 0;
	gamexit = 1;
	word_789_94E0 = 2;
	word_789_94E6 = word_789_94E4 = 5;
	word_789_94FE = 0;
	word_789_9500 = 0;
	word_789_9506 = 0xF19;
	word_789_9494 = 0;
	word_789_9494 = 0;
	word_789_8476 = 0;
	word_789_94C4 = 1;
	word_789_9490 = 0;
	word_789_847A = 0;
	dword_789_94D0 = dword_789_ADDA;
	word_789_94CC = 0;
	word_789_94CE = 0;
	sub_0_14CD();
	if (indemo != 0)
		goto loc_0_194E;

loc_0_191E:
	if (!leveldone)
	{
		lives--;
		goto loc_0_1944;
	}

loc_0_192B:
	level++;
	if (level > _numlevels)
	{
		lives = 0;
		gamestate = 2;
	}

loc_0_1944:
	if (lives != 0)
		goto loc_0_1666;

loc_0_194E:
	;
}

/***************************************************************************/
/***************************************************************************/

/*=========================*/
/*			   */
/* m a i n   p r o g r a m */
/*			   */
/*=========================*/
//void sub_0_1953(void)
void main (void)
{
	_numlevels = 1;
	_maxplayers = 1;

	_cgaok = 0;
	_egaok = 1;
	_vgaok = 0;

	_extension = "DD2";

	_setupgame();

	screencenterx = 25;
	word_789_8220 = 0x20;

	sub_0_37E();

	word_789_94C8 = unk_789_1D40;

	while (1)
	{
		dodemo();
		WaitEndSound();
		playloop();
		if (gamestate == 2)
		{
			sub_0_548();
		}
		gameover();
	}
}

