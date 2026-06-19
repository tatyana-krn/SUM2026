/* Kurnosova Tatuana, 10-6, 04.06.2026 */
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define N 4

INT mas[N];
BOOL IsParity = TRUE;

VOID Swap( INT *A, INT *B )
{
  INT C;

  C = *A;
  *A = *B;
  *B = C;
}


VOID Store( VOID )
{
  FILE *F;
  INT i;

  F = fopen("PERM.TXT", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N - 1; i++)
    fprintf(F, "%d, ", mas[i]);
  fprintf(F, "%d - parity: %s\n", mas[N - 1], IsParity ? "even" : "odd");
  fclose(F);
}


VOID Go( INT Pos )
{
  INT i;

  if (Pos == N)
  {
    Store();
    return;
  }
  for (i = Pos; i < N; i++)
  {
    Swap(&mas[Pos], &mas[i]);
    IsParity = !IsParity;
    Go(Pos + 1);
    Swap(&mas[Pos], &mas[i]);   
  }
  IsParity = !IsParity;
}

VOID main( VOID )
{
  INT i;

  for (i = 0; i < N; i++)
    mas[i] = i + 1;
  Go(0);  
}



