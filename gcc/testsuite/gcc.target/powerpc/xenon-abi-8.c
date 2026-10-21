/* Xenon VMX128 ABI (plan 4.3): in a frame larger than 32 KiB the 16-bit
   offsets in r0 cannot reach the save area from the new stack pointer.
   As for the AltiVec saves, the prologue keeps a pointer to the top of the
   vector save area in r12 (left by the stack allocation) and saves
   relative to it.  The six slots are within the red zone, so the epilogue
   pops the frame through the back chain first and restores from r1
   (xenon-abi-19.c has a save area below the red zone).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mstvx128 (12[2-7]),12,0\M} 6 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler {\mstdux 1,1,0\M} } } */
/* { dg-final { scan-assembler {\mld 1,0\(1\)} } } */
/* { dg-final { scan-assembler-not {\mstvx128 (12[2-7]),1,} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (char *);

void
keep6_bigframe (v4sf *restrict o, const v4sf *restrict a)
{
  char buf[40000];
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g (buf);
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
