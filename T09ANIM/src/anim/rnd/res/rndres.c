#include "rndres.h"

VOID TK6_RndResInit( VOID )
{
  TK6_RndShdInit();
  TK6_RndTexInit();
  TK6_RndMtlInit();
}

VOID TK6_RndResClose( VOID )
{
  TK6_RndMtlClose();
  TK6_RndTexClose();
  TK6_RndShdClose();
}
