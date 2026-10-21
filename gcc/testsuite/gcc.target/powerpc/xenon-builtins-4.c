/* Xenon VMX128-only builtins (step 7): argument types.  The word-granular
   builtins take vector float/signed int/unsigned int (and bool int for
   vrlimi and vpermwi) with matching types; vpkd3d packs vector float;
   vupkd3d accepts any non-bool 128-bit vector; the dot products, vmaddcfp
   and vmulfp take vector float only.  Front-end errors.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

typedef __vector float vf;
typedef __vector signed int vsi;
typedef __vector unsigned int vui;
typedef __vector __bool int vbi;
typedef __vector signed short vss;
typedef __vector __bool short vbs;
typedef __vector __pixel vp;

vf t1 (vf d, vsi b) { return __builtin_xenon_vrlimi (d, b, 1, 1); }	/* { dg-error "invalid parameter combination for AltiVec intrinsic '__builtin_xenon_vrlimi'" } */
vss t2 (vss b) { return __builtin_xenon_vpermwi (b, 1); }	/* { dg-error "invalid parameter combination for AltiVec intrinsic '__builtin_xenon_vpermwi'" } */
vsi t3 (vsi d, vsi b) { return __builtin_xenon_vpkd3d (d, b, 0, 1, 0); }	/* { dg-error "invalid parameter combination for AltiVec intrinsic '__builtin_xenon_vpkd3d'" } */
vbi t4 (vbi d, vf b) { return __builtin_xenon_vpkd3d (d, b, 0, 1, 0); }	/* { dg-error "invalid parameter combination for AltiVec intrinsic '__builtin_xenon_vpkd3d'" } */
vf t5 (vbs b) { return __builtin_xenon_vupkd3d (b, 0); }	/* { dg-error "invalid parameter combination for AltiVec intrinsic '__builtin_xenon_vupkd3d'" } */
vf t6 (vp b) { return __builtin_xenon_vupkd3d (b, 0); }	/* { dg-error "invalid parameter combination for AltiVec intrinsic '__builtin_xenon_vupkd3d'" } */
vf t7 (vf b) { return __builtin_xenon_vrlimi (b, 1, 1); }	/* { dg-error "too few arguments" } */
/* { dg-error "incompatible type for argument 2" "" { target *-*-* } .-1 } */
vf t8 (vsi a, vf b) { return __builtin_xenon_vmsum4fp (a, b); }	/* { dg-error "incompatible type for argument 1|incompatible type" } */

/* Accepted combinations.  */
vbi ok1 (vbi d, vbi b) { return __builtin_xenon_vrlimi (d, b, 1, 1); }
vui ok2 (vui b) { return __builtin_xenon_vpermwi (b, 1); }
vsi ok3 (vsi d, vf b) { return __builtin_xenon_vpkd3d (d, b, 0, 1, 0); }
vf ok4 (vss b) { return __builtin_xenon_vupkd3d (b, 3); }
