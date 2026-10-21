/* Xenon VMX128 ABI (plan 4.3): the save area holds a slot for each saved
   register only, in register order.  An asm that clobbers v70, v100 and
   the volatile v40 saves v70 and v100 (and not v40) in two adjacent
   16-byte slots; a leaf with only those fits in the red zone, so there is
   no stack frame: v70 at -32, v100 at -16 from the stack pointer.  Sparse
   save sets are always saved inline, also when optimizing for size.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Os -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mli 0,-32\M} 2 } } */
/* { dg-final { scan-assembler-times {\mli 0,-16\M} 2 } } */
/* { dg-final { scan-assembler-times {\mstvx128 70,1,0\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstvx128 100,1,0\M} 1 } } */
/* { dg-final { scan-assembler-times {\mlvx128 70,1,0\M} 1 } } */
/* { dg-final { scan-assembler-times {\mlvx128 100,1,0\M} 1 } } */
/* { dg-final { scan-assembler-not {\mstvx128 40,} } } */
/* { dg-final { scan-assembler-not {\mstdu\M} } } */
/* { dg-final { scan-assembler-not {\m_savevr128_} } } */
/* { dg-final { scan-assembler {\.cfi_offset 158, -32} } } */
/* { dg-final { scan-assembler {\.cfi_offset 188, -16} } } */

void
clobber_sparse (void)
{
  __asm__ volatile ("# clobbers" ::: "v40", "v70", "v100");
}
