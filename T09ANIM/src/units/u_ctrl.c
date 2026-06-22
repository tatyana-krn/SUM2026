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
  Uni->Speed = 7;
}
static VOID TK6_UnitClose( tk6UNIT_CONTROL *Uni, tk6ANIM *Ani )
{
}

static VOID TK6_UnitResponse( tk6UNIT_CONTROL *Uni, tk6ANIM *Ani )
{
  VEC d;
  INT modes[2];
 
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['W'])
  {
    glGetIntegerv(GL_POLYGON_MODE, modes);

    if (modes[0] == GL_FILL)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (Ani->KeysClick[VK_ESCAPE])
    TK6_AnimExit();

  d = VecNormalize(VecSubVec(Uni->CamAt, Uni->CamLoc));

  Uni->CamLoc = 
    VecAddVec(Uni->CamLoc,
      VecMulNum(d, Ani->GlobalDeltaTime * (Ani->Mdz)));

  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] * 
        Ani->Mdx)); 

  if (Ani->Keys[VK_LBUTTON])
  {
    VEC dir, right;

    Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->Mdx * 0.005));
    
    dir = VecMulNum(VecNormalize(VecSubVec(Uni->CamAt, Uni->CamLoc)), -1);
    right = VecNormalize(VecCrossVec(dir, VecSet(0, 1, 0)));
    Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotate(Ani->Mdy * 0.005, right));
  }
  
  d = VecNormalize(VecSubVec(Uni->CamAt, Uni->CamLoc));;
/*
  Uni->CamLoc = 
    VecAddVec(Uni->CamLoc,
    VecMulNum(d, Ani->GlobalDeltaTime * Uni->Speed * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));
  
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Uni->Speed * Ani->Mdz));
  */
  Uni->CamLoc =
  PointTransform(Uni->CamLoc,
    MatrRotateY(-Uni->Speed * Ani->Mdx * 0.03));

  Uni->CamLoc =
  PointTransform(Uni->CamLoc,
    MatrRotateX(Uni->Speed * Ani->Mdy * 0.03));

  Uni->CamLoc =
  PointTransform(Uni->CamLoc,
    MatrRotateY(-Uni->Speed * Ani->Mdy * 0.03));
  
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(d, Ani->GlobalDeltaTime * (Uni->Speed / 100.0 + Ani->Keys[VK_SHIFT]) *
      (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN] + Ani->Mdz)));

  /*
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(d, Ani->GlobalDeltaTime * (Uni->Speed + 10 * Ani->Keys[VK_SHIFT]) * 10 * Ani->Mdz));

  
    Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
                  Uni->Speed * Ani->Mdx));

    Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(-Ani->Keys[VK_RBUTTON] *
                   Uni->Speed * Ani->Mdx));
  */
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
  Uni->Response = (VOID *)TK6_UnitResponse;

  return (tk6UNIT *)Uni;
}
