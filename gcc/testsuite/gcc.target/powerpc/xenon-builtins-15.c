/* <xenon_vmx.h> in a unit compiled for another CPU: using one of its
   functions in a function that is not compiled for Xenon is an error (the
   always_inline function cannot be inlined across the target mismatch),
   as with the x86 intrinsic headers.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */

#include <xenon_vmx.h>

vector float
bad (vector float a, vector float b)
{
  return xenon_vmsum4fp (a, b);
}

/* { dg-error "inlining failed in call to 'always_inline' 'xenon_vmsum4fp': target specific option mismatch" "" { target *-*-* } 0 } */
