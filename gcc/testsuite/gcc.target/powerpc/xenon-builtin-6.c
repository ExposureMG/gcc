/* The Xenon builtin gating follows the per-function cpu: in a -mcpu=xenon
   unit the integer multiply intrinsics are an error, also next to a
   target("cpu=cell") function that may use them (xenon-builtin-3.c scans
   that function's code).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-prune-output "overloaded builtin" } */

#include <altivec.h>

typedef __vector unsigned char vuc;
typedef __vector unsigned short vus;

__attribute__ ((target ("cpu=cell")))
vus cell_mule (vuc a, vuc b) { return vec_mule (a, b); }

vus xenon_mule (vuc a, vuc b) { return vec_mule (a, b); }	/* { dg-error "is not supported on Xenon: the .vmuleub. instruction" } */
