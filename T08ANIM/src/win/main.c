/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include <time.h>

#include "def.h"

#define WND_CLASS_NAME "simmer praaactise 676767"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrewInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hwnd;
  WNDCLASS wc;
  MSG msg;
  
  SetDbgMemHooks();

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

  hwnd = CreateWindowA(WND_CLASS_NAME, "lele le can cancan", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 500, 670, NULL, NULL, hInstance, NULL);
  CreateWindowA("BUTTON", "lele", WS_CHILD | WS_VISIBLE, 0, 0, 28, 67, hwnd, (HMENU)12, hInstance, NULL);
 
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{

  HDC hDC;
  PAINTSTRUCT ps;
  static HPEN hPen, hOldPen;
  static BOOL is_select = FALSE;
  static INT W, H;
  static HBITMAP hBm;
  INT len = 300;


  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 10, NULL);
    return 0;
  case WM_SIZE:
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
    return 0;
  case WM_COMMAND:
    if (LOWORD(wParam) == 12)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;  
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "r u sure abt it?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;
  case WM_ERASEBKGND:
    return 0;
  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}


