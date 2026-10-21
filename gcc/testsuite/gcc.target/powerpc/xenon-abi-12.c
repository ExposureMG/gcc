/* Xenon VMX128 ABI (plan 4.3): -mcpu=xenon -mno-vmx128 is the AltiVec-only
   Xenon compiler: v64-v127 are fixed, so nothing saves them.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -mno-vmx128" } */
/* { dg-final { scan-assembler-times {\mstvx (2[6-9]|3[01]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-not {\m[a-z]+128\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep6 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
