## Introduction
Close Tasks is a simple software that was written in the C programming language. <br>
Its purpose is to help the user close unwanted tasks that keep opening up. <br>
You can make this software start together with windows to terminate unwanted software right after boot. <br>
<br>
<br>
### How to Use
When starting the program for the first time you'll notice a `config.txt` file appear in the program's folder. <br>
Inside that file you'll see that the first line contains numbers. <br>
Their format is: `<Time Intervals> / <Repeats>`, <br>
Time intervals represent how many seconds the program will wait before trying to close the tasks again, <br>
Repeats represent how many times to repeat this process. <br>
Any line written after the first one should contain the name of the process that you want to close, `Example: Notepad.exe`. <br>
<br>
<br>
### Launch Close Tasks on Boot
Right click on `CloseTasks.exe` and click on `Create shortcut`. <br>
Press `Win + R` and type `shell:startup`, hit enter. <br>
Now copy the shortcut to this folder. <br>
<br>
<br>
### Miscellaneous Info
This software uses the taskkill command in windows to terminate tasks. <br>
<br>
<br>

Compiler used: gcc 14.2.0.
>Note: This software is complete and will not receive further updates.
<br />

```diff
- created by https://github.com/000Daniel
```
Publish date: 9.12.2024 <br />
