/* Xenon VMX128 ABI (plan 4.3): out-of-line saves in a leaf without a stack
   frame.  Optimizing for size, a leaf that only clobbers v125-v127 calls
   _savevr128_125 / _restvr128_125 with r0 = r1: the slots are in the red
   zone, the routine uses no stack, and the call only needs LR saved in the
   caller's frame header.  (With two registers a leaf that saves LR only
   for the routine saves them inline: xenon-abi-25.c, s345-fix finding 6.)
   With more callee-saved registers (r31, f31, v31) the VMX128 area sits
   below the AltiVec one; the out-of-line FPR restore saves LR anyway, so
   two registers go out of line there.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Os -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_125\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_125\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_126\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_126\M} 1 } } */
/* { dg-final { scan-assembler {\mmr 0,1\M} } } */
/* { dg-final { scan-assembler {\maddi 0,1,-32\M} } } */
/* { dg-final { scan-assembler-not {\mstdu\M} } } */
/* { dg-final { scan-assembler {\.cfi_offset 215, -16\M} } } */
/* { dg-final { scan-assembler {\.cfi_offset 215, -48\M} } } */
/* { dg-final { scan-assembler {\.cfi_offset 108, -32\M} } } */

void
leaf3 (void)
{
  __asm__ volatile ("# clobbers v125 v126 v127" ::: "v125", "v126", "v127");
}

void
leaf_mixed (void)
{
  __asm__ volatile ("# clobbers" ::: "v126", "v127", "r31", "fr31", "v31");
}
