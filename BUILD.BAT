@echo off
echo. > BUILD.LOG
del *.obj
del rddici.exe

tcc -IX:\INCLUDE -c dave2.c >> BUILD.LOG
if ERRORLEVEL 1 goto error
tcc -IX:\INCLUDE -c -B pcrlib_c.c >> BUILD.LOG
if ERRORLEVEL 1 goto error
tcc -IX:\INCLUDE -c cpanel.c >> BUILD.LOG
if ERRORLEVEL 1 goto error

tasm /MX /ZI dave2asm.asm >> BUILD.LOG
if ERRORLEVEL 1 goto error
tasm /MX /ZI pcrlib_a.asm >> BUILD.LOG
if ERRORLEVEL 1 goto error

tlink /i /x /c X:\LIB\c0s.obj dave2.obj dave2asm.obj pcrlib_c.obj cpanel.obj pcrlib_a.obj, rddici,, X:\LIB\cs.lib >> BUILD.LOG
if ERRORLEVEL 1 goto error

lzexe rddici.exe

del *.obj
del rddici.old
echo SUCCESS!
goto end

:error
echo ERROR! Please read BUILD.LOG for further information.

:end