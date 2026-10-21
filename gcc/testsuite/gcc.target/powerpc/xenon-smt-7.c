/* Less unrolling when throughput-bound under SMT (plan 5.4): the -O2 small
   loop unrolling (-munroll-only-small-loops) unrolls a 3-insn loop 4 times
   (3 extra copies) with an idle sibling ...  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -fdump-rtl-loop2_unroll-details" } */
/* { dg-final { scan-rtl-dump {Unrolled loop 3 times} "loop2_unroll" } } */

long f (long *a, long n)
{
  long s = 0;
  for (long i = 0; i < n; i++)
    s += a[i];
  return s;
}
