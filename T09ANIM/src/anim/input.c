#include "anim.h"

INT TK6_MouseWheel;

static VOID TK6_MouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(TK6_Anim.hWnd, &pt);

  TK6_Anim.Mdx = 0;
  TK6_Anim.Mdy = 0;

  TK6_Anim.Mx = pt.x;
  TK6_Anim.My = pt.y;

  TK6_Anim.Mdz = TK6_Anim.Mz = TK6_MouseWheel = 0;
}

static VOID TK6_MouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(TK6_Anim.hWnd, &pt);

  TK6_Anim.Mdx = pt.x - TK6_Anim.Mx;
  TK6_Anim.Mdy = pt.y - TK6_Anim.My;

  TK6_Anim.Mx = pt.x;
  TK6_Anim.My = pt.y;

  TK6_Anim.Mdz = TK6_MouseWheel;
  TK6_Anim.Mz += TK6_MouseWheel;
  TK6_MouseWheel = 0;
}

static VOID TK6_KeyboardInit( VOID )
{
  INT i;

  GetKeyboardState(TK6_Anim.Keys);
  for (i = 0; i < 256; i++)
    TK6_Anim.Keys[i] >>= 7;
  memcpy(TK6_Anim.KeysOld, TK6_Anim.Keys, 256);

}

static VOID TK6_KeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(TK6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    TK6_Anim.Keys[i] >>= 7;
    TK6_Anim.KeysClick[i] = TK6_Anim.Keys[i] && !TK6_Anim.KeysOld[i];
  }

  memcpy(TK6_Anim.KeysOld, TK6_Anim.Keys, 256);

}

VOID TK6_InputInit( VOID )
{
  TK6_MouseInit();
  TK6_KeyboardInit();
}

VOID TK6_InputResponse( VOID )
{
  TK6_MouseResponse();
  TK6_KeyboardResponse();
}
