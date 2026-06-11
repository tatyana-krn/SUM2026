/* Kurnosova Tatuana, 10-6, 09.06.2026 */
#include "anim.h"



DOUBLE
      TK6_GlobalTime, TK6_GlobalDeltaTime, /* Global time and interframe interval */
      TK6_Time, TK6_DeltaTime,             /* Time with pause and interframe interval */
      TK6_FPS;                             /* Frames per second value */
BOOL
    TK6_IsPause;                           /* Pause flag */


static UINT64
    StartTime,   
    OldTime,     
    OldTimeFPS,  
    PauseTime,   
    TimePerSec,  
    FrameCounter;

VOID TK6_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;

  QueryPerformanceCounter(&t);
  StartTime = t.QuadPart;

  PauseTime = 0;
  FrameCounter = 0;
  TK6_IsPause = FALSE;
  TK6_Time = TK6_DeltaTime = 0;
  TK6_FPS = 30;
}

VOID TK6_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);

  /* Global time */
  TK6_GlobalTime = (DOUBLE)(t.QuadPart - StartTime) / TimePerSec;
  TK6_GlobalDeltaTime = (DOUBLE)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (!TK6_IsPause)
  {
    TK6_Time = (DOUBLE)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
    TK6_DeltaTime = TK6_DeltaTime;
  }
  else
  {
    TK6_DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }

  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > 3 * TimePerSec)
  {
    TK6_FPS = FrameCounter * TimePerSec / (DOUBLE)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}