del rddici.exe

tcc -a -f- -v -G -O -Z -vi- -weas -IX:\INCLUDE -LX:\LIB -P-.C -c dave2.c
tcc -a -f- -v -G -O -Z -vi- -weas -IX:\INCLUDE -LX:\LIB -P-.C -c pcrlib_c.c
tcc -a -f- -v -G -O -Z -vi- -weas -IX:\INCLUDE -LX:\LIB -P-.C -c cpanel.c
tcc -a -f- -v -G -O -Z -vi- -weas -IX:\INCLUDE -LX:\LIB -P-.C -c delme.c

tasm /MX /ZI /O dave2asm.asm
tasm /MX /ZI /O pcrlib_a.asm
tasm /MX /ZI /O rleasm.asm

tlink /x /c X:\LIB\c0s.obj dave2.obj dave2asm.obj pcrlib_c.obj cpanel.obj pcrlib_a.obj rleasm.obj delme.obj, rddici,, X:\LIB\cs.lib

lzexe rddici.exe
unlzexe rddici.exe

del *.obj
del rddici.old
del rddici.olz
