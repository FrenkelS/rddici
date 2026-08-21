; Reconstructed Dangerous Dave in Copyright Infringement Source Code
; Copyright (C) 2026 Frenkel Smeijers
;
; The code in this file is primarily based on:
;  Reconstructed Commander Keen 1-3 Source Code
;   Copyright (C) 2021-2025 K1n9_Duk3
;  Hovertank 3-D Source Code
;   Copyright (C) 1993-2014 Flat Rock Software
;  The Catacomb Source Code
;   Copyright (C) 1993-2014 Flat Rock Software
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along
; with this program; if not, write to the Free Software Foundation, Inc.,
; 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

	MASM

	locals
	.MODEL	SMALL,C

;============================================================================
;
;                 Gamer's Edge Library, ASM section
;
;============================================================================



;============================================================================
;
;                      EGA Graphic routines
;
;============================================================================

SCREENWIDTH	equ	48

PORTTILESWIDE	equ	21
PORTTILESHIGH	equ	14
PORTTILECOUNT	=	(PORTTILESWIDE*PORTTILESHIGH)

	.DATA

	EXTRN	xormask:WORD
	EXTRN	oldtiles, view, background, originx, originy: WORD
	EXTRN	priority: BYTE
	EXTRN	obj, altobj: objtype
	EXTRN	pics:WORD

EXTRN drawoffs1:WORD
EXTRN drawoffs0:WORD
EXTRN drawpage:WORD

EXTRN screenseg:WORD

EXTRN tileptr:WORD

EXTRN word_789_238:WORD
EXTRN word_789_4A2:WORD
EXTRN word_789_4A4:WORD
EXTRN word_789_1556:WORD
EXTRN word_789_8478:WORD
EXTRN word_789_847C:WORD
EXTRN word_789_848E:WORD
EXTRN word_789_947C:WORD
EXTRN word_789_9486:WORD
EXTRN word_789_94A2:WORD
EXTRN word_789_94CA:WORD
EXTRN word_789_94CC:WORD
EXTRN dword_789_94D0:DWORD
EXTRN word_789_94D4:WORD

objtype	STRUC

active	dw	?
class 	dw	?
x	db	?
y	db	?
stage	db	?
delay	db	?
dir	dw	?
hp	db	?
oldx	db	?
oldy	db	?
oldtile dw	?
filler1	db	?

think 	db	?
contact db	?
solid	db	?
firstchar dw	?
psize	db	?
stages	db	?
dirmask db	?
speed 	dw	?
hitpoints db	?
damage 	db	?
points 	dw	?
filler2	db	?,?

objtype	ENDS

obp	dw	?		;temporary for turbo's BP


pel			dw	0
screenstart	dw	0


IMPLEMENT_ME_sub_0_2383 db 'Implement me: sub_0_2383', 0


LABEL word_789_6C8 WORD
	dw    0h,   20h,   40h,   60h,   80h,  0A0h,  0C0h,  0E0h
	dw  100h,  120h,  140h,  160h,  180h,  1A0h,  1C0h,  1E0h
	dw  200h,  220h,  240h,  260h,  280h,  2A0h,  2C0h,  2E0h
	dw  300h,  320h,  340h,  360h,  380h,  3A0h,  3C0h,  3E0h
	dw  400h,  420h,  440h,  460h,  480h,  4A0h,  4C0h,  4E0h
	dw  500h,  520h,  540h,  560h,  580h,  5A0h,  5C0h,  5E0h
	dw  600h,  620h,  640h,  660h,  680h,  6A0h,  6C0h,  6E0h
	dw  700h,  720h,  740h,  760h,  780h,  7A0h,  7C0h,  7E0h
	dw  800h,  820h,  840h,  860h,  880h,  8A0h,  8C0h,  8E0h
	dw  900h,  920h,  940h,  960h,  980h,  9A0h,  9C0h,  9E0h
	dw 0A00h, 0A20h, 0A40h, 0A60h, 0A80h, 0AA0h, 0AC0h, 0AE0h
	dw 0B00h, 0B20h, 0B40h, 0B60h, 0B80h, 0BA0h, 0BC0h, 0BE0h
	dw 0C00h, 0C20h, 0C40h, 0C60h, 0C80h, 0CA0h, 0CC0h, 0CE0h
	dw 0D00h, 0D20h, 0D40h, 0D60h, 0D80h, 0DA0h, 0DC0h, 0DE0h
	dw 0E00h, 0E20h, 0E40h, 0E60h, 0E80h, 0EA0h, 0EC0h, 0EE0h
	dw 0F00h, 0F20h, 0F40h, 0F60h, 0F80h, 0FA0h, 0FC0h, 0FE0h


LABEL EGAtileloc WORD
	dw   604h,  606h,  608h,  60Ah,  60Ch,  60Eh,  610h
	dw   612h,  614h,  616h,  618h,  61Ah,  61Ch,  61Eh
	dw   620h,  622h,  624h,  626h,  628h,  62Ah,  62Ch
	dw   904h,  906h,  908h,  90Ah,  90Ch,  90Eh,  910h
	dw   912h,  914h,  916h,  918h,  91Ah,  91Ch,  91Eh
	dw   920h,  922h,  924h,  926h,  928h,  92Ah,  92Ch
	dw  0C04h, 0C06h, 0C08h, 0C0Ah, 0C0Ch, 0C0Eh, 0C10h
	dw  0C12h, 0C14h, 0C16h, 0C18h, 0C1Ah, 0C1Ch, 0C1Eh
	dw  0C20h, 0C22h, 0C24h, 0C26h, 0C28h, 0C2Ah, 0C2Ch
	dw  0F04h, 0F06h, 0F08h, 0F0Ah, 0F0Ch, 0F0Eh, 0F10h
	dw  0F12h, 0F14h, 0F16h, 0F18h, 0F1Ah, 0F1Ch, 0F1Eh
	dw  0F20h, 0F22h, 0F24h, 0F26h, 0F28h, 0F2Ah, 0F2Ch
	dw  1204h, 1206h, 1208h, 120Ah, 120Ch, 120Eh, 1210h
	dw  1212h, 1214h, 1216h, 1218h, 121Ah, 121Ch, 121Eh
	dw  1220h, 1222h, 1224h, 1226h, 1228h, 122Ah, 122Ch
	dw  1504h, 1506h, 1508h, 150Ah, 150Ch, 150Eh, 1510h
	dw  1512h, 1514h, 1516h, 1518h, 151Ah, 151Ch, 151Eh
	dw  1520h, 1522h, 1524h, 1526h, 1528h, 152Ah, 152Ch
	dw  1804h, 1806h, 1808h, 180Ah, 180Ch, 180Eh, 1810h
	dw  1812h, 1814h, 1816h, 1818h, 181Ah, 181Ch, 181Eh
	dw  1820h, 1822h, 1824h, 1826h, 1828h, 182Ah, 182Ch
	dw  1B04h, 1B06h, 1B08h, 1B0Ah, 1B0Ch, 1B0Eh, 1B10h
	dw  1B12h, 1B14h, 1B16h, 1B18h, 1B1Ah, 1B1Ch, 1B1Eh
	dw  1B20h, 1B22h, 1B24h, 1B26h, 1B28h, 1B2Ah, 1B2Ch
	dw  1E04h, 1E06h, 1E08h, 1E0Ah, 1E0Ch, 1E0Eh, 1E10h
	dw  1E12h, 1E14h, 1E16h, 1E18h, 1E1Ah, 1E1Ch, 1E1Eh
	dw  1E20h, 1E22h, 1E24h, 1E26h, 1E28h, 1E2Ah, 1E2Ch
	dw  2104h, 2106h, 2108h, 210Ah, 210Ch, 210Eh, 2110h
	dw  2112h, 2114h, 2116h, 2118h, 211Ah, 211Ch, 211Eh
	dw  2120h, 2122h, 2124h, 2126h, 2128h, 212Ah, 212Ch
	dw  2404h, 2406h, 2408h, 240Ah, 240Ch, 240Eh, 2410h
	dw  2412h, 2414h, 2416h, 2418h, 241Ah, 241Ch, 241Eh
	dw  2420h, 2422h, 2424h, 2426h, 2428h, 242Ah, 242Ch
	dw  2704h, 2706h, 2708h, 270Ah, 270Ch, 270Eh, 2710h
	dw  2712h, 2714h, 2716h, 2718h, 271Ah, 271Ch, 271Eh
	dw  2720h, 2722h, 2724h, 2726h, 2728h, 272Ah, 272Ch
	dw  2A04h, 2A06h, 2A08h, 2A0Ah, 2A0Ch, 2A0Eh, 2A10h
	dw  2A12h, 2A14h, 2A16h, 2A18h, 2A1Ah, 2A1Ch, 2A1Eh
	dw  2A20h, 2A22h, 2A24h, 2A26h, 2A28h, 2A2Ah, 2A2Ch
	dw  2D04h, 2D06h, 2D08h, 2D0Ah, 2D0Ch, 2D0Eh, 2D10h
	dw  2D12h, 2D14h, 2D16h, 2D18h, 2D1Ah, 2D1Ch, 2D1Eh
	dw  2D20h, 2D22h, 2D24h, 2D26h, 2D28h, 2D2Ah, 2D2Ch


	.CODE


EXTRN	WaitVBL:NEAR
EXTRN	_quit:NEAR


;========================================================================

;{=========================================}
;{                                         }
;{ DRAWOBJ                                 }
;{ Draws the object to TILES in the proper }
;{ direction and state.                    }
;{                                         }
;{=========================================}
;
;Procedure DrawObj;
;var
;  objpri,bsize,tilenum:integer;
;Begin
;  tilenum:=obj.firstchar + obj.size * obj.size
;  *((integer(obj.dir) and obj.dirmask) * obj.stages + obj.stage);
;  obj.oldtile:= tilenum;
;  obj.oldx:=obj.x;
;  obj.oldy:=obj.y;
;  objpri:=priority[tilenum];
;  For y:=obj.y to obj.y+obj.size-1 do
;    for x:=obj.x to obj.x+obj.size-1 do
;      Begin
;	if priority[view[y,x]]<=objpri then
;	  view[y,x]:=tilenum;
;	inc(tilenum);
;      end;
;End;
;
;
byte_0_10AC db 0, 0, 0, 0, 0, 0

word_0_19B2 dw 0

byte_0_19B4 db 0
db    0
db    0
db  20h
db  50h
db    0
db  50h
db  20h
db 0A0h
db    0
db 0A0h
db  20h
db 0F0h
db    0
db 0F0h
db  20h
db  40h
db    1
db  40h
db  21h
db  90h
db    1
db  90h
db  21h
db 0E0h
db    1
db 0E0h
db  21h
db  30h
db    2
db  30h
db  22h
db  80h
db    2
db  80h
db  22h
db 0D0h
db    2
db 0D0h
db  22h
db  20h
db    3
db  20h
db  23h
db  70h
db    3
db  70h
db  23h
db 0C0h
db    3
db 0C0h
db  23h
db  10h
db    4
db  10h
db  24h
db  60h
db    4
db  60h
db  24h
db 0B0h
db    4
db 0B0h
db  24h
db    0
db    5
db    0
db  25h
db  50h
db    5
db  50h
db  25h
db 0A0h
db    5
db 0A0h
db  25h
db 0F0h
db    5
db 0F0h
db  25h
db  40h
db    6
db  40h
db  26h
db  90h
db    6
db  90h
db  26h
db 0E0h
db    6
db 0E0h
db  26h
db  30h
db    7
db  30h
db  27h
db  80h
db    7
db  80h
db  27h
db 0D0h
db    7
db 0D0h
db  27h
db  20h
db    8
db  20h
db  28h
db  70h
db    8
db  70h
db  28h
db 0C0h
db    8
db 0C0h
db  28h
db  10h
db    9
db  10h
db  29h
db  60h
db    9
db  60h
db  29h
db 0B0h
db    9
db 0B0h
db  29h
db    0
db  0Ah
db    0
db  2Ah
db  50h
db  0Ah
db  50h
db  2Ah
db 0A0h
db  0Ah
db 0A0h
db  2Ah
db 0F0h
db  0Ah
db 0F0h
db  2Ah
db  40h
db  0Bh
db  40h
db  2Bh
db  90h
db  0Bh
db  90h
db  2Bh
db 0E0h
db  0Bh
db 0E0h
db  2Bh
db  30h
db  0Ch
db  30h
db  2Ch
db  80h
db  0Ch
db  80h
db  2Ch
db 0D0h
db  0Ch
db 0D0h
db  2Ch
db  20h
db  0Dh
db  20h
db  2Dh
db  70h
db  0Dh
db  70h
db  2Dh
db 0C0h
db  0Dh
db 0C0h
db  2Dh
db  10h
db  0Eh
db  10h
db  2Eh
db  60h
db  0Eh
db  60h
db  2Eh
db 0B0h
db  0Eh
db 0B0h
db  2Eh
db    0
db  0Fh
db    0
db  2Fh
db  50h
db  0Fh
db  50h
db  2Fh
db 0A0h
db  0Fh
db 0A0h
db  2Fh
db 0F0h
db  0Fh
db 0F0h
db  2Fh
db  40h
db  10h
db  40h
db  30h
db  90h
db  10h
db  90h
db  30h
db 0E0h
db  10h
db 0E0h
db  30h
db  30h
db  11h
db  30h
db  31h
db  80h
db  11h
db  80h
db  31h
db 0D0h
db  11h
db 0D0h
db  31h
db  20h
db  12h
db  20h
db  32h
db  70h
db  12h
db  70h
db  32h
db 0C0h
db  12h
db 0C0h
db  32h
db  10h
db  13h
db  10h
db  33h
db  60h
db  13h
db  60h
db  33h
db 0B0h
db  13h
db 0B0h
db  33h
db    0
db  14h
db    0
db  34h
db  50h
db  14h
db  50h
db  34h
db 0A0h
db  14h
db 0A0h
db  34h
db 0F0h
db  14h
db 0F0h
db  34h
db  40h
db  15h
db  40h
db  35h
db  90h
db  15h
db  90h
db  35h
db 0E0h
db  15h
db 0E0h
db  35h
db  30h
db  16h
db  30h
db  36h
db  80h
db  16h
db  80h
db  36h
db 0D0h
db  16h
db 0D0h
db  36h
db  20h
db  17h
db  20h
db  37h
db  70h
db  17h
db  70h
db  37h
db 0C0h
db  17h
db 0C0h
db  37h
db  10h
db  18h
db  10h
db  38h
db  60h
db  18h
db  60h
db  38h
db 0B0h
db  18h
db 0B0h
db  38h
db    0
db  19h
db    0
db  39h
db  50h
db  19h
db  50h
db  39h
db 0A0h
db  19h
db 0A0h
db  39h
db 0F0h
db  19h
db 0F0h
db  39h
db  40h
db  1Ah
db  40h
db  3Ah
db  90h
db  1Ah
db  90h
db  3Ah
db 0E0h
db  1Ah
db 0E0h
db  3Ah
db  30h
db  1Bh
db  30h
db  3Bh
db  80h
db  1Bh
db  80h
db  3Bh
db 0D0h
db  1Bh
db 0D0h
db  3Bh
db  20h
db  1Ch
db  20h
db  3Ch
db  70h
db  1Ch
db  70h
db  3Ch
db 0C0h
db  1Ch
db 0C0h
db  3Ch
db  10h
db  1Dh
db  10h
db  3Dh
db  60h
db  1Dh
db  60h
db  3Dh
db 0B0h
db  1Dh
db 0B0h
db  3Dh
db    0
db  1Eh
db    0
db  3Eh
db  50h
db  1Eh
db  50h
db  3Eh
db 0A0h
db  1Eh
db 0A0h
db  3Eh
db 0F0h
db  1Eh
db 0F0h
db  3Eh
db    0
db    0
db  30h
db    0
db  60h
db    0
db  90h
db    0
db 0C0h
db    0
db 0F0h
db    0
db  20h
db    1
db  50h
db    1
db  80h
db    1
db 0B0h
db    1
db 0E0h
db    1
db  10h
db    2
db  40h
db    2
db  70h
db    2
db 0A0h
db    2
db 0D0h
db    2
db    0
db    3
db  30h
db    3
db  60h
db    3
db  90h
db    3
db 0C0h
db    3
db 0F0h
db    3
db  20h
db    4
db  50h
db    4
db  80h
db    4
db 0B0h
db    4
db 0E0h
db    4
db  10h
db    5
db  40h
db    5
db  70h
db    5
db 0A0h
db    5
db 0D0h
db    5
db    0
db    6
db  30h
db    6
db  60h
db    6
db  90h
db    6
db 0C0h
db    6
db 0F0h
db    6
db  20h
db    7
db  50h
db    7
db  80h
db    7
db 0B0h
db    7
db 0E0h
db    7
db  10h
db    8
db  40h
db    8
db  70h
db    8
db 0A0h
db    8
db 0D0h
db    8
db    0
db    9
db  30h
db    9
db  60h
db    9
db  90h
db    9
db 0C0h
db    9
db 0F0h
db    9
db  20h
db  0Ah
db  50h
db  0Ah
db  80h
db  0Ah
db 0B0h
db  0Ah
db 0E0h
db  0Ah
db  10h
db  0Bh
db  40h
db  0Bh
db  70h
db  0Bh
db 0A0h
db  0Bh
db 0D0h
db  0Bh
db    0
db  0Ch
db  30h
db  0Ch
db  60h
db  0Ch
db  90h
db  0Ch
db 0C0h
db  0Ch
db 0F0h
db  0Ch
db  20h
db  0Dh
db  50h
db  0Dh
db  80h
db  0Dh
db 0B0h
db  0Dh
db 0E0h
db  0Dh
db  10h
db  0Eh
db  40h
db  0Eh
db  70h
db  0Eh
db 0A0h
db  0Eh
db 0D0h
db  0Eh
db    0
db  0Fh
db  30h
db  0Fh
db  60h
db  0Fh
db  90h
db  0Fh
db 0C0h
db  0Fh
db 0F0h
db  0Fh
db  20h
db  10h
db  50h
db  10h
db  80h
db  10h
db 0B0h
db  10h
db 0E0h
db  10h
db  10h
db  11h
db  40h
db  11h
db  70h
db  11h
db 0A0h
db  11h
db 0D0h
db  11h
db    0
db  12h
db  30h
db  12h
db  60h
db  12h
db  90h
db  12h
db 0C0h
db  12h
db 0F0h
db  12h
db  20h
db  13h
db  50h
db  13h
db  80h
db  13h
db 0B0h
db  13h
db 0E0h
db  13h
db  10h
db  14h
db  40h
db  14h
db  70h
db  14h
db 0A0h
db  14h
db 0D0h
db  14h
db    0
db  15h
db  30h
db  15h
db  60h
db  15h
db  90h
db  15h
db 0C0h
db  15h
db 0F0h
db  15h
db  20h
db  16h
db  50h
db  16h
db  80h
db  16h
db 0B0h
db  16h
db 0E0h
db  16h
db  10h
db  17h
db  40h
db  17h
db  70h
db  17h
db 0A0h
db  17h
db 0D0h
db  17h
db    0
db  18h
db  30h
db  18h
db  60h
db  18h
db  90h
db  18h
db 0C0h
db  18h
db 0F0h
db  18h
db  20h
db  19h
db  50h
db  19h
db  80h
db  19h
db 0B0h
db  19h
db 0E0h
db  19h
db  10h
db  1Ah
db  40h
db  1Ah
db  70h
db  1Ah
db 0A0h
db  1Ah
db 0D0h
db  1Ah
db    0
db  1Bh
db  30h
db  1Bh
db  60h
db  1Bh
db  90h
db  1Bh
db 0C0h
db  1Bh
db 0F0h
db  1Bh
db  20h
db  1Ch
db  50h
db  1Ch
db  80h
db  1Ch
db 0B0h
db  1Ch
db 0E0h
db  1Ch
db  10h
db  1Dh
db  40h
db  1Dh
db  70h
db  1Dh
db 0A0h
db  1Dh
db 0D0h
db  1Dh
db    0
db  1Eh
db  30h
db  1Eh
db  60h
db  1Eh
db  90h
db  1Eh
db 0C0h
db  1Eh
db 0F0h
db  1Eh
db  20h
db  1Fh
db  50h
db  1Fh
db  80h
db  1Fh
db 0B0h
db  1Fh
db 0E0h
db  1Fh
db  10h
db  20h
db  40h
db  20h
db  70h
db  20h
db 0A0h
db  20h
db 0D0h
db  20h
db    0
db  21h
db  30h
db  21h
db  60h
db  21h
db  90h
db  21h
db 0C0h
db  21h
db 0F0h
db  21h
db  20h
db  22h
db  50h
db  22h
db  80h
db  22h
db 0B0h
db  22h
db 0E0h
db  22h
db  10h
db  23h
db  40h
db  23h
db  70h
db  23h
db 0A0h
db  23h
db 0D0h
db  23h
db    0
db  24h
db  30h
db  24h
db  60h
db  24h
db  90h
db  24h
db 0C0h
db  24h
db 0F0h
db  24h
db  20h
db  25h
db  50h
db  25h
db  80h
db  25h
db 0B0h
db  25h
db 0E0h
db  25h
db  10h
db  26h
db  40h
db  26h
db  70h
db  26h
db 0A0h
db  26h
db 0D0h
db  26h
db    0
db  27h
db  30h
db  27h
db  60h
db  27h
db  90h
db  27h
db 0C0h
db  27h
db 0F0h
db  27h
db  20h
db  28h
db  50h
db  28h
db  80h
db  28h
db 0B0h
db  28h
db 0E0h
db  28h
db  10h
db  29h
db  40h
db  29h
db  70h
db  29h
db 0A0h
db  29h
db 0D0h
db  29h
db    0
db  2Ah
db  30h
db  2Ah
db  60h
db  2Ah
db  90h
db  2Ah
db 0C0h
db  2Ah
db 0F0h
db  2Ah
db  20h
db  2Bh
db  50h
db  2Bh
db  80h
db  2Bh
db 0B0h
db  2Bh
db 0E0h
db  2Bh
db  10h
db  2Ch
db  40h
db  2Ch
db  70h
db  2Ch
db 0A0h
db  2Ch
db 0D0h
db  2Ch
db    0
db  2Dh
db  30h
db  2Dh
db  60h
db  2Dh
db  90h
db  2Dh
db 0C0h
db  2Dh
db 0F0h
db  2Dh
db  20h
db  2Eh
db  50h
db  2Eh
db  80h
db  2Eh
db 0B0h
db  2Eh
db 0E0h
db  2Eh
db  10h
db  2Fh
db  40h
db  2Fh
db  70h
db  2Fh
db 0A0h
db  2Fh
db 0D0h
db  2Fh
db    0
db  30h
db  30h
db  30h
db  60h
db  30h
db  90h
db  30h
db 0C0h
db  30h
db 0F0h
db  30h
db  20h
db  31h
db  50h
db  31h


;====================
;
; RF_ForceRefresh
; Forces refresh to redraw all tiles (both pages)
;
;====================

PROC	RF_ForceRefresh
	USES	DI
	PUBLIC	RF_ForceRefresh

	mov	cx, PORTTILECOUNT
	mov	ax, ds
	mov	es, ax
	mov	di, offset drawoffs1
	mov	ax, -1
	rep stosw
	mov	cx, PORTTILECOUNT
	mov	di, offset drawoffs0
	rep stosw
	ret
ENDP


sub_0_1D6E:
db 56h, 57h
db 55h, 2Eh, 0FFh, 16h, 0ACh, 19h, 0E8h, 0DDh, 37h, 0BAh, 0D4h, 03h, 0B0h, 0Ch, 0EEh, 42h
db 8Ah, 0C5h, 0EEh, 4Ah, 0B0h, 0Dh, 0EEh, 42h, 8Ah, 0C1h, 0EEh, 8Bh, 0Eh, 20h, 82h, 0BAh
db 0DAh, 03h, 0ECh, 0A8h, 01h, 74h, 0FBh, 0ECh, 0A8h, 01h, 75h, 0FBh, 0E2h, 0F4h, 0FCh, 33h
db 0FFh, 89h, 3Eh, 0A2h, 04h, 89h, 3Eh, 0A4h, 04h, 0B2h, 20h, 0A1h, 0D4h, 94h, 0F6h, 0E2h
db 8Bh, 0E8h, 0A1h, 0D1h, 94h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 0BBh, 28h, 00h
db 0F7h, 0E3h, 8Bh, 0D8h, 03h, 1Eh, 86h, 94h, 0B9h, 14h, 00h, 8Bh, 31h, 0A1h, 54h, 15h
db 0Bh, 0C0h, 75h, 0Dh, 3Bh, 0B5h, 2Ah, 82h, 74h, 1Ah, 89h, 0B5h, 2Ah, 82h, 0EBh, 0Bh
db 90h, 3Bh, 0B5h, 0D2h, 7Fh, 74h, 0Dh, 89h, 0B5h, 0D2h, 7Fh, 0FFh, 06h, 0A2h, 04h, 2Eh
db 0FFh, 16h, 0AEh, 19h, 3Bh, 0BEh, 7Ch, 84h, 75h, 18h, 2Eh, 0FFh, 16h, 0B0h, 19h, 0B2h
db 20h, 8Bh, 86h, 8Eh, 84h, 0F6h, 0E2h, 8Bh, 0E8h, 0A1h, 78h, 84h, 01h, 06h, 0A2h, 04h
db 0EBh, 0E2h, 47h, 47h, 0E2h, 0B5h, 8Bh, 0CBh, 0A1h, 0A2h, 04h, 8Bh, 1Eh, 0A4h, 04h, 3Bh
db 87h, 0A2h, 94h, 8Bh, 0D9h, 73h, 0Bh, 2Eh, 0FFh, 16h, 0B2h, 19h, 0FFh, 06h, 0A2h, 04h
db 0EBh, 0E4h, 0A1h, 0A4h, 04h, 05h, 02h, 00h, 0A3h, 0A4h, 04h, 3Dh, 18h, 00h, 74h, 02h
db 0EBh, 86h, 5Dh, 5Fh, 5Eh, 0C3h


PROC	sub_0_1E46
	PUBLIC	sub_0_1E46

mov     bl, byte ptr pel
push    bx
call    sub_0_1F42
call    WaitVBL
mov     dx, 3DAh
in      al, dx
mov     dx, 3C0h
mov     al, 33h
out     dx, al
pop     ax
out     dx, al
cld
xor     di, di
mov     word_789_4A2, di
mov     word_789_4A4, di
mov     dl, 20h
mov     ax, word_789_94D4
mul     dl
mov     bp, ax
mov     ax, word ptr dword_789_94D0+1
shr     ax, 1
shr     ax, 1
shr     ax, 1
shr     ax, 1
mov     bx, word_789_947C
mul     bx
mov     bx, ax
mov     ax, word_789_94CC+1
shr     ax, 1
shr     ax, 1
shr     ax, 1
shr     ax, 1
shl     ax, 1
add     bx, ax
add     bx, word_789_9486

loc_0_1E97:
mov     cx, 21

loc_0_1E9A:
mov     si, [bx+di]
test    word_789_94CA, 3
jnz     short loc_0_1EAC
shl     si, 1
mov     si, word_789_238[si]
mov     [bx+di], si

loc_0_1EAC:
mov     ax, drawpage
or      ax, ax
jnz     short loc_0_1EC0
cmp     si, drawoffs0[di]
jz      short loc_0_1ED1
mov     drawoffs0[di], si
jmp     short loc_0_1ECA
nop

loc_0_1EC0:
cmp     si, drawoffs1[di]
jz      short loc_0_1ED1
mov     drawoffs1[di], si

loc_0_1ECA:
inc     word_789_4A2
call    sub_0_202F

loc_0_1ED1:
cmp     di, word_789_847C[bp]
jnz     short loc_0_1EED
call    sub_0_2383
mov     dl, 20h
mov     ax, word_789_848E[bp]
mul     dl
mov     bp, ax
mov     ax, word_789_8478
add     word_789_4A2, ax
jmp     short loc_0_1ED1

loc_0_1EED:
inc     di
inc     di
loop    loc_0_1E9A

loc_0_1EF1:
mov     cx, bx
mov     ax, word_789_4A2
mov     bx, word_789_4A4
cmp     ax, word_789_94A2[bx]
mov     bx, cx
jnb     short loc_0_1F0D
call    cs:word_0_19B2
inc     word_789_4A2
jmp     short loc_0_1EF1

loc_0_1F0D:
add     bx, word_789_1556
mov     ax, word_789_4A4
add     ax, 2
mov     word_789_4A4, ax
cmp     ax, 1Ch
jz      short locret_0_1F22
jmp     loc_0_1E97

locret_0_1F22:
retn
ENDP


sub_0_1F23:
db 0B8h, 00h, 0B8h, 8Eh, 0C0h, 0A0h, 0D1h, 94h, 0D0h, 0E8h, 24h, 07h, 0B3h
db 28h, 0F6h, 0E3h, 8Bh, 0C8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 05h, 00h, 0B8h, 0A3h, 0EAh
db 10h, 0C3h


PROC	sub_0_1F42

cli
mov     cx, screenstart
mov     dx, 3D4h
mov     al, 0Ch
out     dx, al
inc     dx
mov     al, ch
out     dx, al
dec     dx
mov     al, 0Dh
out     dx, al
inc     dx
mov     al, cl
out     dx, al
sti
mov     ax, screenstart
shr     ax, 1
shr     ax, 1
shr     ax, 1
shr     ax, 1
add     ax, 0A000h
mov     screenseg, ax
mov     screenstart, 4
mov     al, byte ptr word_789_94CC+1
test    al, 8
jz      short loc_0_1F7C
inc     screenstart

loc_0_1F7C:
and     al, 7
mov     pel, ax
mov     ax, drawpage
or      ax, ax
jnz     short loc_0_1FA0
mov     cx, 0A300h
mov     es, cx
mov     al, byte ptr dword_789_94D0+1
and     al, 0Fh
mov     bl, 48
mul     bl
add     ax, 3600h
add     screenstart, ax
jmp     short loc_0_1FB5
nop

loc_0_1FA0:
mov     cx, 0A000h
mov     es, cx
mov     al, byte ptr dword_789_94D0+1
and     al, 0Fh
mov     bl, 48
mul     bl
add     ax, 600h
add     screenstart, ax

loc_0_1FB5:
mov     ax, 105h
mov     dx, 3CEh
out     dx, ax
retn
ENDP


sub_0_1FBD:
db 57h, 0D1h, 0E6h
db 8Bh, 0B4h, 0C8h, 04h, 8Bh, 0BDh, 0C8h, 07h, 0A1h, 98h, 0AEh, 8Eh, 0D8h, 0A5h, 0A5h, 81h
db 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h
db 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h
db 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h
db 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h
db 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h
db 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 8Ch, 0D0h, 8Eh, 0D8h, 5Fh, 0C3h


PROC	sub_0_202F
	push    di

	shl     si, 1
	mov     si, word_789_6C8[si]
	mov     di, EGAtileloc[di]
	mov     ax, tileptr+2
	mov     ds, ax
	movsb
	movsb
REPT 15
	add	di, SCREENWIDTH-2
	movsb
	movsb
ENDM
	mov     ax, ss
	mov     ds, ax
	pop     di
	retn
ENDP


sub_0_2092:
db 57h, 0D1h, 0E6h, 8Bh, 0B4h, 0C8h, 04h, 0D1h, 0E6h, 0BEh, 00h, 00h, 0BFh, 00h
db 1Eh, 0A1h, 98h, 0AEh, 8Eh, 0D8h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h
db 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h
db 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h
db 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h
db 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 0A5h, 0A5h, 83h, 0C7h
db 00h, 0A5h, 0A5h, 83h, 0C7h, 00h, 5Fh, 8Ch, 0D0h, 8Eh, 0D8h, 0C3h

sub_0_20FC:
db 57h, 0D1h, 0E6h, 0BEh
db 00h, 00h, 0BFh, 80h, 25h, 0A1h, 98h, 0AEh, 8Eh, 0D8h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h
db 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h
db 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h
db 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h
db 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 83h, 0C7h, 00h
db 0A4h, 0A4h, 83h, 0C7h, 00h, 0A4h, 0A4h, 8Ch, 0D0h, 8Eh, 0D8h, 5Fh, 0C3h

sub_0_215D:
db 0C3h

sub_0_215E:
db 55h, 57h
db 53h, 51h, 52h, 8Bh, 96h, 7Eh, 84h, 8Bh, 8Eh, 84h, 84h, 8Bh, 0BEh, 82h, 84h, 0D1h
db 0E7h, 8Bh, 0BDh, 0A6h, 04h, 8Bh, 9Eh, 80h, 84h, 83h, 0E3h, 0FEh, 0D1h, 0E3h, 81h, 0C3h
db 0B4h, 19h, 8Bh, 0B6h, 86h, 84h, 8Bh, 0AEh, 8Ah, 84h, 0A1h, 82h, 0AEh, 8Eh, 0D8h, 8Eh
db 0D0h, 0FFh, 0E7h

loc_0_2193:
db 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h, 0FAh, 26h, 8Ah, 05h, 22h, 46h
db 00h, 0Ah, 04h, 0AAh, 46h, 45h, 0E2h, 0EBh
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 5Ah
db 59h, 5Bh, 5Fh, 5Dh, 0C3h, 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h, 0FAh, 26h, 8Bh, 05h
db 23h, 46h, 00h, 0Bh, 04h, 0ABh, 46h, 46h, 45h, 45h, 0E2h, 0E9h
mov ax, @data
db 8Eh
db 0D8h, 8Eh, 0D0h, 5Ah, 59h, 5Bh, 5Fh, 5Dh, 0C3h, 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h
db 0FAh, 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh, 04h, 0ABh, 26h, 8Ah, 05h, 22h, 46h, 02h
db 0Ah, 44h, 02h, 0AAh, 83h, 0C6h, 03h, 83h, 0C5h, 03h, 0E2h, 0DDh
mov ax, @data
db 8Eh
db 0D8h, 8Eh, 0D0h, 5Ah, 59h, 5Bh, 5Fh, 5Dh, 0C3h, 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h
db 0FAh, 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h
db 0Bh, 44h, 02h, 0ABh, 83h, 0C6h, 04h, 83h, 0C5h, 04h, 0E2h, 0DDh
mov ax, @data
db 8Eh
db 0D8h, 8Eh, 0D0h, 5Ah, 59h, 5Bh, 5Fh, 5Dh, 0C3h, 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h
db 0FAh, 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h
db 0Bh, 44h, 02h, 0ABh, 26h, 8Ah, 05h, 22h, 46h, 04h, 0Ah, 44h, 04h, 0AAh, 83h, 0C6h
db 05h, 83h, 0C5h, 05h, 0E2h, 0D3h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 5Ah, 59h, 5Bh
db 5Fh, 5Dh, 0C3h, 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h, 0FAh, 26h, 8Bh, 05h, 23h, 46h
db 00h, 0Bh, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h, 0Bh, 44h, 02h, 0ABh, 26h, 8Bh
db 05h, 23h, 46h, 04h, 0Bh, 44h, 04h, 0ABh, 83h, 0C6h, 06h, 83h, 0C5h, 06h, 0E2h, 0D3h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 5Ah, 59h, 5Bh, 5Fh, 5Dh, 0C3h, 2Eh, 8Bh, 3Fh
db 83h, 0C3h, 02h, 03h, 0FAh, 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh, 04h, 0ABh, 26h, 8Bh
db 05h, 23h, 46h, 02h, 0Bh, 44h, 02h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 04h, 0Bh, 44h
db 04h, 0ABh, 26h, 8Ah, 05h, 22h, 46h, 06h, 0Ah, 44h, 06h, 0AAh, 83h, 0C6h, 07h, 83h
db 0C5h, 07h, 0E2h, 0C9h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 5Ah, 59h, 5Bh, 5Fh, 5Dh
db 0C3h, 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h, 0FAh, 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh
db 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h, 0Bh, 44h, 02h, 0ABh, 26h, 8Bh, 05h, 23h
db 46h, 04h, 0Bh, 44h, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 06h, 0Bh, 44h, 06h, 0ABh
db 83h, 0C6h, 08h, 83h, 0C5h, 08h, 0E2h, 0C9h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 5Ah
db 59h, 5Bh, 5Fh, 5Dh, 0C3h, 2Eh, 8Bh, 3Fh, 83h, 0C3h, 02h, 03h, 0FAh, 26h, 8Bh, 05h
db 23h, 46h, 00h, 0Bh, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h, 0Bh, 44h, 02h, 0ABh
db 26h, 8Bh, 05h, 23h, 46h, 04h, 0Bh, 44h, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 06h
db 0Bh, 44h, 06h, 0ABh, 26h, 8Ah, 05h, 22h, 46h, 08h, 0Ah, 44h, 08h, 0AAh, 83h, 0C6h
db 09h, 83h, 0C5h, 09h, 0E2h, 0BFh
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 5Ah, 59h, 5Bh
db 5Fh, 5Dh, 0C3h


PROC	sub_0_2383

	push offset IMPLEMENT_ME_sub_0_2383
	call _quit
;db 55h, 57h, 53h, 51h, 52h, 0BAh, 0CEh, 03h, 0B8h, 05h, 00h, 0EFh, 0B8h
db 03h, 00h, 0A3h, 02h, 0Ch, 8Bh, 86h, 84h, 84h, 0A3h, 0FAh, 0Bh, 8Bh, 86h, 86h, 84h
db 0A3h, 0FCh, 0Bh, 8Bh, 86h, 8Ah, 84h, 0A3h, 0FEh, 0Bh, 8Bh, 9Eh, 82h, 84h, 0D1h, 0E3h
db 8Bh, 87h, 0BAh, 04h, 0A3h, 0F8h, 0Bh, 8Bh, 9Eh, 80h, 84h, 0D1h, 0E3h, 2Eh, 8Bh, 87h
db 44h, 1Bh, 03h, 86h, 7Eh, 84h, 0A3h, 00h, 0Ch, 0BAh, 0CEh, 03h, 0B8h, 04h, 00h, 0EFh
db 0BAh, 0C4h, 03h, 0B8h, 02h, 01h, 0EFh, 8Bh, 3Eh, 00h, 0Ch, 8Bh, 0Eh, 0FAh, 0Bh, 8Bh
db 36h, 0FCh, 0Bh, 8Bh, 2Eh, 0FEh, 0Bh, 0A1h, 72h, 0AEh, 8Eh, 0D8h, 36h, 0FFh, 16h, 0F8h
db 0Bh, 0BAh, 0CEh, 03h, 0B8h, 04h, 01h, 0EFh, 0BAh, 0C4h, 03h, 0B8h, 02h, 02h, 0EFh, 8Bh
db 3Eh, 00h, 0Ch, 8Bh, 0Eh, 0FAh, 0Bh, 8Bh, 36h, 0FCh, 0Bh, 8Bh, 2Eh, 0FEh, 0Bh, 0A1h
db 76h, 0AEh, 8Eh, 0D8h, 36h, 0FFh, 16h, 0F8h, 0Bh, 0BAh, 0CEh, 03h, 0B8h, 04h, 02h, 0EFh
db 0BAh, 0C4h, 03h, 0B8h, 02h, 04h, 0EFh, 8Bh, 3Eh, 00h, 0Ch, 8Bh, 0Eh, 0FAh, 0Bh, 8Bh
db 36h, 0FCh, 0Bh, 8Bh, 2Eh, 0FEh, 0Bh, 0A1h, 7Ah, 0AEh, 8Eh, 0D8h, 36h, 0FFh, 16h, 0F8h
db 0Bh, 0BAh, 0CEh, 03h, 0B8h, 04h, 03h, 0EFh, 0BAh, 0C4h, 03h, 0B8h, 02h, 08h, 0EFh, 8Bh
db 3Eh, 00h, 0Ch, 8Bh, 0Eh, 0FAh, 0Bh, 8Bh, 36h, 0FCh, 0Bh, 8Bh, 2Eh, 0FEh, 0Bh, 0A1h
db 7Eh, 0AEh, 8Eh, 0D8h, 36h, 0FFh, 16h, 0F8h, 0Bh, 0B8h, 05h, 01h, 0BAh, 0CEh, 03h, 0EFh
db 0BAh, 0C4h, 03h, 0B0h, 02h, 0B4h, 0Fh, 0EFh, 5Ah, 59h, 5Bh, 5Fh, 5Dh, 0C3h
ENDP


;===========================================================================
;
; sprite routines
;
;===========================================================================

;EGAone
sub_0_247E:
db 8Eh, 0D0h
db 26h, 8Ah, 05h, 22h, 46h, 00h, 0Ah, 04h, 0AAh, 46h, 45h, 83h, 0C7h, 2Fh, 0E2h, 0F0h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 0C3h

;=================

;EGAtwo
sub_0_2498:
db 8Eh, 0D0h, 26h, 8Bh, 05h, 23h, 46h, 00h
db 0Bh, 04h, 0ABh, 83h, 0C6h, 02h, 83h, 0C5h, 02h, 83h, 0C7h, 2Eh, 0E2h, 0ECh
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 0C3h

;=================

;EGAthree
sub_0_24B6:
db 8Eh, 0D0h, 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh, 04h
db 0ABh, 26h, 8Ah, 05h, 22h, 46h, 02h, 0Ah, 44h, 02h, 0AAh, 83h, 0C6h, 03h, 83h, 0C5h
db 03h, 83h, 0C7h, 2Dh, 0E2h, 0E2h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 0C3h

;=================

;EGAfour
sub_0_24DE:
db 8Eh, 0D0h
db 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h, 0Bh
db 44h, 02h, 0ABh, 83h, 0C6h, 04h, 83h, 0C5h, 04h, 83h, 0C7h, 2Ch, 0E2h, 0E2h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 0C3h

;=================

;EGAfive
sub_0_2506:
db 8Eh, 0D0h, 26h, 8Bh, 05h, 23h, 46h, 00h, 0Bh, 04h
db 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h, 0Bh, 44h, 02h, 0ABh, 26h, 8Ah, 05h, 22h, 46h
db 04h, 0Ah, 44h, 04h, 0AAh, 83h, 0C6h, 05h, 83h, 0C5h, 05h, 83h, 0C7h, 2Bh, 0E2h, 0D8h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 0C3h

;=================

;EGAsix
sub_0_2538:
db 8Eh, 0D0h, 26h, 8Bh, 05h, 23h, 46h, 00h
db 0Bh, 04h, 0ABh, 26h, 8Bh, 05h, 23h, 46h, 02h, 0Bh, 44h, 02h, 0ABh, 26h, 8Bh, 05h
db 23h, 46h, 04h, 0Bh, 44h, 04h, 0ABh, 83h, 0C6h, 06h, 83h, 0C5h, 06h, 83h, 0C7h, 2Ah
db 0E2h, 0D8h
mov ax, @data
db 8Eh, 0D8h, 8Eh, 0D0h, 0C3h

	END
