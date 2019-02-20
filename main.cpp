/*
	Timerapp GUI (WinAPI/MFC) x86
	Code::Blocks 17.12, MinGW, Windows 7 Ultimate x86

	---

	The meter spent in the program.

    Life is so around the pc that it would be time
    This is to turn into statistics.
    Change, I'm waiting for change.

    ---

	Счетчик проведенного времени в программе.

	Жизнь настолько вокруг компа, что пора бы
	это превратить в статистику.
	Перемен, я жду перемен.

	---

	17.02.2019
	Alexandr Baklankin (intbad)
*/

#include <Windows.h>
#include <fstream>
#include <tlhelp32.h>
#include <conio.h>
#include <ctime>
#include "resource.h"

#define LOCALE_WORDS_COUNT 13

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE          hSnap;
PROCESSENTRY32  proc;
DWORD           PID;

time_t          rawtime;
struct tm       *time_start;

std::ifstream   fin;
std::ofstream   fout;

std::string     *arr_names;
std::string     *arr_names_temp;
std::string     temp;
std::string     str_old_exe;

std::string     str_locale_cur[LOCALE_WORDS_COUNT] = {
        "PAUSE",
        "FORCESAVE",
        "S",
        "It looks like this is the first launch.\n\n \
It is recommended to run for the first time \
with administrator rights in the future \
it is possible without them, if everything works.",
        "First time?",
        "Session saved",
        "TIMERAPP\n\n\
The program for keeping statistics of time spent in different programs.\n\n \
In the folder and with the program there is a folder \
/ apps / - contains text files with the names of programs that \
contain time spent in it \n \
as well as the file \
\"stats.txt\" - which stores the recordings of the sessions of the program \
running at the time of the program. \n \n \
During inactivity it is recommended to use a pause. \n\n\
 The program was created by Alexander (intbad) Baklankin \
(Collapsar Team):\n \
- github.com/born-to-die\n \
- twitter.com/blinchikofwar\n\n \
Donate for food: \n \
Z424887729275 - WebMoney USD \n \
R178567199856 - WebMoney RUB \n \
https://money.yandex.ru/to/410015048142149 - Yandex Money (RUB) \
        ",
        "About",
        "Session",
        "If you want the program to run with Windows, then click \"Yes\". \
If you want to cancel or remove, then \"No\".",
        "Autoload",
        "No access.\nFor autorun, you need administrator rights.",
        "Error"
    };

const
    std::string str_locale_rus[LOCALE_WORDS_COUNT] = {
        "ПАУЗА",
        "СОХРАНИТЬ",
        "С",
        "Похоже, это первый запуск.\n\n\
Рекомендуется в первый раз запустить \
с правами администратора, в дальнейшем \
можно без них, если всё работает.",
        "Первый раз?",
        "Сессия сохранена",
        "TIMERAPP\n\n\
Программа для ведения статистики времени проведенного в разных программах.\n\n\
В папке и с программой есть папка \
/apps/ - содержит текстовые файлы с названиями программ, которые \
содержат проведенное время в ней \n\
а также файл\
\"stats.txt\" - в котором хранятся записи сессий работы программы \
с запущенными в момент работы программы.\n\n\
Во время неактивности рекомендуется пользоваться паузой.\n\n \
Программу создал Александр (intbad) Бакланкин (Collapsar Team):\n \
- github.com/born-to-die\n \
- twitter.com/blinchikofwar\n\n \
Пожертвовать на еду:\n \
Z424887729275 - WebMoney USD\n \
R178567199856 - WebMoney RUB\n \
https://money.yandex.ru/to/410015048142149 - Yandex Money (RUB) \
        ",
        "О программе",
        "Сессия",
        "Если вы хотите, чтобы программа запускалась вместе с Windows, \
то нажмите \"Да\". Если хотите отменить или убрать, то \"Нет\".",
        "Автозагрузка",
        "Нет доступа.\nДля автозапуска нужны права администратора",
        "Ошибка"
    };


const int uTaskbarCreatedMsg = RegisterWindowMessageA("TaskbarCreated");
unsigned int    *arr_seconds;
unsigned int    *arr_seconds_temp;
unsigned int    int_seconds = 0;

unsigned short  count_execs = 0;
unsigned short  past_active_in_arr = 0;

char            c_title[16] = {0};

bool            b_first_process = true;
bool            run = true;
void saveOnHDD() {

    unsigned short hours;
    unsigned short minutes;
    unsigned short seconds;

    fout.open("history.txt", std::ios::app);

    fout << time_start->tm_year + 1900 << "/"
        << time_start->tm_mon + 1 << "/"
        << time_start->tm_mday << " "
        << time_start->tm_hour << ":"
        << time_start->tm_min << ":"
        << time_start->tm_sec << " - ";

    time_t rawtime2;
    struct tm * time_end;
    time(&rawtime2);
    time_end = localtime(&rawtime2);

    fout << time_end->tm_year + 1900 << "/"
        << time_end->tm_mon + 1 << "/"
        << time_end->tm_mday << " "
        << time_end->tm_hour << ":"
        << time_end->tm_min << ":"
        << time_end->tm_sec << "\n\n";

        localtime(&rawtime);

    for(short i = 0; i < count_execs; i++) {

        hours = arr_seconds[i] / 3600;
        minutes = (arr_seconds[i] - hours * 3600) / 60;
        seconds = arr_seconds[i] - (hours * 3600 + minutes * 60);

        fout << "\t" << hours << ":" << minutes << ":"
            << seconds << "\t" << arr_names[i] << "\n";

    }

    fout << "\n";

    fout.close();

    // UPLOAD APPS

    for(short i = 0; i < count_execs; i++) {

        fin.open(".\\apps\\" + arr_names[i] + ".txt");

        if(fin) {
            fin >> hours >> minutes >> seconds;
            int_seconds = hours * 3600 + minutes * 60 + seconds;
        } else {
            int_seconds = 0;
        }

        fin.close();

        fout.open(".\\apps\\" + arr_names[i] + ".txt");

        arr_seconds[i] += int_seconds;

        hours = arr_seconds[i] / 3600;
        minutes = (arr_seconds[i] - hours * 3600) / 60;
        seconds = arr_seconds[i] - (hours * 3600 + minutes * 60);

        fout << hours << " " << minutes << " " << seconds;

        fout.close();
    }
}

void HideTrayIcon(HWND hWnd)
{
    NOTIFYICONDATA nid;
    memset(&nid, 0, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = 1;
    Shell_NotifyIcon(NIM_DELETE, &nid);
}
void CreateSystemTrayIcon(HWND hwnd, HANDLE hIcon) {
    NOTIFYICONDATA nid;
    memset(&nid, 0, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 200;
    nid.hIcon = (HICON)hIcon;
    lstrcpy (nid.szTip, "Timerapp");
    Shell_NotifyIcon(NIM_ADD, &nid);
    ShowWindow(hwnd, SW_HIDE);
}

int  WINAPI  WinMain(HINSTANCE  hInstance,
  HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    time ( &rawtime );
    time_start = localtime ( &rawtime );

    arr_names           = new std::string[count_execs];
    arr_names_temp      = new std::string[count_execs];

    arr_seconds         = new unsigned int[count_execs];
    arr_seconds_temp    = new unsigned int[count_execs];

    if(GetSystemDefaultLangID() == 1049)
        for(short i = 0; i < LOCALE_WORDS_COUNT; i++)
            str_locale_cur[i] = str_locale_rus[i];

    HWND hwnd;
    MSG msg;
    WNDCLASS w;
    memset(&w, 0, sizeof(WNDCLASS));
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.lpfnWndProc = WndProc;
    w.hInstance = hInstance;
    w.hbrBackground = CreateSolidBrush(0x3A393A);
    w.lpszClassName = "Timerapp_wnd";
    RegisterClass(&w);

    hwnd = CreateWindow("Timerapp_wnd", "Timerapp",
        WS_BORDER | WS_SYSMENU,
        210, 46, 200, 72,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    CreateDirectory(".\\apps", NULL);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LONG WINAPI WndProc(HWND hwnd, UINT Message,
  WPARAM wparam, LPARAM lparam) {

    HDC hdc;
    HINSTANCE hInst;
    PAINTSTRUCT ps;

    static HWND hBtnForce;
    static HWND hBtnPause;
    static HWND hBtnAbout;
    static HWND hBtnAuto;
    static HWND hBtnSession;
    static HWND hStatStatus;
    static HBRUSH hBrush = CreateSolidBrush(0x3A393A);

    static HFONT hFont = CreateFont(14, 0, 0, 0,
        FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Console");

    static HANDLE hIcon = LoadImage(hInst, "timerapp.ico", IMAGE_ICON, 32, 32,
        LR_LOADFROMFILE);

    static UINT s_uTaskbarRestart;

    if (Message == uTaskbarCreatedMsg) {
                // Destroy any existing icon and recreate it
                CreateSystemTrayIcon(hwnd, hIcon);
                return 0;
            }

    switch (Message) {

        case WM_CREATE: {

            s_uTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));

            hInst = ((LPCREATESTRUCT)lparam)->hInstance;

            SendMessage(hwnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)hIcon);

            HFONT hFont=CreateFont(15,5,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
                             DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                             VARIABLE_PITCH,"Console");

            hBtnForce = CreateWindow("button", str_locale_cur[1].c_str(),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                3, 3, 78, 18, hwnd, 0, hInst, NULL);
            ShowWindow(hBtnForce, SW_SHOWNORMAL);

            hBtnPause = CreateWindow("button", str_locale_cur[0].c_str(),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                83, 3, 40, 18, hwnd, 0, hInst, NULL);
            ShowWindow(hBtnPause, SW_SHOWNORMAL);

            hBtnSession = CreateWindow("button", str_locale_cur[2].c_str(),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                125, 3, 20, 18, hwnd, 0, hInst, NULL);
            ShowWindow(hBtnAbout, SW_SHOWNORMAL);

            hBtnAuto = CreateWindow("button", "A",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                147, 3, 20, 18, hwnd, 0, hInst, NULL);
            ShowWindow(hBtnAbout, SW_SHOWNORMAL);

            hBtnAbout = CreateWindow("button", "?",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                169, 3, 20, 18, hwnd, 0, hInst, NULL);
            ShowWindow(hBtnAbout, SW_SHOWNORMAL);

            hStatStatus = CreateWindow("static", "OK!",
                WS_CHILD | WS_VISIBLE,
                3, 25, 265, 20, hwnd, 0, hInst, NULL);
            ShowWindow(hStatStatus, SW_SHOWNORMAL);

            std::ifstream file("history.txt");
            if (!file.is_open())  {
                MessageBox(hwnd,
                           str_locale_cur[3].c_str(),
                           str_locale_cur[4].c_str(),
                             MB_OK);
            }

            file.close();

            SetTimer(hwnd, 1, 1000, NULL);

            break;

        }
        case WM_LBUTTONDOWN:
        {
            CreateSystemTrayIcon(hwnd, hIcon);
        }
        break;
        case WM_USER + 200:
            if(lparam == WM_LBUTTONDBLCLK)
            {
                HideTrayIcon(hwnd);
                ShowWindow(hwnd, SW_NORMAL);
            }
            break;
        case WM_COMMAND: {
            if (lparam == (LPARAM)hBtnForce) {
                saveOnHDD();
                SetWindowText(hStatStatus, str_locale_cur[5].c_str());
            } else if (lparam == (LPARAM)hBtnPause) {
                if(run) {
                    run = false;
                    KillTimer(hwnd, 1);
                    SetWindowText(hStatStatus, str_locale_cur[0].c_str());
                }else {
                    run = true;
                    SetTimer(hwnd, 1, 1000, NULL);
                    SetWindowText(hStatStatus, "OK!");
                }
            } else if (lparam == (LPARAM)hBtnAbout) {
                MessageBox(hwnd, str_locale_cur[6].c_str(),
                           str_locale_cur[7].c_str(), MB_OK);
            } else if (lparam == (LPARAM)hBtnAuto) {

                HKEY hKey;
                char sz_path[0x100];

                GetModuleFileName(NULL, sz_path, sizeof(sz_path));

                RegCreateKeyEx(
                    HKEY_LOCAL_MACHINE,
                    "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                    NULL, "", REG_OPTION_NON_VOLATILE, KEY_SET_VALUE,
                    NULL, &hKey, NULL
                );

                DWORD result;

                if (hKey) {

                    if(MessageBox(hwnd, str_locale_cur[9].c_str(),
                        str_locale_cur[10].c_str(),
                        MB_YESNO) == IDYES) {


                        result = RegSetValueEx(hKey, "Timerapp",
                            NULL, REG_SZ, (LPBYTE)sz_path,
                            strlen(sz_path));

                        if(result != ERROR_SUCCESS)
                            MessageBox(hwnd, str_locale_cur[11].c_str(),
                                       str_locale_cur[12].c_str(),
                                       MB_OK);
                    }
                    else {

                        result = RegDeleteValue(hKey, "Timerapp");

                        if(result != ERROR_SUCCESS)
                            MessageBox(hwnd, str_locale_cur[11].c_str(),
                                       str_locale_cur[12].c_str(),
                                       MB_OK);
                    }

                    RegCloseKey(hKey);
                }

            } else if (lparam == (LPARAM)hBtnSession) {

                std::string str_session;
                unsigned short hours;
                unsigned short minutes;
                unsigned short seconds;

                for(short i = 0; i < count_execs; i++) {

                    hours = arr_seconds[i] / 3600;
                    minutes = (arr_seconds[i] - hours * 3600) / 60;
                    seconds = arr_seconds[i] - (hours * 3600 + minutes * 60);

                    str_session += std::to_string(hours)
                        + ":" + std::to_string(minutes)
                        + ":" + std::to_string(seconds)
                        + "\t" + arr_names[i].c_str() + "\n";

                }

                MessageBox(hwnd, str_session.c_str(), str_locale_cur[8].c_str(), MB_OK);
            }
            break;
        }
        case WM_PAINT:

            hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);

            break;
        case WM_CTLCOLORSTATIC: {

            HDC hdcStatic = (HDC) wparam;
            SetTextColor(hdcStatic, 0xFFFFFF);
            SetBkColor(hdcStatic, 0x3A393A);
            SelectObject(hdcStatic, hFont);

            return (INT_PTR)hBrush;
        }

        break;
        case WM_DRAWITEM: {

            LPDRAWITEMSTRUCT Item =(LPDRAWITEMSTRUCT)lparam;

            SetBkMode(Item->hDC, TRANSPARENT);
            SetTextColor(Item->hDC, 0xFFFFFF);
            SelectObject(Item->hDC, hFont);

            if(Item->itemState & ODS_SELECTED)
                FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
            else
                FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(GRAY_BRUSH));

            int len = GetWindowTextLength(Item->hwndItem);
            char* buf = new char[len + 1];
            GetWindowTextA(Item->hwndItem, buf, len + 1);
            DrawTextA(Item->hDC, buf, len, &Item->rcItem, DT_CENTER);

            return true;
        }
        case WM_DESTROY:
            saveOnHDD();
            PostQuitMessage(0);
            break;
        case WM_TIMER: {

                hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                HWND handle = GetForegroundWindow();

                GetWindowText(handle, c_title, 16);
                GetWindowThreadProcessId(handle, (LPDWORD) &PID);

                proc = {0};
                proc.dwSize = sizeof(PROCESSENTRY32);

                do{
                    if(proc.th32ProcessID == PID) break;
                }while (Process32Next(hSnap, &proc));

                if(!b_first_process) {

                    temp = proc.szExeFile;

                    if(temp != str_old_exe) {

                        SetWindowText(hStatStatus, proc.szExeFile);

                        bool not_found = true;

                        for(short i = 0; i < count_execs; i++) {

                            if(arr_names[i] == str_old_exe) {
                                not_found = false;
                                past_active_in_arr = i;
                            }
                        }

                        if(not_found) {

                            for(short i = 0; i < count_execs; i++) {
                                arr_names_temp[i] = arr_names[i];
                                arr_seconds_temp[i] = arr_seconds[i];
                            }

                            count_execs++;

                            arr_names = new std::string[count_execs];
                            arr_seconds = new unsigned int[count_execs];

                            for(short i = 0; i < count_execs - 1; i++) {
                                arr_names[i] = arr_names_temp[i];
                                arr_seconds[i] = arr_seconds_temp[i];
                            }

                            arr_names_temp = new std::string[count_execs];
                            arr_seconds_temp = new unsigned int[count_execs];

                            past_active_in_arr = count_execs - 1;

                            arr_names[past_active_in_arr] = str_old_exe;
                            arr_seconds[past_active_in_arr] = int_seconds;

                        }
                        else {
                            arr_seconds[past_active_in_arr] += int_seconds;
                        }

                        int_seconds = 0;
                    }
                }

                int_seconds++;

                str_old_exe = proc.szExeFile;
                b_first_process = false;

            break;
        }
        default:
            return DefWindowProc(hwnd, Message, wparam, lparam);
    }
    return 0;
}
