/* Kurnosova Tatuana, 10-6, 09.06.2026 */
#ifndef __rndres_h_
#define __rndres_h_ 

#include "def.h"

#define TK6_STR_MAX 300

VOID TK6_RndResInit( VOID );
VOID TK6_RndResClose( VOID );

typedef struct tagtk6SHADER
{
  CHAR Name[TK6_STR_MAX]; /* Shader filename prefix */
  UINT ProgId;            /* Shader program  Id */
} tk6SHADER;

/* Shaders stock maximum size */
#define TK6_MAX_SHADERS 30

/* Array of shaders */
extern tk6SHADER TK6_RndShaders[TK6_MAX_SHADERS];
/* Shadres array store size */
extern INT TK6_RndShadersSize;

VOID TK6_RndShdInit( VOID );
VOID TK6_RndShdClose( VOID );
VOID TK6_RndShdUpdate( VOID );
INT TK6_RndShdAdd( CHAR *ShaderFileNamePrefix );



typedef struct tagtk6TEXTURE
{
  CHAR Name[TK6_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} tk6TEXTURE;

/* Textures stock */
#define TK6_MAX_TEXTURES 3000
extern tk6TEXTURE TK6_RndTextures[TK6_MAX_TEXTURES]; /* Array of textures */
extern INT TK6_RndTexturesSize;                      /* Textures array store size */
 
VOID TK6_RndTexInit( VOID );
VOID TK6_RndTexClose( VOID );
INT TK6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT TK6_RndTexAddFromFile( CHAR *FileName );



typedef struct tagtk6MATERIAL
{
  CHAR Name[TK6_STR_MAX]; /* Material name */
 
  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */
 
  FLT Trans;                /* Transparency factor */
 
  INT Tex[8];               /* Texture references from texture table (or -1) */
 
  INT ShdNo;                /* Shader number in shader table */
} tk6MATERIAL;
 
/* Material stock */
#define TK6_MAX_MATERIALS 300
extern tk6MATERIAL TK6_RndMaterials[TK6_MAX_MATERIALS]; /* Array of materials */
extern INT TK6_RndMaterialsSize;                        /* Materials array */

tk6MATERIAL TK6_RndMtlGetDef( VOID );
VOID TK6_RndMtlInit( VOID );
VOID TK6_RndMtlClose( VOID );
INT TK6_RndMtlAdd( tk6MATERIAL *Mtl );
UINT TK6_RndMtlApply( INT MtlNo );
#endif 