/* Xenon VMX128 ABI (s345-fix finding 6): at -Os the out-of-line
   _savevr128_N/_restvr128_N routines are used from two registers only
   when LR is saved anyway (a call, or another out-of-line routine);
   otherwise the bl costs mflr/std/ld/mtlr, so a leaf saves two registers
   inline (8 instructions either way, no calls, no LR slot) and goes out
   of line from three.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Os -maltivec -mdejagnu-cpu=xenon" } */
/* leaf2: v126, v127 inline from the red zone; no LR traffic.  */
/* { dg-final { scan-assembler-times {\mstvx128 12[67],1,0\M} 2 } } */
/* { dg-final { scan-assembler-times {\mlvx128 12[67],1,0\M} 2 } } */
/* leaf3: v125-v127 out of line; call2: v126, v127 out of line.  */
/* { dg-final { scan-assembler-times {\mbl _savevr128_125\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_125\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_126\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_126\M} 1 } } */
/* { dg-final { scan-assembler-times {\mmflr 0\M} 2 } } */

void g (void);

void
leaf2 (void)
{
  __asm__ volatile ("# clobbers v126 v127" ::: "v126", "v127");
}

void
leaf3 (void)
{
  __asm__ volatile ("# clobbers v125 v126 v127" ::: "v125", "v126", "v127");
}

void
call2 (void)
{
  __asm__ volatile ("# clobbers v126 v127" ::: "v126", "v127");
  g ();
}
