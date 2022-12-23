windres -J rc -O coff -i resource.rc -o resources.res
gcc main_frame.c main_frame_paint.c main_controls.c resources.res -o Win32_GUI -lgdi32 -Riched32 -Riched20 -mwindows -L ComCtl32.Lib

@echo off
if %errorlevel%==1 (goto error)
Win32_GUI.exe
exit

:error
pause
exit