/* Kurnosova Tatuana, 10-6, 09.06.2026 */
#include "anim.h"


tk6ANIM TK6_Anim;

VOID TK6_AnimInit( HWND hWnd )
{
  memset(&TK6_Anim, 0, sizeof(tk6ANIM));
  TK6_Anim.hWnd = hWnd;
  TK6_RndInit(hWnd);
  TK6_Anim.hDC = TK6_hRndDC;
  TK6_Anim.H = TK6_RndFrameH;
  TK6_Anim.W = TK6_RndFrameW;
  TK6_TimerInit();
}

VOID TK6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < TK6_Anim.NumOfUnits; i++)
  {
    TK6_Anim.Units[i]->Close(TK6_Anim.Units[i], &TK6_Anim);
    free(TK6_Anim.Units[i]);
  }
  TK6_RndClose();
  memset(&TK6_Anim, 0, sizeof(tk6ANIM));
  
}

VOID TK6_AnimResize( INT W, INT H )
{
  TK6_RndResize(W, H);
  TK6_Anim.W = W;
  TK6_Anim.H = H;
  TK6_AnimRender();
}

VOID TK6_AnimCopyFrame( VOID )
{
  TK6_RndCopyFrame();
}

VOID TK6_AnimRender( VOID )
{
  INT i;
  
  TK6_TimerResponse();

  for (i = 0; i < TK6_Anim.NumOfUnits; i++)
    TK6_Anim.Units[i]->Response(TK6_Anim.Units[i], &TK6_Anim);

  TK6_RndStart();
  for (i = 0; i < TK6_Anim.NumOfUnits; i++)
    TK6_Anim.Units[i]->Render(TK6_Anim.Units[i], &TK6_Anim);
  TK6_RndEnd();
}

VOID TK6_AnimFlipFullScreen( VOID )
{
}

VOID TK6_AnimExit( VOID )
{
  static BOOL IsFinalizeStart = FALSE;
}


VOID TK6_AnimUnitAdd( tk6UNIT *Uni )
{
  if (TK6_Anim.NumOfUnits < TK6_MAX_UNITS)
    TK6_Anim.Units[TK6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &TK6_Anim);
}