/* Xenon scheduler sanity: sched2 uses the xenon.md automaton (the dump
   prints each insn's reservation with the xe_* units).  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -fsched-verbose=2 -fdump-rtl-sched2" } */
/* { dg-final { scan-rtl-dump {xe_fxu} "sched2" } } */
/* { dg-final { scan-rtl-dump {xe_lsu} "sched2" } } */
/* { dg-final { scan-rtl-dump {xe_vsu1} "sched2" } } */
/* { dg-final { scan-rtl-dump {xenon_issue} "sched2" } } */
/* { dg-final { scan-rtl-dump {xe_lfd} "sched2" } } */

double g (double *a, long *b, int n)
{
  double s = 0;
  long t = 0;
  for (int i = 0; i < n; i++)
    {
      s += a[i] * a[i + 1];
      t += b[i] * 3 + (b[i] >> 2);
    }
  return s + t;
}
