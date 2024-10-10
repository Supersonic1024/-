#include <stdio.h>
#include "defs.h"

/* Routine for computing C = A * B + C */
void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
  int i, j, p;

  for (j = 0; j < n; j++) /* Loop over the rows of C */
  {
    for (p = 0; p < k; p++) /* Loop over the columns of C */
    {
      for (i = 0; i < m; i++)
      { /* Update C( i,j ) with the inner product of the ith row of A and the jth column of B */
        C(i, j) = C(i, j) + A(i, p) * B(p, j);
      }
    }
  }
}