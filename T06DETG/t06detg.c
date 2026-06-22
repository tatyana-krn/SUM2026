/* Kurnosova Tatuana, 10-6, 040.06.2026 */
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef DOUBLE DBL;

#define MAX 3
DBL A[MAX][MAX];
INT N;
BOOL IsParity = TRUE;
DBL Det = 0;

VOID Swap( INT *A, INT *B )
{
  INT C;

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

VOID Go( INT Pos )
{
  INT i;
  DBL prod = 1;

  if (Pos == NUM)
  {
    for (prod = 1, i = 0; i < NUM; i++)
      prod *= A[i][mas[i]];
    Determinant += (IsParity * 2 - 1) * prod;
  }
  else
    for (i = Pos; i < NUM; i++)
    {
      if (i != Pos)
      {
        Swap(&mas[Pos], &mas[i]);
        IsParity = !IsParity;
      }
      Go(Pos + 1);
      if (i != Pos)
      {
        Swap(&mas[Pos], &mas[i]);
        IsParity = !IsParity;
      }
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






/* Swap two double values function */
VOID Swap( DBL *A, DBL *B )
{
  DBL temp;

  temp = *A;
  *A = *B;
  *B = temp;
} /* End of 'Swap' function */

/**/

DBL Detg( VOID )
{
  INT max_row, max_col, i, x, y, k;
  DBL coef, det;

  det = 1;
  for (i = 0; i < N; i++)
  {
    /* look for maximum matrix element */
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_row = y, max_col = x;
    if (A[max_row][max_col] == 0)
    {
      det = 0;
      break;
    }
    /* move max element to [i][i] position */
    if (max_row != i)
    {
      /* Swap max_row and i row (elements/columns: [i..N-1]) */
      for (x = i; x < N; x++)
        Swap(&A[max_row][x], &A[i][x]);
      det = -det;
    }
    if (max_col != i)
    {
      /* Swap max_col and i column (elements/columns: [0..N-1]) */
      for (y = i; y < N; y++)
        Swap(&A[y][max_col], &A[y][i]);
      det = -det;
    }
    /* Subtrack from every row k:[i+1..N-1] row [i] multipled by (A[k][i] / A[i][i]) */
    for (k = i + 1; k < N; k++)
    {
      coef = A[k][i] / A[i][i];
      A[k][i] = 0;
      for (x = i + 1; x < N; x++)
        A[k][x] -= A[i][x] * coef;
    }
    det *= A[i][i];
  }
  return det;
} /* End of '' function */
 

void main( void )
{
  LoadMatrix("IN.txt");

  printf("%lf", Detg());
  //Debugg();
  _getch();
} /* End of 'main' function */


