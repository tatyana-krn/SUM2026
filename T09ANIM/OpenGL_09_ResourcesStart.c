
ѕостроение наборов ресурсов дл€ графического вывода
---> Resourses
Resourses Stocks

–есурсы: ћј——»¬џ √ЋќЅјЋ№Ќџ’ ƒјЌЌџ’ ƒЋя ќ—¬≈ў≈Ќ»я » ѕќ—“–ќ≈Ќ»я —÷≈Ќџ


/* *** stock */
#define VG4_MAX_*** ???
vg4***** VG4_Rnd***[VG4_MAX_***];
INT VG4_Rnd***Size;

VOID VG4_Rnd***Init( VOID );
VOID VG4_Rnd***Close( VOID );

  Shaders     Shd
  Textures    Tex
  Materials   Mtl

–есурсы шейдеров
1.Shaders:
src/anim/rnd/res/rndshd.c Ц реализаци€ шейдеров


- ’ранение шейдеров

#define VG4_STR_MAX 300

/* Shader program store type */
typedef struct tagvg4SHADER
{
  CHAR Name[VG4_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} vg4SHADER;

/* Shaders stock */
#define VG4_MAX_SHADERS 30
vg4SHADER VG4_RndShaders[VG4_MAX_SHADERS]; /* Array of shaders */
INT VG4_RndShadersSize;                    /* Shadres array store size */

!!! функции: ƒобавить, ќчистить все, ќбновить
VOID VG4_RndShdInit( VOID ); Ц загрузка шейдера по умолчанию (/BIN/SHADERS/DEFAULT/ ***.GLSL)
VOID VG4_RndShdClose( VOID ); Ц выгрузка всех шейдеров

INT VG4_RndShdAdd( CHAR *ShaderFileNamePrefix ); - запоминает в массив
  -- возвращает номер добавленного шейдера в массиве (RndShaders)
VOID VG4_RndShdUpdate( VOID ); - обновл€ет все шейдера

-- уже есть
static UINT VG4_RndShdLoad( CHAR *ShaderFileNamePrefix ); - загрузка шейдера из файла
static VOID VG4_RndShdFree( UINT ProgId ); - удаление шейдера

  -- вспомогательные функции:
static CHAR * VG4_RndShdLoadTextFromFile( CHAR *FileName ); - загрузка текста
static VOID VG4_RndShdLog( CHAR *ShaderName, CHAR *PartName, CHAR *Text ); - запись лога

–есурсы текстур
2.Textures:
src/anim/rnd/res/rndtex.c

“ип представлени€:
/* Texture representation type */
typedef struct tagvg4TEXTURE
{
  CHAR Name[VG4_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} vg4TEXTURE;

—клад хранени€:

/* Textures stock */
#define VG4_MAX_TEXTURES 3000
vg4TEXTURE VG4_RndTextures[VG4_MAX_TEXTURES]; /* Array of textures */
INT VG4_RndTexturesSize;                      /* Textures array store size */

VG4_RndTexInit( VOID ); Ц инициализаци€ массива
VG4_RndTexClose( VOID ); Ц выгрузка всех текстур

INT VG4_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
  -- текстура из картинки
  -- возвращает номер добавленной текстуры в массиве (RndTextures)

??? INT VG4_RndTexAdd( CHAR *FileName ); - запоминает в массив
  - текстура из файла

–есурсы материалов
3.Materials:
src/anim/rnd/res/rndmtl.с Ц реализаци€ материалов

- у каждого примитива добавим материал Ц свойство поверхности дл€ тонировани€ (через шейдер), освещени€ и т.п.:

у каждого примитива добавим поле:
vg4PRIM:
{
  . . .
  INT MtlNo; /* Material number in material array */
}

/* Material store type */
typedef struct tagvg4MATERIAL
{
  CHAR Name[VG4_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */

  FLT Trans;                /* Transparency factor */

  INT Tex[8];               /* Texture references from texture table (or -1) */

  INT ShdNo;                /* Shader number in shader table */
} vg4MATERIAL;

/* Material stock */
#define VG4_MAX_MATERIALS 300
vg4MATERIAL VG4_RndMaterials[VG4_MAX_MATERIALS]; /* Array of materials */
INT VG4_RndMaterialsSize;                        /* Materials array */

!!! насильно добавим материал по умолчанию (RndMtlInit) Ц 0-й Ц "DEFAULT"

vg4MATERIAL VG4_RndMtlGetDef( VOID )
{
  vg4MATERIAL def_mtl =
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

VOID VG4_RndMtlInit( VOID ); Ц создани€ материала по умолчанию
например:
{
  vg4MATERIAL def = VG4_RndMtlGetDef();

  VG4_RndMaterialsSize = 0;
  VG4_RndMtlAdd(&def);
}
VOID VG4_RndMtlClose( VOID ); Ц обнул€ет размер таблицы


INT VG4_RndMtlAdd( vg4MATERIAL *Mtl );
  -- возвращает номер добавленного материала в массиве (RndMaterials)

  if (VG4_RndMaterialsSize >= VG4_MAX_MATERIALS)
    return 0;
  VG4_RndMaterials[VG4_RndMaterialsSize] = *Mtl;
  return VG4_RndMaterialsSize++;


UINT VG4_RndMtlApply( INT MtlNo );
  как ее написать (возвращает ProgId от шейдера):
{
  UINT prg;
  vg4MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= VG4_RndMaterialsSize)
    MtlNo = 0;
  mtl = &VG4_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= VG4_RndShadersSize)
    prg = 0;
  prg = VG4_RndShaders[prg].ProgId;

  if (prg == 0)
    return 0;

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, VG4_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, VG4_Anim.GlobalTime);

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

  . . .

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
    if (mtl->Tex[i] != -1 && mtl->Tex[i] >= 0 && mtl->Tex[i] < VG4_RndTexturesSize)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, VG4_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
    if ((loc = glGetUniformLocation(prg, tnamew)) != -1)
      glUniform1f(loc, VG4_RndTextures[mtl->Tex[i]].W);
    if ((loc = glGetUniformLocation(prg, tnameh)) != -1)
      glUniform1f(loc, VG4_RndTextures[mtl->Tex[i]].H);
  }
  return prg;
}

PrimDraw:
  MATR
    w = MatrMulMatr(Pr->Trans, World),
    winv = MatrTranspose(MatrInverse(w)),
    wvp = MatrMulMatr(w, VG4_RndMatrVP);
  UINT ProgId;
  . . .
  if ((ProgId = VG4_RndMtlApply(Pr->MtlNo)) == 0)
    return;

  /* Pass render uniforms */
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, w.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);

  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1) !!! VEC VG4_RndCamLoc
    glUniform3fv(loc, 1, &VG4_RndCamLoc.X);
  . . .
  glUseProgram(0);

static struct
{
  char *Name;
  float amb[3], dif[3], spec[3], shin;
} MatLib[] =
{
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},               32},
  {"Brass",         {0.329412,0.223529,0.027451}, {0.780392,0.568627,0.113725}, {0.992157,0.941176,0.807843}, 27.8974},
  {"Bronze",        {0.2125,0.1275,0.054},       {0.714,0.4284,0.18144},       {0.393548,0.271906,0.166721},  25.6},
  {"Chrome",        {0.25, 0.25, 0.25},          {0.4, 0.4, 0.4},              {0.774597, 0.774597, 0.774597}, 76.8},
  {"Copper",        {0.19125,0.0735,0.0225},     {0.7038,0.27048,0.0828},      {0.256777,0.137622,0.086014},  12.8},
  {"Gold",          {0.24725,0.1995,0.0745},     {0.75164,0.60648,0.22648},    {0.628281,0.555802,0.366065},  51.2},
  {"Peweter",       {0.10588,0.058824,0.113725}, {0.427451,0.470588,0.541176}, {0.3333,0.3333,0.521569},      9.84615},
  {"Silver",        {0.19225,0.19225,0.19225},   {0.50754,0.50754,0.50754},    {0.508273,0.508273,0.508273},  51.2},
  {"Polished Silver", {0.23125,0.23125,0.23125}, {0.2775,0.2775,0.2775},       {0.773911,0.773911,0.773911},  89.6},
  {"Turquoise",     {0.1, 0.18725, 0.1745},      {0.396, 0.74151, 0.69102},    {0.297254, 0.30829, 0.306678}, 12.8},
  {"Ruby",          {0.1745, 0.01175, 0.01175},  {0.61424, 0.04136, 0.04136},  {0.727811, 0.626959, 0.626959}, 76.8},
  {"Polished Gold", {0.24725, 0.2245, 0.0645},   {0.34615, 0.3143, 0.0903},    {0.797357, 0.723991, 0.208006}, 83.2},
  {"Polished Bronze", {0.25, 0.148, 0.06475},    {0.4, 0.2368, 0.1036},        {0.774597, 0.458561, 0.200621}, 76.8},
  {"Polished Copper", {0.2295, 0.08825, 0.0275}, {0.5508, 0.2118, 0.066},      {0.580594, 0.223257, 0.0695701}, 51.2},
  {"Jade",          {0.135, 0.2225, 0.1575},     {0.135, 0.2225, 0.1575},      {0.316228, 0.316228, 0.316228}, 12.8},
  {"Obsidian",      {0.05375, 0.05, 0.06625},    {0.18275, 0.17, 0.22525},     {0.332741, 0.328634, 0.346435}, 38.4},
  {"Pearl",         {0.25, 0.20725, 0.20725},    {1.0, 0.829, 0.829},          {0.296648, 0.296648, 0.296648}, 11.264},
  {"Emerald",       {0.0215, 0.1745, 0.0215},    {0.07568, 0.61424, 0.07568},  {0.633, 0.727811, 0.633},       76.8},
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},                32.0},
  {"Black Rubber",  {0.02, 0.02, 0.02},          {0.01, 0.01, 0.01},           {0.4, 0.4, 0.4},                10.0},
};
#define MAT_N (sizeof(MatLib) / sizeof(MatLib[0]))
