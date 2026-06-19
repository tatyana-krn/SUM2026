/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include <stdio.h>


#include "units/units.h"

#define WND_CLASS_NAME "Z z z lelele"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrewInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hwnd;
  WNDCLASS wc;
  MSG msg;
  CONSOLE_FONT_INFOEX cfi = {0};
  HWND hConWnd;

  SetDbgMemHooks();

  /* Create console */
  AllocConsole();
 
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
  cfi.dwFontSize.Y = 18;
  cfi.FontWeight = FW_BOLD;
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
 
  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");
  printf("\x1b[38;2;%d;%d;%dm \x1b[48;2;%d;%d;%dm", 255, 255, 0, 0, 102, 102);
  printf("Группа компьютерной графики ФМЛ № 30\n");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 0, 255, 0, 90, 90, 90);
  printf("Computer Graphics Support Group\n");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 255, 255, 255, 0, 0, 0);
  fflush(stdout);
 
  hConWnd = GetConsoleWindow();
  /* MoveWindow(hConWnd, 2560 + 1920 / 2, 0, 1920 / 2, 1080, FALSE); */
  SetWindowPos(hConWnd, HWND_TOP, 1920 / 2, 0, 1920 / 2, 1000, 0);


  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_HAND);
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;

  if(!RegisterClass(&wc))
  {
    MessageBox(NULL, "oh shit", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  
  hwnd = CreateWindow(WND_CLASS_NAME, "lele le can cancan", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 1950, 0, 900, 700, NULL, NULL, hInstance, NULL);
 
  TK6_AnimUnitAdd(TK6_UnitCreateCow());
  TK6_AnimUnitAdd(TK6_UnitCreateCow());
  TK6_AnimUnitAdd(TK6_UnitCreateCow());
  TK6_AnimUnitAdd(TK6_UnitCreateCow());
  TK6_AnimUnitAdd(TK6_UnitCreateCow());
  
  TK6_AnimUnitAdd(TK6_UnitCreateControl());
  
  /*
  ShowWindow(hwnd, CmdShow);
  */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hwnd, WM_TIMER, 47, 0);
  /*
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  */
  return msg.wParam;
  
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMinTrackSize.y += 100;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) + 
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
   return 0;
  case WM_CREATE:
    SetTimer(hWnd, 47, 1, NULL);
    TK6_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    TK6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;
  case WM_TIMER:
    TK6_AnimRender();
    TK6_AnimCopyFrame();
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    TK6_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_DESTROY:
    TK6_AnimClose();
    PostQuitMessage(30);
    KillTimer(hWnd, 47);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}


