/* Xenon VMX128-only builtins (step 7) need -mvmx128 (-mcpu=xenon): the
   overloaded names on another AltiVec CPU are front-end errors naming the
   option.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */

typedef __vector float vf;
typedef __vector unsigned int vui;

vf f1 (vf d, vf b) { return __builtin_xenon_vrlimi (d, b, 1, 1); }	/* { dg-error "'__builtin_xenon_vrlimi_v4sf' requires the '-mcpu=xenon' option" } */
vui f2 (vui b) { return __builtin_xenon_vpermwi (b, 1); }	/* { dg-error "'__builtin_xenon_vpermwi_v4si' requires the '-mcpu=xenon' option" } */
vf f3 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, 0, 1, 0); }	/* { dg-error "'__builtin_xenon_vpkd3d_v4sf' requires the '-mcpu=xenon' option" } */
vf f4 (vui b) { return __builtin_xenon_vupkd3d (b, 0); }	/* { dg-error "'__builtin_xenon_vupkd3d_v4si' requires the '-mcpu=xenon' option" } */
