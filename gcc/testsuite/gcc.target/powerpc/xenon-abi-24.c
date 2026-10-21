/* Xenon VMX128 ABI (s345-fix finding 5): as xenon-abi-23.c, optimized,
   with a frame pointer.  The EH data words below the VMX128 area are read
   before the frame is popped (addi 1,31,SIZE).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -fno-omit-frame-pointer -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mld [3-6],[0-9]+\(31\)} 4 } } */
/* { dg-final { scan-assembler {\maddi 1,31,1[0-9]{3}\M} } } */
/* { dg-final { scan-assembler-not {\maddi 1,31,[0-9]+\n(?:[^\n]*\n)*?\tld [3-6],} } } */

void
eh_ret (long offset, void *handler)
{
  __builtin_unwind_init ();
  __builtin_eh_return (offset, handler);
}
