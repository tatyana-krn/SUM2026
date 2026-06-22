/* Kurnosova Tatuana, 10-6, 040.06.2026 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

typedef DOUBLE DBL;

#define MAX 7
DBL A[MAX][MAX];
INT P[MAX], N = 0;
DBL Det = 0;
BOOL IsParity = TRUE;



VOID Swap( INT *A, INT *B )
{
  INT C = *A;

  *A = *B;
  *B = C;
}

VOID Debug( VOID )
{
  INT i, j;

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
      printf("%10.3f", A[i][j]);
    printf("\n");
  }
}

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &N);
  if (N < 0)
    N = 0;
  else
    if (N > MAX)
      N = MAX;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
}

VOID Go( INT Pos )
{
  INT i;
  DBL prod;

  if (Pos == N)
  {
    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += prod * (IsParity ? 1 : -1);
    return;
  }
  else
    for (i = Pos; i < N; i++)
    {
      if (i != Pos)
      {
        Swap(&P[Pos], &P[i]);
        IsParity = !IsParity;
      }
      Go(Pos + 1);
      if (i != Pos)
      {
        Swap(&P[Pos], &P[i]);
        IsParity = !IsParity;
      }
    }
}


VOID main( VOID )
{
  INT i;

  for (i = 0; i < MAX; i++)
    P[i] = i;

  if (!LoadMatrix("IN.txt"))
  {
    printf("fail\n");
  }
  else
  {
    Debug();
    Go(0);
    printf("Solution : %lf", Det);
  }
  _getch();
}