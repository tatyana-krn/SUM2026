        /* Kurnosova Tatuana, 10-6, 040.06.2026 */
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef DOUBLE DBL;

#define N 3
DBL A[N][N];
INT NUM;

INT mas[N];
BOOL IsParity = TRUE;

VOID Swap( INT *A, INT *B )
{
  INT C;

  C = *A;
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


VOID Go( INT Pos )
{
  INT i;
  DBL Determinant = 0, prod = 1;

  if (Pos == N)
  {
    for (prod = 1, i = 0; i < NUM; i++)
      prod *= A[i][mas[i]];
    Determinant += (IsParity * 2 - 1) * prod;
  }
  for (i = Pos; i < N; i++)
  {
    Swap(&mas[Pos], &mas[i]);
    IsParity = !IsParity;
    Go(Pos + 1);
    Swap(&mas[Pos], &mas[i]);   
  }
  IsParity = !IsParity; 
  printf("%d\n", Determinant);
}

VOID Debug( VOID )
{
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
      printf("%10.3f", A[i][j]);
    printf("\n");
  }
}

VOID main( VOID )
{
  INT i;

  for (i = 0; i < N; i++)
    mas[i] = i + 1;
  LoadMatrix("IN.TXT");
  Go(0); 
  _getch();
}

VOID main( VOID )
{
  DBL Det;

  LoadMatrix("b.txt");
  Det = f();

  printf("%f", Det);
  getchar();
}
