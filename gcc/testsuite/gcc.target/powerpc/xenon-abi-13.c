/* Xenon VMX128 ABI (plan 4.3): a function that calls __builtin_eh_return
   (the libgcc unwinder's _Unwind_RaiseException and friends) saves every
   callee-saved register, all of v64-v127 included, inline (never out of
   line), with CFI for each, and its eh_return epilogue restores them: the
   unwinder installs the landing pad's v64-v127 by writing these slots.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mstvx128 (6[4-9]|[7-9][0-9]|1[01][0-9]|12[0-7]),1[12]?,0\M} 64 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (6[4-9]|[7-9][0-9]|1[01][0-9]|12[0-7]),1[12]?,0\M} 64 } } */
/* { dg-final { scan-assembler-times {\mstvx (2[0-9]|3[01]),1[12]?,0\M} 12 } } */
/* { dg-final { scan-assembler-not {\m_savevr128_} } } */
/* { dg-final { scan-assembler {\.cfi_offset 152, } } } */
/* { dg-final { scan-assembler {\.cfi_offset 215, } } } */

void
eh_ret (long offset, void *handler)
{
  __builtin_unwind_init ();
  __builtin_eh_return (offset, handler);
}
