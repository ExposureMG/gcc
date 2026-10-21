/* Xenon vectorizer costs (plan 5.5): plain float loops are vectorized.  A
   VMX op costs its issue cycle (1, like a scalar FP op; its 12-cycle
   latency is the scheduler's business), a vector load its stream throughput
   2.93 -> 3, a vector store 3.50 -> 4, a scalar float load 2.93 -> 3 and a
   scalar store 2.93 -> 3 (FINDINGS 3.4, generated table VEC_*).  The
   vectorizer's own unrolling keeps the generic 4-wide issue model with an
   idle sibling (xenon-smt-10.c: halved with a busy one).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump-times {optimized: loop vectorized} 5 "vect" } } */
/* { dg-final { scan-tree-dump {1 times vector_load costs 3 in body} "vect" } } */
/* { dg-final { scan-tree-dump {1 times vector_store costs 4 in body} "vect" } } */
/* { dg-final { scan-tree-dump {1 times vector_stmt costs 1 in body} "vect" } } */
/* { dg-final { scan-tree-dump {1 times scalar_load costs 3 in prologue} "vect" } } */
/* { dg-final { scan-tree-dump {1 times scalar_store costs 3 in prologue} "vect" } } */
/* { dg-final { scan-tree-dump {28:\d+: optimized: loop vectorized using 16 byte vectors and unroll factor 16} "vect" } } */
/* { dg-final { scan-assembler {\mvaddfp(128)?\M} } } */
/* { dg-final { scan-assembler {\mvmaddfp(128)?\M|\mvmaddcfp128\M} } } */
/* { dg-final { scan-assembler {\mvsel(128)?\M} } } */
/* { dg-final { scan-assembler {\mvcfsx\M|\mvcsxwfp128\M} } } */

#define N 1024
float a[N], b[N], c[N], d[N];
int ia[N];

void add (void)
{
  for (int i = 0; i < N; i++)
    a[i] = b[i] + c[i];
}

void fma4 (void)
{
  for (int i = 0; i < N; i++)
    a[i] = b[i] * c[i] + d[i];
}

void scale (float k)
{
  for (int i = 0; i < N; i++)
    a[i] = b[i] * k;
}

void maxf (void)
{
  for (int i = 0; i < N; i++)
    a[i] = b[i] > c[i] ? b[i] : c[i];
}

void cvt (void)
{
  for (int i = 0; i < N; i++)
    a[i] = (float) ia[i];
}
