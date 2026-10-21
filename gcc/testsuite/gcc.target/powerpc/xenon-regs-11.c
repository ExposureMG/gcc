/* Xenon VMX128 (plan 4.1): v32-v127 in an asm clobber list are only valid
   with the VMX128 registers; without them (here -mno-vmx128, likewise any
   other CPU) they are rejected as before they existed.  v0-v31 are
   fine.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -mno-vmx128" } */

void
f (void)
{
  __asm__ __volatile__ ("" ::: "v31");
  __asm__ __volatile__ ("" ::: "v40"); /* { dg-error "register 'v40' in an 'asm' clobber list needs the Xenon VMX128 registers" } */
}
