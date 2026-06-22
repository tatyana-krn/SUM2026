/* Kurnosova Tatuana, 10-6, 03.06.2026 */
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define WND_CLASS_NAME "simmer praaactise 676767"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );


VOID DrawHand( HWND hWnd, HDC hDC, INT Xc, INT Yc, INT L, INT W, DOUBLE alpha)
{
  INT i;
  POINT pnts[] =
  {
    {L, 0}, {L - W / 2, W / 2}, {L / 2, 3 * W / 2}, {L / 4, W}, {0, 3 * W / 4}, {-W, W / 4}, {-2 * W, W}, {-7 * W / 4, 0}, {-2 * W, -W}, {-W, -W / 4}, {0, -3 * W / 2}, {L / 5, -4 * W / 3}, {3 * L / 10, -2 * W}, {2 * L / 3, -4 * W / 3}, {4 * L / 5, -W}
  };
  POINT pnts_res[15];
  
  for (i = 0; i < 15; i++)
  {
    pnts_res[i].x = (INT)(Xc + pnts[i].x * cos(alpha) + pnts[i].y * sin(alpha));
    pnts_res[i].y = (INT)(Yc - pnts[i].y * cos(alpha) + pnts[i].x * sin(alpha));
  }
  Polygon(hDC, pnts_res, sizeof(pnts_res) / sizeof(pnts_res[0]));  
  //Ellipse(hDC, 2 * L / 3, , 2 * L / 3 + 2, Y + 2);
}
/*
    angle = (st.wSecond - 15 + st.wMilliseconds / 1000.0) * pi / 30;
    DrawHand(hWnd, hMemDC, W / 2, H / 2, 250, 30, angle);
    pt.x = W / 2 + (INT)(cos(angle) * 250);
    pt.y = H / 2 + (INT)(sin(angle) * 250);
    MoveToEx(hMemDC, W / 2, H / 2, NULL);
    LineTo(hMemDC, pt.x, pt.y);
*/
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
  CreateWindowA("BUTTON", "soda mode", WS_CHILD | WS_VISIBLE, 80, 0, 100, 80, hwnd, (HMENU)13, hInstance, NULL);
  
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static HFONT hFnt, hOldFnt;
  static BOOL flag = FALSE;
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  static HPEN hPen, hOldPen;
  static BOOL is_select = FALSE;
  static INT W, H;
  static HDC hMemDC, hDCClock, hDCSoda;
  static HBITMAP hBm, hBmClock, hBmSoda;
  POINT pt;
  BITMAP bm, bms;
  DOUBLE pi = 3.14159265359, angle;
  INT len = 300;
  CHAR Buf[100];

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC  = CreateCompatibleDC(hDC);
    hDCClock = CreateCompatibleDC(hMemDC);
    hBmClock = CreateCompatibleBitmap(hDCClock, W, H);
    hBmClock = LoadImage(NULL, "cloke.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    
    
    hFnt =
      CreateFont(1, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        VARIABLE_PITCH | FF_SWISS, "");
    hOldFnt = SelectObject(hMemDC, hFnt);

    
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 10, NULL);

    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    //SelectObject(hMemDC, hBm);

    return 0;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
    SelectObject(hMemDC, hBm);
    SetDCBrushColor(hMemDC, RGB(23, 67, 86));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    Rectangle(hMemDC, 0, 0, W, H); 
    
    
    GetObject(hBmClock, sizeof(bm), &bm);    
    SelectObject(hDCClock, hBmClock);
    BitBlt(hMemDC, (W - bm.bmWidth) / 2, (H - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hDCClock, 0, 0, SRCCOPY);
    
    GetLocalTime(&st);

    hPen = CreatePen(PS_SOLID, 3, RGB(200, 0, 0));
    hOldPen = SelectObject(hMemDC, hPen);
    angle = (st.wSecond - 15 + st.wMilliseconds / 1000.0) * pi / 30;
    DrawHand(hWnd, hMemDC, W / 2, H / 2, 250, 30, angle);

    hPen = CreatePen(PS_SOLID, 5, RGB(0, 100, 0));
    hOldPen = SelectObject(hMemDC, hPen);
    angle = (st.wMinute - 15 + (st.wSecond - 15) / 60.0) * pi / 30;
    DrawHand(hWnd, hMemDC, W / 2, H / 2, 80, 20, angle);

    hPen = CreatePen(PS_SOLID, 7, RGB(0, 0, 70));
    hOldPen = SelectObject(hMemDC, hPen);
    angle = (st.wHour - 2.78 + (st.wMinute - 15) / 60.0) * pi / 6;
    DrawHand(hWnd, hMemDC, W / 2, H / 2, 40, 5, angle);
    
    SetBkColor(hMemDC, RGB(220, 220, 180));
   
    SetTextColor(hMemDC, RGB(0, 0, 0));   
    TextOut(hMemDC, 100, 60, Buf, sprintf(Buf, ">>> размер: %d x %d\n>>> время: %i:%i:%i", W, H, st.wHour, st.wMinute, st.wSecond));
    SelectObject(hMemDC, hOldFnt);
    DeleteObject(hFnt);

    if (flag)
    {
      GetObject(hBmSoda, sizeof(bms), &bms);    
      SelectObject(hDCSoda, hBmSoda);
      SetStretchBltMode(hDCSoda, COLORONCOLOR);                                             
      StretchBlt(hMemDC, 0, 0, W, H, hDCSoda, 0, 0, bms.bmWidth, bms.bmHeight, SRCCOPY);
    }

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_COMMAND:
    if (LOWORD(wParam) == 12)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    if (LOWORD(wParam) == 13)
    {
      if (flag)
        flag = FALSE;
      else if (!flag)
          flag = TRUE;
    }
    return 0;  
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    
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
    DeleteObject(hPen);
    DeleteObject(hOldPen);
    DeleteObject(hBmSoda);
    DeleteObject(hBmClock);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}