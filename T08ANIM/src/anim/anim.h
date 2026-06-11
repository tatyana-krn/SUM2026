/* Kurnosova Tatuana, 10-6, 09.06.2026 */
#ifndef __anim_h_
#define __anim_h_ 

#include "def.h"
#include "rnd/rnd.h"

#define TK6_MAX_UNITS 7

#define UNIT_BASE_FIELDS \
  VOID (*Init)( tk6UNIT *Uni, tk6ANIM *Ani );       \
  VOID (*Close)( tk6UNIT *Uni, tk6ANIM *Ani );      \
  VOID (*Response)( tk6UNIT *Uni, tk6ANIM *Ani );   \
  VOID (*Render)( tk6UNIT *Uni, tk6ANIM *Ani )

typedef struct tagtk6UNIT tk6UNIT;
typedef struct tagtk6ANIM tk6ANIM;

typedef struct tagtk6ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H; 
 
  tk6UNIT * Units[TK6_MAX_UNITS];
  INT NumOfUnits; 

  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */
};

struct tagtk6UNIT
{
  UNIT_BASE_FIELDS;
};


extern tk6ANIM TK6_Anim;

VOID TK6_AnimInit( HWND hWnd );
VOID TK6_AnimClose( VOID );
VOID TK6_AnimResize( INT W, INT H );
VOID TK6_AnimCopyFrame( HDC hDC );
VOID TK6_AnimRender( VOID );
VOID TK6_AnimFlipFullScreen( VOID );
VOID TK6_AnimExit( VOID );

VOID TK6_AnimUnitAdd( tk6UNIT *Uni );
tk6UNIT * TK6_AnimUnitCreate( INT Size );


extern DOUBLE
      TK6_GlobalTime, TK6_GlobalDeltaTime, /* Global time and interframe interval */
      TK6_Time, TK6_DeltaTime,             /* Time with pause and interframe interval */
      TK6_FPS;                             /* Frames per second value */
extern BOOL TK6_IsPause;
VOID TK6_TimerInit( VOID );
VOID TK6_TimerResponse( VOID );

#endif /* __anim_h_ */
 
/* END OF 'anim.h' FILE */
