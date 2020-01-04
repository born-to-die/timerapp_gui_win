# timerapp GUI WIN (WinAPI, MFC)
![timerapp screen](https://raw.githubusercontent.com/born-to-die/timerapp_gui_win/master/screens/screen_1.png)

DESCRIPTION

Program to track time spent in other programs (executable files)

ALGORITHM

During operation, it takes the active process of the system and 
starts counting the duration of its work in seconds. When changing 
the process does the same for him. When you press the "Escap" saves time

CONTROL

* SAVE - save time in applications and session
* PAUSE - stop counting
* C - information about the current session
* A - program for automatic download.
* ? - program information
* click on the background of the window - hide the program in the tray

PROBLEMS

1. When loading at startup
2. ~~When you restart the Windows shell, for example, when going to sleep and back, it disappears from the tray~~
3. When you turn off the computer, without turning off the program, it may not save the current session
4. When you turn on the computer at startup, the session may not be saved

SYSTEM REQUIREMENTS

Windows

---

ОПИСАНИЕ

Программа для отслеживания времени, проведенного в других программах 
(исполняемые файлы)

УПРАВЛЕНИЕ

* СОХРАНИТЬ - сохранить время в приложениях и сессию
* ПАУЗА - прекратить счёт
* С - информация о текущей сессии
* А - поместить программу в автозагрузку, может попросить права администратора
* ? - информация о программе
* щелкнуть по фону окна - спрятать программу в трей

ПРОБЛЕМЫ

1. При помещении в автозагрузку может при загрузки сообщать, что это первый запуск
2. ~~При перезагрузки оболочки Windows, например при уходе в сон и обратно, пропадает из трея~~
3. При выключении компьютера, без выключения программы, может не сохранить текущуюю сессию
4. При включении компьютера находясь в автозагрузке может не сохранять сессию

АЛГОРИТМ

Во время работы берет активный процесс системы и начинает считать 
продолжительность его работы в секундах. При смене процесса тоже 
самое делает для него. При нажатии клавиши "Escape" сохраняет время 
на диск в формате название_процесса.ехе.тхт, в котором указано 
общее время работы в формате "часы минуты секунды". И так для каждого процесса. 

СИСТ. ТРЕБ. 

Windows
