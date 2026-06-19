/* Kurnosova Tatuana, 10-6, 09.06.2026 */
#include <stdio.h>


#include "units.h"


typedef struct tagtk6UINT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc, CamDir, CamAt;
  DBL Speed;
} tk6UNIT_CONTROL;

static VOID TK6_UnitInit( tk6UNIT_CONTROL *Uni, tk6ANIM *Ani )
{
  Uni->CamLoc = VecSet(8, 8, 8);
  Uni->CamDir = VecSet(3, 3, 3);
  Uni->CamAt = VecSet(0, 0, 0);
  Uni->Speed = 10;
}
static VOID TK6_UnitClose( tk6UNIT_CONTROL *Uni, tk6ANIM *Ani )
{
}

static VOID TK6_UnitResponse( tk6UNIT_CONTROL *Uni, tk6ANIM *Ani )
{
  VEC d;
 
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  if (Ani->KeysClick[VK_ESCAPE])
    TK6_AnimExit();

  d = VecNormalize(VecSubVec(Uni->CamAt, Uni->CamLoc));;
  //Uni->CamLoc += Uni->CamDir * Uni->Speed * d;

  Uni->CamLoc = 
    VecAddVec(Uni->CamLoc,
    VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));

  TK6_RndCamSet(Uni->CamLoc, Uni->CamAt, VecSet(0, 1, 0));

}



static VOID TK6_UnitRender( tk6UNIT_CONTROL *Uni, tk6ANIM *Ani )
{
  CHAR Buf[102];
  static DBL OldTime;
  
  if (Ani->GlobalTime - OldTime > 2)
  {
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
    OldTime = Ani->GlobalTime;
  }
}

tk6UNIT * TK6_UnitCreateControl( VOID )
{
  tk6UNIT_CONTROL *Uni;
 
  Uni = (tk6UNIT_CONTROL *)TK6_AnimUnitCreate(sizeof(tk6UNIT_CONTROL));
  if (Uni == NULL)
    return NULL;
  Uni->Init = (VOID *)TK6_UnitInit;
  Uni->Close = (VOID *)TK6_UnitClose;
  Uni->Render = (VOID *)TK6_UnitRender;
  return (tk6UNIT *)Uni;
}
