/* Kurnosova Tatuana, 10-6, 06.06.2026 */
#include <windows.h>
#include <math.h>
#include <time.h>

#include "gl.h"

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

  hwnd = CreateWindowA(WND_CLASS_NAME, " lele le can cancan", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 500, 670, NULL, NULL, hInstance, NULL);
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
  SYSTEMTIME st;
  static HPEN hPen, hOldPen;
  static BOOL is_select = FALSE;
  static INT W, H;
  static HDC hMemDC, hDCClock;
  static HBITMAP hBm, hBmClock;
  POINT pt;
  BITMAP bm;
  DOUBLE pi = 3.14159265359, angle;
  INT len = 300;


  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC  = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 10, NULL);

    GLB_Init(1.0);

    hDCClock = CreateCompatibleDC(hMemDC);
    hBmClock = CreateCompatibleBitmap(hDCClock, W, H);
    hBmClock = LoadImage(NULL, "cloke.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);


    GLB_Resize(W, H);


    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);

    return 0;
  case WM_LBUTTONDOWN:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_TIMER:
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
    pt.x = W / 2 + (INT)(cos(angle) * 250);
    pt.y = H / 2 + (INT)(sin(angle) * 250);
    MoveToEx(hMemDC, W / 2, H / 2, NULL);
    LineTo(hMemDC, pt.x, pt.y);
    
    GLB_Draw(hMemDC, 2.0);

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_COMMAND:
    if (LOWORD(wParam) == 12)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
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
    DeleteObject(hBmClock);
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}









/* Kurnosova Tatuana, 10-6, 06.06.2026 */
#include <math.h>
#include <time.h>

#include "gl.h"
#include "mth.h"

#define Pi 3.14159265358979323846

#define GLB_MAX(A,B)((A) < (B) ? (B) : (A))
#define GLB_MIN(A,B)((A) < (B) ? (A) : (B))


static VEC GLB_Geom[GLB_GRID_H][GLB_GRID_W];
 
static DBL Ws, Hs;
static DBL GLB_Ws, GLB_Hs;
static DBL ProjSize = 1, GLB_Wp, GLB_Hp, GLB_ProjDist = 1;

COLORREF ColorTo255( VEC Color, INT I )
{
  INT
    R = (INT)(Color.X * 255 * (I % 6) - 70),
    G = (INT)(Color.Y * 255 * (I % 6) - 70),
    B = (INT)(Color.Z * 255 * (I % 6) - 70);

  R = GLB_MIN(255, GLB_MAX(0, R));
  G = GLB_MIN(255, GLB_MAX(0, G));
  B = GLB_MIN(255, GLB_MAX(0, B));

  return RGB(R, G, B);
}

VEC RotateZ( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * Pi / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
}

VEC RotateX( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * Pi / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.X;
  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Y * si + P.Z * co;
  return NewP;
}

VEC RotateY( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * Pi / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.Z * si + P.X * co;
  NewP.Y = P.Y;
  NewP.Z = P.Z * co - P.X * si;
  return NewP;
} 

VOID GLB_Init( DBL R )
{
  INT i, j;
  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      DBL theta = i * Pi / GLB_GRID_H, phi = j * 2 * Pi / GLB_GRID_W;
 
      GLB_Geom[i][j].X = R * sin(theta) * sin(phi);
      GLB_Geom[i][j].Y = R * cos(theta);
      GLB_Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
}


VOID GLB_Resize( INT W, INT H )
{
  GLB_Ws = Ws;
  GLB_Hs = Hs;
}

///

VOID GLB_Draw( HDC hMemDC, DBL Rs )
{
  INT i, j;
  DBL t = clock() / (DBL)CLOCKS_PER_SEC;
  MATR m;
  VEC L, P, N;
  static POINT pnts[GLB_GRID_H][GLB_GRID_W];

  L = VecNormalize(VecSet(1, 1, sin(t)));

  m = MatrMulMatr4(MatrRotateX(sin(t)), MatrRotateY(sin(t)), MatrRotateZ(3 * sin(t)), MatrTranslate(VecSet(0, 0, 0)));

  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      DBL xp, yp;
      P =  VectorTransform(GLB_Geom[i][j], m);
      P.Z -= 1;

      xp = P.X * GLB_ProjDist / -P.Z;
      yp = P.Y * GLB_ProjDist / -P.Z;

      pnts[i][j].x = (INT)(GLB_Ws / 2 + xp * GLB_Ws / GLB_Wp + 4 * P.Z);
      pnts[i][j].y = (INT)(GLB_Hs / 2 - yp * GLB_Hs / GLB_Hp);
    }

  SelectObject(hMemDC, GetStockObject(DC_BRUSH));
  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {
      VEC C = {0.34, 0.21, 0.6};
      DBL nl;
      POINT pnt[4];

      N = VectorTransform(GLB_Geom[i][j], m);

      nl = VecDotVec(N, L);
      if (nl < 0.2)
        nl = 0.2;

      C = VecMulNum(C, nl);

      pnt[0] = pnts[i][j];
      pnt[1] = pnts[i][j + 1];
      pnt[2] = pnts[i + 1][j + 1];
      pnt[3] = pnts[i + 1][j];

      SetDCBrushColor(hMemDC, ColorTo255(C, (INT)(i + t * 10)));

      if ((pnt[0].x - pnt[1].x) * (pnt[0].y + pnt[1].y)
        + (pnt[1].x - pnt[2].x) * (pnt[1].y + pnt[2].y)
        + (pnt[2].x - pnt[3].x) * (pnt[2].y + pnt[3].y)
        + (pnt[3].x - pnt[0].x) * (pnt[3].y + pnt[0].y) >= 0)
        Polygon(hMemDC, pnt, 4);
    }
}




/* Kurnosova Tatuana, 10-6, 06.06.2026 */
#include <windows.h>


#define GLB_GRID_W 31
#define GLB_GRID_H 18

typedef DOUBLE DBL;

typedef struct
{
  DBL X, Y, Z;
} VEC;
 
VOID GLB_Init( DBL R );
VOID GLB_Draw( HDC hMemDC, DBL Rs );
VOID GLB_Resize( INT W, INT H );












#ifndef __mth_h_
#define __mth_h_

#include <math.h>
#include <windows.h>

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

#define VecAddVec3(A, B, C) VecAddVec(A, VecAddVec(B, C))
#define VecAddVec4(A, B, C, A) VecAddVec(VecAddVec(A, B), VecAddVec(C, D))
#define MatrMulMatr3(A, B, C) MatrMulMatr(A, (B, C))
#define MatrMulMatr4(A, B, C, D) MatrMulMatr(MatrMulMatr(A, B), MatrMulMatr(C, D))

#define UnitMatrix \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
}

/* Base float point types */
typedef double DBL;
typedef float FLT;

typedef struct tagMATR
{
  DBL A[4][4]; /* Matrix elements */
} MATR;

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v = {X, Y, Z};

  return v;
} /* End of 'VecSet' function */

__inline VEC VecSetAll( DBL A )
{
  VEC r = {A, A, A};

  return r;
} /* End of 'VecMulNum' function */

__inline MATR MatrIdentity( VOID )
{
  MATR I = UnitMatrix;

  return I;
} /* End of 'VecMulNum' function */

__inline MATR MatrSet( DBL A00, DBL A01, DBL A02, DBL A03,
                       DBL A10, DBL A11, DBL A12, DBL A13,
                       DBL A20, DBL A21, DBL A22, DBL A23,
                       DBL A30, DBL A31, DBL A32, DBL A33 )
{
  MATR m =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };

  return m;
} /* End of 'VecMulNum' function */

__inline MATR MatrTranslate( VEC T )
{
  return MatrSet(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 T.X, T.Y, T.Z, 1);
}

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Vector by matrix multiplication (with homogenious devide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M )
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

__inline VEC VecMulNum( VEC V1, DBL N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
} /* End of 'VecMulNum' function */

__inline VEC VecDivNum( VEC V1, DBL N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
} /* End of 'VecDivNum' function */

__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.X * V2.Z - V1.Z * V2.X,
                V1.X * V2.Y - V1.X * V2.Y);
} /* End of 'VecCrossVec' function */

__inline DBL VecLen( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'VecLen' function */

__inline DBL VecLen2( VEC V )
{
  return VecLen(V) * VecLen(V);
} /* End of 'VecLen2' function */

__inline VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
} /* End of 'VecNormalize' function */

__inline VEC PointTransform( VEC V, MATR M )
{
  return
    VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
           V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
           V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'PointTransform' function */

__inline VEC VectorTransform( VEC V, MATR M )
{
  return
    VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
           V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
           V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'VectorTransform' function */

__inline MATR MatrTranspose( MATR M )
{
  return MatrSet(M.A[0][0], M.A[0][1], M.A[0][2], M.A[0][3],
                 M.A[1][0], M.A[1][1], M.A[1][2], M.A[1][3],
                 M.A[2][0], M.A[2][1], M.A[2][2], M.A[2][3],
                 M.A[3][0], M.A[3][1], M.A[3][2], M.A[3][3]);
} /* End of 'VecMulNum' function */

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR r = {{{0}}};

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'VecMulNum' function */

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

__inline MATR MatrInverse( MATR M )
{
  DBL det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return MatrIdentity();

  /* Build adjoint matrix */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]) / det;

  return r;
} /* End of 'MatrInverse' function */


__inline MATR MatrScale( VEC S )
{
  return MatrSet(S.X, 0,   0,   0,
                 0,   S.Y, 0,   0,
                 0,   0,   S.Z, 0,
                 0,   0,   0,   1);
} /* End of 'MatrScale' function */

__inline MATR MatrRotateX( DBL AngleInDegree )
{
  return MatrSet(1, 0,                   0,                  0,
                 0, cos(AngleInDegree),  sin(AngleInDegree), 0,
                 0, -sin(AngleInDegree), cos(AngleInDegree), 0,
                 0, 0,                   0,                  1);
} /* End of 'VecMulNum' function */

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  return MatrSet(cos(AngleInDegree),   0, sin(AngleInDegree), 0,
                 0,                    1,                  0, 0,
                 sin(AngleInDegree),   0, cos(AngleInDegree), 0,
                 0,                    0, 0,                  1);
} /* End of 'VecMulNum' function */

__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  return MatrSet(cos(AngleInDegree),   sin(AngleInDegree), 0, 0,
                 -sin(AngleInDegree),  cos(AngleInDegree), 0, 0,
                 0,                    0,                  1, 0,
                 0,                    0,                  0, 1);
} /* End of 'VecMulNum' function */

__inline MATR MatrRotate( DBL AngleInDegree, VEC V )
{
  VEC R = VecNormalize(V);

  return MatrTranspose(MatrSet(cos(AngleInDegree) + R.X * R.X * (1 - cos(AngleInDegree)), R.X * R.Y * (1 - cos(AngleInDegree)) + R.Z * sin(AngleInDegree), R.X * R.Z * (1  - cos(AngleInDegree)) - R.Y * sin(AngleInDegree), 1,
                       R.Y * R.X * (1 - cos(AngleInDegree)) - R.Z * sin(AngleInDegree), cos(AngleInDegree) + R.Y * R.Y * (1 - cos(AngleInDegree)), R.Y * R.Z * (1 - cos(AngleInDegree)) + R.X * sin(AngleInDegree), 1,
                       R.Z * R.X * (1 - cos(AngleInDegree)) + R.Y * cos(AngleInDegree), R.Z * R.Y * (1 - cos(AngleInDegree)) - R.X * cos(AngleInDegree), cos(AngleInDegree) + R.Z * R.Z * (1 - cos(AngleInDegree)), 1,
                       1, 1, 1, 1));
} /* End of 'VecMulNum' function */

#endif /* __mth_h_ */