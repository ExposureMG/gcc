/* Xenon VMX128 ABI (s345-fix finding 1, inline restores): as
   xenon-abi-21.c, with --param=xenon-vmx128-ool-threshold=0, so that the
   20 registers are restored inline (li 0,OFF; lvx128 vN,11,0) before the
   frame is popped by mr 1,11; sched2 moved the pop above some of them.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon --param=xenon-vmx128-ool-threshold=0" } */
/* { dg-final { scan-assembler-times {\mlvx128 (10[89]|11[0-9]|12[0-7]),11,0\M} 20 } } */
/* { dg-final { scan-assembler {\mld 11,0\(1\)} } } */
/* { dg-final { scan-assembler {\mmr 1,11\M} } } */
/* { dg-final { scan-assembler-not {\m_restvr128_} } } */
/* { dg-final { scan-assembler-not {\mmr 1,11\n(?:[^\n]*\n)*?[^\n]*\mlvx128 [0-9]+,11,0} } } */
/* { dg-final { scan-assembler-not {\.cfi_restore 215\n(?:[^\n]*\n)*?[^\n]*\mlvx128 [0-9]+,11,0} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
void h (void *);
int loc (int);

#define L4(b) v4sf x##b##0 = a[b*4+0], x##b##1 = a[b*4+1], \
  x##b##2 = a[b*4+2], x##b##3 = a[b*4+3];
#define S4(b) o[b*4+0] = x##b##3; o[b*4+1] = x##b##2; \
  o[b*4+2] = x##b##1; o[b*4+3] = x##b##0;

int
big_inline (v4sf *restrict o, const v4sf *restrict a, int z)
{
  char buf[40000];
  L4 (0) L4 (1) L4 (2) L4 (3) L4 (4)
  h (buf);
  S4 (0) S4 (1) S4 (2) S4 (3) S4 (4)
  return loc (z);
}
