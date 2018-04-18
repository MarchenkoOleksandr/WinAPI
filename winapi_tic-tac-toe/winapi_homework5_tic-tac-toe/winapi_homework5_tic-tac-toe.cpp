#include <windows.h>
#include "resource.h"
#include <ctime>
#include <vector>
using namespace std;
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define SIDE 148
#define DISTANCE 7

wchar_t str[100] = { 0 };
HANDLE hImgO, hImgX, hBackground;
bool hard_level = false, player_first = false;

struct Button
{
	HWND hButton;
	HMENU hMenuButton;
	wchar_t symbol;
};

vector <Button> hVec_buttons(9);
void Start_game(HWND);
void CreateButton(HWND, int);
void Delete_Buttons();
BOOL PlayerGame(int b);
void CompGameSimple();
void CompGameHard();
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL Check_result();

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
		hImgO = LoadImage(nullptr, L"o.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		hImgX = LoadImage(nullptr, L"x.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		hBackground = LoadImage(nullptr, L"white.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		SendDlgItemMessage(hWindow, IDC_RADIO1, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
		SendDlgItemMessage(hWindow, IDC_RADIO3, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
		Start_game(hWindow);
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) >= 20001 && LOWORD(wParam) <= 20009)
		{
			if (!PlayerGame(LOWORD(wParam)))
				return TRUE;
			if (Check_result())
				return TRUE;
			else if (hard_level)
				CompGameHard();
			else
				CompGameSimple();
			Check_result();
		}
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			Delete_Buttons();
			Start_game(hWindow);
			if (!player_first && hard_level)
				CompGameHard();
			else if (!player_first && !hard_level)
				CompGameSimple();
			break;
		}
		return TRUE;
	}
	return FALSE;
}

void Start_game(HWND hWindow)
{
	for (int i = 0; i < 9; i++)
		CreateButton(hWindow, i);

	if (SendDlgItemMessage(hWindow, IDC_RADIO1, BM_GETCHECK, 0, 0) == BST_CHECKED)
		hard_level = false;
	else
		hard_level = true;

	if (SendDlgItemMessage(hWindow, IDC_RADIO3, BM_GETCHECK, 0, 0) == BST_CHECKED)
		player_first = true;
	else
		player_first = false;
}

void CreateButton(HWND hWindow, int i)
{
	Button temp;
	temp.hMenuButton = (HMENU)(20001 + i);
	temp.hButton = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP,
		(SIDE + DISTANCE) * (i % 3), (SIDE + DISTANCE) * (i / 3), SIDE, SIDE, hWindow, temp.hMenuButton, (HINSTANCE)GetWindowLong(hWindow, GWL_HINSTANCE), NULL);
	SendMessage(temp.hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBackground);
	temp.symbol = NULL;
	hVec_buttons[i] = temp;
}

void Delete_Buttons()
{
	for (auto i = hVec_buttons.begin(); i != hVec_buttons.end(); i++)
		DestroyWindow(i->hButton);
}

BOOL PlayerGame(int b)
{
	for (int i = 0; i < 9; i++)
	{
		if (hVec_buttons[i].hMenuButton == (HMENU)b)
		{
			if (hVec_buttons[i].symbol == NULL)
			{
				SendMessage(hVec_buttons[i].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgX);
				hVec_buttons[i].symbol = L'x';
				return TRUE;
			}
			else
				return FALSE;
		}
	}
}

void CompGameSimple()
{
	srand((unsigned)time(NULL));
	int i = rand() % 9;
	if (hVec_buttons[i].symbol == NULL)
	{
		SendMessage(hVec_buttons[i].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[i].symbol = L'o';
	}
	else
	{
		do
		{
			i = (i + 1) % 9;
			if (hVec_buttons[i].symbol == NULL)
			{
				SendMessage(hVec_buttons[i].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
				hVec_buttons[i].symbol = L'o';
				return;
			}
		} while (true);
	}
}

void CompGameHard()
{
	//Старт гри
	if (hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	//останній пепеможний хід
	else if (hVec_buttons[0].symbol == L'o' && hVec_buttons[1].symbol == L'o' && hVec_buttons[2].symbol == NULL)
	{
		SendMessage(hVec_buttons[2].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[2].symbol = L'o';
	}
	else if (hVec_buttons[0].symbol == L'o' && hVec_buttons[2].symbol == L'o' && hVec_buttons[1].symbol == NULL)
	{
		SendMessage(hVec_buttons[1].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[1].symbol = L'o';
	}
	else if (hVec_buttons[1].symbol == L'o' && hVec_buttons[2].symbol == L'o' && hVec_buttons[0].symbol == NULL)
	{
		SendMessage(hVec_buttons[0].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[0].symbol = L'o';
	}

	else if (hVec_buttons[3].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[5].symbol == NULL)
	{
		SendMessage(hVec_buttons[5].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[5].symbol = L'o';
	}
	else if (hVec_buttons[3].symbol == L'o' && hVec_buttons[5].symbol == L'o' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'o' && hVec_buttons[5].symbol == L'o' && hVec_buttons[3].symbol == NULL)
	{
		SendMessage(hVec_buttons[3].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[3].symbol = L'o';
	}

	else if (hVec_buttons[6].symbol == L'o' && hVec_buttons[7].symbol == L'o' && hVec_buttons[8].symbol == NULL)
	{
		SendMessage(hVec_buttons[8].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[8].symbol = L'o';
	}
	else if (hVec_buttons[6].symbol == L'o' && hVec_buttons[8].symbol == L'o' && hVec_buttons[7].symbol == NULL)
	{
		SendMessage(hVec_buttons[7].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[7].symbol = L'o';
	}
	else if (hVec_buttons[7].symbol == L'o' && hVec_buttons[8].symbol == L'o' && hVec_buttons[6].symbol == NULL)
	{
		SendMessage(hVec_buttons[6].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[6].symbol = L'o';
	}

	else if (hVec_buttons[0].symbol == L'o' && hVec_buttons[3].symbol == L'o' && hVec_buttons[6].symbol == NULL)
	{
		SendMessage(hVec_buttons[6].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[6].symbol = L'o';
	}
	else if (hVec_buttons[0].symbol == L'o' && hVec_buttons[6].symbol == L'o' && hVec_buttons[3].symbol == NULL)
	{
		SendMessage(hVec_buttons[3].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[3].symbol = L'o';
	}
	else if (hVec_buttons[3].symbol == L'o' && hVec_buttons[6].symbol == L'o' && hVec_buttons[0].symbol == NULL)
	{
		SendMessage(hVec_buttons[0].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[0].symbol = L'o';
	}

	else if (hVec_buttons[1].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[7].symbol == NULL)
	{
		SendMessage(hVec_buttons[7].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[7].symbol = L'o';
	}
	else if (hVec_buttons[1].symbol == L'o' && hVec_buttons[7].symbol == L'o' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'o' && hVec_buttons[7].symbol == L'o' && hVec_buttons[1].symbol == NULL)
	{
		SendMessage(hVec_buttons[1].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[1].symbol = L'o';
	}

	else if (hVec_buttons[2].symbol == L'o' && hVec_buttons[5].symbol == L'o' && hVec_buttons[8].symbol == NULL)
	{
		SendMessage(hVec_buttons[8].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[8].symbol = L'o';
	}
	else if (hVec_buttons[2].symbol == L'o' && hVec_buttons[8].symbol == L'o' && hVec_buttons[5].symbol == NULL)
	{
		SendMessage(hVec_buttons[5].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[5].symbol = L'o';
	}
	else if (hVec_buttons[5].symbol == L'o' && hVec_buttons[8].symbol == L'o' && hVec_buttons[2].symbol == NULL)
	{
		SendMessage(hVec_buttons[2].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[2].symbol = L'o';
	}

	else if (hVec_buttons[0].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[8].symbol == NULL)
	{
		SendMessage(hVec_buttons[8].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[8].symbol = L'o';
	}
	else if (hVec_buttons[0].symbol == L'o' && hVec_buttons[8].symbol == L'o' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'o' && hVec_buttons[8].symbol == L'o' && hVec_buttons[0].symbol == NULL)
	{
		SendMessage(hVec_buttons[0].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[0].symbol = L'o';
	}

	else if (hVec_buttons[2].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[6].symbol == NULL)
	{
		SendMessage(hVec_buttons[6].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[6].symbol = L'o';
	}
	else if (hVec_buttons[2].symbol == L'o' && hVec_buttons[6].symbol == L'o' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'o' && hVec_buttons[6].symbol == L'o' && hVec_buttons[2].symbol == NULL)
	{
		SendMessage(hVec_buttons[2].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[2].symbol = L'o';
	}
	//запобігання перемозі суперника
	else if (hVec_buttons[0].symbol == L'x' && hVec_buttons[1].symbol == L'x' && hVec_buttons[2].symbol == NULL)
	{
		SendMessage(hVec_buttons[2].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[2].symbol = L'o';
	}
	else if (hVec_buttons[0].symbol == L'x' && hVec_buttons[2].symbol == L'x' && hVec_buttons[1].symbol == NULL)
	{
		SendMessage(hVec_buttons[1].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[1].symbol = L'o';
	}
	else if (hVec_buttons[1].symbol == L'x' && hVec_buttons[2].symbol == L'x' && hVec_buttons[0].symbol == NULL)
	{
		SendMessage(hVec_buttons[0].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[0].symbol = L'o';
	}

	else if (hVec_buttons[3].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[5].symbol == NULL)
	{
		SendMessage(hVec_buttons[5].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[5].symbol = L'o';
	}
	else if (hVec_buttons[3].symbol == L'x' && hVec_buttons[5].symbol == L'x' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'x' && hVec_buttons[5].symbol == L'x' && hVec_buttons[3].symbol == NULL)
	{
		SendMessage(hVec_buttons[3].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[3].symbol = L'o';
	}

	else if (hVec_buttons[6].symbol == L'x' && hVec_buttons[7].symbol == L'x' && hVec_buttons[8].symbol == NULL)
	{
		SendMessage(hVec_buttons[8].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[8].symbol = L'o';
	}
	else if (hVec_buttons[6].symbol == L'x' && hVec_buttons[8].symbol == L'x' && hVec_buttons[7].symbol == NULL)
	{
		SendMessage(hVec_buttons[7].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[7].symbol = L'o';
	}
	else if (hVec_buttons[7].symbol == L'x' && hVec_buttons[8].symbol == L'x' && hVec_buttons[6].symbol == NULL)
	{
		SendMessage(hVec_buttons[6].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[6].symbol = L'o';
	}

	else if (hVec_buttons[0].symbol == L'x' && hVec_buttons[3].symbol == L'x' && hVec_buttons[6].symbol == NULL)
	{
		SendMessage(hVec_buttons[6].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[6].symbol = L'o';
	}
	else if (hVec_buttons[0].symbol == L'x' && hVec_buttons[6].symbol == L'x' && hVec_buttons[3].symbol == NULL)
	{
		SendMessage(hVec_buttons[3].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[3].symbol = L'o';
	}
	else if (hVec_buttons[3].symbol == L'x' && hVec_buttons[6].symbol == L'x' && hVec_buttons[0].symbol == NULL)
	{
		SendMessage(hVec_buttons[0].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[0].symbol = L'o';
	}

	else if (hVec_buttons[1].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[7].symbol == NULL)
	{
		SendMessage(hVec_buttons[7].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[7].symbol = L'o';
	}
	else if (hVec_buttons[1].symbol == L'x' && hVec_buttons[7].symbol == L'x' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'x' && hVec_buttons[7].symbol == L'x' && hVec_buttons[1].symbol == NULL)
	{
		SendMessage(hVec_buttons[1].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[1].symbol = L'o';
	}

	else if (hVec_buttons[2].symbol == L'x' && hVec_buttons[5].symbol == L'x' && hVec_buttons[8].symbol == NULL)
	{
		SendMessage(hVec_buttons[8].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[8].symbol = L'o';
	}
	else if (hVec_buttons[2].symbol == L'x' && hVec_buttons[8].symbol == L'x' && hVec_buttons[5].symbol == NULL)
	{
		SendMessage(hVec_buttons[5].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[5].symbol = L'o';
	}
	else if (hVec_buttons[5].symbol == L'x' && hVec_buttons[8].symbol == L'x' && hVec_buttons[2].symbol == NULL)
	{
		SendMessage(hVec_buttons[2].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[2].symbol = L'o';
	}

	else if (hVec_buttons[0].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[8].symbol == NULL)
	{
		SendMessage(hVec_buttons[8].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[8].symbol = L'o';
	}
	else if (hVec_buttons[0].symbol == L'x' && hVec_buttons[8].symbol == L'x' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'x' && hVec_buttons[8].symbol == L'x' && hVec_buttons[0].symbol == NULL)
	{
		SendMessage(hVec_buttons[0].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[0].symbol = L'o';
	}

	else if (hVec_buttons[2].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[6].symbol == NULL)
	{
		SendMessage(hVec_buttons[6].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[6].symbol = L'o';
	}
	else if (hVec_buttons[2].symbol == L'x' && hVec_buttons[6].symbol == L'x' && hVec_buttons[4].symbol == NULL)
	{
		SendMessage(hVec_buttons[4].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[4].symbol = L'o';
	}
	else if (hVec_buttons[4].symbol == L'x' && hVec_buttons[6].symbol == L'x' && hVec_buttons[2].symbol == NULL)
	{
		SendMessage(hVec_buttons[2].hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImgO);
		hVec_buttons[2].symbol = L'o';
	}
	else
		CompGameSimple();
}

BOOL Check_result()
{
	if ((hVec_buttons[0].symbol == L'x' && hVec_buttons[1].symbol == L'x' && hVec_buttons[2].symbol == L'x') || 
		(hVec_buttons[3].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[5].symbol == L'x') ||
		(hVec_buttons[6].symbol == L'x' && hVec_buttons[7].symbol == L'x' && hVec_buttons[8].symbol == L'x') ||
		(hVec_buttons[0].symbol == L'x' && hVec_buttons[3].symbol == L'x' && hVec_buttons[6].symbol == L'x') ||
		(hVec_buttons[1].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[7].symbol == L'x') || 
		(hVec_buttons[2].symbol == L'x' && hVec_buttons[5].symbol == L'x' && hVec_buttons[8].symbol == L'x') || 
		(hVec_buttons[0].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[8].symbol == L'x') || 
		(hVec_buttons[2].symbol == L'x' && hVec_buttons[4].symbol == L'x' && hVec_buttons[6].symbol == L'x'))
	{
		MessageBox(NULL, TEXT("Вітаємо!\nВи виграли"), TEXT("Перемога"), MB_OK | MB_ICONINFORMATION);
		return TRUE;
	}
	else if ((hVec_buttons[0].symbol == L'o' && hVec_buttons[1].symbol == L'o' && hVec_buttons[2].symbol == L'o') ||
		(hVec_buttons[3].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[5].symbol == L'o') ||
		(hVec_buttons[6].symbol == L'o' && hVec_buttons[7].symbol == L'o' && hVec_buttons[8].symbol == L'o') ||
		(hVec_buttons[0].symbol == L'o' && hVec_buttons[3].symbol == L'o' && hVec_buttons[6].symbol == L'o') ||
		(hVec_buttons[1].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[7].symbol == L'o') ||
		(hVec_buttons[2].symbol == L'o' && hVec_buttons[5].symbol == L'o' && hVec_buttons[8].symbol == L'o') ||
		(hVec_buttons[0].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[8].symbol == L'o') ||
		(hVec_buttons[2].symbol == L'o' && hVec_buttons[4].symbol == L'o' && hVec_buttons[6].symbol == L'o'))
	{
		MessageBox(NULL, TEXT("Цього разу переміг комп'ютер"), TEXT("Програш"), MB_OK | MB_ICONINFORMATION);
		return TRUE;
	}
	else if (hVec_buttons[0].symbol != NULL && hVec_buttons[1].symbol != NULL && hVec_buttons[2].symbol != NULL &&
		hVec_buttons[3].symbol != NULL && hVec_buttons[4].symbol != NULL && hVec_buttons[5].symbol != NULL &&
		hVec_buttons[6].symbol != NULL && hVec_buttons[7].symbol != NULL && hVec_buttons[8].symbol != NULL)
	{
		MessageBox(NULL, TEXT("Цього разу ніхто не переміг"), TEXT("Нічия"), MB_OK | MB_ICONINFORMATION);
		return TRUE;
	}
	else
		return FALSE;
}