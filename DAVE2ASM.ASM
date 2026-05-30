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


	.CODE

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


sub_0_1E46:
db 8Ah, 1Eh, 0F4h, 0Bh, 53h, 0E8h, 0F4h, 00h, 0E8h, 05h
db 37h, 0BAh, 0DAh, 03h, 0ECh, 0BAh, 0C0h, 03h, 0B0h, 33h, 0EEh, 58h, 0EEh, 0FCh, 33h, 0FFh
db 89h, 3Eh, 0A2h, 04h, 89h, 3Eh, 0A4h, 04h, 0B2h, 20h, 0A1h, 0D4h, 94h, 0F6h, 0E2h, 8Bh
db 0E8h, 0A1h, 0D1h, 94h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 8Bh, 1Eh, 7Ch, 94h
db 0F7h, 0E3h, 8Bh, 0D8h, 0A1h, 0CDh, 94h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h
db 0E0h, 03h, 0D8h, 03h, 1Eh, 86h, 94h, 0B9h, 15h, 00h, 8Bh, 31h, 0F7h, 06h, 0CAh, 94h
db 03h, 00h, 75h, 08h, 0D1h, 0E6h, 8Bh, 0B4h, 38h, 02h, 89h, 31h, 0A1h, 54h, 15h, 0Bh
db 0C0h, 75h, 0Dh, 3Bh, 0B5h, 2Ah, 82h, 74h, 18h, 89h, 0B5h, 2Ah, 82h, 0EBh, 0Bh, 90h
db 3Bh, 0B5h, 0D2h, 7Fh, 74h, 0Bh, 89h, 0B5h, 0D2h, 7Fh, 0FFh, 06h, 0A2h, 04h, 0E8h, 5Eh
db 01h, 3Bh, 0BEh, 7Ch, 84h, 75h, 16h, 0E8h, 0A9h, 04h, 0B2h, 20h, 8Bh, 86h, 8Eh, 84h
db 0F6h, 0E2h, 8Bh, 0E8h, 0A1h, 78h, 84h, 01h, 06h, 0A2h, 04h, 0EBh, 0E4h, 47h, 47h, 0E2h
db 0A9h, 8Bh, 0CBh, 0A1h, 0A2h, 04h, 8Bh, 1Eh, 0A4h, 04h, 3Bh, 87h, 0A2h, 94h, 8Bh, 0D9h
db 73h, 0Bh, 2Eh, 0FFh, 16h, 0B2h, 19h, 0FFh, 06h, 0A2h, 04h, 0EBh, 0E4h, 03h, 1Eh, 56h
db 15h, 0A1h, 0A4h, 04h, 05h, 02h, 00h, 0A3h, 0A4h, 04h, 3Dh, 1Ch, 00h, 74h, 03h, 0E9h
db 75h, 0FFh, 0C3h


sub_0_1F23:
db 0B8h, 00h, 0B8h, 8Eh, 0C0h, 0A0h, 0D1h, 94h, 0D0h, 0E8h, 24h, 07h, 0B3h
db 28h, 0F6h, 0E3h, 8Bh, 0C8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 05h, 00h, 0B8h, 0A3h, 0EAh
db 10h, 0C3h

sub_0_1F42:
db 0FAh, 8Bh, 0Eh, 0F6h, 0Bh, 0BAh, 0D4h, 03h, 0B0h, 0Ch, 0EEh, 42h, 8Ah, 0C5h
db 0EEh, 4Ah, 0B0h, 0Dh, 0EEh, 42h, 8Ah, 0C1h, 0EEh, 0FBh, 0A1h, 0F6h, 0Bh, 0D1h, 0E8h, 0D1h
db 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 05h, 00h, 0A0h, 0A3h, 0EAh, 10h, 0C7h, 06h, 0F6h, 0Bh, 04h
db 00h, 0A0h, 0CDh, 94h, 0A8h, 08h, 74h, 04h, 0FFh, 06h, 0F6h, 0Bh, 24h, 07h, 0A3h, 0F4h
db 0Bh, 0A1h, 54h, 15h, 0Bh, 0C0h, 75h, 18h, 0B9h, 00h, 0A3h, 8Eh, 0C1h, 0A0h, 0D1h, 94h
db 24h, 0Fh, 0B3h, 30h, 0F6h, 0E3h, 05h, 00h, 36h, 01h, 06h, 0F6h, 0Bh, 0EBh, 16h, 90h
db 0B9h, 00h, 0A0h, 8Eh, 0C1h, 0A0h, 0D1h, 94h, 24h, 0Fh, 0B3h, 30h, 0F6h, 0E3h, 05h, 00h
db 06h, 01h, 06h, 0F6h, 0Bh, 0B8h, 05h, 01h, 0BAh, 0CEh, 03h, 0EFh, 0C3h

sub_0_1FBD:
db 57h, 0D1h, 0E6h
db 8Bh, 0B4h, 0C8h, 04h, 8Bh, 0BDh, 0C8h, 07h, 0A1h, 98h, 0AEh, 8Eh, 0D8h, 0A5h, 0A5h, 81h
db 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h
db 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h
db 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h
db 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h
db 0A5h, 81h, 0EFh, 0B4h, 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 81h, 0EFh, 0B4h
db 1Fh, 0A5h, 0A5h, 81h, 0C7h, 0FCh, 1Fh, 0A5h, 0A5h, 8Ch, 0D0h, 8Eh, 0D8h, 5Fh, 0C3h

sub_0_202F:
db 57h
db 0D1h, 0E6h, 8Bh, 0B4h, 0C8h, 06h, 8Bh, 0BDh, 0A8h, 09h, 0A1h, 98h, 0AEh, 8Eh, 0D8h, 0A4h
db 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h
db 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h
db 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h
db 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh
db 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 83h, 0C7h, 2Eh, 0A4h, 0A4h, 8Ch, 0D0h, 8Eh, 0D8h
db 5Fh, 0C3h

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

sub_0_2383:
db 55h, 57h, 53h, 51h, 52h, 0BAh, 0CEh, 03h, 0B8h, 05h, 00h, 0EFh, 0B8h
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
