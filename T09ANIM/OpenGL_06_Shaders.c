Шейдеры и программируемый графический конвейер
Shaders (шейдеры)

Графический конвейер OpenGL

Fixed Graphics Pipeline – Programmable Graphics Pipeline
Fixed:
 

Programmable:

 

Vertex data ->
  Vertex shader ->
    Tessellation control shader ->
      Tessellation evaluation shader ->
        Geometry shader ->
          Rasterizer (assembling primitives, rasterization) ->
            Fragment shader ->
              Raster operation (stencil, alpha, scissor, depth, blending) ->
                Framebuffer

Geometry Rendering Data Flow




















CPU -> Vertex attributes -> VS
-- передаем для каждой вершины набор атрибутов (позиция, цвет) через VBO
shader -> Varying variables -> shader
  -- переменные от каждой вершины могут быть переданы для дальнейшей обработки, например, после вершинного шейдера они интерполируются от вершины к вершине во время растеризации и стоятся для каждого пикселя и приходят во фрагментный шейдер, от фрагментного шейдера выходные данные поступают в буфер кадр
Uniform variables – globals
  -- глобальные переменные, доступные во всех шейдерах одной шейдерно программы (например, матрица преобразований, время, текстурные сэмплеры, параметры освещения и т.п.)

Основы GLSL – языка программирования шейдеров OpenGL
Основы GLSL
есть еще CG (nVidia, C for Graphics), HLSL (Microsoft Direct 3D, High Level Shader Language)
все (почти) как в Си

Базовые типы:
  bool – true | false
  int, uint
  sampler (sampler1D, sampler 2D, sampler3D, samplerCube)
  float
  double (???)

структурные:
  Vectors:
    bvec2, bvec3, bvec4 (bool)
    ivec2, ivec3, ivec4 (int)
    uvec2, uvec3, uvec4 (uint)
    vec2, vec3, vec4    (float)
    dvec2, dvec3, dvec4 (double)

  Matrices (float):
    mat2, mat3, mat4 (2x2, 3x3, 4x4)
    mat2x3, mat2x4, mat3x2, mat3x4, mat4x2, mat4x3 (rows x columns)
  Matrices (double):
    dmat2, dmat3, dmat4
    dmat2x3, dmat2x4, dmat3x2, dmat3x4, dmat4x2, dmat4x3

Запись числовых констант:
  1.5 – float
  1.5f – float
  1.5lf – double

Инициализация:
  float a = 13.47;
  bool is_space = false;
  ivec3 a = ivec3(1, 2, 3);
  uvec3 b = uvec3(1, -2, 3);
  vec3 v = vec3(1.0, 2.0, 3);
  vec4 v1 = vec4(1.0, vec3(4, 5, 6));
  vec4 v2 = vec4(vec3(4, 5, 6), 8);
  vec4 v3 = vec4(vec2(4, 5), vec2(13, 8));
  vec4 v4 = vec4(0);    <=>    vec4(0, 0, 0, 0);


заполнение матрицы по столбцам:
  mat3 m = mat3(vec3(1, 2, 3),
                vec3(3, 4, 5),
                vec3(6, 7, 8));

  mat4 Translate = mat4(vec4(1, 0, 0, 0),
                        vec4(0, 1, 0, 0),
                        vec4(0, 0, 1, 0),
                        vec4(dx, dy, dz, 1));

Доступ к компонентам векторов:
  vec2 p;

  p[0] = 1.0;   <-- x
  p.x = 1;      <=> p.r = 1;
  p.y = 3.3;

любой вектор (vec4 и т.п.) – это набор полей:
  {x, y, z, w} - геометрия
  {r, g, b, a} - цвет
  {s, t, p, q} – текстурирование

  p.x == p.r == p.s

swizzle:
  vec3 v = p.xxy;  <=> vec3(p.x, p.x, p.y);
  p.xy = p.yx;
  vec4 c = v.rrrr;
  vec4 r = v.rryy;
  vec3 color;
  color.rg = vec2(0, 1);
  color.b = 0;

Доступ к компонентам матриц:
  mat4 m;

  m[0] = vec4(1); -- весь первый столбец в 1
  m[2][1] = 30.50;

mat4 MatrTranslate( vec3 T )
{
  return mat4(vec4(1, 0, 0, 0),
              vec4(0, 1, 0, 0),
              vec4(0, 0, 1, 0),
              vec4(T.x, T.y, T.z, 1));
}

void main( void )
{
  mat4 m = MatrTranslate(vec3(0, 0, -5));
  vec4 p = m * vec4(1, 0, 0, 1);
  vec3 p1 = (m * vec4(1, 2, 3, 1)).xyz;
}

Операции:
  mat3 T, R, M;
  vec3 v, b;
  float f;

  b = v + f;    (b.x = v.x + f, b.y = v.y + f, ... )

  !!! используется нотация вектор-столбец:
  b = M * v;
  M = R * T;
  b = R * T * v;

преобразования скалярных типов:
  int a = int(47.13);
  bool c = false;
  float x = float(c);  <-- 0.0

Агрегатные типы:
  структуры:
  struct Light
  {
    vec3 Pos;
    vec4 Color;
    float Attenuation;
  };
. . .
  Light L = Light(vec3(8, 8, 8), vec4(1, 0, 0, 1), 0.47);

  L = Light(vec3(8, 18, 8), vec4(1, 0, 0.8, 1), 0.47);

  массивы:
  Light ls[10];

  for (int i = 0; i < 10; i++)
  {
    ls[i].Attenuation = i / 10.0;
  }
  for (int i = 0; i < ls.length(); i++)
    . . .
  vec4 v;
  v.length() --> 4

Препроцессор:
#error
#pragma
#version
#define
#if #ifdef #ifndef #else #endif #elif

!!! SPIR-V

Точка входа:

void main( void )
{
}

Выход из фрагментного шейдера с отменой вывода пикселя (фрагмента):
discard;

Информация про действия с векторами:

float t = dot(V1, V2);
vec3 V3 = cross(V1, V2);

T X = max(X1, X2)
T X = min(X1, X2)

T X = clamp(X1, A, B)   <=> X1 < A ? A : X1 > B : B : X1 <=> min(max(X1, A), B)
T X = mix(A, B, t)      <=> A * (1 – t) + B * t       (lerp)

sin cos tan asin acos atan(x) atan(y, x) pow(x, y) log exp sqrt inversesqrt
abs sign floor round trunc ceil mod

float t = length(V1)
float t = distance(P1, P2)
vec V = normalize(V1)





vec V = faceforward(N, I, Nref)      dot(I, Nref) < 0 ? N : -N








v = I + dot(-I, N) * N
R = -I + 2 * v = -I + 2 * I + 2 * dot(-I, N) * N
R = I – 2 * dot(N, I) * N

vec V = reflect(I, N)             I – 2 * dot(N, I) * N

mat M;
mat T = inverse(M)
mat T = transpose(M)
float d = determinant(M)

Использование и загрузка шейдеров
Основы использования шейдверов на GLSL

Примеры шейдеров:

!!! соглашения:
шейдера одной шейдерной программ располагаются в отдельной директории и называются одинаково:
/bin/shaders/default/
  vert.glsl
  frag.glsl
/bin/shaders/land/
  vert.glsl
  frag.glsl
/bin/shaders/sphere/
  vert.glsl
  geom.glsl
  frag.glsl

пример default:

vert.glsl
// версия языка шейдера (3.3)
#version 330

// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// глобальные переменные (произведение матриц: World * View * Proj)
uniform mat4 MatrWVP;

// выходные параметры (varying)
out vec4 DrawColor;

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
}
frag.glsl
// версия языка шейдера (3.3)
#version 330

// выходные параметры - цвет рисования
layout(location = 0) out vec4 OutColor;

// входные параметры (varying)
in vec4 DrawColor;

void main( void )
{
  OutColor = DrawColor;
}

Загрузка шейдеров
  "bin/shaders/" + FileNamePrefix + "/vert.glsl"
  load text + create vertex shader
  attach source & compile
  "bin/shaders/" + FileNamePrefix + "frag.glsl"
  load text + create fragment shader
  attach source & compile

  create program
  attach shaders
  link
  --> ProgId – целочисленный номер шейдерной программы

Подключение при рисовании
  MATR wvp = MatrMulMatr(World, MatrMulMatr(VG4_RndMatrView, VG4_RndMatrProj));
  UINT ProgId = VG4_RndShaders[0].ProgId;
  INT loc;

  glUseProgram(ProgId);
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, VG4_Anim.Time);
. . .
  glUseProgram(0); 

Интеграция в проект:

Модули:
  /src/anim/rnd/res/
     В проекте: Source Files->Animation System->Render System->Resources

    rndres.h
    rndres.c
    rndshd.c

Модуль для работы с шейдерами (rndshd.c):

-= 13.06.2025 =-
::10:00
Загрузка шейдеров:
src/anim/rnd/res/rndshd.c – реализация, объявления -  src/anim/rnd/res/rndres.h

/***
 * Shaders support
 ***/

/* Shader program store type */
typedef struct tagvg4SHADER
{
  CHAR Name[VG4_STR_MAX]; /* Shader filename prefix */
  UINT ProgId;            /* Shader program Id */
} vg4SHADER;

/* Shaders stock maximum size */
#define VG4_MAX_SHADERS 30

/* Array of shaders */
extern vg4SHADER VG4_RndShaders[VG4_MAX_SHADERS];
/* Shadres array store size */
extern INT VG4_RndShadersSize;

/***
 * Base shaders functions
 ***/

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
static VOID VG4_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("bin/shaders/shd{30}vg4.log", "a")) == NULL)
    return;
  fprintf(F, "%s : %s\n%s\n\n", FileNamePrefix, ShaderName, Text);
  fclose(F);
} /* End of 'VG4_RndLoadTextFromFile' function */

/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */
static CHAR * VG4_RndLoadTextFromFile( CHAR *FileName )
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
} /* End of 'VG4_RndLoadTextFromFile' function */

/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (UINT) load shader program Id.
 */
static UINT VG4_RndShdLoad( CHAR *FileNamePrefix )
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
  INT res, i, NoofS = sizeof(shd) / sizeof(shd[0]);
  UINT prg;
  BOOL is_ok = TRUE;
  static CHAR Buf[10000];

  for (i = 0; i < NoofS; i++)
  {
    /* Build shader name */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shd[i].Name);

    /* Load shader text from file */
    txt = VG4_RndLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      VG4_RndShdLog(FileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    shd[i].Id = glCreateShader(shd[i].Type);
    if (shd[i].Id == 0)
    {
      free(txt);
      VG4_RndShdLog(FileNamePrefix, shd[i].Name, "Error shader create");
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
      VG4_RndShdLog(FileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  /* Create program */
  if (is_ok)
    if ((prg = glCreateProgram()) == 0)
    {
      VG4_RndShdLog(FileNamePrefix, "PROG", "Error create program");
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
      glGetProgramiv(shd[i].Id, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
        VG4_RndShdLog(FileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }

  /* Handle errors */
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
} /* End of 'VG4_RndShdLoad' function */

/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT ProgId;
 * RETUNS: None.
 */
static VOID VG4_RndShdFree( UINT ProgId )
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
} /* End of 'VG4_RndShdFree' function */

/***
 * Shaders stock functions
 ***/

/* Array of shaders */
vg4SHADER VG4_RndShaders[VG4_MAX_SHADERS];

/* Shadres array store size */
INT VG4_RndShadersSize;

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load:
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT VG4_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;

  for (i = 0; i < VG4_RndShadersSize; i++)
    if (strcmp(ShaderFileNamePrefix, VG4_RndShaders[i].Name) == 0)
      return i;
  if (VG4_RndShadersSize >= VG4_MAX_SHADERS)
    return 0;
  strncpy(VG4_RndShaders[VG4_RndShadersSize].Name, ShaderFileNamePrefix, VG4_STR_MAX - 1);
  VG4_RndShaders[VG4_RndShadersSize].ProgId = VG4_RndShdLoad(ShaderFileNamePrefix);
  return VG4_RndShadersSize++;
} /* End of 'VG4_RndShdAdd' function */

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < VG4_RndShadersSize; i++)
  {
    VG4_RndShdFree(VG4_RndShaders[i].ProgId);
    VG4_RndShaders[i].ProgId = VG4_RndShdLoad(VG4_RndShaders[i].Name);
  }
} /* End of 'VG4_RndShdUpdate' function */

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndShdInit( VOID )
{
  VG4_RndShdAdd("default");
} /* End of 'VG4_RndShdInit' function */

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < VG4_RndShadersSize; i++)
    VG4_RndShdFree(VG4_RndShaders[i].ProgId);
  VG4_RndShadersSize = 0;
} /* End of 'VG4_RndShdClose' function */
