/* vec_nabs on vector float without VSX (Xenon, Cell): the builtin used to
   name the VSX pattern xvnabssp and ended as an unrecognizable insn.  It
   now sets the sign bits with vspltisw/vslw/vor (their VMX128 forms when
   the registers are v32-v127, plan step 5).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mvspltisw(128)?\M} } } */
/* { dg-final { scan-assembler {\mvslw(128)?\M} } } */
/* { dg-final { scan-assembler {\mvor(128)?\M} } } */
/* { dg-final { scan-assembler-not {\mxvnabssp\M} } } */

#include <altivec.h>

vector float f (vector float a) { return vec_nabs (a); }
vector float g (vector float a) { return __builtin_altivec_nabs_v4sf (a); }
