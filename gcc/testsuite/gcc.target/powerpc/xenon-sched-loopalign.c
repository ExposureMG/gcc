/* Xenon: a loop top runs 5-7 cycles per iteration faster at byte 4 or 12 of
   its 16-byte fetch group than at 0 or 8 (FINDINGS 4, plan 5.3).  The loop
   label gets an 8-byte aligned label and one nop in front of it.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\.p2align 3\n\.L[0-9]+:\n\tnop\n\.L[0-9]+:} } } */
/* { dg-final { scan-assembler-not {\tnop\n\t} } } */

int a[1000];

void f (void)
{
  for (int i = 0; i < 1000; i++)
    a[i] = a[i] * 3 + i;
}

void g (int *p, int n)
{
  for (int i = 0; i < n; i++)
    p[i] = p[i] * 5 + i;
}
