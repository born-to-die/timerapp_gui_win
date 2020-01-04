# timerapp GUI WIN (WinAPI, MFC)
![timerapp screen](https://raw.githubusercontent.com/born-to-die/timerapp_gui_win/master/screens/screen_1.png)

Version 2.0
(however, in the program version itself is not maintained)

DESCRIPTION

Program to track time spent in other programs (executable files)

ALGORITHM

During operation, it takes the active process of the system and 
starts counting the duration of its work in seconds. When changing 
the process does the same for him. When you press the "Escap" saves time

VERSIONS

In the original, each time the active process was changed, the program 
recorded the time of the past immediately to the file. In version 2, the 
program stores all values in memory, and only after pressing the key, 
the output unloads data to disk.


This can have an unpleasant effect, if the program closes unexpectedly 
(blackout, system errors), then all data will be lost. If this bothers you, 
then download the project from the old branch.

SYSTEM REQUIREMENTS

Windows

---
Версия 2.0
(однако, в самой программе версии не ведутся)

ОПИСАНИЕ

Программа для отслеживания времени, проведенного в других программах 
(исполняемые файлы)

АЛГОРИТМ

Во время работы берет активный процесс системы и начинает считать 
продолжительность его работы в секундах. При смене процесса тоже 
самое делает для него. При нажатии клавиши "Escape" сохраняет время 
на диск в формате название_процесса.ехе.тхт, в котором указано 
общее время работы в формате "часы минуты секунды". И так для каждого процесса. 

ВЕРСИИ

В оригинале, каждый раз когда изменялся активный процесс, программа записывала время 
работы прошлой сразу в файл. В версии 2 программа хранит все значения в памяти, и только 
после нажатия клавиши выход выгружает данные на диск.

Это может иметь неприятный эффект, если программа неожиданно 
закроется (отключение света, ошибки системы), то все данные будут потеряны. 

Если это вас беспокоит, то скачайте проект из старого коммита.

СИСТ. ТРЕБ. 

Windows
