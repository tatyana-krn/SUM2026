/* Kurnosova Tatuana, 10-6, 040.06.2026 */
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <windows.h>

typedef DOUBLE DBL;

#define MAX 7
DBL A[MAX][MAX], Det = 0;
INT N;
BOOL IsParity = TRUE;

VOID Swap( DBL *A, DBL *B )
{
  DBL C;

  C = *A;
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

  N = 0;
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

VOID Go( VOID )
{
  INT i, y, x, k, max_r, max_c;
  DBL coef;

  Det = 1;
  for (i = 0; i < N; i++)
  {
    max_r = max_c = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_r][max_c]))
          max_r = y, max_c = x;

    if (A[max_r][max_c] == 0)
    {
      Det = 0;
      break;
    }

    if (max_r != i)
    {
      for (x = i; x < N; x++)
        Swap(&A[max_r][x], &A[i][x]);
      Det = -Det;
    }
    
    if (max_c != i)
    {
      for (y = i; y < N; y++)
        Swap(&A[y][max_c], &A[y][i]);
      Det = -Det;
    }
    for (k = i + 1; k < N; k++)
    {
      coef = A[k][i] / A[i][i];
      A[k][i] = 0;
      for (x = i + 1; x < N; x++)
        A[k][x] -= A[i][x] * coef;
    }

    Det *= A[i][i];
  }
}

VOID main( VOID )
{
  if (!LoadMatrix("IN.txt"))
  {
    printf("fail\n");
  }
  else
  {
    Debug();
    Go();
    printf("Solution (GAUSS): %lf", Det);
  }
  _getch();
}
