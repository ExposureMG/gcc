/* Xenon VMX128 ABI (plan 4.3): v64-v127 are callee-saved.  Six vector
   values that live across a call and only move take v127 down to v122
   (xenon-regs-9.c); the prologue saves exactly those six with stvx128 and
   the offset in r0, the epilogue restores them with lvx128 from the same
   slots, and the CFI describes the saves (.eh_frame numbers 120 + N - 32:
   v122 = 210, v127 = 215).  No other vector register is saved.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mstvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-not {\mstvx128 ([3-9][0-9]|1[01][0-9]|12[01]),1,} } } */
/* { dg-final { scan-assembler-not {\mstvx\M} } } */
/* { dg-final { scan-assembler {\.cfi_offset 215, } } } */
/* { dg-final { scan-assembler {\.cfi_offset 210, } } } */
/* { dg-final { scan-assembler-not {\.cfi_offset (209|1[2-9][0-9]|20[0-8]), } } } */
/* { dg-final { scan-assembler-not {\m_savevr128_} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep6 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
