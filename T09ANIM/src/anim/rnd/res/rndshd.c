/* FILE NAME  : rndshd.c
 * PROGRAMMER : TK6
 * LAST UPDATE: 13.06.2026
 * PURPOSE    : 3D animation project.
 *              Rendering resources declaration module.
 */
 
#include <stdio.h>
#include <time.h>
 
#include "anim/rnd/rnd.h"
 
/* Global shader */
tk6SHADER TK6_RndShaders[TK6_MAX_SHADERS]; /* Array of shaders */
INT TK6_RndShadersSize;                    /* Shadres array store size */

/* Save log to file function.
 * ARGUMENTS:
 *   - shader prefix:
 *       CHAR *FileNamePrefix;
 *   - shader name:
 *       CHAR *ShaderName;
 *   - error text:
 *       CHAR *Text;
 * RETURNS: None.
 */
static VOID TK6_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text )
{
  FILE *F;
 
  if ((F = fopen("bin/shaders/shd{30}tk6.log", "a")) == NULL)
    return;
  fprintf(F, "%s : %s\n%s\n\n", FileNamePrefix, ShaderName, Text);
  fclose(F);
} /* End of 'TK6_RndLoadTextFromFile' function */
 
/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */
static CHAR * TK6_RndLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *txt;
 
  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;
 
  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
 
  /* Allocate memory */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);
 
  /* Load text */
  rewind(F);
  fread(txt, 1, flen, F);
  fclose(F);
  return txt;
} /* End of 'TK6_RndLoadTextFromFile' function */
 
/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (UINT) load shader program Id.
 */
static UINT TK6_RndShdLoad( CHAR *FileNamePrefix )
{
  CHAR *txt;
  struct
  {
    CHAR *Name;  /* Shader name (e.g. "VERT") */
    INT Type;    /* Shader type (e.g. GL_VERTEX_SHADER) */
    INT Id;      /* Created shader Id */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER, 0},
    {"FRAG", GL_FRAGMENT_SHADER, 0},
  };
  INT i, res, NoofS = sizeof(shd) / sizeof(shd[0]);
  UINT prg;
  BOOL is_ok = TRUE;
  static CHAR Buf[10000];
 
  for (i = 0; i < NoofS; i++)
  {
    /* Build shader file name */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shd[i].Name);
 
    /* Load shader text from file */
    txt = TK6_RndLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      TK6_RndShdLog(FileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    shd[i].Id = glCreateShader(shd[i].Type);
    if (shd[i].Id == 0)
    {
      TK6_RndShdLog(FileNamePrefix, shd[i].Name, "Error create shader");
      is_ok = FALSE;
      break;
    }
 
    /* Send shader source text to OpenGL */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);
 
    /* Compile shader */
    glCompileShader(shd[i].Id);
 
    /* Errors handle */
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      TK6_RndShdLog(FileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }
 
  /* Create program */
  if (is_ok)
    if ((prg = glCreateProgram()) == 0)
    {
      TK6_RndShdLog(FileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shader programs */
      for (i = 0; i < NoofS; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link program */
      glLinkProgram(prg);
      /* Errors handle */
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
        TK6_RndShdLog(FileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }

  /* Error handle */
  if (!is_ok)
  {
    /* Delete all shaders */
    for (i = 0; i < NoofS; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Delete program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'TK6_RndShdLoad' function */
 
/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT ProgId;
 * RETUNS: None.
 */
static VOID TK6_RndShdFree( UINT ProgId )
{
  INT shds[5], n, i;
 
  if (ProgId == 0 || !glIsProgram(ProgId))
    return;
 
  glGetAttachedShaders(ProgId, 5, &n, shds);
  for (i = 0; i < n; i++)
    if (glIsShader(shds[i]))
    {
      glDetachShader(ProgId, shds[i]);
      glDeleteShader(shds[i]);
    }
  glDeleteProgram(ProgId);
} /* End of 'TK6_RndShdFree' function */

/* Shaders initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TK6_RndShdInit( VOID )
{
  TK6_RndShadersSize = 0;
  TK6_RndShdAdd("default");
} /* End of 'TK6_RndResInit' function */
 
/* Shaders deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TK6_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < TK6_RndShadersSize; i++)
    TK6_RndShdFree(TK6_RndShaders[i].ProgId);
  TK6_RndShadersSize = 0;
} /* End of 'TK6_RndResInit' function */

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load:
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT TK6_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;

  for (i = 0; i < TK6_RndShadersSize; i++)
    if (strcmp(ShaderFileNamePrefix, TK6_RndShaders[i].Name) == 0)
      return i;
  if (TK6_RndShadersSize >= TK6_MAX_SHADERS)
    return 0;
  strncpy(TK6_RndShaders[TK6_RndShadersSize].Name, ShaderFileNamePrefix, TK6_STR_MAX - 1);
  TK6_RndShaders[TK6_RndShadersSize].ProgId = TK6_RndShdLoad(ShaderFileNamePrefix);
  return TK6_RndShadersSize++;
} /* End of 'TK6_RndShdAdd' function */

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TK6_RndShdUpdate( VOID )
{
  INT t = clock(), i;
  static INT old_time;
 
  if (t - old_time > 2 * CLOCKS_PER_SEC)
  {
    for (i = 0; i < TK6_RndShadersSize; i++)
    {
      TK6_RndShdFree(TK6_RndShaders[i].ProgId);
      TK6_RndShaders[i].ProgId = TK6_RndShdLoad(TK6_RndShaders[i].Name);
    }
    old_time = t;
  }
} /* End of 'TK6_RndShdUpdate' function */
 
/* END OF 'rndshd.c' FILE */
