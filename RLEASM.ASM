; Reconstructed Dangerous Dave in Copyright Infringement Source Code
; Copyright (C) 2026 Frenkel Smeijers
;
; The code in this file is primarily based on:
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
;
;==========================================================
;
;	Initialize our variables & buffers
;
;==========================================================

PROC	InitRLE ; sub_0_5D34		;internal

	mov	[ss:ByteCnt],0
	mov	[ss:DifCnt],1
	mov	[ss:RLELen],0
	mov	[ss:RLELen+2],0
	mov	SI,[ss:SrcIdx]	;init Source Index (SrcIdx)
	mov	DI,[ss:RLEIdx]	; & dest index (RLEIdx)

	ret
ENDP


;==========================================================
;
;	Output a compression string (repeated bytes)
;
;==========================================================


PROC	OutputRep ; sub_0_5D5B

	push	SI
	push	ES	;save needed registers
	push	AX

	mov	ES,[ss:RLESeg]	;get dest buffer seg
	mov	AX,[ss:ByteCnt]
	sub	AL,2	;get byte count - 3 (already -1)
	stosb

	mov	AL,[ss:LastByte]	; & output byte to repeat
	stosb

	mov	SI,[ss:SrcIdx]
	inc	SI	;update source index (length + 1)
	add	SI,[ss:ByteCnt]
	mov	[ss:SrcIdx],SI	; & save for next output

	add	[ss:RLELen],2	;update RLE length
	adc	[ss:RLELen+2],0

	mov	[ss:ByteCnt],0	;we output everything!

	pop	AX
	pop	ES	;restore registers

;	mov	BL,[ES:SI]
;	mov	[LastByte],BL	;this is 'last' byte!

	pop	SI

	ret

	ENDP

;==========================================================
;
;	Output a unique string with count byte preceeding
;
;==========================================================


PROC	OutputDif ; sub_0_5D96

	push	ES	;save important registers
	push	AX
	push	SI

	mov	ES,[ss:RLESeg]	;get dest buffer seg
	mov	AX,[ss:DifCnt]
	add	AL,7Fh	; & save byte count - 1 with hi bit set
	stosb

	mov	CX,[ss:DifCnt]	;get # of bytes to copy

	mov	AX,CX	;# of bytes read (count is -1)
	inc	AX
	add	[ss:RLELen],AX	;update RLE length
	adc	[ss:RLELen+2],0

	mov	SI,[ss:SrcIdx]	; & starting position in source
	push	DS
	mov	DS,[ss:SrcSeg]	;set source seg
	REP	movsb
	pop	DS	; & copy the string

	mov	[ss:DifCnt],0	;we output everything!

	mov	[ss:SrcIdx],SI	;save for next output

	pop	SI
	pop	AX
	pop	ES	;restore registers

	ret

	ENDP

;==========================================================
;
;	EXPAND a RLE compressed file segment
;
;==========================================================


	PUBLIC	Expand
PROC	Expand ; sub_0_5DD6

	call	InitRLE	;init our vars & buffers

	xchg	SI,DI	;swap SI and DI
	mov	DX,1
@@bank:
	mov	ES,[ss:SrcSeg]	;point to source buffer
	xor	AH,AH
@@get:
	cmp	DI,0FF00h	;don't bog us down!
	jb	@@ok

	mov	AX,DI
	shr	AX,1
	shr	AX,1	;offset / 16
	shr	AX,1
	shr	AX,1
	add	[ss:SrcSeg],AX	; + seg reg = normalized ptr
;	and	DI,0Fh
	mov	DI,0

	mov	AX,SI
	shr	AX,1
	shr	AX,1	;offset / 16
	shr	AX,1
	shr	AX,1
	add	[ss:RLESeg],AX	; + segment register = normalized ptr
	and	SI,0Fh
	jmp	SHORT @@bank
@@ok:
	sub	[ss:SrcLen],DX	;subtract out # of chars
	sbb	[ss:SrcLen+2],0
	jc	@@Done	;exit if we are done!
	xor	DX,DX
@@0:
	push	DS
	mov	DS,[ss:RLESeg]	;point to RLE buffer
	lodsb		; & get code byte
	cmp	AL,80h
	jb	@@rpt	;repeat code?
	sub	AL,7Fh
	mov	CX,AX	; no- string length = code - $80 + 1
	mov	DX,AX

	REP	movsb	;copy string
	pop	DS
	jmp	SHORT @@get	; & do some more
@@rpt:
	add	AL,3	;# of bytes = code + 3
	mov	CX,AX
	mov	DX,AX
	lodsb		;get byte to repeat
	REP	stosb
	pop	DS	; & store that many bytes
	jmp	SHORT @@get
@@Done:
	ret

	ENDP

;==========================================================
;
;	COMPRESS data buffer using RLE
;
;==========================================================

	PUBLIC	Compress
PROC	Compress ; sub_0_5E43

	call	InitRLE	;init vars
@@bank:
	mov	ES,[ss:SrcSeg]	;get first byte of data
	mov	AL,[ES:SI]

	mov	[ss:LastByte],AL	; & set as last byte
@@get:
	cmp	SI,0FF00h	;don't bog us down!
	jb	@@ok

	call	@@Done	;send any patial strings!

	mov	AX,SI
	shr	AX,1
	shr	AX,1	;offset / 16
	shr	AX,1
	shr	AX,1
	add	[ss:SrcSeg],AX	; + segment register = normalized
	and	SI,0Fh
	mov	[ss:SrcIdx],0	;which is our new 'start'

	mov	AX,DI
	shr	AX,1
	shr	AX,1	;offset / 16
	shr	AX,1
	shr	AX,1
	add	[ss:RLESeg],AX	; + segment register = normalized ptr
	and	DI,0Fh
	inc	[ss:DifCnt]
	jmp	SHORT @@bank
@@ok:
	sub	[ss:SrcLen],1	;subtract out # of chars
	sbb	[ss:SrcLen+2],0
	jnc	@@0
	jmp	NEAR @@Done	;exit if we are done!
@@0:
	inc	SI	;get next byte
	mov	AL,[ES:SI]
	cmp	AL,[ss:LastByte]
	jne	@@dif	;same as last byte?

	inc	[ss:ByteCnt]	; yes- count it
	cmp	[ss:ByteCnt],MinCnt
	jb	@@get

	cmp	[ss:DifCnt],1	;ok, got a rep string...already have dif?
	jbe	@@same
	dec	[ss:DifCnt]
	call	OutputDif	; yes- output that string first
@@same:
	mov	[ss:DifCnt],0	;force even if not output!
	cmp	[ss:ByteCnt],MaxCnt
	;TODO jb	@@get	;have we gotten too big?
	db 72h, 10h ;TODO

	call	OutputRep	; yes, save string & continue

	cmp	AL,[ES:SI+1]	;another repeat?
	jne	@@gget
	cmp	AL,[ES:SI+2]
        jne	@@gget
	inc	SI	;need this for >127 byte strings!
@@gget:	jmp	NEAR @@get
;-------------------------------------------------------------
@@dif:
	mov	CX,[ss:ByteCnt]	;any dups yet?
	jcxz	@@cnt

	cmp	CX,MinCnt	;repeats before now?
	jb	@@add

	call	OutputRep	; yes- save repeat bytes
	jmp	SHORT @@cnt
@@add:
	add	[ss:DifCnt],CX	;add bad repeats to dif count
	mov	[ss:ByteCnt],0	; & no more dupes!
@@cnt:
	inc	[ss:DifCnt]	;count byte as different
	mov	[ss:LastByte],AL

	cmp	[ss:DifCnt],MaxCnt-MinCnt-1	;still in range?
	jb	@@goget

	call	OutputDif	; no- output string & continue

;	mov	AL,[ES:SI+1]
	cmp	AL,[ES:SI+1]
	jne	@@goget	;if repeat follows, specialize!
	cmp	AL,[ES:SI+2]
	jne	@@goget
	inc	SI	;why?
        inc	[ss:DifCnt]
@@goget:
	jmp	NEAR @@get
;-------------------------------------------------------------
@@Done:
	mov	CX,[ss:ByteCnt]	;any reps left?
	jcxz	@@1

	cmp	CX,MinCnt	; yes- enough to output?
	jb	@@2

	cmp	[ss:DifCnt],1	; yes, any difs before these?
	jb	@@3

	call	OutputDif	; yes- output them
@@3:
	call	OutputRep	;output rep bytes
	jmp	SHORT @@4
@@2:
	add	[ss:DifCnt],CX	;update dif count w/small rep
@@1:
	mov	CX,[ss:DifCnt]	;anything to output?
	jcxz	@@4
	call	OutputDif	;output dif string
@@4:
	ret

	ENDP

;============================================================================

	IDEAL

	DATASEG

tickcount	dw	?

timecall	dw	?
timeax		dw	?
timebx		dw	?
timecx		dw	?
timedx		dw	?
timesi		dw	?
timedi		dw	?
timebp		dw	?
timees		dw	?

	PUBLIC	timecall,timeax,timebx,timecx,timedx,timesi,timedi,timebp,timees

	CODESEG

;=============
;
; TIMESUB
; Returns the number of iterations of the given subroutine
; are possible in 1/18 second
;
;=============

	EXTRN	stopsound:NEAR
PROC	timesub	ticks:WORD ; sub_0_5F5B
	USES	si,di,bp
	PUBLIC	timesub

	mov	ax,[ticks]
	mov	[ss:tickcount],ax

	mov	ax,351ch	;call bios to get int 1c
	int	21h
	mov	[cs:@@old1c],bx
	mov	ax,es
	mov	[cs:@@old1c+2],ax

	push	ds

	push	cs
	pop	ds
	lea	dx,[@@intermark]
	mov	ax,251ch	;call bios to set int 1c
	int	21h

	pop	ds

	call	stopsound	;make sure the timer is going normal speed
	xor	ax,ax
	mov	[cs:@@counter],ax	;set counter to 0
	mov	[cs:@@mark],ax	;will be inc to 1 after int 1c

@@wait1:
	mov	ax,[cs:@@mark]	;wait for the interrupt to go off once
	cmp	ax,0
	je	@@wait1

@@countmore:
	xor	ax,ax
	mov	[cs:@@mark],ax	;will be inc to 1 after int 1c
@@count:
	push	bp
	mov	ax,[ss:timees]
	mov	es,ax
	mov	ax,[ss:timeax]
	mov	bx,[ss:timebx]
	mov	cx,[ss:timecx]
	mov	dx,[ss:timedx]
	mov	si,[ss:timesi]
	mov	di,[ss:timedi]
	mov	bp,[ss:timebp]
	call	[ss:timecall]
	pop	bp
	inc	[cs:@@counter]
	mov	ax,[cs:@@mark]	;has the interrupt gone off again?
	or	ax,ax
	je	@@count
	dec	[ss:tickcount]
	jne	@@countmore

	push	ds

	mov	ax,[cs:@@old1c+2]
	mov	ds,ax
	mov	dx,[cs:@@old1c]
	mov	ax,251ch	;call bios to set int 1c
	int	21h

	pop	ds

	mov	ax,[cs:@@counter]	;return value
	ret

@@intermark:
	inc	[WORD cs:@@mark]
	jmp	[DWORD cs:@@old1C]


@@old1C	dw	0,0		;old [1Ch*4]
@@counter	dw	0
@@mark	dw	0		;set to 1 when the int has occured

ENDP

	END
