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
VOID GLB_Draw( HDC hDC );
VOID GLB_Resize( INT W, INT H );
