/* Kurnosova Tatuana, 10-6, 01.06.2026 */
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>


void Session( int A, int B )
{
  int i, M;
  char s[40];
  
  i = MessageBox(NULL, "think a number for 0 to 100", "6", MB_OK | MB_ICONINFORMATION);
  if (A > B)
    M = A, A = B, B = M;
  while (A != B)
  {
    M = (A + B) / 2;
    sprintf(s, "less - yes; more - no; = - cancel\n%i?", M);
    i = MessageBox(NULL, s, "TK6", MB_YESNOCANCEL | MB_ICONINFORMATION);
    if (i == IDCANCEL)
      A = M, B = M;
    else if (i == IDNO)
      A = M + 1, M = A;
    else if (i == IDYES)
      B = M - 1, M = B;
    
    if (B == A)
      A = M, B = M;
    if (B < A)
      MessageBox(NULL, "stop lying", "6", MB_OK | MB_ICONINFORMATION), B = A, M = B;
  }
  sprintf(s, "Your number is %i", M);
  MessageBox(NULL, s, "TK6", MB_OK | MB_ICONINFORMATION);
  
}

void main( void )
{
  int A = 0, B = 100;

  Session(A, B);
  _getch();
}