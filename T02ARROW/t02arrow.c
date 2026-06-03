/* Kurnosova Tatuana, 10-6, 01.06.2026 */

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WND_CLASS_NAME "simmer praaactise 676767"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );


VOID DrawArrow( HWND hWnd, HDC hDC, INT Xc, INT Yc, INT L, INT W)
{
  INT X, Y, i;
  DOUBLE len, sina, cosa;
  POINT pt;
  POINT pn[] =
  {
    {L, 0}, {-W, -W}, {0, 0}, {-W, W}
  };
  POINT pnts[] =
  {
    {L, 0}, {L - W / 2, W / 2}, {L / 2, 3 * W / 2}, {L / 4, W}, {0, 3 * W / 4}, {-W, W / 4}, {-2 * W, W}, {-7 * W / 4, 0}, {-2 * W, -W}, {-W, -W / 4}, {0, -3 * W / 2}, {L / 5, -4 * W / 3}, {3 * L / 10, -2 * W}, {2 * L / 3, -4 * W / 3}, {4 * L / 5, -W}
  };
  POINT pnts_res[15];
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  len = sqrt((Xc - pt.x) * (Xc - pt.x) + (Yc - pt.y) * (Yc - pt.y));
  X = (INT)((pt.x - Xc) * L * 2 / len / 3 + Xc);
  Y = (INT)((pt.y - Yc) * L * 2 / len / 3 + Yc);

  
  cosa = (pt.x - Xc) / len;
  sina = (pt.y - Yc) / len; 

  for (i = 0; i < 15; i++)
  {
    pnts_res[i].x = Xc + pnts[i].x * cosa + pnts[i].y * sina;
    pnts_res[i].y = Yc - pnts[i].y * cosa + pnts[i].x * sina;
  }
  
  Polygon(hDC, pnts_res, sizeof(pnts_res) / sizeof(pnts_res[0]));  
  Ellipse(hDC, X, Y, X + 2, Y + 2);
  //MoveToEx(hDC, Xc, Yc, NULL);
  //LineTo(hDC, X, Y);
}


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

  hwnd = CreateWindowA(WND_CLASS_NAME, " lele le can cancan", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 500, 670, NULL, NULL, hInstance, NULL);
  CreateWindowA("BUTTON", "lele", WS_CHILD | WS_VISIBLE, 0, 0, 28, 67, hwnd, (HMENU)12, hInstance, NULL);
  CreateWindowA("BUTTON", "random", WS_CHILD | WS_VISIBLE, 40, 0, 60, 40, hwnd, (HMENU)13, hInstance, NULL);
  CreateWindowA("BUTTON", "static", WS_CHILD | WS_VISIBLE, 100, 0, 60, 40, hwnd, (HMENU)14, hInstance, NULL);
  
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
  static BOOL is_select = FALSE;
  static INT W, H, fl = 1;
  HPEN hPen, hOldPen;
  INT i, j;
  static HDC hMemDC, hDCWater;
  static HBITMAP hBm, hBmWater;
  BITMAP bm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC  = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 100, NULL);

    hDCWater = CreateCompatibleDC(hMemDC);
    hBmWater = CreateCompatibleBitmap(hDCWater, W, H);
    hBmWater = LoadImage(NULL, "water.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm  = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);

    return 0;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_COMMAND:
    if (LOWORD(wParam) == 12)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (LOWORD(wParam) == 13)
      fl = 1;
    else if (LOWORD(wParam) == 14)
      fl = 0;
    return 0;
  
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    GetObject(hBmWater, sizeof(bm), &bm);    
    SelectObject(hDCWater, hBmWater);
    StretchBlt(hMemDC, 0, 0, W, H, hDCWater, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    SetDCBrushColor(hMemDC, RGB(0, 30, 47));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    hPen = CreatePen(PS_SOLID, rand() % 5 + 2, RGB(rand() % 255, rand() % 255, rand() % 255));
    hOldPen = SelectObject(hMemDC, hPen);
    
    switch (fl)
    {
    case 0:
      for (i = 0; i < H; i += 167)
        for (j = 0; j < W; j += 167)
          DrawArrow(hWnd, hMemDC, j, i, 60, 12);
      break;
    case 1:
      srand(20);
      for (i = 0; i < 90; i++)
        DrawArrow(hWnd, hMemDC, rand() % W, rand() % H, 60, 12);
      break;
    }

    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);
    DeleteObject(hOldPen);


    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
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
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}