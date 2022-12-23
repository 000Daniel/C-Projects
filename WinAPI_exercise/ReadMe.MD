# C WinAPI - basic text editor
    This software is an exercise to learn WinAPI (Windows.h). <br />
    This software is unfinished as of this time. <br />
    I used GCC version 12.2.0 to compile this software. <br />
    You can use this project as a framework/base to make your own projects. <br />
    <br />
##  Compiling
    Unpack all the source files exactly as they are into a project folder. <br />
    Run the `run.bat` Batch file, it uses `windres` to compile the resources file and `gcc` to compile the program. <br />
    <br />
##  Program Structure
    `main_frame.c` contains the `WinMain()` function which starts the whole program. <br />
    each `.c` file contails a header file `.h` so all C files could access eachother. <br />
    `main_controls.c` is responsible for adding all the controls into the GUI (Rich Edit, Buttons, etc'...). <br />
    `main_controls.c` also contains all functions that aren't for window creation or window painting. <br />
    `main_frame_paint.c` is responsible for drawing on the window(drawing texts, lines, custom scrollbar and more). <br />
    You'll also notice that `main_frame.c` contains `WindowProcedure()` handling which handles all the incoming WinAPI calls/messages. <br />
    <br />
    ![image](https://user-images.githubusercontent.com/90350173/209368379-b9ce18ae-8cc5-4612-83b9-d0aca25f6742.png)
    <br />
    <br />
    <br />
    <br />
This software was written in Visual Studio Community 2022.
>Note: This software project is incomplete and will probably not recieve a lot of support overtime.
<br />

```diff
- created by https://github.com/000Daniel
```
Began development on: 16.12.2022 <br />
Publish date: 23.12.2022 <br />
