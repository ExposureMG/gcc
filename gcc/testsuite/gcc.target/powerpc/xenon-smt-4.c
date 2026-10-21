/* -mxenon-smt only describes the Xenon sibling thread: without Xenon tuning
   it is ignored with a warning (plan 5.4; -Wxenon-smt), and the code is unchanged (the
   cost runner compares it with the compiler before step 7).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=power7 -mxenon-smt=busy" } */
/* { dg-warning "'-mxenon-smt=' only affects code tuned for Xenon" "" { target *-*-* } 0 } */

int f (int *a, int n)
{
  int s = 0;
  for (int i = 0; i < n; i++)
    s += a[i];
  return s;
}
