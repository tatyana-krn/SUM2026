/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#ifndef __rnd_h_
#define __rnd_h_ 

#define GLEW_STATIC
#include <glew.h>
#include "def.h"

extern HWND TK6_hRndWnd;
extern HDC TK6_hRndDC;
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
  VEC P;   /* Vertex position */
  VEC2 T;  /* Vertex texture coordinate */
  VEC N;   /* Vertex normal */
  VEC4 C;  /* Vertex color */
} tk6VERTEX;
 
/*
typedef struct tagtk6PRIM
{
  tk6VERTEX *V;
  INT NumOfV;
 
  INT *I;
  INT NumOfI;
 
  MATR Trans;
} tk6PRIM;
*/
typedef enum tagtk6PRIM_TYPE
{
  TK6_RND_PRIM_POINTS,   /* Array of points  – GL_POINTS */
  TK6_RND_PRIM_LINES,    /* Line segments (by 2 points) – GL_LINES */
  TK6_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles – GL_TRIANGLES */
} tk6PRIM_TYPE;
 
 
/* Primitive representation type */
typedef struct tagtk6PRIM
{
  tk6PRIM_TYPE Type; /* Primitive type */
 
  INT
    VA,              /* Vertex array Id */
    VBuf,            /* Vertex buffer Id */
    IBuf;            /* Index buffer Id (if 0 - use only vertex buffer) */
 
  INT NumOfElements; /* Number of indices/vecrtices */
 
  VEC MinBB, MaxBB;  /* Bound box */
 
  MATR Trans;   /* Additional transformation matrix */
} tk6PRIM;
 
 
VOID TK6_RndInit( HWND hWnd );
VOID TK6_RndClose( VOID );
VOID TK6_RndResize( INT W, INT H );
VOID TK6_RndCopyFrame( VOID );
VOID TK6_RndStart( VOID );
VOID TK6_RndEnd( VOID );
VOID TK6_RndProjSet( VOID );
VOID TK6_RndCamSet( VEC Loc, VEC At, VEC Up );


VOID TK6_RndPrimFree( tk6PRIM *Pr );
VOID TK6_RndPrimDraw( tk6PRIM *Pr, MATR World );
VOID TK6_RndPrimFree( tk6PRIM *Pr );
VOID TK6_RndPrimCreate( tk6PRIM *Pr, tk6PRIM_TYPE Type, tk6VERTEX *V, INT NoofV, INT *Ind, INT NoofI );
BOOL TK6_RndPrimCreateSphere( tk6PRIM *Pr, DBL R, INT W, INT H );

BOOL TK6_RndPrimLoad( tk6PRIM *Pr, CHAR *FileName );


#endif /* __rnd_h_ */
 
/* END OF 'rnd.h' FILE */