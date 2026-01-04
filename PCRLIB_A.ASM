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

	IDEAL
	MODEL SMALL,C

;
;offsets into .SPK file at segment SPKRfile, offset 0
;
;sound records each take up 16 bytes, and start at $10, and continue to $3F0

snd_start	equ	0
snd_priority	equ	2
snd_samples	equ	3
snd_name	equ	4


;
; EGA registers
;
SCindex		equ	3C4h
SCmapmask	equ	2
GCindex		equ	3CEh
GCmode 		equ	5


	DATASEG


;
; stuff for pics and sprites
;
STRUC	picstruct		; sprite info extracted by refresh
width		dw	?
height		dw	?
shapeptr	dw	?
ENDS

	EXTRN pictable: picstruct	; array of picture info
	EXTRN charptr:WORD
	EXTRN tileptr:WORD
	EXTRN picptr:WORD
	EXTRN spriteptr:WORD	;paraligned pointers
	EXTRN grmode:WORD
	EXTRN crtcaddr:WORD

;
; stuff for pics and sprites
;
STRUC	picstruct		; sprite info extracted by refresh
width		dw	?
height		dw	?
shapeptr	dw	?
ENDS

	EXTRN	imageinfo: picstruct	; array of picture info

SPKactive dw    0                   ;set non zero when started

SoundData	dd	?	;two word pointer to SPKR file, PARA aligned
soundmode	dw	1       ;0=nosound, 1=SPKR, 2= adlib...
OldInt8		dd	?	;StartupSPK saves here, Shutdown restores
ExtraInts	db	?	;number of PlaySPKR's to a regular int 8
Intcount	db	?	;counter for extraints, call OldInt8 at 0
sndspeed	dw	?	;timer count speed

SndPtr		dw	?	;Pointer to frequency of current sound
SndPriority	db	?	;current sound's priority

pausesndptr	dw	?
pausepriority	db	?
pauseextraints	db	?
pauseintcount	db	?
pausespeed	dw	?

_dontplay	dw	0	;set to 1 to avoid all interrupt and timer stuff

xormask		dw	0	;to invert characters
_yshift		dw	0	;to shift char lines

screenseg	dw	0a000h	;changes with grmode, page flips, & scrolls

	PUBLIC soundmode,SoundData,xormask,screenseg,_dontplay

	  CODESEG

	PUBLIC CGAylookup,EGAylookup,VGAylookup

;========
;
; YLOOKUP has the offsets from screen mem of each screen line (multiples of 8)
;
;========

CGAylookup dw    0,8192,  80,8272, 160,8352, 240,8432, 320,8512, 400,8592, 480,8672
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

EGAylookup dw    0,  48,  96, 144, 192, 240, 288, 336, 384, 432, 480, 528, 576, 624
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
    dw 12096,12144,12192,12240

byte_0_50EA db 0
db 30h
db 30h
db 30h
db 60h
db 30h
db 90h
db 30h
db 0C0h
db 30h
db 0F0h
db 30h
db 20h
db 31h
db 50h
db 31h
db   0
db   0

VGAylookup dw    0, 320, 640, 960,1280,1600,1920,2240,2560,2880,3200,3520,3840,4160
    dw 4480,4800,5120,5440,5760,6080,6400,6720,7040,7360,7680,8000,8320,8640
    dw 8960,9280,9600,9920,10240,10560,10880,11200,11520,11840,12160,12480,12800,13120
    dw 13440,13760,14080,14400,14720,15040,15360,15680,16000,16320,16640,16960,17280,17600
    dw 17920,18240,18560,18880,19200,19520,19840,20160,20480,20800,21120,21440,21760,22080
    dw 22400,22720,23040,23360,23680,24000,24320,24640,24960,25280,25600,25920,26240,26560
    dw 26880,27200,27520,27840,28160,28480,28800,29120,29440,29760,30080,30400,30720,31040
    dw 31360,31680,32000,32320,32640,-32576,-32256,-31936,-31616,-31296,-30976,-30656,-30336,-30016
    dw -29696,-29376,-29056,-28736,-28416,-28096,-27776,-27456,-27136,-26816,-26496,-26176,-25856,-25536
    dw -25216,-24896,-24576,-24256,-23936,-23616,-23296,-22976,-22656,-22336,-22016,-21696,-21376,-21056
    dw -20736,-20416,-20096,-19776,-19456,-19136,-18816,-18496,-18176,-17856,-17536,-17216,-16896,-16576
    dw -16256,-15936,-15616,-15296,-14976,-14656,-14336,-14016,-13696,-13376,-13056,-12736,-12416,-12096
    dw -11776,-11456,-11136,-10816,-10496,-10176,-9856,-9536,-9216,-8896,-8576,-8256,-7936,-7616
    dw -7296,-6976,-6656,-6336,-6016,-5696,-5376,-5056,-4736,-4416,-4096,-3776,-3456,-3136
    dw -2816,-2496,-2176,-1856


;=======================================================================

;========
;
; StartupSound
;
; Sets up the new INT 8 ISR and various internal pointers.
; Assumes that the calling program has pointer SOUNDDATA to something
; meaningful...
;
;========
;sub_0_528C:
PROC StartupSound
	PUBLIC	StartupSound

	test	[_dontplay],0ffffh
	je	@@dowork
	ret
@@dowork:
	test	[SPKactive],0FFFFh	;see if library is active
	jne	@@started		;library was allready started

@@start:
	call	NEAR PTR StopSound	;make sure nothing is playing

	mov	ax,3508h	;call bios to get int 8
	int	21h
	mov	[WORD oldint8],bx
	mov	ax,es
	mov	[WORD oldint8+2],ax

	mov	ax,1
	mov	[extraints],al		;the timer is still going at the
	mov	[intcount],al		;normal rate now

	push	ds

	push	cs
	pop	ds
	lea	dx,[UpdateSPKR]
	mov	ax,2508h	;call bios to set int 8
	int	21h

	pop	ds

	inc	[SPKactive]	;sound routines are now active

@@started:
	mov	ax,1
	mov	[soundmode],ax ;set soundmode to SPKR
	ret

ENDP

;=======================================================================

;========
;
; ShutdownSound
;
;========
;sub_0_52CE:
PROC	ShutdownSound
	PUBLIC ShutdownSound

	test	[_dontplay],0ffffh
	je	@@dowork
	ret
@@dowork:
	mov	al,36h		;tell the timer chip we are going to
	out	43h,al		;change the speed of timer 0
	mov	al,0		;system expects 0000 for rate
	out	40h,al		;low
	out	40h,al		;high

	mov	ax,[SPKactive]
	cmp	ax,0
	je	@@done		;sound library wasn't started...

	push	ds

	mov	dx,[WORD Oldint8]
	mov	ax,[WORD Oldint8+2]
	mov	ds,ax
	mov	ax,2508h	;call bios to set int 8
	int	21h

	pop	ds

	mov	[SPKactive],0	;sound routines are now inactive

	in	al,61h		;get peripheral (speaker) port value
	and	al,11111101b	;turn speaker off
	out	61h,al

@@done:
	ret

ENDP

;=========================================================================

;===========
;
; PlaySound (soundnum)
;
; If the sound's priority is >= the current priority, SoundPtr, SndPriority,
; and the timer speed are changed
;
;===========
;sub_0_5306:
PROC	PlaySound playnum:WORD
	PUBLIC PlaySound

	test	[_dontplay],0ffffh
	je	@@dowork
	ret
@@dowork:
	mov	ax,[playnum]	;index into the sound headers
	push	si

	mov	si,ax
	shl	si,1
	shl	si,1
	shl	si,1
	shl	si,1

	mov	ax,[WORD SoundData+2]
	mov	es,ax		;point es: to the spkr file

	mov	al,[es:si+snd_Priority]	;priority table (one byte each)
	cmp	al,[SndPriority]
	jb	@@playdone	;current sound has higher priority
	mov	[SndPriority],al
	mov	ax,[es:si+snd_Start]	;offset in .SPK file
	mov	[SndPtr],ax	;store it in the sound playing table

	mov	bl,[es:si+snd_samples]	;samples / regular timer tick (1-255)
	mov	[extraints],bl	;an OldInt8 will be called after this
	mov	[intcount],bl	;many UpdateSPKR times have been called

	cmp	bl,1		;sample rate of 0 or 1 = 0000 for timer
	ja	@@oktodiv
	xor	bx,bx
	jmp	@@settimer

@@oktodiv:
	xor	bh,bh
	xor	ax,ax
	mov	dx,1
	div	bx		;$10000 / samples = timer rate

	mov	bx,ax
	mov	[sndspeed],bx	;save off the timer rate

@@settimer:
	mov	al,36h		;tell the timer chip we are going to
	out	43h,al		;change the speed of timer 0

	mov	al,bl		;low byte of sample rate
	out	40h,al
	mov	al,bh		;high byte of sample rate
	out	40h,al

@@playdone:
	pop	si
	ret

ENDP


;======================================================================

;===========
;
; StopSound
;
;===========
;sub_0_536D:
PROC	StopSound
	PUBLIC	StopSound

	test	[_dontplay],0ffffh
	je	@@dowork
	ret
@@dowork:
	xor	ax,ax		;set to 0
	mov	[SndPtr],ax
	mov	[SndPriority],al
	mov	[sndspeed],ax

	in	al,61h		;get peripheral (speaker) port value
	and	al,11111101b	;turn speaker off
	out	61h,al

	mov	al,36h		;tell the timer chip we are going to
	out	43h,al		;change the speed of timer 0

	mov	al,0		;back to normal clock speed
	out	40h,al
	out	40h,al

	inc	al
	mov	[extraints],al  ;one bios int / int8
	mov	[intcount],al

	ret
ENDP

;======================================================================

;===========
;
; PauseSound
;
;===========
;sub_0_539A:
PROC	PauseSound
	PUBLIC	PauseSound

	test	[_dontplay],0ffffh
	je	@@dowork
	ret
@@dowork:
	mov	ax,[SndPtr]	;save off the current values
	mov	[pausesndptr],ax
	mov	al,[SndPriority]
	mov	[pausepriority],al
	mov	al,[extraints]
	mov	[pauseextraints],al
	mov	al,[intcount]
	mov	[pauseintcount],al
	mov	ax,[sndspeed]
	mov	[pausespeed],ax
	call	StopSound
	ret
ENDP

;======================================================================

;===========
;
; ContinueSound
;
;===========
;sub_0_53C5:
PROC	ContinueSound
	PUBLIC	ContinueSound

	test	[_dontplay],0ffffh
	je	@@dowork
	ret
@@dowork:
	mov	ax,[pausesndptr]
	mov	[SndPtr],ax	;restore the old values
	mov	al,[pausepriority]
	mov	[SndPriority],al
	mov	al,[pauseextraints]
	mov	[extraints],al
	mov	al,[pauseintcount]
	mov	[intcount],al

	mov	bx,[pausespeed]

	mov	al,36h		;tell the timer chip we are going to
	out	43h,al		;change the speed of timer 0

	mov	al,bl		;low byte of sample rate
	out	40h,al
	mov	al,bh		;high byte of sample rate
	out	40h,al


	ret
ENDP

;======================================================================

;========
;
; WaitendSound
; Just waits around until the current sound stops playing
;
;========
sub_0_53F7:
PROC	WaitEndSound
	PUBLIC WaitEndSound

	test	[_dontplay],0ffffh
	je	@@dowork
	ret
@@dowork:
	pushf
	call FAR PTR UpdateSPKR	;in case a sound was just started and hasn't
				;been hit by an INT yet
@@wait:
	mov	ax,[sndptr]
	cmp	ax,0		;when the ptr is 0, nothing is on
	jne	@@wait

	ret

ENDP

;=========================================================================

;========
;
; UpdateSPKR
; only called by interrupt $8!
;
;=========
;sub_0_540F:
PROC	UpdateSPKR FAR

	push	ax
	push	bx
	push	cx
	push	si
	push	ds
	push	es

	mov	ax,_DATA
	mov	ds,ax		;ds to this data segment
	mov	ax,[WORD SoundData+2]
	mov	es,ax		;es to sound file

	mov	al,20h
	out	20h,al		;we got the interrupt here

	dec	[intcount]	;see if it is time for a BIOS int 8...
	jne	@@dosounds

	mov	al,[extraints]
	mov	[intcount],al	;reset interrupt counter

	pushf			;so the IRET from bios returns right
	call	[OldInt8]	;call the old BIOS timer routine

@@dosounds:
;
; play the speaker
;
	mov	si,[SndPtr]
	cmp	si,0
	je	@@nosound	;nothing playing

	mov	bx,[es:si]
	inc	[SndPtr]
	inc	[SndPtr]

	cmp	bx,0
	je	@@nosound	;a zero frequency is no sound, but don't stop

	cmp	bx,-1		;a -1 frequency is end of sound
	jne	@@playfreq

	call	StopSound
	jmp	@@doneplay

@@nosound:
	in	al,61h		;get peripheral (speaker) port value
	and	al,11111100b	;turn speaker off
	out	61h,al
	jmp	@@doneplay

@@playfreq:
	test	[soundmode],0FFh	;if soundon=0, don't play anything
	je	@@nosound

	mov	al,10110110b	;write to channel 2 (speaker) timer
	out	43h,al
	mov	al,bl
	out	42h,al		;low byte
	mov	al,bh
	out	42h,al		;high byte

	in	al,61h		;get peripheral (speaker) port value
	or	al,00000011b	;turn speaker on to timer
	out	61h,al

@@doneplay:
	pop	es
	pop	ds
	pop	si
	pop	cx
	pop	bx
	pop	ax

	iret


ENDP

;==========================================================================

;========================================================================
	DATASEG

rndindex	dw	?

rndtable db    0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66
    db   74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36
    db   95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188
    db   52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224
    db  149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242
    db  145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0
    db  175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235
    db   25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113
    db   94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75
    db  136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196
    db  135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113
    db   80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241
    db   24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224
    db  145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95
    db   28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226
    db   71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36
    db   17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106
    db  197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136
    db  120, 163, 236, 249

;
; Random # Generator vars
;
indexi		dw	?	;Rnd#Generator
indexj		dw	?
LastRnd		dw	?
RndArray	dw	17 dup (?)

baseRndArray	dw	1,1,2,3,5,8,13,21,54,75,129,204
   		dw	323,527,850,1377,2227

	CODESEG

sub_0_5482:
db 55h, 8Bh, 0ECh, 56h, 57h, 8Ch, 0D8h, 8Eh, 0C0h, 0BFh, 0F4h, 11h, 0BEh, 16h
db 12h, 0B9h, 11h, 00h, 0FCh, 0F3h, 0A5h, 0C7h, 06h, 0F2h, 11h, 00h, 00h, 0C7h, 06h, 0EEh
db 11h, 22h, 00h, 0C7h, 06h, 0F0h, 11h, 0Ah, 00h, 8Bh, 46h, 04h, 3Dh, 00h, 00h, 74h
db 0Eh, 0B4h, 2Ch, 0CDh, 21h, 89h, 16h, 14h, 12h, 33h, 0D1h, 89h, 16h, 0FCh, 11h, 0B8h
db 0FFh, 0FFh, 50h, 0E8h, 05h, 00h, 58h, 5Fh, 5Eh, 5Dh, 0C3h

sub_0_54CB:
db 55h, 8Bh, 0ECh, 56h, 53h
db 51h, 52h, 8Bh, 46h, 04h, 50h, 0BAh, 0FFh, 0FFh, 0D1h, 0E0h, 72h, 04h, 0D1h, 0EAh, 0EBh
db 0F8h, 8Bh, 1Eh, 0EEh, 11h, 8Bh, 36h, 0F0h, 11h, 8Bh, 87h, 0F2h, 11h, 13h, 84h, 0F2h
db 11h, 89h, 87h, 0F2h, 11h, 03h, 06h, 0F2h, 11h, 0A3h, 0F2h, 11h, 4Bh, 4Bh, 75h, 03h
db 0BBh, 22h, 00h, 4Eh, 4Eh, 75h, 03h, 0BEh, 22h, 00h, 89h, 1Eh, 0EEh, 11h, 89h, 36h
db 0F0h, 11h, 59h, 23h, 0C2h, 3Bh, 0C1h, 76h, 02h, 0D1h, 0E8h, 5Ah, 59h, 5Bh, 5Eh, 5Dh
db 0C3h 

sub_0_5521:
db 55h, 8Bh, 0ECh, 56h, 57h, 8Bh, 46h, 04h, 3Dh, 00h, 00h, 0BAh, 00h, 00h, 0EBh
db 09h, 90h, 0B4h, 2Ch, 0CDh, 21h, 81h, 0E2h, 0FFh, 00h, 89h, 16h, 0ECh, 10h, 5Fh, 5Eh
db 5Dh, 0C3h

sub_0_5542:
db 8Bh, 1Eh, 0ECh, 10h, 43h, 81h, 0E3h, 0FFh, 00h, 89h, 1Eh, 0ECh, 10h, 8Ah
db 87h, 0EEh, 10h, 32h, 0E4h, 0C3h

sub_0_5556:
db 56h, 57h, 8Bh, 16h, 60h, 0C3h, 83h, 0C2h, 06h, 0ECh
db 0A8h, 08h, 75h, 0FBh, 0ECh, 0A8h, 08h, 74h, 0FBh, 5Fh, 5Eh, 0C3h

;=======================================================================

;====================
;
; EGAplane
; Sets read/write mode 0 and selects the given plane (0-3)
; for reading and writing
;
;====================

PROC	EGAplane plane:WORD
	PUBLIC	EGAplane

	mov	dx,GCindex
	mov	ax,GCmode
	out	dx,ax		;set read / write mode 0

	mov	dx,GCindex
	mov	al,4		;read map select
	mov	ah,[BYTE plane] ;read from this plane number
	out	dx,ax
	mov	dx,SCindex
	mov	al,SCmapmask
	mov	ah,1
	mov	cl,[BYTE plane]	;write to this plane only
	shl	ah,cl
	out	dx,ax
	ret

ENDP

;=======================================================================

;====================
;
; EGAlatch
; Sets write mode 1 with all planes selected
;
;====================

PROC	EGAlatch plane:WORD
	PUBLIC	EGAlatch

	mov	dx,GCindex
	mov	ax,1*256 + GCmode
	out	dx,ax		;set read 0 / write mode 1

	mov	dx,SCindex
	mov	ax,15*256 + SCmapmask	;write to all planes
	out	dx,ax
	ret
ENDP

;=======================================================================

sub_0_55A1:
db 55h, 8Bh, 0ECh, 56h, 57h, 0A1h, 0EAh, 10h, 8Eh, 0C0h, 8Bh, 76h, 08h, 8Bh, 7Eh
db 04h, 8Bh, 5Eh, 06h, 0D1h, 0E3h, 0D1h, 0E3h, 0D1h, 0E3h, 0D1h, 0E3h, 0A1h, 9Ch, 0AFh, 3Dh
db 01h, 00h, 75h, 07h, 0E8h, 1Ch, 00h, 5Fh, 5Eh, 5Dh, 0C3h, 3Dh, 02h, 00h, 75h, 07h
db 0E8h, 6Bh, 00h, 5Fh, 5Eh, 5Dh, 0C3h, 3Dh, 03h, 00h, 75h, 03h, 0E8h, 0B8h, 01h, 5Fh
db 5Eh, 5Dh, 0C3h

sub_0_55E3:
db 0D1h, 0E6h, 0D1h, 0E6h, 0D1h, 0E6h, 0D1h, 0E6h, 0D1h, 0E7h, 2Eh, 03h, 0BFh
db 5Ah, 4Dh, 8Bh, 1Eh, 0E8h, 10h, 0A1h, 6Ch, 0AEh, 8Eh, 0D8h, 0FCh, 0ADh, 33h, 0C3h, 0ABh
db 81h, 0C7h, 0FEh, 1Fh, 0ADh, 33h, 0C3h, 0ABh, 81h, 0EFh, 0B2h, 1Fh, 0ADh, 33h, 0C3h, 0ABh
db 81h, 0C7h, 0FEh, 1Fh, 0ADh, 33h, 0C3h, 0ABh, 81h, 0EFh, 0B2h, 1Fh, 0ADh, 33h, 0C3h, 0ABh
db 81h, 0C7h, 0FEh, 1Fh, 0ADh, 33h, 0C3h, 0ABh, 81h, 0EFh, 0B2h, 1Fh, 0ADh, 33h, 0C3h, 0ABh
db 81h, 0C7h, 0FEh, 1Fh, 0ADh, 33h, 0C3h, 0ABh, 0B8h, 89h, 07h, 8Eh, 0D8h, 0C3h

sub_0_563E:
db 0D1h, 0E6h
db 0D1h, 0E6h, 0D1h, 0E6h, 2Eh, 03h, 0BFh, 0EAh, 4Eh, 8Ch, 0D9h, 0BAh, 0CEh, 03h, 0B8h, 05h
db 00h, 0EFh, 0FCh, 8Bh, 0DEh, 8Bh, 0CFh, 0BAh, 0CEh, 03h, 0B8h, 04h, 00h, 0EFh, 0BAh, 0C4h
db 03h, 0B8h, 02h, 01h, 0EFh, 8Bh, 16h, 0E8h, 10h, 0A1h, 6Ch, 0AEh, 8Eh, 0D8h, 0ACh, 32h
db 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh
db 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h
db 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh
db 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 8Bh, 0F3h, 8Bh, 0F9h, 52h, 0BAh, 0CEh, 03h, 0B8h, 04h
db 01h, 0EFh, 0BAh, 0C4h, 03h, 0B8h, 02h, 02h, 0EFh, 5Ah, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h
db 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh
db 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h
db 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h
db 0C7h, 2Fh, 8Bh, 0F3h, 8Bh, 0F9h, 52h, 0BAh, 0CEh, 03h, 0B8h, 04h, 02h, 0EFh, 0BAh, 0C4h
db 03h, 0B8h, 02h, 04h, 0EFh, 5Ah, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h
db 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h
db 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh
db 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 8Bh, 0F3h
db 8Bh, 0F9h, 52h, 0BAh, 0CEh, 03h, 0B8h, 04h, 03h, 0EFh, 0BAh, 0C4h, 03h, 0B8h, 02h, 08h
db 0EFh, 5Ah, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh
db 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h
db 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh
db 83h, 0C7h, 2Fh, 0ACh, 32h, 0C2h, 0AAh, 83h, 0C7h, 2Fh, 0BAh, 0C4h, 03h, 0B8h, 02h, 0Fh
db 0EFh, 0B8h, 89h, 07h, 8Eh, 0D8h, 0C3h

sub_0_5797:
db 0D1h, 0E6h, 0D1h, 0E6h, 0D1h, 0E6h, 0D1h, 0E6h, 0D1h
db 0E6h, 0D1h, 0E6h, 0D1h, 0E7h, 0D1h, 0E7h, 0D1h, 0E7h, 2Eh, 03h, 0BFh, 0FCh, 50h, 8Bh, 1Eh
db 0E8h, 10h, 0A1h, 6Ch, 0AEh, 8Eh, 0D8h, 0FCh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh
db 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 81h, 0C7h, 38h, 01h, 0ADh, 33h, 0C3h, 0ABh
db 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 81h, 0C7h, 38h, 01h
db 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh
db 81h, 0C7h, 38h, 01h, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh
db 0ADh, 33h, 0C3h, 0ABh, 81h, 0C7h, 38h, 01h, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh
db 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 81h, 0C7h, 38h, 01h, 0ADh, 33h, 0C3h, 0ABh
db 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 81h, 0C7h, 38h, 01h
db 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh
db 81h, 0C7h, 38h, 01h, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh, 0ADh, 33h, 0C3h, 0ABh
db 0ADh, 33h, 0C3h, 0ABh, 0B8h, 89h, 07h, 8Eh, 0D8h, 0C3h

sub_0_585A:
db 55h, 8Bh, 0ECh, 56h, 57h, 0FCh
db 0A1h, 0EAh, 10h, 8Eh, 0C0h, 8Bh, 76h, 08h, 8Bh, 7Eh, 04h, 8Bh, 5Eh, 06h, 0D1h, 0E3h
db 0A1h, 9Ch, 0AFh, 8Bh, 0Eh, 98h, 0AEh, 8Eh, 0D9h, 3Dh, 01h, 00h, 75h, 06h, 0E8h, 1Fh
db 00h, 0EBh, 14h, 90h, 3Dh, 02h, 00h, 75h, 06h, 0E8h, 84h, 00h, 0EBh, 09h, 90h, 3Dh
db 03h, 00h, 75h, 03h, 0E8h, 0EAh, 00h, 0B8h, 89h, 07h, 8Eh, 0D8h, 5Fh, 5Eh, 5Dh, 0C3h

;=======================
;
; CGAtileout
; SI= tile #, BX=y, DI=x
;
;=======================
PROC	CGAtileout
	PUBLIC	CGAtileout

	shl	si,1
	shl	si,1
	shl	si,1
	shl	si,1
	shl	si,1
	shl	si,1

	shl	bx,1

	add	di,[CGAylookup + BX]	;destination on screen

	movsw
	movsw
	add	di,8188
	movsw
	movsw
	sub	di,8116
	movsw
	movsw
	add	di,8188
	movsw
	movsw
	sub	di,8116
	movsw
	movsw
	add	di,8188
	movsw
	movsw
	sub	di,8116
	movsw
	movsw
	add	di,8188
	movsw
	movsw
	sub	di,8116
	movsw
	movsw
	add	di,8188
	movsw
	movsw
	sub	di,8116
	movsw
	movsw
	add	di,8188
	movsw
	movsw
	sub	di,8116
	movsw
	movsw
	add	di,8188
	movsw
	movsw
	sub	di,8116
	movsw
	movsw
	add	di,8188
	movsw
	movsw

	ret
ENDP

;=======================
;
; EGAtileout
; SI= tile #, BX=y, DI=x
;
;=======================
PROC	EGAtileout
	PUBLIC	EGAtileout

	shl	si,1
	shl	si,1
	shl	si,1
	shl	si,1
	shl	si,1
	add	di,[EGAylookup + BX]	;destination on screen

	mov	dx,SCindex
	mov	al,SCmapmask
	out	dx,al
	mov	al,1111b
	inc	dx
	out	dx,al

	mov	dx,GCindex
	mov	al,GCmode
	out	dx,al
	mov	al,1
	inc	dx
	out	dx,al

	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb
	add	di,46
	movsb
	movsb

	ret
ENDP


;=======================
;
; VGAtileout
; SI= tile #, BX=y, DI=x
;
;=======================
PROC	VGAtileout
	PUBLIC	VGAtileout

	mov	ah,al
	xor	al,al			;fast multiply by 256
	add	di,[VGAylookup + BX]	;destination on screen

	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,304
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw

	ret
ENDP



;=======================================================================

sub_0_5A47:
db 55h, 8Bh, 0ECh, 56h, 57h, 0FCh, 8Bh, 76h, 08h
db 0D1h, 0E6h, 0D1h, 0E6h, 0D1h, 0E6h, 0D1h, 0E6h, 8Bh, 84h, 0D2h, 0E3h, 2Eh, 0A3h, 0A9h, 5Ah
db 8Bh, 84h, 0D4h, 0E3h, 2Eh, 0A3h, 0ADh, 5Ah, 8Bh, 0B4h, 0D6h, 0E3h, 0A1h, 0EAh, 10h, 8Eh
db 0C0h, 8Bh, 7Eh, 04h, 8Bh, 5Eh, 06h, 0D1h, 0E3h, 0A1h, 9Ch, 0AFh, 8Bh, 0Eh, 88h, 0AEh
db 8Eh, 0D9h, 3Dh, 01h, 00h, 75h, 06h, 0E8h, 25h, 00h, 0EBh, 14h, 90h, 3Dh, 02h, 00h
db 75h, 06h, 0E8h, 40h, 00h, 0EBh, 09h, 90h, 3Dh, 03h, 00h, 75h, 03h, 0E8h, 69h, 00h
db 0B8h, 89h, 17h, 8Eh, 0D8h, 5Fh, 5Eh, 5Dh, 0C3h

picwidth	dw	?	;used by all modes
picxcoord	dw	?
picheight	dw	?

;===========================================================================

;============
;
; CgaDrawpic
; SI= source; DI= xcoordinate; BX= ycoordinate*2
;
;============

PROC	CgaDrawpic

	shr	di,1		;x / 4 + ylookup[y] = screen location
	shr	di,1
	mov	[WORD cs:picxcoord],di

@@drawrow:
	mov	cx,[WORD cs:picwidth] 	;number of words to write
	mov	di,[WORD cs:CGAylookup+bx]
	add	bx,2
	add	di,[WORD cs:picxcoord]
@@drawword:
	movsb
	loop	@@drawword
	dec	[WORD cs:picheight]
	jnz	@@drawrow

	ret
ENDP



;=======================================================================

;============
;
; EgaDrawpic
; SI= source; DI= xcoordinate; BX= ycoordinate
;
;============

PROC	EgaDrawpic

	mov	ax,105h
	mov	dx,3ceh		;set write mode 1
	out	dx,ax

	shr	di,1		;x / 8 + ylookup[y] = screen location
	shr	di,1
	shr	di,1
	mov	[WORD cs:picxcoord],di

	mov	ax,0A000h	;segment of screen memory
	mov	es,ax

@@drawrow:
	mov	cx,[WORD cs:picwidth] 	;number of bytes to write
	mov	di,[WORD cs:EGAylookup+bx]
	add	bx,2
	add	di,[WORD cs:picxcoord]
@@drawword:
	movsb
	loop	@@drawword
	dec	[WORD cs:picheight]
	jnz	@@drawrow

	ret
ENDP


;=======================================================================

;============
;
; VgaDrawpic
; SI= source; DI= xcoordinate; BX= ycoordinate
;
;============

PROC	VgaDrawpic

	mov	[WORD cs:picxcoord],di

@@drawrow:
	mov	cx,[WORD cs:picwidth] 	;number of words to write
	mov	di,[WORD cs:VGAylookup+bx]
	add	bx,2
	add	di,[WORD cs:picxcoord]
@@drawword:
	movsb
	loop	@@drawword
	dec	[WORD cs:picheight]
	jnz	@@drawrow

	ret
ENDP

;===========================================================================

	MASM
;อออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
;
; Name:	VideoID
;
; Function:	Detects the presence of various video subsystems
;
; int VideoID;
;
; Subsystem ID values:
; 	 0  = (none)
; 	 1  = MDA
; 	 2  = CGA
; 	 3  = EGA
; 	 4  = MCGA
; 	 5  = VGA
; 	80h = HGC
; 	81h = HGC+
; 	82h = Hercules InColor
;
;อออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

;อออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
;
; Equates
;
;อออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
VIDstruct	STRUC		; corresponds to C data structure

Video0Type	DB	?	; first subsystem type
Display0Type	DB	? 	; display attached to first subsystem

Video1Type	DB	?	; second subsystem type
Display1Type	DB	?	; display attached to second subsystem

VIDstruct	ENDS


Device0	EQU	word ptr Video0Type[di]
Device1	EQU	word ptr Video1Type[di]


MDA	EQU	1	; subsystem types
CGA	EQU	2
EGA	EQU	3
MCGA	EQU	4
VGA	EQU	5
HGC	EQU	80h
HGCPlus	EQU	81h
InColor	EQU	82h

MDADisplay	EQU	1	; display types
CGADisplay	EQU	2
EGAColorDisplay	EQU	3
PS2MonoDisplay	EQU	4
PS2ColorDisplay	EQU	5

TRUE	EQU	1
FALSE	EQU	0

;อออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
;
; Program
;
;อออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

Results	VIDstruct <>	;results go here!

EGADisplays	DB	CGADisplay	; 0000b, 0001b	(EGA switch values)
	DB	EGAColorDisplay	; 0010b, 0011b
	DB	MDADisplay	; 0100b, 0101b
	DB	CGADisplay	; 0110b, 0111b
	DB	EGAColorDisplay	; 1000b, 1001b
	DB	MDADisplay	; 1010b, 1011b

DCCtable	DB	0,0	; translate table for INT 10h func 1Ah
	DB	MDA,MDADisplay
	DB	CGA,CGADisplay
	DB	0,0
	DB	EGA,EGAColorDisplay
	DB	EGA,MDADisplay
	DB	0,0
	DB	VGA,PS2MonoDisplay
	DB	VGA,PS2ColorDisplay
	DB	0,0
	DB	MCGA,EGAColorDisplay
	DB	MCGA,PS2MonoDisplay
	DB	MCGA,PS2ColorDisplay

TestSequence	DB	TRUE	; this list of flags and addresses
	DW	FindPS2	;  determines the order in which this
			;  program looks for the various
EGAflag	DB	?	;  subsystems
	DW	FindEGA

CGAflag	DB	?
	DW	FindCGA

Monoflag	DB	?
	DW	FindMono

NumberOfTests	EQU	($-TestSequence)/3


	PUBLIC	VideoID
VideoID	PROC	near

	push	bp	; preserve caller registers
	mov	bp,sp
	push	ds
	push	si
	push	di

	push	cs
	pop	ds
	ASSUME	DS:@Code

; initialize the data structure that will contain the results

	lea	di,Results	; DS:DI -> start of data structure

	mov	Device0,0	; zero these variables
	mov	Device1,0

; look for the various subsystems using the subroutines whose addresses are
; tabulated in TestSequence; each subroutine sets flags in TestSequence
; to indicate whether subsequent subroutines need to be called

	mov	byte ptr CGAflag,TRUE
	mov	byte ptr EGAflag,TRUE
	mov	byte ptr Monoflag,TRUE

	mov	cx,NumberOfTests
	mov	si,offset TestSequence

@@L01:	lodsb		; AL := flag
	test	al,al
	lodsw		; AX := subroutine address
	jz	@@L02	; skip subroutine if flag is false

	push	si
	push	cx
	call	ax	; call subroutine to detect subsystem
	pop	cx
	pop	si

@@L02:	loop	@@L01

; determine which subsystem is active

	call	FindActive

	mov	al,Results.Video0Type
	mov	ah,0	; was:  Results.Display0Type

	pop	di	; restore caller registers and return
	pop	si
	pop	ds
	mov	sp,bp
	pop	bp
	ret

VideoID	ENDP


;
; FindPS2
;
; This subroutine uses INT 10H function 1Ah to determine the video BIOS
; Display Combination Code (DCC) for each video subsystem present.
;

FindPS2	PROC	near

	mov	ax,1A00h
	int	10h	; call video BIOS for info

	cmp	al,1Ah
	jne	@@L13	; exit if function not supported (i.e.,
			;  no MCGA or VGA in system)

; convert BIOS DCCs into specific subsystems & displays

	mov	cx,bx
	xor	bh,bh	; BX := DCC for active subsystem

	or	ch,ch
	jz	@@L11	; jump if only one subsystem present

	mov	bl,ch	; BX := inactive DCC
	add	bx,bx
	mov	ax,[bx+offset DCCtable]

	mov	Device1,ax

	mov	bl,cl
	xor	bh,bh	; BX := active DCC

@@L11:	add	bx,bx
	mov	ax,[bx+offset DCCtable]

	mov	Device0,ax

; reset flags for subsystems that have been ruled out

	mov	byte ptr CGAflag,FALSE
	mov	byte ptr EGAflag,FALSE
	mov	byte ptr Monoflag,FALSE

	lea	bx,Video0Type[di]  ; if the BIOS reported an MDA ...
	cmp	byte ptr [bx],MDA
	je	@@L12

	lea	bx,Video1Type[di]
	cmp	byte ptr [bx],MDA
	jne	@@L13

@@L12:	mov	word ptr [bx],0    ; ... Hercules can't be ruled out
	mov	byte ptr Monoflag,TRUE

@@L13:	ret

FindPS2	ENDP


;
; FindEGA
;
; Look for an EGA.  This is done by making a call to an EGA BIOS function
;  which doesn't exist in the default (MDA, CGA) BIOS.

FindEGA	PROC	near	; Caller:	AH = flags
			; Returns:	AH = flags
			;		Video0Type and
			;		 Display0Type updated

	mov	bl,10h	; BL := 10h (return EGA info)
	mov	ah,12h	; AH := INT 10H function number
	int	10h	; call EGA BIOS for info
			; if EGA BIOS is present,
			;  BL <> 10H
			;  CL = switch setting
	cmp	bl,10h
	je	@@L22	; jump if EGA BIOS not present

	mov	al,cl
	shr	al,1	; AL := switches/2
	mov	bx,offset EGADisplays
	xlat		; determine display type from switches
	mov	ah,al	; AH := display type
	mov	al,EGA	; AL := subystem type
	call	FoundDevice

	cmp	ah,MDADisplay
	je	@@L21	; jump if EGA has a monochrome display

	mov	CGAflag,FALSE	; no CGA if EGA has color display
	jmp	short @@L22

@@L21:	mov	Monoflag,FALSE	; EGA has a mono display, so MDA and
			;  Hercules are ruled out
@@L22:	ret

FindEGA	ENDP

;
; FindCGA
;
; This is done by looking for the CGA's 6845 CRTC at I/O port 3D4H.
;
FindCGA	PROC	near	; Returns:	VIDstruct updated

	mov	dx,3D4h	; DX := CRTC address port
	call	Find6845
	jc	@@L31	; jump if not present

	mov	al,CGA
	mov	ah,CGADisplay
	call	FoundDevice

@@L31:	ret

FindCGA	ENDP

;
; FindMono
;
; This is done by looking for the MDA's 6845 CRTC at I/O port 3B4H.  If
; a 6845 is found, the subroutine distinguishes between an MDA
; and a Hercules adapter by monitoring bit 7 of the CRT Status byte.
; This bit changes on Hercules adapters but does not change on an MDA.
;
; The various Hercules adapters are identified by bits 4 through 6 of
; the CRT Status value:
;
; 000b = HGC
; 001b = HGC+
; 101b = InColor card
;

FindMono	PROC	near	; Returns:	VIDstruct updated

	mov	dx,3B4h	; DX := CRTC address port
	call	Find6845
	jc	@@L44	; jump if not present

	mov	dl,0BAh	; DX := 3BAh (status port)
	in	al,dx
	and	al,80h
	mov	ah,al	; AH := bit 7 (vertical sync on HGC)

	mov	cx,8000h	; do this 32768 times
@@L41:	in	al,dx
	and	al,80h	; isolate bit 7
	cmp	ah,al
	loope	@@L41	; wait for bit 7 to change
	jne	@@L42	; if bit 7 changed, it's a Hercules

	mov	al,MDA	; if bit 7 didn't change, it's an MDA
	mov	ah,MDADisplay
	call	FoundDevice
	jmp	short @@L44

@@L42:	in	al,dx
	mov	dl,al	; DL := value from status port
	and	dl,01110000b	; mask bits 4 thru 6

	mov	ah,MDADisplay	; assume it's a monochrome display

	mov	al,HGCPlus	; look for an HGC+
	cmp	dl,00010000b
	je	@@L43	; jump if it's an HGC+

	mov	al,HGC	; look for an InColor card or HGC
	cmp	dl,01010000b
	jne	@@L43	; jump if it's not an InColor card

	mov	al,InColor	; it's an InColor card
	mov	ah,EGAColorDisplay

@@L43:	call	FoundDevice

@@L44:	ret

FindMono	ENDP

;
; Find6845
;
; This routine detects the presence of the CRTC on a MDA, CGA or HGC.
; The technique is to write and read register 0Fh of the chip (cursor
; low).  If the same value is read as written, assume the chip is
; present at the specified port addr.
;

Find6845	PROC	near	; Caller:  DX = port addr
			; Returns: cf set if not present
	mov	al,0Fh
	out	dx,al	; select 6845 reg 0Fh (Cursor Low)
	inc	dx
	in	al,dx	; AL := current Cursor Low value
	mov	ah,al	; preserve in AH
	mov	al,66h	; AL := arbitrary value
	out	dx,al	; try to write to 6845

	mov	cx,100h
@@L51:	loop	@@L51	; wait for 6845 to respond

	in	al,dx
	xchg	ah,al	; AH := returned value
			; AL := original value
	out	dx,al	; restore original value

	cmp	ah,66h	; test whether 6845 responded
	je	@@L52	; jump if it did (cf is reset)

	stc		; set carry flag if no 6845 present

@@L52:	ret

Find6845	ENDP


;
; FindActive
;
; This subroutine stores the currently active device as Device0.  The
; current video mode determines which subsystem is active.
;

FindActive	PROC	near

	cmp	word ptr Device1,0
	je	@@L63	; exit if only one subsystem

	cmp	Video0Type[di],4	; exit if MCGA or VGA present
	jge	@@L63	;  (INT 10H function 1AH
	cmp	Video1Type[di],4	;  already did the work)
	jge	@@L63

	mov	ah,0Fh
	int	10h	; AL := current BIOS video mode

	and	al,7
	cmp	al,7	; jump if monochrome
	je	@@L61	;  (mode 7 or 0Fh)

	cmp	Display0Type[di],MDADisplay
	jne	@@L63	; exit if Display0 is color
	jmp	short @@L62

@@L61:	cmp	Display0Type[di],MDADisplay
	je	@@L63	; exit if Display0 is monochrome

@@L62:	mov	ax,Device0	; make Device0 currently active
	xchg	ax,Device1
	mov	Device0,ax

@@L63:	ret

FindActive	ENDP


;
; FoundDevice
;
; This routine updates the list of subsystems.
;

FoundDevice	PROC	near	; Caller:    AH = display #
			;	     AL = subsystem #
			; Destroys:  BX
	lea	bx,Video0Type[di]
	cmp	byte ptr [bx],0
	je	@@L71	; jump if 1st subsystem

	lea	bx,Video1Type[di]	; must be 2nd subsystem

@@L71:	mov	[bx],ax	; update list entry
	ret

FoundDevice	ENDP

	END
