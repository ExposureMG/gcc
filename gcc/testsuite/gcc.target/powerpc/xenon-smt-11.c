/* -Wno-xenon-smt silences the warning that -mxenon-smt= only affects code
   tuned for Xenon, so a makefile can pass the option to every CPU, also
   with -Werror.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=power7 -mxenon-smt=busy -Wno-xenon-smt -Werror" } */

int f (int *a, int n)
{
  int s = 0;
  for (int i = 0; i < n; i++)
    s += a[i];
  return s;
}
