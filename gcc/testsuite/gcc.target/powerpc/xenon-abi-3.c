/* Xenon VMX128 ABI (plan 4.3): optimizing for size, a function that saves
   two or more of v64-v127, all from the first one to v127, calls the
   out-of-line libgcc routines: r0 = end of the save area, then
   bl _savevr128_N / bl _restvr128_N (libgcc/config/rs6000/crtsavevr128.S).
   The CFI still describes each register's slot.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Os -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_122\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_122\M} 1 } } */
/* { dg-final { scan-assembler {\maddi 0,1,} } } */
/* { dg-final { scan-assembler-not {\mstvx128 [0-9]+,1,0\M} } } */
/* { dg-final { scan-assembler-not {\mlvx128 [0-9]+,1,0\M} } } */
/* { dg-final { scan-assembler {\.cfi_offset 215, } } } */
/* { dg-final { scan-assembler {\.cfi_offset 210, } } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep6 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g ();
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
