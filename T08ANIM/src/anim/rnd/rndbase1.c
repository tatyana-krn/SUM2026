/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include "rnd.h"

HWND TK6_hRndWnd;        /* Work window handle */
HDC TK6_hRndDCFrame;     /* Work window memory device context  */
HBITMAP TK6_hRndBmFrame; /* Work window background bitmap handle */
INT TK6_RndFrameW, TK6_RndFrameH; /* Work window size */
 
DBL
  TK6_RndProjSize = 0.1,     /* Project plane fit square */
  TK6_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  TK6_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */
 
MATR
  TK6_RndMatrView, /* View coordinate system matrix */
  TK6_RndMatrProj, /* Projection coordinate system matrix */
  TK6_RndMatrVP;   /* Stored (View * Proj) matrix */



VOID TK6_RndInit( HWND hWnd )
{
}

VOID VG4_RndClose( VOID )
{
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
}

VOID TK6_RndEnd( VOID )
{
}


VOID VG4_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = VG4_RndProjSize;
  /* Correct aspect ratio */
  if (VG4_RndFrameW > VG4_RndFrameH)
    rx *= (DBL)VG4_RndFrameW / VG4_RndFrameH;
  else
    ry *= (DBL)VG4_RndFrameH / VG4_RndFrameW;
  VG4_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      VG4_RndProjDist, VG4_RndProjFarClip);
  VG4_RndMatrVP = MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj);
}
 
 
VOID VG4_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  VG4_RndMatrView = MatrView(Loc, At, Up);
  VG4_RndMatrVP = MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj);
}
 
 
VOID VG4_RndPrimDraw( vg4PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr(World, VG4_RndMatrVP);

    VEC p = VecMulMatr(Prim->V[i], wvp);
 
    pnts[i].x = (INT)((P.X + 1) * VG4_RndFrameW / 2);
    pnts[i].y = (INT)((-P.Y + 1) * VG4_RndFrameH / 2);
}
 


 
