/* Reconstructed Dangerous Dave in Copyright Infringement Source Code
 * Copyright (C) 2026 Frenkel Smeijers
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

//long RLEcompress (void far *source, long length, void far *dest);
void sub_0_5CC7(void)
{
asm {
db 8Bh, 46h, 04h, 36h, 0A3h, 3Eh
db 12h, 8Bh, 46h, 06h, 36h, 0A3h, 3Ah, 12h, 8Bh, 46h, 0Ch, 36h, 0A3h, 3Ch, 12h, 8Bh
db 46h, 0Eh, 36h, 0A3h, 38h, 12h, 8Bh, 46h, 08h, 36h, 0A3h, 40h, 12h, 8Bh, 46h, 0Ah
db 36h, 0A3h, 42h, 12h, 0E8h, 4Ch, 01h, 36h, 0A1h, 44h, 12h, 36h, 8Bh, 16h, 46h, 12h
}
}

//void RLEExpand (char far *source, char far *dest, long length);
void sub_0_5D02(void)
{
asm {
db 8Bh, 46h, 04h, 36h, 0A3h, 3Ch, 12h, 8Bh, 46h, 06h, 36h
db 0A3h, 38h, 12h, 8Bh, 46h, 08h, 36h, 0A3h, 3Eh, 12h, 8Bh, 46h, 0Ah, 36h, 0A3h, 3Ah
db 12h, 8Bh, 46h, 0Ch, 36h, 0A3h, 40h, 12h, 8Bh, 46h, 0Eh, 36h, 0A3h, 42h, 12h, 0E8h
db 0A4h, 00h
}
}