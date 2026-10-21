/* ... and only twice with a busy sibling, which owns every other issue
   cycle, so half the unrolling hides the same latency (SMT_BUSY_UNROLL_DIV
   = rnd (1.91 / 1.00) = 2).  Same source as xenon-smt-7.c.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mxenon-smt=busy -fdump-rtl-loop2_unroll-details" } */
/* { dg-final { scan-rtl-dump {Unrolled loop 1 times} "loop2_unroll" } } */
/* { dg-final { scan-rtl-dump-not {Unrolled loop [2-9] times} "loop2_unroll" } } */

long f (long *a, long n)
{
  long s = 0;
  for (long i = 0; i < n; i++)
    s += a[i];
  return s;
}
