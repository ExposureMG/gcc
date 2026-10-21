/* Xenon VMX128-only builtins (step 7): with -mcpu=xenon -mno-vmx128 the
   overloaded names are front-end errors naming -mvmx128.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -mno-vmx128" } */

typedef __vector float vf;

vf f1 (vf d, vf b) { return __builtin_xenon_vrlimi (d, b, 1, 1); }	/* { dg-error "'__builtin_xenon_vrlimi_v4sf' requires the '-mvmx128' option" } */
vf f2 (vf b) { return __builtin_xenon_vupkd3d (b, 1); }	/* { dg-error "'__builtin_xenon_vupkd3d_v4si' requires the '-mvmx128' option" } */
