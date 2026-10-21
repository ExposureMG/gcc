/* Xenon vectorizer costs (plan 5.5): building a vector from scalars goes
   through memory: one store per element and a vector load that hits them,
   VEC_XFER 40 + 3 * VEC_CONSTRUCT_ELT 3 = 49 for four lanes.  A loop with a
   runtime stride is vectorized only in its stride-1 version (the
   element-wise version "will never be profitable"); Cell tuning (generic
   costs, vec_construct 5) vectorizes the float element-wise version too.
   The scalar side of a variable shift costs slw's 16 cycles (microcoded,
   FINDINGS 2.4) against 1 for vslw, so it is vectorized.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump {b\[_1\] 1 times vec_construct costs 49 in body} "vect" } } */
/* { dg-final { scan-tree-dump {fb\[_1\] 1 times vec_construct costs 49 in body} "vect" } } */
/* { dg-final { scan-tree-dump {<< _3 1 times scalar_stmt costs 16 in prologue} "vect" } } */
/* { dg-final { scan-tree-dump {not vectorized: vector version will never be profitable} "vect" } } */
/* { dg-final { scan-tree-dump-times {optimized: loop vectorized} 3 "vect" } } */

#define N 1024
int a[N], b[64 * N], c[N];
float fa[N], fb[64 * N];

void sv (int s)
{
  for (int i = 0; i < N; i++)
    a[i] = b[i * s] + 1;
}

void fsv (int s)
{
  for (int i = 0; i < N; i++)
    fa[i] = fb[i * s] * 2.0f;
}

void shl (void)
{
  for (int i = 0; i < N; i++)
    a[i] = c[i] << (b[i] & 31);
}
