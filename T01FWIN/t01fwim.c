/* Kurnosova Tatuana, 10-6, 01.06.2026 */

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define WND_CLASS_NAME "simmer praaactise 676767"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrewInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hwnd;
  WNDCLASS wc;
  MSG msg;
  
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_WAIT);
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

  hwnd = CreateWindowA(WND_CLASS_NAME, " lele le can cancan", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 228, 670, NULL, NULL, hInstance, NULL);
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
  POINT pt;
  static BOOL is_select = FALSE;
  static INT W, H;

  switch (Msg)
  {
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    return 0;
  case WM_COMMAND:
    if (LOWORD(wParam) == 12)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
   case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
 
    SetDCBrushColor(hDC, RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(hDC, RGB(52, 42, 67));

    Ellipse(hDC, 0, 0, W, H);
 

    SelectObject(hDC, GetStockObject(WHITE_BRUSH));
    SetDCPenColor(hDC, RGB(0, 0, 0));
    SelectObject(hDC, GetStockObject(DC_PEN));

    Ellipse(hDC, 30, 30, W / 2 + rand() % 30, H / 2 + rand() % 30);
 
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
 
    SetDCPenColor(hDC, RGB(0, 255, 0));
    Rectangle(hDC, pt.x - 5, pt.y - 5, pt.x + 5, pt.y + 5);
 

    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "r u sure abt it?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;
  case WM_DESTROY:
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}