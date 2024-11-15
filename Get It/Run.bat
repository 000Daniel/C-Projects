@echo off
windres -o resource.o resource.rc
gcc -mwindows -lgdi32 -lcomctl32 -o GetIt resource.o GetIt.c Functions.c Elements.c
REM resource.o
"GetIt.exe"
echo.
pause