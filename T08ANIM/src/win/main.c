/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include <time.h>

#include "def.h"
#include "anim/rnd/rnd.h"

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

  hwnd = CreateWindowA(WND_CLASS_NAME, "lele le can cancan", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 1950, 0, 900, 700, NULL, NULL, hInstance, NULL);
 
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
  static tk6PRIM Pr, PrSphere, Pr1;
 

 

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 47, 2, NULL);
    TK6_RndInit(hWnd);
    TK6_RndPrimCreateSphere(&PrSphere, 1, 20, 10);
    if (TK6_RndPrimLoad(&Pr1, "bin/models/cow.obj"))
    if (TK6_RndPrimCreate(&Pr, 4, 6))
    {
      Pr.V[0].P = VecSet(0, 0, 0);
      Pr.V[1].P = VecSet(2, 0, 0);
      Pr.V[2].P = VecSet(0, 2, 0);
      Pr.V[3].P = VecSet(2, 2, 0);
 
      Pr.I[0] = 0;
      Pr.I[1] = 1;
      Pr.I[2] = 2;
 
      Pr.I[3] = 2;
      Pr.I[4] = 1;
      Pr.I[5] = 3;
    }
    
    return 0;
  case WM_SIZE:
    TK6_RndResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
    TK6_RndStart();
    TK6_RndPrimDraw(&PrSphere, MatrRotateY(30 * clock() / 1000.0));
    TK6_RndPrimDraw(&Pr1, MatrTranslate(VecSet(0, 2 * sin(clock()) + 2, 0)));
    TK6_RndPrimDraw(&Pr, MatrRotateY(30 * clock() / 1000.0));
    TK6_RndEnd();

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_COMMAND:
    return 0;  
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    TK6_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "r u sure abt it?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;
  case WM_ERASEBKGND:
    return 1;
  case WM_DESTROY:
    TK6_RndPrimFree(&PrSphere);
    TK6_RndPrimFree(&Pr);
    TK6_RndPrimFree(&Pr1);
    TK6_RndClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(30);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}


