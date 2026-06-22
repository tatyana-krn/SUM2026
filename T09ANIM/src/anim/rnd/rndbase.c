/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include "rnd.h"

#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")



VOID TK6_RndInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };

  TK6_hRndWnd = hWnd;
 
  /* Prepare frame compatible device contesxt */
  TK6_hRndDC = GetDC(hWnd);
 
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(TK6_hRndDC, &pfd);
 
  DescribePixelFormat(TK6_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(TK6_hRndDC, i, &pfd);
 
  /* OpenGL init: rendering context setup */
  TK6_hRndGLRC = wglCreateContext(TK6_hRndDC);
  wglMakeCurrent(TK6_hRndDC, TK6_hRndGLRC);
 
  if (glewInit() != GLEW_OK)
    exit(0);
 
  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(TK6_hRndDC, PixelAttribs, NULL, 1, &i, &i);//
  if ((hRC = wglCreateContextAttribsARB(TK6_hRndDC, NULL, ContextAttribs)) != NULL)
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(TK6_hRndGLRC);
    TK6_hRndGLRC = hRC;
    wglMakeCurrent(TK6_hRndDC, TK6_hRndGLRC);
  }

  #ifndef NDEBUG
    OutputDebugString(glGetString(GL_VERSION));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_VENDOR));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_RENDERER));
    OutputDebugString("\n");
  #endif /* NDEBUG */
  
  /* Render parameters setup */
  glEnable(GL_DEPTH_TEST);
 
  TK6_RndProjSize = 0.1;
  TK6_RndProjDist = TK6_RndProjSize;
  TK6_RndProjFarClip = 300;
  TK6_RndFrameW = 47;
  TK6_RndFrameH = 47;
  TK6_RndCamSet(VecSet(12, 12, 12), VecSet(0, 0, 0), VecSet(0, 1, 0));

  TK6_RndResInit();
}

VOID TK6_RndClose( VOID )
{
  TK6_RndResClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(TK6_hRndGLRC);
  ReleaseDC(TK6_hRndWnd, TK6_hRndDC);
}

VOID TK6_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);
 
  /* Setup projection */
  TK6_RndFrameW = W;
  TK6_RndFrameH = H;
  TK6_RndProjSet();
}

VOID TK6_RndCopyFrame( VOID )
{
  SwapBuffers(TK6_hRndDC);
}

VOID TK6_RndStart( VOID )
{
  VEC4 ClearColor = {0.20, 0.77, 0.87, 1};
  FLT DepthClearValue = 1;

  TK6_RndShdUpdate();

  /* Clear frame */
  glClearBufferfv(GL_COLOR, 0, &ClearColor.X);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);
}

VOID TK6_RndEnd( VOID )
{
  glFinish();
}


VOID TK6_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = TK6_RndProjSize;
  /* Correct aspect ratio */
  if (TK6_RndFrameW > TK6_RndFrameH)
    rx *= (DBL)TK6_RndFrameW / TK6_RndFrameH;
  else
    ry *= (DBL)TK6_RndFrameH / TK6_RndFrameW;
  TK6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      TK6_RndProjDist, TK6_RndProjFarClip);
  TK6_RndMatrVP = MatrMulMatr(TK6_RndMatrView, TK6_RndMatrProj);
}
 
 
VOID TK6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  TK6_RndMatrView = MatrView(Loc, At, Up);
  TK6_RndMatrVP = MatrMulMatr(TK6_RndMatrView, TK6_RndMatrProj);

  TK6_RndCamLoc = Loc;
}