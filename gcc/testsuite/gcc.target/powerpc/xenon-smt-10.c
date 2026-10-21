/* -mxenon-smt and the vectorizer (plans 5.4 and 5.5).  With a busy sibling
   the vectorizer's own unrolling assumes half the issue width (one vector
   add per iteration: VF 4 x 2 instead of x 4; xenon-vect-cost-1.c has the
   idle x 4), and a transfer through memory
   (vec_to_scalar, the reduction result) costs 40 cycles of latency, which
   is rnd (40 * 100 / 191) = 21 throughput-bound issue cycles of this
   thread.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -mxenon-smt=busy -ffast-math -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump {18:\d+: optimized: loop vectorized using 16 byte vectors and unroll factor 8} "vect" } } */
/* { dg-final { scan-tree-dump {vec_to_scalar costs 21 in epilogue} "vect" } } */

#define N 1024
float a[N], b[N], c[N];

void add (void)
{
  for (int i = 0; i < N; i++)
    a[i] = b[i] + c[i];
}

float sum (void)
{
  float s = 0;
  for (int i = 0; i < N; i++)
    s += a[i];
  return s;
}
