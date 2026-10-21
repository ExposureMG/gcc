/* Xenon VMX128-only builtins (step 7) need -mvmx128 (-mcpu=xenon): the
   direct builtins on another AltiVec CPU are expansion-time errors naming
   the option.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */

typedef __vector float vf;
typedef __vector signed int vsi;

vf f1 (vf a, vf b) { return __builtin_xenon_vmsum3fp (a, b); }	/* { dg-error "'__builtin_xenon_vmsum3fp' requires the '-mcpu=xenon' option" } */
vf f2 (vf a, vf b) { return __builtin_xenon_vmsum4fp (a, b); }	/* { dg-error "'__builtin_xenon_vmsum4fp' requires the '-mcpu=xenon' option" } */
vf f3 (vf a, vf d, vf b) { return __builtin_xenon_vmaddcfp (a, d, b); }	/* { dg-error "'__builtin_xenon_vmaddcfp' requires the '-mcpu=xenon' option" } */
vf f4 (vf a, vf b) { return __builtin_xenon_vmulfp (a, b); }	/* { dg-error "'__builtin_xenon_vmulfp' requires the '-mcpu=xenon' option" } */
vsi f5 (vsi d, vsi b) { return __builtin_xenon_vrlimi_v4si (d, b, 1, 1); }	/* { dg-error "'__builtin_xenon_vrlimi_v4si' requires the '-mcpu=xenon' option" } */
vf f6 (vf b) { return __builtin_xenon_vpermwi_v4sf (b, 1); }	/* { dg-error "'__builtin_xenon_vpermwi_v4sf' requires the '-mcpu=xenon' option" } */
