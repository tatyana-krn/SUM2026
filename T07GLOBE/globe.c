/* Kurnosova Tatuana, 10-6, 06.06.2026 */
#include <math.h>
#include <time.h>

#include "globe.h"
#include "mth.h"

#define Pi 3.14159265358979323846

#define GLB_MAX(A,B)((A) < (B) ? (B) : (A))
#define GLB_MIN(A,B)((A) < (B) ? (A) : (B))

static VEC GLB_Geom[GLB_GRID_H][GLB_GRID_W];
static VEC GLB_NGeom[GLB_GRID_H][GLB_GRID_W];
 
static DBL Ws, Hs;
static DBL GLB_Ws, GLB_Hs;
static DBL ProjSize = 1, GLB_W, GLB_H, GLB_ProjDist = 1;

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
  {
    DBL theta = i * PI / (GLB_GRID_H - 1);

    for (j = 0; j < GLB_GRID_W; j++)
    {
      DBL phi = j * 2 * PI / (GLB_GRID_W - 1);

      GLB_Geom[i][j].X = R * sin(theta) * sin(phi);
      GLB_Geom[i][j].Y = R * cos(theta);
      GLB_Geom[i][j].Z = R * sin(theta) * cos(phi);

      GLB_NGeom[i][j].X = sin(theta) * sin(phi);
      GLB_NGeom[i][j].Y = cos(theta);
      GLB_NGeom[i][j].Z = sin(theta) * cos(phi);
    }
  }

  for (i = 0; i < GLB_GRID_H; i++)
  {
    for (j = 0; j < GLB_GRID_W; j++)
    {
      VEC N1 = VecCrossVec(VecSubVec(GLB_NGeom[i][j + 1], GLB_NGeom[i][j]), VecSubVec(GLB_NGeom[i + 1][j], GLB_NGeom[i][j]));
      VEC N2 = VecCrossVec(VecSubVec(GLB_NGeom[i][j + 1], GLB_NGeom[i][j]), VecSubVec(GLB_NGeom[i + 1][j], GLB_NGeom[i][j]));
      GLB_NGeom[i][j] = VecNeg(VecNormalize(VecAddVec(N1, N2)));
      
    }
  }
} /* End of 'GLB_Init' function */

VOID GLB_Resize( INT Ws, INT Hs )
{
  GLB_Ws = Ws;
  GLB_Hs = Hs;

  if (GLB_Ws >= GLB_Hs)
    GLB_W = ProjSize * GLB_Ws / GLB_Hs, GLB_H = ProjSize;
  else
    GLB_H = ProjSize * GLB_Hs / GLB_Ws, GLB_W = ProjSize;
} /* End of 'GLB_Resize' function */
/*
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
*/

VOID GLB_Draw( HDC hDC )
{
  INT i, j;
  DBL t = clock() / (DBL)CLOCKS_PER_SEC;
  MATR m;
  VEC L, P, N;
  static POINT pnts[GLB_GRID_H][GLB_GRID_W];

  //L = VecNormalize(VecSet(1, 1, sin(t)));

  m = MatrMulMatr4(MatrRotateX(2 * cos(t)), MatrRotateY(cos(t)), MatrRotateZ(cos(t)), MatrTranslate(VecSet(0.5 * cos(t), 0, 0)));

  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      P = VectorTransform(GLB_Geom[i][j], m);
      P.Z = -(--P.Z);

      pnts[i][j].x = (INT)(GLB_Ws / 2 + (P.X * GLB_ProjDist / P.Z) * GLB_Ws / GLB_W);
      pnts[i][j].y = (INT)(GLB_Hs / 2 - (P.Y * GLB_ProjDist / P.Z) * GLB_Hs / GLB_H);
    }
  
  
  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {
      POINT pnt[4];

      SetDCBrushColor(hDC, RGB(rand() % 255, rand() % 255, rand() % 255));
      SelectObject(hDC, GetStockObject(DC_BRUSH));
    
      pnt[0] = pnts[i][j];
      pnt[1] = pnts[i][j + 1];
      pnt[2] = pnts[i + 1][j + 1];
      pnt[3] = pnts[i + 1][j];
     
      if ((pnt[0].x - pnt[1].x) * (pnt[0].y + pnt[1].y)
        + (pnt[1].x - pnt[2].x) * (pnt[1].y + pnt[2].y)
        + (pnt[2].x - pnt[3].x) * (pnt[2].y + pnt[3].y)
        + (pnt[3].x - pnt[0].x) * (pnt[3].y + pnt[0].y) >= 0)
        Polygon(hDC, pnt, 4);
    }
} /* End of 'GLB_Draw' function */