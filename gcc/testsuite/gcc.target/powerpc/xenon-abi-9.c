/* Xenon VMX128 ABI (plan 4.3): v64-v127 saved in a function with a dynamic
   stack allocation (alloca): the saves are made right after the frame is
   allocated, below the AltiVec area, and the epilogue resets the stack
   pointer from the frame pointer r31 (not from the moved stack pointer)
   before restoring them from the red zone.  tools/check_xenon_abi_frames.py
   checks the slots.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mstvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler {\mstdux [0-9]+,1,} } } */
/* { dg-final { scan-assembler {\maddi 1,31,} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (char *);

void
keep6_alloca (v4sf *restrict o, const v4sf *restrict a, long n)
{
  char *p = __builtin_alloca (n);
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g (p);
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
