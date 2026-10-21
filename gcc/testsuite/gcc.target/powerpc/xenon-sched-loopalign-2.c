/* Xenon: an explicit -falign-loops=N replaces the loop-top pad.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -falign-loops=32" } */
/* { dg-final { scan-assembler {\.p2align 5} } } */
/* { dg-final { scan-assembler-not {\.p2align 3\n\.L[0-9]+:\n\tnop\n} } } */

void f (int *a, int n)
{
  for (int i = 0; i < n; i++)
    a[i] = a[i] * 3 + i;
}
