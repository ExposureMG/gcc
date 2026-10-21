/* Xenon VMX128 ABI (plan 4.3): --param=xenon-vmx128-ool-threshold=N sets
   the number of saved v64-v127 from which a function optimized for speed
   uses the out-of-line routines.  With 4: six registers go out of line,
   three stay inline.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon --param=xenon-vmx128-ool-threshold=4" } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_122\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_122\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstvx128 (12[5-7]),1,0\M} 3 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (12[5-7]),1,0\M} 3 } } */
/* { dg-final { scan-assembler-not {\m_savevr128_125\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep6 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}

void
keep3 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2];
  g ();
  o[0] = x2; o[1] = x1; o[2] = x0;
}
