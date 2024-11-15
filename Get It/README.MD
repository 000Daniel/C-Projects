## Introduction
Get It is a simple to-do list program written entirely in the C program language. <br>
It has the neat feature of working like a pop-up, where it is displayed above other programs. <br>
This helps the users keep track of their tasks while working with a different program. <br>
<br>
This project was an exercise to learn more about the Win32 API. <br>
Unlike my previous [WinAPI exercise project](https://github.com/000Daniel/C-Projects/tree/main/WinAPI_exercise), this software is complete and ready for everyday use! <br>
<br>
### Software Architecture
`GetIt.C` begins the program and has the main procedure function. <br>
From there it calls `Elements.C` to create the titlebar's buttons, <br>
and it creates the 'hContainer' window that holds the tasks in the todo list. <br>
`Elements.C` referes a lot to `Functions.C` to handle the program's logic. <br>
<br>
### Miscellaneous Info
[The software's logo](https://icon-icons.com/icon/files-folders-text-lines-list-archive-document-doc-file/258923)
<br>
<br>

This software was programmed in Visual Studio Community 2022 (gcc 14.2.0).
>Note: This software is complete and will not receive further updates.
<br />

```diff
- created by https://github.com/000Daniel
```
Began development on: 25.10.2024 <br />
Publish date: 15.11.2024 <br />
