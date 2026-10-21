/* Xenon VMX128 ABI (plan 4.3): shrink-wrapping.  The early exit needs no
   frame; the path with the calls saves v122-v127 and restores them.
   The epilogue's CFI restore notes keep the unwind info consistent on
   both paths (dwarf2cfi would fail otherwise).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mstvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler {\.cfi_restore 215\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep6_sw (v4sf *restrict o, const v4sf *restrict a, int n)
{
  if (n == 0)
    return;
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  for (int i = 0; i < n; i++)
    g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
