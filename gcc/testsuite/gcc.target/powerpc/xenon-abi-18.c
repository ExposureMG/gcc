/* Xenon VMX128 ABI (plan 4.3): with -mabi=no-altivec the prologue saves no
   vector register (as for every CPU, it saves v20-v31 only in the AltiVec
   ABI), so v64-v127 are made call-used: nothing saves them and no value
   is kept in them across a call.  (Upstream, without the AltiVec ABI the
   compiler assumes v0-v31 survive calls without saving them; that is
   unchanged and not a Xenon matter.)  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -mabi=no-altivec" } */
/* { dg-final { scan-assembler-not {\.cfi_offset (1[2-9][0-9]|20[0-9]|21[0-5]),} } } */
/* { dg-final { scan-assembler-not {\mlvx128 (6[4-9]|[7-9][0-9]|1[01][0-9]|12[0-7]),} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep6 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
