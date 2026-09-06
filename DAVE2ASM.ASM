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
EXTRN originxglobal:DWORD
EXTRN originyglobal:DWORD
EXTRN mapplane:WORD
EXTRN mapbwide:WORD
EXTRN mapbytesextra:WORD
EXTRN drawpage:WORD

EXTRN screenseg:WORD

EXTRN tileptr:DWORD
EXTRN egaspriteptr:DWORD


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


type847C STRUC

word_789_847C	dw	?
px				dw	?
py				dw	?
imagewidth		dw	?
imageheight		dw	?
shapeptr		dd	?
maskptr			dd	?
word_789_848E	dw	?
unk12			db	12 dup (?)

type847C ENDS


obp	dw	?		;temporary for turbo's BP


pel			dw	0
screenstart	dw	0


EGAdraws	dw	0,EGAone,EGAtwo,EGAthree,EGAfour,EGAfive,EGAsix

spriteroutine 	dw 	0

blockheight	dw	0
_three		dw	0	; is written to but never used for anything


EXTRN type847C_789_847C:type847C
EXTRN word_789_94A2:WORD
EXTRN word_789_94CA:WORD
EXTRN word_789_94D4:WORD


word_789_238 dw 0,1,2,3,4,5,6,7,8,9
	dw 0Ah,0Bh,0Ch,0Eh,0Fh
	dw 10h
	dw 0Dh
	dw 11h,12h,13h,14h,15h,16h,17h,18h,19h,1Ah,1Bh,1Ch,1Dh,1Eh,1Fh
	dw 20h,21h,22h,23h,24h,25h,26h,27h,28h,29h,2Ah,2Bh,2Ch,2Dh,2Eh,2Fh
	dw 30h,31h,32h,33h,34h,35h,36h,37h,38h,39h,3Ah,3Bh,3Dh,3Eh,3Fh,3Ch
	dw 41h,42h,43h,40h,44h,45h,46h,47h,48h,49h,4Ah,4Bh,4Ch,4Dh

word_789_4A2 dw 0
word_789_4A4 dw 0
word_789_BFC dw 0
word_789_BFE dw 0
word_789_C00 dw 0
word_789_8478 dw 0


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
byte_0_10AC dw 0
byte_0_10AE dw 0
byte_0_10B0 dw 0
word_0_19B2 dw 0

CGAylookup_0 dw    0,8192,  80,8272, 160,8352, 240,8432, 320,8512, 400,8592, 480,8672
    dw  560,8752, 640,8832, 720,8912, 800,8992, 880,9072, 960,9152,1040,9232
    dw 1120,9312,1200,9392,1280,9472,1360,9552,1440,9632,1520,9712,1600,9792
    dw 1680,9872,1760,9952,1840,10032,1920,10112,2000,10192,2080,10272,2160,10352
    dw 2240,10432,2320,10512,2400,10592,2480,10672,2560,10752,2640,10832,2720,10912
    dw 2800,10992,2880,11072,2960,11152,3040,11232,3120,11312,3200,11392,3280,11472
    dw 3360,11552,3440,11632,3520,11712,3600,11792,3680,11872,3760,11952,3840,12032
    dw 3920,12112,4000,12192,4080,12272,4160,12352,4240,12432,4320,12512,4400,12592
    dw 4480,12672,4560,12752,4640,12832,4720,12912,4800,12992,4880,13072,4960,13152
    dw 5040,13232,5120,13312,5200,13392,5280,13472,5360,13552,5440,13632,5520,13712
    dw 5600,13792,5680,13872,5760,13952,5840,14032,5920,14112,6000,14192,6080,14272
    dw 6160,14352,6240,14432,6320,14512,6400,14592,6480,14672,6560,14752,6640,14832
    dw 6720,14912,6800,14992,6880,15072,6960,15152,7040,15232,7120,15312,7200,15392
    dw 7280,15472,7360,15552,7440,15632,7520,15712,7600,15792,7680,15872,7760,15952
    dw 7840,16032,7920,16112

EGAylookup_0 dw    0,  48,  96, 144, 192, 240, 288, 336, 384, 432, 480, 528, 576, 624
    dw  672, 720, 768, 816, 864, 912, 960,1008,1056,1104,1152,1200,1248,1296
    dw 1344,1392,1440,1488,1536,1584,1632,1680,1728,1776,1824,1872,1920,1968
    dw 2016,2064,2112,2160,2208,2256,2304,2352,2400,2448,2496,2544,2592,2640
    dw 2688,2736,2784,2832,2880,2928,2976,3024,3072,3120,3168,3216,3264,3312
    dw 3360,3408,3456,3504,3552,3600,3648,3696,3744,3792,3840,3888,3936,3984
    dw 4032,4080,4128,4176,4224,4272,4320,4368,4416,4464,4512,4560,4608,4656
    dw 4704,4752,4800,4848,4896,4944,4992,5040,5088,5136,5184,5232,5280,5328
    dw 5376,5424,5472,5520,5568,5616,5664,5712,5760,5808,5856,5904,5952,6000
    dw 6048,6096,6144,6192,6240,6288,6336,6384,6432,6480,6528,6576,6624,6672
    dw 6720,6768,6816,6864,6912,6960,7008,7056,7104,7152,7200,7248,7296,7344
    dw 7392,7440,7488,7536,7584,7632,7680,7728,7776,7824,7872,7920,7968,8016
    dw 8064,8112,8160,8208,8256,8304,8352,8400,8448,8496,8544,8592,8640,8688
    dw 8736,8784,8832,8880,8928,8976,9024,9072,9120,9168,9216,9264,9312,9360
    dw 9408,9456,9504,9552,9600,9648,9696,9744,9792,9840,9888,9936,9984,10032
    dw 10080,10128,10176,10224,10272,10320,10368,10416,10464,10512,10560,10608,10656,10704
    dw 10752,10800,10848,10896,10944,10992,11040,11088,11136,11184,11232,11280,11328,11376
    dw 11424,11472,11520,11568,11616,11664,11712,11760,11808,11856,11904,11952,12000,12048
    dw 12096,12144,12192,12240,12288,12336,12384,12432,12480,12528,12576,12624


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
mov     ax, word ptr originyglobal+1
shr     ax, 1
shr     ax, 1
shr     ax, 1
shr     ax, 1
mov     bx, mapbwide
mul     bx
mov     bx, ax
mov     ax, word ptr originxglobal+1
shr     ax, 1
shr     ax, 1
shr     ax, 1
shr     ax, 1
shl     ax, 1
add     bx, ax
add     bx, mapplane

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
call    DrawTile16

loc_0_1ED1:
cmp     di, type847C_789_847C[bp].word_789_847C
jnz     short loc_0_1EED
call    DrawSprite
mov     dl, 20h
mov     ax, type847C_789_847C[bp].word_789_848E
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
add     bx, mapbytesextra
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
mov     al, byte ptr originxglobal+1
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
mov     al, byte ptr originyglobal+1
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
mov     al, byte ptr originyglobal+1
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


PROC	DrawTile16 ; sub_0_202F
	push    di

	shl     si, 1
	mov     si, word_789_6C8[si]
	mov     di, EGAtileloc[di]
	mov     ax, word ptr tileptr+2
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


PROC	DrawSprite ; sub_0_2383
push bp
push di
push bx
push cx
push dx
mov dx, 3CEh
mov ax, 5
out dx, ax
mov ax, 3
mov _three, ax
mov ax, type847C_789_847C[bp].imageheight
mov blockheight, ax
mov ax, word ptr type847C_789_847C[bp].shapeptr
mov word_789_BFC, ax
mov ax, word ptr type847C_789_847C[bp].maskptr
mov word_789_BFE, ax
mov bx, type847C_789_847C[bp].imagewidth
shl bx, 1
mov ax, EGAdraws[bx]
mov spriteroutine, ax
mov bx, type847C_789_847C[bp].py
shl bx, 1
mov ax, cs:EGAylookup_0[bx]
add ax, type847C_789_847C[bp].px
mov word_789_C00, ax

mov dx, 3CEh
mov ax, 4
out dx, ax
mov dx, 3C4h
mov ax, 102h
out dx, ax
mov di, word_789_C00
mov cx, blockheight
mov si, word_789_BFC
mov bp, word_789_BFE
mov ax, word ptr egaspriteptr[0]+2
mov ds, ax
call ss:spriteroutine

mov dx, 3CEh
mov ax, 104h
out dx, ax
mov dx, 3C4h
mov ax, 202h
out dx, ax
mov di, ds:word_789_C00
mov cx, ds:blockheight
mov si, ds:word_789_BFC
mov bp, ds:word_789_BFE
mov ax, word ptr ds:egaspriteptr[1] + 2
mov ds, ax
call ss:spriteroutine

mov dx, 3CEh
mov ax, 204h
out dx, ax
mov dx, 3C4h
mov ax, 402h
out dx, ax
mov di, ds:word_789_C00
mov cx, ds:blockheight
mov si, ds:word_789_BFC
mov bp, ds:word_789_BFE
mov ax, word ptr ds:egaspriteptr[2] + 2
mov ds, ax
call ss:spriteroutine

mov dx, 3CEh
mov ax, 304h
out dx, ax
mov dx, 3C4h
mov ax, 802h
out dx, ax
mov di, word_789_C00
mov cx, blockheight
mov si, word_789_BFC
mov bp, word_789_BFE
mov ax, word ptr ds:egaspriteptr[3] + 2
mov ds, ax
call ss:spriteroutine

mov ax, 105h
mov dx, 3CEh
out dx, ax
mov dx, 3C4h
mov al, 2
mov ah, 0Fh
out dx, ax
pop dx
pop cx
pop bx
pop di
pop bp
retn
ENDP


;===========================================================================
;
; sprite routines
;
;===========================================================================

PROC EGAone ; sub_0_247E
	mov	ss,ax
@@lineloop:
	mov	al,[es:di]
	and	al,[bp+0]
	or	al,[si]
	stosb
	inc si
	inc bp
	add di, SCREENWIDTH-1
	loop	@@lineloop

	mov	ax,@Data
	mov	ds,ax
	mov	ss,ax
	ret
ENDP

;=================

PROC EGAtwo ; sub_0_2498
	mov	ss,ax
@@lineloop:
	mov	ax,[es:di]
	and	ax,[bp+0]
	or	ax,[si]
	stosw
	add	si,2
	add bp,2
	add	di,SCREENWIDTH-2
	loop	@@lineloop

	mov	ax,@Data
	mov	ds,ax
	mov	ss,ax
	ret
ENDP

;=================

PROC EGAthree ; sub_0_24B6
	mov	ss,ax
@@lineloop:
	mov	ax,[es:di]
	and	ax,[bp+0]
	or	ax,[si]
	stosw
	mov	al,[es:di]
	and	al,[bp+2]
	or	al,[si+2]
	stosb
	add	si,3
	add bp,3
	add	di,SCREENWIDTH-3
	loop	@@lineloop

	mov	ax,@Data
	mov	ds,ax
	mov	ss,ax
	ret
ENDP

;=================

PROC EGAfour ; sub_0_24DE
	mov	ss,ax
@@lineloop:
	mov	ax,[es:di]
	and	ax,[bp+0]
	or	ax,[si]
	stosw
	mov	ax,[es:di]
	and	ax,[bp+2]
	or	ax,[si+2]
	stosw
	add	si,4
	add bp,4
	add	di,SCREENWIDTH-4
	loop	@@lineloop

	mov	ax,@Data
	mov	ds,ax
	mov	ss,ax
	ret
ENDP

;=================

PROC EGAfive ; sub_0_2506
	mov	ss,ax
@@lineloop:
	mov	ax,[es:di]
	and	ax,[bp+0]
	or	ax,[si]
	stosw
	mov	ax,[es:di]
	and	ax,[bp+2]
	or	ax,[si+2]
	stosw
	mov	al,[es:di]
	and	al,[bp+4]
	or	al,[si+4]
	stosb
	add	si,5
	add bp,5
	add	di,SCREENWIDTH-5
	loop	@@lineloop

	mov	ax,@Data
	mov	ds,ax
	mov	ss,ax
	ret
ENDP

;=================

PROC EGAsix ; sub_0_2538
	mov	ss,ax
@@lineloop:
	mov	ax,[es:di]
	and	ax,[bp+0]
	or	ax,[si]
	stosw
	mov	ax,[es:di]
	and	ax,[bp+2]
	or	ax,[si+2]
	stosw
	mov	ax,[es:di]
	and	ax,[bp+4]
	or	ax,[si+4]
	stosw
	add	si,6
	add bp,6
	add	di,SCREENWIDTH-6
	loop	@@lineloop

	mov	ax,@Data
	mov	ds,ax
	mov	ss,ax
	ret
ENDP

	END
