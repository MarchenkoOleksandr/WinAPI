#include <windows.h>
#include <ctime>
#include "resource.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HWND hButton, hYear, hMonth, hDay, hResult;

BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL GetDate(UINT year, UINT month, UINT day);
VOID Int_to_weekday(HWND, UINT);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
}

BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		hButton = GetDlgItem(hWnd, IDOK);
		hYear = GetDlgItem(hWnd, IDC_EDIT1);
		hMonth = GetDlgItem(hWnd, IDC_EDIT2);
		hDay = GetDlgItem(hWnd, IDC_EDIT3);
		hResult = GetDlgItem(hWnd, IDC_STATIC5);
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			time_t current_time;
			struct tm * Date;
			UINT year, month, day;
			time(&current_time);
			Date = localtime(&current_time);
			year = GetDlgItemInt(hWnd, IDC_EDIT1, 0, 0);
			if (year < 1900)
			{
				SetWindowText(hResult, TEXT("Помилка вводу року: \nрік має бути від 1901"));
				return TRUE;
			}
			else if (year > 1900 && year < 1970)
				for (year; year < 1970; year += 28);
			Date->tm_year = year - 1900;

			month = GetDlgItemInt(hWnd, IDC_EDIT2, 0, 0);
			if (month < 1 || month > 12)
			{
				SetWindowText(hResult, TEXT("Помилка вводу місяця: \nмісяць має бути від 1 до 12"));
				return TRUE;
			}
			else
				Date->tm_mon = month - 1;

			day = GetDlgItemInt(hWnd, IDC_EDIT3, 0, 0);
			if (!GetDate(year, month, day))
			{
				SetWindowText(hResult, TEXT("Помилка вводу дня: \nу цьому місяці відсутній такий день"));
				return TRUE;
			}
			else
				Date->tm_mday = day;

			mktime(Date);
			if (mktime(Date) == -1)
			{
				SetWindowText(hResult, TEXT("Помилка розрахунку"));
				return TRUE;
			}
			Int_to_weekday(hResult, Date->tm_wday);
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL GetDate(UINT year, UINT month, UINT day)
{
	if (day >= 1 && day <= 28)
		return TRUE;
	else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day >= 1 && day <= 31))
		return TRUE;	
	else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day >= 1 && day <= 30))
		return TRUE;
	else if ((month == 2) && (day >= 1 && day <= 29) && ((!(year % 4) && year % 100) || !(year % 400)))
		return TRUE;
	else
		return FALSE;
}

VOID Int_to_weekday(HWND hWnd, UINT day)
{
	switch (day)
	{
	case 1:
		SetWindowText(hWnd, TEXT("Понеділок"));
		break;
	case 2:
		SetWindowText(hWnd, TEXT("Вівторок"));
		break;
	case 3:
		SetWindowText(hWnd, TEXT("Середа"));
		break;
	case 4:
		SetWindowText(hWnd, TEXT("Четвер"));
		break;
	case 5:
		SetWindowText(hWnd, TEXT("П'ятниця"));
		break;
	case 6:
		SetWindowText(hWnd, TEXT("Субота"));
		break;
	default:
		SetWindowText(hWnd, TEXT("Неділя"));
		break;
	}
}