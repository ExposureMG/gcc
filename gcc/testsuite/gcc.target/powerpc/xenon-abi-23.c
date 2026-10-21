/* Xenon VMX128 ABI (s345-fix finding 5): at -O0 a __builtin_eh_return
   function has a frame pointer and pops its frame with addi 1,31,SIZE.
   The EH data registers r3-r6 are loaded from slots below the VMX128 area
   (SP0-1536 .. SP0-1512), far below the 288-byte red zone: they must be
   loaded before the pop (through r31), not from deallocated stack after
   it.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O0 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mld [3-6],[0-9]+\(31\)} 4 } } */
/* { dg-final { scan-assembler {\maddi 1,31,1[0-9]{3}\M} } } */
/* { dg-final { scan-assembler-not {\mld [3-6],-[0-9]+\(1\)} } } */
/* { dg-final { scan-assembler-not {\maddi 1,31,[0-9]+\n(?:[^\n]*\n)*?\tld [3-6],} } } */

void
eh_ret (long offset, void *handler)
{
  __builtin_unwind_init ();
  __builtin_eh_return (offset, handler);
}
