/* Kurnosova Tatuana, 10-6, 09.06.2026 */

#include "rnd.h"
#include "stdio.h"
/* Primitive draw function.
 * ARGUMENTS:
 *   - primiktive to be draw:
 *       tk6PRIM *Pr;
 *   - transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */

VOID TK6_RndPrimDraw( tk6PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, TK6_RndMatrVP);
  INT prim_type =
    Pr->Type == TK6_RND_PRIM_LINES ? GL_LINES :
    Pr->Type == TK6_RND_PRIM_TRIMESH ? GL_TRIANGLES :
    GL_POINTS;


  glLoadMatrixf(wvp.A[0]);
 
  glBindVertexArray(Pr->VA);
  
  if (Pr->IBuf == 0)
    glDrawArrays(prim_type, 0, Pr->NumOfElements);
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);

}

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be free:
 *       tk6PRIM *Pr;
 * RETURNS: None.
 */
VOID TK6_RndPrimFree( tk6PRIM *Pr )
{
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->VBuf);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(tk6PRIM));
} /* End of 'TK6_RndPrimFree' function */
 
/* Create primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       tk6PRIM *Pr;
 *   - primitive type:
 *       tk6PRIM_TYPE Type;
 *   - vertex attributes array:
 *       tk6VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 * RETURNS: None.
 */
VOID TK6_RndPrimCreate( tk6PRIM *Pr, tk6PRIM_TYPE Type, tk6VERTEX *V, INT NoofV, INT *Ind, INT NoofI )
{
  memset(Pr, 0, sizeof(tk6PRIM));
  Pr->Trans = MatrIdentity();
  Pr->Type = Type;
 
  glGenVertexArrays(1, &Pr->VA);
 
  /* Vertex data */
  if (V != NULL && NoofV != 0)
  {
    glBindVertexArray(Pr->VA);
    glGenBuffers(1, &Pr->VBuf);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tk6VERTEX) * NoofV, V, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(tk6VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(tk6VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(tk6VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(tk6VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */
 
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
 
    glBindVertexArray(0);
 
    /* Obtain BB */
    if (NoofV > 0)
    {
      INT i;
 
      Pr->MinBB = Pr->MaxBB = V[0].P;
      for (i = 1; i < NoofV; i++)
      {
        Pr->MinBB = VecMinVec(Pr->MinBB, V[i].P);
        Pr->MaxBB = VecMaxVec(Pr->MaxBB, V[i].P);
      }
    }
  }
 
  /* Index data */
  if (Ind != NULL && NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Ind, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;
} /* End of 'TK6_RndPrimCreate' function */

/* Create sphere primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       tk6PRIM *Pr;
 *   - sphere radius:
 *       DBL R;
 *   - split parts counts:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL TK6_RndPrimCreateSphere( tk6PRIM *Pr, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;
  VEC L  = VecNormalize(VecSet(1, -1, -1));
  tk6VERTEX *V;
  INT *Ind, size;
  
  memset(Pr, 0, sizeof(tk6PRIM));

  size = sizeof(tk6VERTEX) * W * H + sizeof(INT) * (H - 1) * (W - 1) * 2 * 3;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + W * H);

  /*
  if (!TK6_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;
  */
  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
    {

      /*
      DBL nl;

      V[k].N = VecSet(sin(theta) * sin(phi),
                          cos(theta),
                          sin(theta) * cos(phi));

      nl = VecDotVec(L, V[k].N);
      if (nl < 0.3)
        nl = 0.1;

      V[k].C = Vec4MulNum(Color, nl);
      */




      V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R * cos(theta),
                            R * sin(theta) * cos(phi));
    }
  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Ind[k++] = i * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j;
      /* top-right */
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j + 1;
      Ind[k++] = (i + 1) * W + j + 1;
    }

  // TK6_RndPrimTriMeshAutoNormals(V, W * H, Ind, (H - 1) * (W - 1));
  TK6_RndPrimCreate(Pr, TK6_RND_PRIM_TRIMESH, V, W * H, Ind, (H - 1) * (W - 1));
  
  
  free(V);
  return TRUE;
} /* End of 'TK6_RndPrimCreateSphere' function */


/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be load:
 *       tk6PRIM *Pr;
 *   - primitve filename (.OBJ):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL TK6_RndPrimLoad( tk6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT i, nv = 0, nf = 0, size;
  VEC L  = VecNormalize(VecSet(1, 1, 1));
  static CHAR Buf[3000];
  tk6VERTEX *V;
  INT *Ind;

  
  memset(Pr, 0, sizeof(tk6PRIM));
 
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  


  /* Count vertices and indices */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';
 
      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
          n++;
        oldc = *ptr++;
      }
 
      nf += n - 2;
    }
  }

  size = sizeof(tk6VERTEX) * nv + sizeof(INT) * nf * 3;
  if ((V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Ind = (INT *)(V + nv);
  memset(V, 0, size);

  /*
  if (!TK6_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }
  */
  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv].C = Vec4Set(0.8, 0.47, 0.29, 1);
      V[nv++].P = VecSet(x, y, z);

    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n, n1, n2, n3;
      INT fvn = 0;
      CHAR *ptr = Buf + 2, oldc = ' ';
 
      while (*ptr != 0)
      {
        if (*ptr != ' ' && oldc == ' ')
        {
          sscanf(ptr, "%d", &n);
          if (n > 0)
            n--;
          else
            if (n < 0)
              n = nv - n;
 
          if (fvn == 0)
            n1 = n;
          else if (fvn == 1)
            n2 = n;
          else
          {
            n3 = n;
 
            Ind[nf++] = n1;
            Ind[nf++] = n2;
            Ind[nf++] = n3;
 
            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }
  fclose(F);
//  TK6_RndPrimTriMeshAutoNormals(V, vn, Ind, nf);
  TK6_RndPrimCreate(Pr, TK6_RND_PRIM_TRIMESH, V, nv, Ind, nf);
  
  
  /*
  for (i = 0; i < Pr->NumOfV; i++)
    Pr->V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    INT
      n0 = Pr->I[i],
      n1 = Pr->I[i + 1],
      n2 = Pr->I[i + 2];
    VEC
      P0 = Pr->V[n0].P,
      P1 = Pr->V[n1].P,
      P2 = Pr->V[n2].P,
      N = VecNormalize(VecCrossVec(VecSubVec(P1, P0), VecSubVec(P2, P0)));

    Pr->V[n0].N = VecAddVec(Pr->V[n0].N, N);
    Pr->V[n1].N = VecAddVec(Pr->V[n1].N, N);
    Pr->V[n2].N = VecAddVec(Pr->V[n2].N, N);
  }
  for (i = 0; i < Pr->NumOfV; i++)
    Pr->V[i].N = VecNormalize(Pr->V[i].N);

  for (i = 0; i < Pr->NumOfV; i++)
  {
    FLT nl  = VecDotVec(Pr->V[i].N, L);

    if (nl < 0.1)
      nl = 0.1;
    //Pr->V[i].C = Vec4Set(Rnd1() * 0.68 * nl + 0.3, 0.12 * nl + 0.20, Rnd1() * 0.67 * nl + 0.3, 1);
    Pr->V[i].C = Vec4Set(0.54 * nl + 0.2, 0.17 * nl + 0.2, 0.80 * nl + 0.2, 1);
  }
  */
  free(V);
  return TRUE;
} /* End of 'TK6_RndPrimLoad' function */
