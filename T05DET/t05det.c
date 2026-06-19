/* Kurnosova Tatuana, 10-6, 040.06.2026 */
#include <stdio.h>
#include <conio.h>
#include <windows.h>

typedef DOUBLE DBL;

#define N 3
DBL Det = 0, A[N][N], prod = 1;
INT NUM, mas[N];
BOOL IsParity = TRUE;


VOID Swap( INT *A, INT *B )
{
  INT C = *A;

  *A = *B;
  *B = C;
}

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  NUM = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &NUM);
  if (NUM < 0)
    NUM = 0;
  else
    if (NUM > N)
      NUM = N;

  for (i = 0; i < NUM; i++)
    for (j = 0; j < NUM; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
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

/// <<< BEPHO 
// HE BEPHO >>>

VOID Go( INT Pos )
{
  INT i;
  
  if (Pos == NUM)
  {
    for (prod = 1, i = 0; i < NUM; i++)
      prod *= A[i][mas[i]];
    Det += prod * (IsParity * 2 - 1);
  }
  for (i = Pos; i < N; i++)
  {
    Swap(&mas[Pos], &mas[i]);
    if (Pos != i)
      IsParity = !IsParity;
    Go(Pos + 1);
    Swap(&mas[Pos], &mas[i]);
    IsParity = !IsParity; 
  }
}

VOID main( VOID )
{
  INT i;

  for (i = 0; i < N; i++)
    mas[i] = i;
  if (LoadMatrix("IN.TXT"))
  {
    Debug();

    Go(0); 
    printf(" Determinant : %d\n", Det);  
  }
  _getch();
}
