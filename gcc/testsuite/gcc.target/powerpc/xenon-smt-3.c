/* -mxenon-smt=same (plan 5.4, FINDINGS 7: sibling running the same code,
   throughput x2.28, latency x1.27): SMT reservations, integer latency
   rnd (2 * 1.27) = 3, FP latency rnd (10 * 1.27) = 13.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mxenon-smt=same -fsched-verbose=2 -fdump-rtl-sched2" } */
/* { dg-final { scan-rtl-dump {:\(xenon_smt1_issue\+xe_fxu\)} "sched2" } } */
/* { dg-final { scan-rtl-dump {:\(xenon_smt1_issue_vec\+xe_vsu1\)} "sched2" } } */
/* { dg-final { scan-rtl-dump-not {:\(xenon_issue} "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+3--> b\s+0: i\s+\d+ %3=%3\^%5 } "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+13--> b\s+0: i\s+\d+ %1=%1\*%3 } "sched2" } } */

long
chain (long x, long a, long b, long c, long d)
{
  x += a;
  x ^= b;
  x += c;
  x |= d;
  return x;
}

double
fchain (double x, double a, double b)
{
  x = x + a;
  return x * b;
}
