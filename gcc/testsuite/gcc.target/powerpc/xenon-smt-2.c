/* -mxenon-smt=idle is the default (plan 5.4): the measured tables, no SMT
   reservation, the integer ALU latency 2 (chain at cycles 0, 2, 4, 6).
   Same source as xenon-smt-1.c.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -fsched-verbose=2 -fdump-rtl-sched2" } */
/* { dg-final { scan-rtl-dump {:\(xenon_issue\+xe_fxu\)} "sched2" } } */
/* { dg-final { scan-rtl-dump-not {xenon_smt} "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+0--> b\s+0: i\s+\d+ %3=%3\+%4 } "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+2--> b\s+0: i\s+\d+ %3=%3\^%5 } "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+4--> b\s+0: i\s+\d+ %3=%3\+%6 } "sched2" } } */
/* { dg-final { scan-rtl-dump {;;\s+6--> b\s+0: i\s+\d+ %3=%3\|%7 } "sched2" } } */

long
chain (long x, long a, long b, long c, long d)
{
  x += a;
  x ^= b;
  x += c;
  x |= d;
  return x;
}
