/* The Xenon builtin gating follows the per-function cpu: a
   target("cpu=cell") function may use the integer multiply intrinsics in a
   -mcpu=xenon unit (xenon-builtin-6.c: a -mcpu=xenon function may not; the
   error test is separate because a compile that fails writes no assembly
   to scan).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-prune-output "overloaded builtin" } */
/* { dg-final { scan-assembler {\mvmuleub\M} } } */
/* { dg-final { scan-assembler {\mvsumsws\M} } } */

#include <altivec.h>

typedef __vector unsigned char vuc;
typedef __vector unsigned short vus;
typedef __vector signed int vsi;

__attribute__ ((target ("cpu=cell")))
vus cell_mule (vuc a, vuc b) { return vec_mule (a, b); }

__attribute__ ((target ("cpu=cell")))
vsi cell_sums (vsi a, vsi b) { return vec_sums (a, b); }
