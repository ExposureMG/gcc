/* With -funroll-loops and a sibling running the same code (or a busy one)
   the RTL unroll factor is halved too: 4 copies of this loop instead of the
   8 an idle sibling gets.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mxenon-smt=same -funroll-loops -fdump-rtl-loop2_unroll-details" } */
/* { dg-final { scan-rtl-dump {Unrolled loop 3 times} "loop2_unroll" } } */
/* { dg-final { scan-rtl-dump-not {Unrolled loop 7 times} "loop2_unroll" } } */

void f (int *a, int *b, int n)
{
  for (int i = 0; i < n; i++)
    a[i] += b[i];
}
