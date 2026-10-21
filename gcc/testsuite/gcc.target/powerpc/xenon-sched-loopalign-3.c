/* Xenon: no loop-top pad when optimizing for size.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Os -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\tnop\M} } } */
/* { dg-final { scan-assembler-not {\.p2align 3} } } */

void f (int *a, int n)
{
  for (int i = 0; i < n; i++)
    a[i] = a[i] * 3 + i;
}
