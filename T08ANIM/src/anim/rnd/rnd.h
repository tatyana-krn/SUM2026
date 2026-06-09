/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#ifndef __rnd_h_
#define __rnd_h_ 

#include "def.h"

VOID TK6_RndInit( HWND hWnd );
VOID VG4_RndClose( VOID );
VOID TK6_RndResize( INT W, INT H );
VOID TK6_RndCopyFrame( HDC hDC );
VOID TK6_RndStart( VOID );
VOID TK6_RndEnd( VOID );
VOID TK6_RndProjSet( VOID );
VOID TK6_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID TK6_RndPrimDraw( tk6PRIM *Pr, MATR World );


#endif /* __rnd_h_ */
 
/* END OF 'rnd.h' FILE */