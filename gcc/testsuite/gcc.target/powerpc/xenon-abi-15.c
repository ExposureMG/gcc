/* Xenon VMX128 ABI (plan 4.3): __builtin_setjmp / __builtin_longjmp.  The
   receiver treats every register as clobbered, so a function using
   __builtin_setjmp saves and restores the callee-saved v64-v127 it (or the
   receiver) may change; tools/check_xenon_abi_frames.py checks that what
   is saved is restored from the same slot.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mbl _savevr128_64\M|\mstvx128 127,} } } */
/* { dg-final { scan-assembler {\mbl _restvr128_64\M|\mlvx128 127,} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void *buf[5];
void g (void);

int
catcher (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2];
  if (__builtin_setjmp (buf))
    return 1;
  g ();
  o[0] = x2; o[1] = x1; o[2] = x0;
  return 0;
}
