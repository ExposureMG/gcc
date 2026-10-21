/* -mxenon-smt=busy (plan 5.4, FINDINGS 7): with a busy sibling thread the
   Xenon threads alternate issue cycles, so every insn issues with a
   xenon_smt1_issue* reservation (the sibling owns the next cycle), and the
   integer ALU latency is 3 (2.02 -> 2.42 measured, rounded up) instead of 2:
   the dependent chain issues at cycles 0, 3, 6, 9.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mxenon-smt=busy -fsched-verbose=2 -fdump-rtl-sched2" } */
/* { dg-final { scan-rtl-dump {:\(xenon_smt1_issue\+xe_fxu\)} "sched2" } } */
/* { dg-final { scan-rtl-dump-not {:\(xenon_issue} "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+0--> b\s+0: i\s+\d+ %3=%3\+%4 } "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+3--> b\s+0: i\s+\d+ %3=%3\^%5 } "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+6--> b\s+0: i\s+\d+ %3=%3\+%6 } "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+9--> b\s+0: i\s+\d+ %3=%3\|%7 } "sched2" } } */

long
chain (long x, long a, long b, long c, long d)
{
  x += a;
  x ^= b;
  x += c;
  x |= d;
  return x;
}
