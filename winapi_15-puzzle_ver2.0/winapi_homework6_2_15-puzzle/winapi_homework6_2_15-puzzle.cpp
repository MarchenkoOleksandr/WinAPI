#include <windows.h>
#include "resource.h"
#include <commctrl.h>
#include <ctime>
#include <vector>
using namespace std;
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define SIDE 98
#define DISTANCE 2

int steps = 0, n = 0, null_cell = 0, start_game = 0, start_pause = 0;
wchar_t str[100] = { 0 };
int width[] = { 100, 100, 200, -1 };
HFONT hFont;
HMENU hMenu;
HWND hStatusbar, hProgressbar;

struct Button
{
	HWND hButton;
	POINT lefttop;
	int number;
};

vector <Button> hVec_buttons(16);
void Start_game(HWND);
void Delete_Buttons();
void CreateButton(HWND, int, int);
void SwapButtons(HWND, int);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
void MoveIfEmpty(HWND);
BOOL Check_result(HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
}

BOOL CALLBACK DialogProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hWindow, 0);
		return TRUE;
	case WM_INITDIALOG:
		hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, {TEXT("Arial")});
		hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hWindow, hMenu);

		hStatusbar = CreateWindowEx(WS_EX_CLIENTEDGE, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SBARS_SIZEGRIP,
			0, 0, 0, 0, hWindow, (HMENU)STATUSBAR, 0, 0);
		SendDlgItemMessage(hWindow, STATUSBAR, SB_SETPARTS, 4, (LPARAM)width);

		hProgressbar = CreateWindowEx(WS_EX_CLIENTEDGE, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_SMOOTH /*| PBS_VERTICAL | PBS_MARQUEE*/,
			200, 400, 200, 20, hWindow, (HMENU)ID_PROGRESSBAR, 0, 0);
		SendMessage(hProgressbar, PBM_SETRANGE, 0, MAKELONG(0, 15));
		SendMessage(GetDlgItem(hWindow, ID_PROGRESSBAR), PBM_SETSTEP, 1, 0);

		Start_game(hWindow);
		return TRUE;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			start_pause = clock();
		if (wParam == SIZE_RESTORED)
			start_game = start_game + clock() - start_pause;
		return TRUE;
	case WM_TIMER:
		wsprintf(str, TEXT("Час гри: %02i:%02i"), (clock() - start_game) / CLOCKS_PER_SEC / 60, (clock() - start_game) / CLOCKS_PER_SEC % 60);
		SendDlgItemMessage(hWindow, STATUSBAR, SB_SETTEXT, 2, (LPARAM)str);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEW_GAME:
			Delete_Buttons();
			Start_game(hWindow);		
			break;
		case ID_EXIT:
			if (MessageBox(NULL, TEXT("Гра ще не завершена!\nВи дійсно бажаєте покинути гру?"), TEXT("Вихід"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES)
			{
				DeleteObject(hFont);
				EndDialog(hWindow, 0);
			}
			break;
		case ID_RULES:
			MessageBox(NULL, TEXT("Мета гри — переміщаючи квадрати добитися впорядкування їх за номерами, зробивши при цьому якомога менше переміщень за менший проміжок часу"), TEXT("Правила"), MB_OK | MB_ICONINFORMATION);
			break;
		case ID_ABOUT:
			MessageBox(NULL, TEXT("Розробив Олександр Марченко\nАкадемія ШАГ"), TEXT("Про автора"), MB_OK | MB_ICONINFORMATION);
			break;

		}
		MoveIfEmpty((HWND)lParam);
		wsprintf(str, TEXT("Ходів: %i"), steps);
		SendDlgItemMessage(hWindow, STATUSBAR, SB_SETTEXT, 0, (LPARAM)str);
		
		if (Check_result(hWindow))
		{
			KillTimer(hWindow, IDT_TIMER);
			wsprintf(str, TEXT("Вітаємо!\nВи закінчили гру за %i ходів.\nВам знадобилося %02i хвилин(и) %02i секунд(и) "), steps, (clock() - start_game) / CLOCKS_PER_SEC / 60, (clock() - start_game) / CLOCKS_PER_SEC % 60);
			MessageBox(NULL, str, TEXT("Кінець гри"), MB_OK | MB_ICONINFORMATION);
			if (MessageBox(NULL, TEXT("Бажаєте зіграти ще раз?"), TEXT(""), MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION) == IDYES)
			{
				Delete_Buttons();
				Start_game(hWindow);
			}
			else
			{
				DeleteObject(hFont);
				EndDialog(hWindow, 0);
			}
		}
		return TRUE;
	}
	return FALSE;
}

void Start_game(HWND hWindow)
{
	start_game = clock();
	steps = 0;
	SetTimer(hWindow, IDT_TIMER, 1000, NULL);
	wsprintf(str, TEXT("Ходів: %i"), steps);
	SendDlgItemMessage(hWindow, STATUSBAR, SB_SETTEXT, 0, (LPARAM)str);
	null_cell = 15;
	srand((unsigned)time(NULL));
	n = rand() % 15 + 1;
	for (int i = 0; i < 15; i++)
	{
		CreateButton(hWindow, i, n);
		SendMessage(hVec_buttons[i].hButton, WM_SETFONT, (WPARAM)hFont, NULL);
		n = (n + 7) % 15 + 1;
	}
	hVec_buttons[15].hButton = NULL;
	hVec_buttons[15].lefttop.x = (SIDE + DISTANCE) * 3;
	hVec_buttons[15].lefttop.y = (SIDE + DISTANCE) * 3;
	hVec_buttons[15].number = 0;
}

void Delete_Buttons()
{
	for (auto i = hVec_buttons.begin(); i != hVec_buttons.end(); i++)
		DestroyWindow(i->hButton);
}

void CreateButton(HWND hWindow, int i, int n)
{
	Button temp;
	temp.lefttop.x = (SIDE + DISTANCE) * (i % 4);
	temp.lefttop.y = (SIDE + DISTANCE) * (i / 4);
	temp.number = n;
	wsprintf(str, TEXT("%i"), n);
	temp.hButton = CreateWindow(TEXT("BUTTON"), str, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		temp.lefttop.x, temp.lefttop.y, SIDE, SIDE, hWindow, NULL, (HINSTANCE)GetWindowLong(hWindow, GWL_HINSTANCE), NULL);
	hVec_buttons[i] = temp;
}

void MoveIfEmpty(HWND hwndCtl)
{
	for (int i = 0; i < 16; i++)
	{
		if (hVec_buttons[i].hButton == hwndCtl)
		{
			if (hVec_buttons[i].lefttop.x + SIDE + DISTANCE == hVec_buttons[null_cell].lefttop.x && hVec_buttons[i].lefttop.y == hVec_buttons[null_cell].lefttop.y)
			{
				SwapButtons(hwndCtl, i);
				return;
			}
			else if (hVec_buttons[i].lefttop.x - SIDE - DISTANCE == hVec_buttons[null_cell].lefttop.x && hVec_buttons[i].lefttop.y == hVec_buttons[null_cell].lefttop.y)
			{
				SwapButtons(hwndCtl, i);
				return;
			}
			else if (hVec_buttons[i].lefttop.x == hVec_buttons[null_cell].lefttop.x && hVec_buttons[i].lefttop.y + SIDE + DISTANCE == hVec_buttons[null_cell].lefttop.y)
			{
				SwapButtons(hwndCtl, i);
				return;
			}
			else if (hVec_buttons[i].lefttop.x == hVec_buttons[null_cell].lefttop.x && hVec_buttons[i].lefttop.y - SIDE - DISTANCE == hVec_buttons[null_cell].lefttop.y)
			{
				SwapButtons(hwndCtl, i);
				return;
			}
		}
	}
}

void SwapButtons(HWND hwndCtl, int i)
{
	PlaySound(TEXT("Startup.wav"),NULL, SND_ASYNC | SND_FILENAME);
	MoveWindow(hwndCtl, hVec_buttons[null_cell].lefttop.x, hVec_buttons[null_cell].lefttop.y, SIDE, SIDE, 1);
	swap(hVec_buttons[i].hButton, hVec_buttons[null_cell].hButton);
	swap(hVec_buttons[i].number, hVec_buttons[null_cell].number);
	null_cell = i;
	steps++;
}

BOOL Check_result(HWND hWindow)
{
	int test = 0;
	for (int i = 0; i < 15; i++)
	{
		if (i + 1 == hVec_buttons[i].number)
			test++;
	}
	SendMessage(GetDlgItem(hWindow, ID_PROGRESSBAR), PBM_SETPOS, test, 0);
	if (test == 15)
		return TRUE;
	else
		return FALSE;
}