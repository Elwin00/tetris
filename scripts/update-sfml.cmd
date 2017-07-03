@echo off
setlocal

echo :: SFML libray update ::

:: find the latest compiled library
set sfml-root=..\..\SFML
for /d %%F in (%sfml-root%\SFML-*) do (
    set sfml-version=%%F
    :: ~nx = just filename + extension
    set sfml-version-name=%%~nxF
)
echo :: detected SFML version: %sfml-version-name% ::

:: check the destination folder
set outdir=..\src\packages\SFML

echo :: copying header files ::
:: xcopy /S with subdirs, /Y overwrite w/o asking, /I assume dirs, /Q silent
xcopy /S /Y /I /Q %sfml-version%\include %outdir%\include
echo :: copying libraries ::
xcopy /S /Y /I /Q %sfml-version%\lib %outdir%\lib
echo :: copying external libraries ::
xcopy /Y /Q %sfml-version%\extlibs\bin\x64\openal32.dll %outdir%\bin\

echo :: finished ::
