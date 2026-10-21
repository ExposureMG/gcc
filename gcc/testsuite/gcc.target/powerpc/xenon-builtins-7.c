/* Xenon VMX128-only builtins (step 7): with -mcpu=xenon -mno-vmx128 they
   are errors naming -mvmx128 (expansion-time errors).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -mno-vmx128" } */

typedef __vector float vf;

vf f1 (vf a, vf b) { return __builtin_xenon_vmsum3fp (a, b); }	/* { dg-error "'__builtin_xenon_vmsum3fp' requires the '-mvmx128' option" } */
vf f2 (vf a, vf d, vf b) { return __builtin_xenon_vmaddcfp (a, d, b); }	/* { dg-error "'__builtin_xenon_vmaddcfp' requires the '-mvmx128' option" } */
vf f3 (vf d, vf b) { return __builtin_xenon_vpkd3d_v4sf (d, b, 0, 1, 0); }	/* { dg-error "'__builtin_xenon_vpkd3d_v4sf' requires the '-mvmx128' option" } */
