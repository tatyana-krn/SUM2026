/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include "rnd.h"                                                                                                            




VOID TK6_RndInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);

  TK6_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  TK6_hRndWnd = hWnd;
  TK6_hRndBmFrame = NULL;
  TK6_RndResize(100, 100);
  TK6_RndCamSet(VecSetAll(5), VecSetAll(0), VecSet(0, 1, 0));
}

VOID TK6_RndClose( VOID )
{
  DeleteObject(TK6_hRndBmFrame);
  DeleteDC(TK6_hRndDCFrame);
}

VOID TK6_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(TK6_hRndWnd);
 
  if (TK6_hRndBmFrame != NULL)
    DeleteObject(TK6_hRndBmFrame);
  TK6_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(TK6_hRndWnd, hDC);		
 
  SelectObject(TK6_hRndDCFrame, TK6_hRndBmFrame);
 
  TK6_RndFrameW = W;
  TK6_RndFrameH = H;
 
  TK6_RndProjSet();
}
VOID TK6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, TK6_RndFrameW, TK6_RndFrameH, TK6_hRndDCFrame, 0, 0, SRCCOPY);
}

VOID TK6_RndStart( VOID )
{
  SetDCPenColor(TK6_hRndDCFrame, RGB(10, 189, 220));
  SelectObject(TK6_hRndDCFrame, GetStockObject(DC_PEN));
  SelectObject(TK6_hRndDCFrame, GetStockObject(BLACK_BRUSH));
  Rectangle(TK6_hRndDCFrame, 0, TK6_RndFrameH, TK6_RndFrameW, 0);
}

VOID TK6_RndEnd( VOID )
{
}


VOID TK6_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = TK6_RndProjSize;
  /* Correct aspect ratio */
  if (TK6_RndFrameW > TK6_RndFrameH)
    rx *= (DBL)TK6_RndFrameW / TK6_RndFrameH;
  else
    ry *= (DBL)TK6_RndFrameH / TK6_RndFrameW;
  TK6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      TK6_RndProjDist, TK6_RndProjFarClip);
  TK6_RndMatrVP = MatrMulMatr(TK6_RndMatrView, TK6_RndMatrProj);
}
 
 
VOID TK6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  TK6_RndMatrView = MatrView(Loc, At, Up);
  TK6_RndMatrVP = MatrMulMatr(TK6_RndMatrView, TK6_RndMatrProj);
}
 
 

 


 
