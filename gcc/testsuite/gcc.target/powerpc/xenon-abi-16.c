/* Xenon VMX128 ABI (plan 4.3): AltiVec v20-v31 and VMX128 v64-v127 saved
   together.  Values used by legacy-only AltiVec arithmetic (integer adds,
   which have no 128 form) take v31 downwards,
   move-only values v127 downwards (xenon-regs-9.c); the two save areas are
   adjacent (VMX128 right below AltiVec), 16-byte aligned, and each
   register has its own slot (tools/check_xenon_abi_frames.py).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mstvx (2[6-9]|3[01]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-times {\mstvx128 (12[2-7]),1,0\M} 6 } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
typedef __attribute__ ((vector_size (16))) int v4si;

void g (void);

void
both (v4si *restrict o, const v4si *restrict a)
{
  v4si x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  v4si y0 = a[6], y1 = a[7], y2 = a[8], y3 = a[9], y4 = a[10], y5 = a[11];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
  o[6] = ((y0 + y1) + (y2 + y3)) + (y4 + y5);
}
