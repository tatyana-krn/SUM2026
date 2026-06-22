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
  POINT *pnts;
 
  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;
 
  /* Build vertex projects */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);
 
 
    pnts[i].x = (INT)((p.X + 1) * TK6_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * TK6_RndFrameH / 2);
  }
 
  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(TK6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL); 
    LineTo(TK6_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(TK6_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(TK6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}
 
 
/* Primitive free function.
 * ARGUMENTS:
 *   - primitive to be free:
 *       tk6PRIM *Pr;
 * RETURNS: None.
 */
VOID TK6_RndPrimFree( tk6PRIM *Pr )
{
  free(Pr->V);
  memset(Pr, 0, sizeof(tk6VERTEX));
}


/* Primitive create function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       tk6PRIM *Pr;
 *   - number of vertecis and indices:
 *       INT NoofV, NoofI;
 * RETURNS:
 *   (BOOL) TRUE if success, FLASE otherwise.
 */
BOOL TK6_RndPrimCreate( tk6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;
 
  memset(Pr, 0, sizeof(tk6PRIM));
  size = sizeof(tk6VERTEX) * NoofV + sizeof(INT) * NoofI;
 
  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);
  return TRUE;
}

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
 
  if (!TK6_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;
 
  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      Pr->V[k++].P = VecSet(R * sin(theta) * sin(phi),
                            R * cos(theta),
                            R * sin(theta) * cos(phi));
 
  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }
  return TRUE;
} /* End of 'TK6_RndPrimCreateSphere' function */


/* Create Cilinder primitive function.
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
BOOL TK6_RndPrimCreateCilinder( tk6PRIM *Pr, DBL R, INT W, INT H, INT Y )
{
  INT i, j, k;
  DBL theta, phi;
 
  if (!TK6_RndPrimCreate(Pr, W * H, (H - 1) * (W - 1) * 2 * 3))
    return FALSE;
 
  /* Fill vertex array */
  for (k = 0, i = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1))
      Pr->V[k++].P = VecSet(R * cos(phi),
                            Y,
                            R * sin(phi));
 
  /* Fill vertex array */
  for (k = 0, i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      /* bottom-left */
      Pr->I[k++] = i * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j;
      /* top-right */
      Pr->I[k++] = (i + 1) * W + j;
      Pr->I[k++] = i * W + j + 1;
      Pr->I[k++] = (i + 1) * W + j + 1;
    }
  return TRUE;
} /* End of 'TK6_RndPrimCreateCilinder' function */

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
  INT nv = 0, nf = 0;
  static CHAR Buf[3000];
 
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
 
  if (!TK6_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }
 
  /* Load model */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x / 4, y / 4, z / 4);
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
 
            Pr->I[nf++] = n1;
            Pr->I[nf++] = n2;
            Pr->I[nf++] = n3;
 
            n2 = n3;
          }
          fvn++;
        }
        oldc = *ptr++;
      }
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'TK6_RndPrimLoad' function */