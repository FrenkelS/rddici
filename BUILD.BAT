del rddici.exe

tcc -IX:\INCLUDE -LX:\LIB -c dave2.c
tcc -IX:\INCLUDE -LX:\LIB -c pcrlib_c.c
tcc -IX:\INCLUDE -LX:\LIB -c cpanel.c
tcc -IX:\INCLUDE -LX:\LIB -c rle.c

tasm /MX /ZI /O dave2asm.asm
tasm /MX /ZI /O pcrlib_a.asm
tasm /MX /ZI /O rleasm.asm

tlink /x /c X:\LIB\c0s.obj dave2.obj dave2asm.obj pcrlib_c.obj cpanel.obj pcrlib_a.obj rle.obj rleasm.obj, rddici,, X:\LIB\cs.lib

lzexe rddici.exe
unlzexe rddici.exe

del *.obj
del rddici.old
del rddici.olz
