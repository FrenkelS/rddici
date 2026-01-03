#include "PCRLIB.H"

#define UNUSED(x) (x = x)

void huge *charptr;		// 8*8 tileset
void huge *tileptr;		// 16*16 tileset
void huge *picptr;		// any size picture set
void huge *spriteptr;		// any size masked and hit rect sprites
void huge *egaspriteptr[4];	// spriteptr for each ega plane's data

unsigned crtcaddr;


void repaintscreen(void)
{
}

void loadgrfiles(void)
{
}

void EGAmove(void)
{
}

void print(const char *str)
{
	UNUSED(str);
}

int get(void)
{
	return 0;
}

void expwin(int width, int height)
{
	UNUSED(width);
	UNUSED(height);
}

void erasewindow(void)
{
}

void drawwindow(int xl, int yl, int xh, int yh)
{
	UNUSED(xl);
	UNUSED(yl);
	UNUSED(xh);
	UNUSED(yh);
}

void setscreenmode(grtype mode)
{
	UNUSED(mode);
}

void huge *bloadin(char *filename)
{
	UNUSED(filename);
	return NULL;
}

void clearkeys(void)
{
}

ControlStruct ControlJoystick(int joynum)
{
	ControlStruct action;
	UNUSED(joynum);
	return action;
}

void ReadJoystick(int joynum, int *xcount, int *ycount)
{
	UNUSED(joynum);
	UNUSED(xcount);
	UNUSED(ycount);
}
