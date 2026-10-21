/* Xenon VMX128 ABI (plan 4.3): v32-v63 are volatile.  A leaf function
   with 40 values live at once allocates v32-v63 first (and v0-v19) and
   saves no vector register (the GPRs it saves for the many load offsets
   fit in the red zone, so there is no stack frame either).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mlvx128 (3[2-9]|[45][0-9]|6[0-3]),} } } */
/* { dg-final { scan-assembler-not {\mlvx128 (6[4-9]|[7-9][0-9]|1[0-2][0-9]),} } } */
/* { dg-final { scan-assembler-not {\mstvx128 [0-9]+,1,} } } */
/* { dg-final { scan-assembler-not {\mstdu\M} } } */
/* { dg-final { scan-assembler-not {\.cfi_offset (7[7-9]|[89][0-9]|10[0-8]|1[2-9][0-9]|20[0-9]|21[0-5]),} } } */

typedef __attribute__ ((vector_size (16))) int v4si;

#define L8(B) v4si x##B##0 = a[B##0], x##B##1 = a[B##1], x##B##2 = a[B##2], \
  x##B##3 = a[B##3], x##B##4 = a[B##4], x##B##5 = a[B##5], x##B##6 = a[B##6], \
  x##B##7 = a[B##7];
#define S8(B) o[B##0] = x##B##7; o[B##1] = x##B##6; o[B##2] = x##B##5; \
  o[B##3] = x##B##4; o[B##4] = x##B##3; o[B##5] = x##B##2; o[B##6] = x##B##1; \
  o[B##7] = x##B##0;

void
mov40 (v4si *restrict o, const v4si *restrict a)
{
  L8 (1) L8 (2) L8 (3) L8 (4) L8 (5)
  __asm__ volatile ("" ::: "memory");
  S8 (5) S8 (4) S8 (3) S8 (2) S8 (1)
}
