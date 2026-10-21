/* Xenon VMX128-only builtins: <xenon_vmx.h> can be included in a unit
   compiled for another CPU (s78-fix; it used to stop with an #error).  Its
   definitions are compiled for Xenon under a pushed target pragma, which
   does not leak: __VMX128__ stays undefined after the header, and the
   unit's own code is Cell code (xenon-builtins-14.c / -15.c use the
   functions).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */
/* { dg-final { scan-assembler-not {\.machine xenon} } } */
/* { dg-final { scan-assembler-not {128\M} } } */

#include <xenon_vmx.h>

#ifdef __VMX128__
#error "__VMX128__ leaked out of xenon_vmx.h"
#endif
#ifndef XENON_MASK_XYZW
#error "the constants of xenon_vmx.h are missing"
#endif

vector float
cell_madd (vector float a, vector float b, vector float c)
{
  return a * b + c;
}
