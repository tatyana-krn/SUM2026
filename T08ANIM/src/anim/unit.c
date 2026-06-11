
#include "anim.h"
/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tk6UNIT *Uni;
 *   - animation context:
 *       tk6ANIM *Ani;
 * RETURNS: None.
 */
static VOID TK6_UnitInit( tk6UNIT *Uni, tk6ANIM *Ani )
{

} /* End of 'TK6_UnitInit' function */
 
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tk6UNIT *Uni;
 *   - animation context:
 *       tk6ANIM *Ani;
 * RETURNS: None.
 */
static VOID TK6_UnitClose( tk6UNIT *Uni, tk6ANIM *Ani )
{
} /* End of 'TK6_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tk6UNIT *Uni;
 *   - animation context:
 *       tk6ANIM *Ani;
 * RETURNS: None.
 */
static VOID TK6_UnitResponse( tk6UNIT *Uni, tk6ANIM *Ani )
{

} /* End of 'TK6_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       tk6UNIT *Uni;
 *   - animation context:
 *       tk6ANIM *Ani;
 * RETURNS: None.
 */
static VOID TK6_UnitRender( tk6UNIT *Uni, tk6ANIM *Ani )
{
} /* End of 'TK6_UnitRender' function */


tk6UNIT * TK6_AnimUnitCreate( INT Size )
{
  tk6UNIT *Uni;
 
  /* Memory allocation */
  if (Size < sizeof(tk6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
 
  /* Setup unit methods */
  Uni->Init = TK6_UnitInit;
  Uni->Close = TK6_UnitClose;
  Uni->Response = TK6_UnitResponse;
  Uni->Render = TK6_UnitRender;
 
  return Uni;
} /* End of 'TK6_AnimUnitCreate' function */
