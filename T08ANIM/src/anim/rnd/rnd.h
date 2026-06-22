/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#ifndef __rnd_h_
#define __rnd_h_ 

#include "def.h"

extern HWND TK6_hRndWnd;
extern HDC TK6_hRndDCFrame;
extern HBITMAP TK6_hRndBmFrame;
extern INT TK6_RndFrameW, TK6_RndFrameH;
 
extern DBL
  TK6_RndProjSize,     /* Project plane fit square */
  TK6_RndProjDist,     /* Distance to project plane from viewer (near) */
  TK6_RndProjFarClip;  /* Distance to project far clip plane (far) */
 
extern MATR
  TK6_RndMatrView, /* View coordinate system matrix */
  TK6_RndMatrProj, /* Projection coordinate system matrix */
  TK6_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Vertex representation type */
typedef struct tagtk6VERTEX
{
  VEC P;  /* Vertex position */
} tk6VERTEX;
 
/* Primitive representation type */
typedef struct tagtk6PRIM
{
  tk6VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */
 
  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */
 
  MATR Trans;   /* Additional transformation matrix */
} tk6PRIM;
 
VOID TK6_RndInit( HWND hWnd );
VOID TK6_RndClose( VOID );
VOID TK6_RndResize( INT W, INT H );
VOID TK6_RndCopyFrame( HDC hDC );
VOID TK6_RndStart( VOID );
VOID TK6_RndEnd( VOID );
VOID TK6_RndProjSet( VOID );
VOID TK6_RndCamSet( VEC Loc, VEC At, VEC Up );


VOID TK6_RndPrimFree( tk6PRIM *Pr );
VOID TK6_RndPrimDraw( tk6PRIM *Pr, MATR World );
VOID TK6_RndPrimFree( tk6PRIM *Pr );
BOOL TK6_RndPrimCreate( tk6PRIM *Pr, INT NoofV, INT NoofI );
BOOL TK6_RndPrimCreateSphere( tk6PRIM *Pr, DBL R, INT W, INT H );

BOOL TK6_RndPrimCreateCilinder( tk6PRIM *Pr, DBL R, INT W, INT H, INT Y );

BOOL TK6_RndPrimLoad( tk6PRIM *Pr, CHAR *FileName );
#endif /* __rnd_h_ */
 
/* END OF 'rnd.h' FILE */