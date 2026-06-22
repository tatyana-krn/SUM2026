/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include "rnd.h"

HWND TK6_hRndWnd;        /* Work window handle */
HDC TK6_hRndDC;     /* Work window memory device context  */
INT TK6_RndFrameW, TK6_RndFrameH; /* Work window size */
HGLRC TK6_hRndGLRC;
 
DBL
  TK6_RndProjSize = 0.1,     /* Project plane fit square */
  TK6_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  TK6_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */
 
MATR
  TK6_RndMatrView, /* View coordinate system matrix */
  TK6_RndMatrProj, /* Projection coordinate system matrix */
  TK6_RndMatrVP;   /* Stored (View * Proj) matrix */

VEC
  TK6_RndCamLoc;

INT 
  TK6_RndShdAddonI[8], 
  TK6_RndShdAddonF[8],
  TK6_RndShdAddonV[8];