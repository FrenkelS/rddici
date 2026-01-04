; The Catacomb Source Code
; Copyright (C) 1993-2014 Flat Rock Software
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

	%NOLIST
	%TITLE	"RLE compression v0.00"
;==========================================================
; RLE compression in assemlby.
; Written by Lane Roath, Copyright (c) 1990 IFD & Softdisk
;
;----------------------------------------------------------
;
;09-Aug-90 0.00	Start coding, converting source C in Dr. Dobb's
;
;==========================================================

	IDEAL

	MODEL	small,C
	STACK	0200


;==========================================================

MinCnt	=	3-1	;min count for repeat
MaxCnt	=	127+MinCnt	;max count (128+ = non repeat string)

	DATASEG

handle		dw	?	;file i/o requirements are here!
flength1	dw	?
flength2	dw	?
buf1		dw	?
buf2		dw	?
foff1		dw	?
foff2		dw	?

; We store our memory allocation results here

RLESeg	dw	?	;RLE packed buf (src/dest)
SrcSeg	dw	?	;original source data

RLEIdx	dw	?	;indexes for read/write bufs
SRCIdx	dw	?

SrcLen	dw	2 dup (?)	;length of source & dest bufs
RLELen	dw	2 dup (?)

ByteCnt	dw	?	;# of sequential bytes
LastByte	db	?	;last byte found
DifCnt	dw	?	;# of different bytes

EndByte	dw	?

;==========================================================
;
	CODESEG

sub_0_5D34:
db 36h, 0C7h, 06h, 48h, 12h, 00h, 00h, 36h, 0C7h, 06h, 4Bh, 12h
db 01h, 00h, 36h, 0C7h, 06h, 44h, 12h, 00h, 00h, 36h, 0C7h, 06h, 46h, 12h, 00h, 00h
db 36h, 8Bh, 36h, 3Eh, 12h, 36h, 8Bh, 3Eh, 3Ch, 12h, 0C3h

sub_0_5D5B:
db 56h, 06h, 50h, 36h, 8Eh
db 06h, 38h, 12h, 36h, 0A1h, 48h, 12h, 2Ch, 02h, 0AAh, 36h, 0A0h, 4Ah, 12h, 0AAh, 36h
db 8Bh, 36h, 3Eh, 12h, 46h, 36h, 03h, 36h, 48h, 12h, 36h, 89h, 36h, 3Eh, 12h, 36h
db 83h, 06h, 44h, 12h, 02h, 36h, 83h, 16h, 46h, 12h, 00h, 36h, 0C7h, 06h, 48h, 12h
db 00h, 00h, 58h, 07h, 5Eh, 0C3h

sub_0_5D96:
db 06h, 50h, 56h, 36h, 8Eh, 06h, 38h, 12h, 36h, 0A1h
db 4Bh, 12h, 04h, 7Fh, 0AAh, 36h, 8Bh, 0Eh, 4Bh, 12h, 8Bh, 0C1h, 40h, 36h, 01h, 06h
db 44h, 12h, 36h, 83h, 16h, 46h, 12h, 00h, 36h, 8Bh, 36h, 3Eh, 12h, 1Eh, 36h, 8Eh
db 1Eh, 3Ah, 12h, 0F3h, 0A4h, 1Fh, 36h, 0C7h, 06h, 4Bh, 12h, 00h, 00h, 36h, 89h, 36h
db 3Eh, 12h, 5Eh, 58h, 07h, 0C3h

sub_0_5DD6:
db 0E8h, 5Bh, 0FFh, 87h, 0F7h, 0BAh, 01h, 00h, 36h, 8Eh
db 06h, 3Ah, 12h, 32h, 0E4h, 81h, 0FFh, 00h, 0FFh, 72h, 26h, 8Bh, 0C7h, 0D1h, 0E8h, 0D1h
db 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 36h, 01h, 06h, 3Ah, 12h, 0BFh, 00h, 00h, 8Bh, 0C6h, 0D1h
db 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 36h, 01h, 06h, 38h, 12h, 83h, 0E6h, 0Fh, 0EBh
db 0CDh, 36h, 29h, 16h, 40h, 12h, 36h, 83h, 1Eh, 42h, 12h, 00h, 72h, 24h, 33h, 0D2h
db 1Eh, 36h, 8Eh, 1Eh, 38h, 12h, 0ACh, 3Ch, 80h, 72h, 0Bh, 2Ch, 7Fh, 8Bh, 0C8h, 8Bh
db 0D0h, 0F3h, 0A4h, 1Fh, 0EBh, 0AFh, 04h, 03h, 8Bh, 0C8h, 8Bh, 0D0h, 0ACh, 0F3h, 0AAh, 1Fh
db 0EBh, 0A3h, 0C3h

sub_0_5E43:
db 0E8h, 0EEh, 0FEh, 36h, 8Eh, 06h, 3Ah, 12h, 26h, 8Ah, 04h, 36h, 0A2h
db 4Ah, 12h, 81h, 0FEh, 00h, 0FFh, 72h, 35h, 0E8h, 0D4h, 00h, 8Bh, 0C6h, 0D1h, 0E8h, 0D1h
db 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 36h, 01h, 06h, 3Ah, 12h, 83h, 0E6h, 0Fh, 36h, 0C7h, 06h
db 3Eh, 12h, 00h, 00h, 8Bh, 0C7h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 0D1h, 0E8h, 36h, 01h
db 06h, 38h, 12h, 83h, 0E7h, 0Fh, 36h, 0FFh, 06h, 4Bh, 12h, 0EBh, 0B9h, 36h, 83h, 2Eh
db 40h, 12h, 01h, 36h, 83h, 1Eh, 42h, 12h, 00h, 73h, 03h, 0E9h, 91h, 00h, 46h, 26h
db 8Ah, 04h, 36h, 3Ah, 06h, 4Ah, 12h, 75h, 40h, 36h, 0FFh, 06h, 48h, 12h, 36h, 83h
db 3Eh, 48h, 12h, 02h, 72h, 9Ch, 36h, 83h, 3Eh, 4Bh, 12h, 01h, 76h, 08h, 36h, 0FFh
db 0Eh, 4Bh, 12h, 0E8h, 0D0h, 0FEh, 36h, 0C7h, 06h, 4Bh, 12h, 00h, 00h, 36h, 81h, 3Eh
db 48h, 12h, 81h, 00h, 72h, 10h, 0E8h, 82h, 0FEh, 26h, 3Ah, 44h, 01h, 75h, 07h, 26h
db 3Ah, 44h, 02h, 75h, 01h, 46h, 0E9h, 69h, 0FFh, 36h, 8Bh, 0Eh, 48h, 12h, 0E3h, 16h
db 83h, 0F9h, 02h, 72h, 05h, 0E8h, 63h, 0FEh, 0EBh, 0Ch, 36h, 01h, 0Eh, 4Bh, 12h, 36h
db 0C7h, 06h, 48h, 12h, 00h, 00h, 36h, 0FFh, 06h, 4Bh, 12h, 36h, 0A2h, 4Ah, 12h, 36h
db 83h, 3Eh, 4Bh, 12h, 7Eh, 72h, 15h, 0E8h, 7Ch, 0FEh, 26h, 3Ah, 44h, 01h, 75h, 0Ch
db 26h, 3Ah, 44h, 02h, 75h, 06h, 46h, 36h, 0FFh, 06h, 4Bh, 12h, 0E9h, 23h, 0FFh

sub_0_5F2F:
db 36h
db 8Bh, 0Eh, 48h, 12h, 0E3h, 1Ah, 83h, 0F9h, 02h, 72h, 10h, 36h, 83h, 3Eh, 4Bh, 12h
db 01h, 72h, 03h, 0E8h, 50h, 0FEh, 0E8h, 12h, 0FEh, 0EBh, 0Fh, 36h, 01h, 0Eh, 4Bh, 12h
db 36h, 8Bh, 0Eh, 4Bh, 12h, 0E3h, 03h, 0E8h, 3Ch, 0FEh, 0C3h

sub_0_5F5B:
db 55h, 8Bh, 0ECh, 56h, 57h
db 55h, 8Bh, 46h, 04h, 36h, 0A3h, 4Fh, 12h, 0B8h, 1Ch, 35h, 0CDh, 21h, 2Eh, 89h, 1Eh
db 08h, 60h, 8Ch, 0C0h, 2Eh, 0A3h, 0Ah, 60h, 1Eh, 0Eh, 1Fh, 0BAh, 0FEh, 5Fh, 0B8h, 1Ch
db 25h, 0CDh, 21h, 1Fh, 0E8h, 0E6h, 0F3h, 33h, 0C0h, 2Eh, 0A3h, 0Ch, 60h, 2Eh, 0A3h, 0Eh
db 60h, 2Eh, 0A1h, 0Eh, 60h, 3Dh, 00h, 00h, 74h, 0F7h, 33h, 0C0h, 2Eh, 0A3h, 0Eh, 60h
db 55h, 36h, 0A1h, 61h, 12h, 8Eh, 0C0h, 36h, 0A1h, 53h, 12h, 36h, 8Bh, 1Eh, 55h, 12h
db 36h, 8Bh, 0Eh, 57h, 12h, 36h, 8Bh, 16h, 59h, 12h, 36h, 8Bh, 36h, 5Bh, 12h, 36h
db 8Bh, 3Eh, 5Dh, 12h, 36h, 8Bh, 2Eh, 5Fh, 12h, 36h, 0FFh, 16h, 51h, 12h, 5Dh, 2Eh
db 0FFh, 06h, 0Ch, 60h, 2Eh, 0A1h, 0Eh, 60h, 0Bh, 0C0h, 74h, 0C4h, 36h, 0FFh, 0Eh, 4Fh
db 12h, 75h, 0B7h, 1Eh, 2Eh, 0A1h, 0Ah, 60h, 8Eh, 0D8h, 2Eh, 8Bh, 16h, 08h, 60h, 0B8h
db 1Ch, 25h, 0CDh, 21h, 1Fh, 2Eh, 0A1h, 0Ch, 60h, 5Dh, 5Fh, 5Eh, 5Dh, 0C3h, 2Eh, 0FFh
db 06h, 0Eh, 60h, 2Eh, 0FFh, 2Eh, 08h, 60h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h

	END