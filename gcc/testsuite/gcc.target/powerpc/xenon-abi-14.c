/* Xenon VMX128 ABI (plan 4.3): __builtin_unwind_init (used by the unwinder
   to capture the caller's registers) makes every callee-saved register
   saved, v64-v127 included; without eh_return they may go out of line.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_64\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_64\M} 1 } } */
/* { dg-final { scan-assembler {\.cfi_offset 152, } } } */

void h (void *);

void
capture (void)
{
  __builtin_unwind_init ();
  h (__builtin_frame_address (0));
}
