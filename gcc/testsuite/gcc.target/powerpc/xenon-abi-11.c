/* Xenon VMX128 ABI (plan 4.3): nothing changes for other CPUs and without
   -mvmx128.  The same call-crossing values as xenon-abi-1.c go to the
   AltiVec callee-saved v31 down to v26, saved with stvx; no *128
   instruction and no _savevr128 call.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */
/* { dg-final { scan-assembler-times {\mstvx (2[6-9]|3[01]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-times {\mlvx (2[6-9]|3[01]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-not {\m[a-z]+128\M} } } */
/* { dg-final { scan-assembler-not {\.cfi_offset (1[2-9][0-9]|20[0-9]|21[0-5]),} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep6 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
