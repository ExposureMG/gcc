/* Xenon cost table (ppcxenon_cost, plan 5.3): 128-byte lines, 32 KB L1
   (256 lines), 1 MB L2; the prefetch pass reports the values it uses.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -fprefetch-loop-arrays -fdump-tree-aprefetch-details" } */
/* { dg-final { scan-tree-dump {L1 cache size: 256 lines, 32 kB} "aprefetch" } } */
/* { dg-final { scan-tree-dump {L1 cache line size: 128\n} "aprefetch" } } */
/* { dg-final { scan-tree-dump {L2 cache size: 1024 kB} "aprefetch" } } */
/* { dg-final { scan-tree-dump {simultaneous prefetches: 7\n} "aprefetch" } } */

void f (double *a, double *b, int n)
{
  for (int i = 0; i < n; i++)
    a[i] = b[i] * 2.0;
}
