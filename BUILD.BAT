del *.obj
del rddici.exe

tcc -IX:\INCLUDE -LX:\LIB -c dave2.c
tcc -IX:\INCLUDE -LX:\LIB -c pcrlib_c.c
tcc -IX:\INCLUDE -LX:\LIB -c cpanel.c
@rem tcc -IX:\INCLUDE -LX:\LIB -c rle.c
tcc -IX:\INCLUDE -LX:\LIB -c stub.c

@rem tasm /MX /ZI /O dave2asm.asm
tasm /MX /ZI /O pcrlib_a.asm
@rem tasm /MX /ZI /O rleasm.asm

@rem tlink /x /c X:\LIB\c0s.obj dave2.obj dave2asm.obj pcrlib_c.obj cpanel.obj pcrlib_a.obj rle.obj rleasm.obj, rddici,, X:\LIB\cs.lib
tlink /x /c X:\LIB\c0s.obj dave2.obj pcrlib_c.obj cpanel.obj pcrlib_a.obj stub.obj, rddici,, X:\LIB\cs.lib

@rem lzexe rddici.exe
@rem unlzexe rddici.exe

del *.obj
@rem del rddici.old
@rem del rddici.olz
