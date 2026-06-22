/* Kurnosova Tatuana, 10-6, 20.06.2026 */

#include "units.h"

typedef struct tagtk6UNIT_G3DM tk6UNIT_G3DM;

struct tagtk6UNIT_G3DM
{
  UNIT_BASE_FIELDS;
  tk6PRIMS Models;
  VEC Pos;
};
 
static VOID TK6_UnitInit( tk6UNIT_G3DM *Uni, tk6ANIM *Ani )
{
  Uni->Pos = VecSet(0, 0, 0);
  TK6_RndPrimsLoad(&Uni->Models, "bin/models/Mi8_old.g3dm");
}

static VOID TK6_UnitClose( tk6UNIT_G3DM *Uni, tk6ANIM *Ani )
{
  TK6_RndPrimsFree(&Uni->Models);
}

static VOID TK6_UnitResponse( tk6UNIT_G3DM *Uni, tk6ANIM *Ani )
{
  //Uni->Pos = VecAddVec(Uni->Pos, VecSet(0.1 * Ani->Mdx, 0.1 * Ani->Mdy, 0.1 * Ani->Mdz));
}

static VOID TK6_UnitRender( tk6UNIT_G3DM *Uni, tk6ANIM *Ani )
{
  TK6_RndPrimsDraw(&Uni->Models, MatrTranslate(Uni->Pos));
}

tk6UNIT * TK6_UnitCreateG3DM( VOID )
{
  tk6UNIT_G3DM *Uni;

  Uni = (tk6UNIT_G3DM *)TK6_AnimUnitCreate(sizeof(tk6UNIT_G3DM));
  if (Uni == NULL)
    return NULL;
  Uni->Init = (VOID *)TK6_UnitInit;
  Uni->Close = (VOID *)TK6_UnitClose;
  Uni->Render = (VOID *)TK6_UnitRender;
  return (tk6UNIT *)Uni;
}