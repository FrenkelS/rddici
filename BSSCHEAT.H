#ifndef BSSCHEAT
#define BSSCHEAT

/*
=============================================================================
BSS LAYOUT RECREATION CHEAT

The order of the uninitialized variables stored in the so-called BSS segment
depends on the names of the variables in Turbo C++ 1.00. To force all these
variables to appear in the correct order, they had to be renamed for the
compiler.

Make sure these names match the ones used in BSSCHEAT.EQU!
=============================================================================
*/

#define originxmin     _o0me
#define originymin     _4KNO
#define EGApage        __jo4
#define mapwidthextra  _X8PZ
#define unused_buffer  _mw1E
#define p_right        _wrr0
#define p_bottom       _leSi
#define px             _ckt_
#define py             _Qw5Z
#define basegravity    _yU68
#define basejumpmove   _kLwY
#define p_left         _DsXc
#define p_top          _Qh9W
#define leveldone      _XCZF
#define ctrl           _dl_p
#define mapbuffer      _8dax
#define coverlist      _ymBy
#define hit            _qCCx
#define oldtiles       _e5D0
#define exitdemo       _y_E3
#define extrahbls      _Q7FD
#define numobj         _uSGs
#define newob          _LVhK
#define tmp_spritenum  _8miL
#define obj            _iOJH
#define oldtiles2      _LakO
#define jumpspeed      _eFLi
#define spritecost     _X3mz
#define faceleft       _TEnH
#define spritelist     _x_oS
#define mapbwide       _EKPx
#define mapwwide       _T7qU
#define basewalkspeed  _BW2e
#define basejumpspeed  _Jt31
#define davesleft      _UrTR
#define mapplane       _fV5x
#define VGAPAL         _Xtvr
#define jumpheld       _np7O
#define lastctrl       _P_xp
#define _dummy_        _1SYo
#define coverlistindex _eBZU
#define tmp_baseshape  _eX_v
#define mindrawcalls   _U3aV
#define playernum      _5xbg
#define spritesshown   _noCz
#define lastextra      _F_Di
#define gamestate      _jzE7
#define jumping        _q6fd
#define spritelistptr  _q_gE
#define levelheader    _Unh_
#define refreshcount   _uoiD
#define originx        _jEJj
#define originy        _bUKD
#define firstsprite    _mQll
#define objlist        _1YmS
#define originxmax     _I_nL
#define originymax     _NKO_

/*
=============================================================================
The "verified" variable names are:

	originxmin
	originymin
	EGApage
	mapwidthextra
	coverlist
	oldtiles
	exitdemo
	oldtiles2
	spritelist
	mapbwide
	mapwwide
	davesleft
	mapplane
	VGAPAL
	lastctrl
	spritesshown
	lastextra
	gamestate
	spritelistptr
	levelheader
	originx
	originy
	firstsprite
	objlist
	originxmax
	originymax

By "verified" I mean that these variables would have the correct relative
order even without renaming them and their names can be found in either the
Keen 1-3 source code or in "The Catacomb"'s source code.

The "davesleft" variable is obviously unique to DDICI, but the Keen 1-3 code
had a "keensleft" variable and the game prints the string "DAVES LEFT" before
printing the value of this variable.

=============================================================================
*/

#endif
