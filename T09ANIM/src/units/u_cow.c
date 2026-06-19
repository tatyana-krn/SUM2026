/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include <time.h>

#include "units.h"

typedef struct tagtk6UNIT_COW tk6UNIT_COW;

struct tagtk6UNIT_COW
{
  UNIT_BASE_FIELDS;
  tk6PRIM Cow;
  VEC Pos;
  DBL Fast;
};
 
static VOID TK6_UnitInit( tk6UNIT_COW *Uni, tk6ANIM *Ani )
{
  //TK6_RndPrimCreateSphere(&Uni->Cow, 3, 40, 40);

  Uni->Pos = VecSet(Rnd1() * -20, -12, Rnd1() * -20);
  TK6_RndPrimLoad(&Uni->Cow, "bin/models/cow.obj");
  Uni->Fast = rand() % 5 + 3;
}
 
static VOID TK6_UnitClose( tk6UNIT_COW *Uni, tk6ANIM *Ani )
{
  TK6_RndPrimFree(&Uni->Cow);
}

static VOID TK6_UnitResponse( tk6UNIT_COW *Uni, tk6ANIM *Ani )
{
  Uni->Pos = VecSubNum(Uni->Pos, Ani->DeltaTime * 2.4);
}

static VOID TK6_UnitRender( tk6UNIT_COW *Uni, tk6ANIM *Ani )
{
  TK6_RndPrimDraw(&Uni->Cow, MatrMulMatr4(MatrRotateX(0), MatrRotateY(10 * Uni->Fast * Ani->Time), MatrRotateZ(5 * cos(Ani->Time * Uni->Fast + 100)), MatrTranslate(Uni->Pos)));
}

tk6UNIT * TK6_UnitCreateCow( VOID )
{
  tk6UNIT_COW *Uni;
 
  Uni = (tk6UNIT_COW *)TK6_AnimUnitCreate(sizeof(tk6UNIT_COW));
  if (Uni == NULL)
    return NULL;
  Uni->Init = (VOID *)TK6_UnitInit;
  Uni->Close = (VOID *)TK6_UnitClose;
  Uni->Render = (VOID *)TK6_UnitRender;
  return (tk6UNIT *)Uni;
}