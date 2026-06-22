#include "anim/anim.h"
 
tk6ANIM TK6_Anim;

tk6MATERIAL TK6_RndMaterials[TK6_MAX_MATERIALS]; /* Array of materials */
INT TK6_RndMaterialsSize;                        /* Materials array */

tk6MATERIAL TK6_RndMtlGetDef( VOID )
{
  tk6MATERIAL def_mtl =
  {
    "Default",
    {0.1, 0.1, 0.1},
    {0.90, 0.90, 0.90},
    {0.30, 0.30, 0.30},
    30, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };
 
  return def_mtl;
}

tk6MATERIAL * TK6_RndMtlGet( INT MtlNo )
{
  if (MtlNo < 0 || MtlNo >= TK6_RndMaterialsSize)
    MtlNo = 0;
  return &TK6_RndMaterials[MtlNo];
}

VOID TK6_RndMtlInit( VOID )
{
  tk6MATERIAL def = TK6_RndMtlGetDef();
 
  TK6_RndMaterialsSize = 0;
  TK6_RndMtlAdd(&def);
}

VOID TK6_RndMtlClose( VOID )
{
}

INT TK6_RndMtlAdd( tk6MATERIAL *Mtl )
{
  if (TK6_RndMaterialsSize >= TK6_MAX_MATERIALS)
    return 0;
  TK6_RndMaterials[TK6_RndMaterialsSize] = *Mtl;
  return TK6_RndMaterialsSize++;
}

UINT TK6_RndMtlApply( INT MtlNo )
{
  UINT prg;
  tk6MATERIAL *mtl;
  INT i, loc;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= TK6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &TK6_RndMaterials[MtlNo];
 
  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= TK6_RndShadersSize)
    prg = 0;
  prg = TK6_RndShaders[prg].ProgId;
 
  if (prg == 0)
    return 0;
 
  glUseProgram(prg);
 
  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);
 
  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";
    CHAR tnamew[] = "Texture0W";
    CHAR tnameh[] = "Texture0H";
    BOOL IsTex = FALSE;

    tname[9] = '0' + i;
    tnamew[7] = '0' + i;
    tnameh[7] = '0' + i;
    if (mtl->Tex[i] != -1 && mtl->Tex[i] >= 0 && mtl->Tex[i] < TK6_RndTexturesSize)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, TK6_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
    if ((loc = glGetUniformLocation(prg, tnamew)) != -1)
      glUniform1f(loc, TK6_RndTextures[mtl->Tex[i]].W);
    if ((loc = glGetUniformLocation(prg, tnameh)) != -1)
      glUniform1f(loc, TK6_RndTextures[mtl->Tex[i]].H);
  }
  return prg;
}
