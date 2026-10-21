/* Xenon VMX128 (plan 4.1, allocation order): values that live across a call
   need callee-saved registers.  Values that only move (loads, stores) take
   v127 downwards (v64-v127 are callee-saved, plan 4.3); values used by
   legacy AltiVec instructions take v31 downwards, which the vor128 copy
   cost makes cheaper for them.  (The prologue/epilogue saves of v64-v127
   are plan step 4.)  Since plan step 5 vector float adds have a VMX128
   form (vaddfp128) and take v127 downwards too; the integer add (vadduwm)
   has none and still needs v0-v31.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mlvx128 127,} } } */
/* { dg-final { scan-assembler {\mlvx128 122,} } } */
/* { dg-final { scan-assembler {\mlvx 31,} } } */
/* { dg-final { scan-assembler {\mlvx 26,} } } */
/* { dg-final { scan-assembler-not {\mlvx (2[0-5]),} } } */
/* { dg-final { scan-assembler-not {\mlvx128 (6[4-9]|[7-9][0-9]|1[01][0-9]|12[01]),} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
typedef __attribute__ ((vector_size (16))) int v4si;

void g (void);

void
keep6 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}

v4si
sum6 (const v4si *restrict a)
{
  v4si x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  return ((x0 + x1) + (x2 + x3)) + (x4 + x5);
}
