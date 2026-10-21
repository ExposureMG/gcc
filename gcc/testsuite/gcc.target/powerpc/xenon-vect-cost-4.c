/* Xenon vectorizer costs (plan 5.5), misaligned accesses: movmisalign loads
   are lvlx + lvrx + vor = 3 + 3 + 1 = 7 (rotating-destination throughput
   2.94/2.93, FINDINGS 3.4) and stores stvlx + stvrx = 4 + 12 (CSV store
   streams 3.50 / 12.48), against 3 / 4 aligned.  A misaligned load next to
   aligned accesses uses lvlx/lvrx; a misaligned store is aligned by peeling
   (3 iterations) and the loads become misaligned instead.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump-times {optimized: loop vectorized} 2 "vect" } } */
/* { dg-final { scan-tree-dump {b\[_1\] 1 times unaligned_load \(misalign 4\) costs 7 in body} "vect" } } */
/* { dg-final { scan-tree-dump {unaligned_load \(misalign 12\) costs 7 in body} "vect" } } */
/* { dg-final { scan-tree-dump {known peeling = 3} "vect" } } */
/* { dg-final { scan-assembler {\mlvlx(128)?\M} } } */
/* { dg-final { scan-assembler {\mlvrx(128)?\M} } } */
/* { dg-final { scan-assembler-not {\mstv[lr]x(128)?\M} } } */
/* { dg-final { scan-assembler-not {\mlvs[lr](128)?\M} } } */

#define N 1024
float a[N], b[N + 4], c[N];

void ld (void)
{
  for (int i = 0; i < N; i++)
    a[i] = b[i + 1] + c[i];
}

void st (void)
{
  for (int i = 0; i < N; i++)
    b[i + 1] = a[i] + c[i];
}
