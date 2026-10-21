/* <xenon_vmx.h> in a unit compiled for another CPU: its functions are
   compiled for Xenon (the header pushes target ("cpu=xenon") when VMX128 is
   off), so a function marked target ("cpu=xenon") or compiled under the
   pragma can use them (the multi-CPU dispatch pattern).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */
/* { dg-final { scan-assembler {\mvmsum4fp128\M} } } */
/* { dg-final { scan-assembler {\mvrlimi128\M} } } */
/* { dg-final { scan-assembler {\mvmulfp128\M} } } */
/* { dg-final { scan-assembler {\.machine xenon} } } */

#include <xenon_vmx.h>

#ifdef __VMX128__
#error "__VMX128__ leaked out of xenon_vmx.h"
#endif

__attribute__ ((target ("cpu=xenon")))
vector float
dot_rot (vector float a, vector float b)
{
  return xenon_vrlimi (xenon_vmsum4fp (a, b), b, XENON_MASK_X, 1);
}

#pragma GCC push_options
#pragma GCC target ("cpu=xenon")
vector float
mul (vector float a, vector float b)
{
  return xenon_vmulfp (a, b);
}
#pragma GCC pop_options

vector float
cell_add (vector float a, vector float b)
{
  return a + b;
}
