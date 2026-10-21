/* Xenon VMX128-only builtins (step 7): the immediates must be literals
   within the instruction fields' defined values (vrlimi mask 0-15, rotate
   0-3; vpermwi 0-255; vpkd3d type 0-6, pack size 1-3, slot 0-3; vupkd3d
   type 0-6).  These are expansion-time errors, so this file contains no
   front-end errors.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

typedef __vector float vf;
typedef __vector unsigned int vui;

vf rl1 (vf d, vf b) { return __builtin_xenon_vrlimi (d, b, 16, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 15, inclusive" } */
vf rl2 (vf d, vf b) { return __builtin_xenon_vrlimi (d, b, -1, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 15, inclusive" } */
vf rl3 (vf d, vf b) { return __builtin_xenon_vrlimi (d, b, 15, 4); }	/* { dg-error "argument 4 must be a literal between 0 and 3, inclusive" } */
vf rl4 (vf d, vf b, int m) { return __builtin_xenon_vrlimi (d, b, m, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 15, inclusive" } */
vf rl5 (vf d, vf b, int r) { return __builtin_xenon_vrlimi (d, b, 1, r); }	/* { dg-error "argument 4 must be a literal between 0 and 3, inclusive" } */
vui rl6 (vui d, vui b) { return __builtin_xenon_vrlimi (d, b, 31, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 15, inclusive" } */
vf rl7 (vf d, vf b) { return __builtin_xenon_vrlimi_v4sf (d, b, 32, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 15, inclusive" } */

vf pw1 (vf b) { return __builtin_xenon_vpermwi (b, 256); }	/* { dg-error "argument 2 must be a literal between 0 and 255, inclusive" } */
vf pw2 (vf b) { return __builtin_xenon_vpermwi (b, -1); }	/* { dg-error "argument 2 must be a literal between 0 and 255, inclusive" } */
vui pw3 (vui b, int p) { return __builtin_xenon_vpermwi (b, p); }	/* { dg-error "argument 2 must be a literal between 0 and 255, inclusive" } */

vf pk1 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, 7, 1, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 6, inclusive" } */
vf pk2 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, -1, 1, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 6, inclusive" } */
vf pk3 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, 0, 0, 0); }	/* { dg-error "argument 4 must be a literal between 1 and 3, inclusive" } */
vf pk4 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, 0, 4, 0); }	/* { dg-error "argument 4 must be a literal between 1 and 3, inclusive" } */
vf pk5 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, 0, 1, 4); }	/* { dg-error "argument 5 must be a literal between 0 and 3, inclusive" } */
vui pk6 (vui d, vf b, int s) { return __builtin_xenon_vpkd3d (d, b, 0, 1, s); }	/* { dg-error "argument 5 must be a literal between 0 and 3, inclusive" } */
vf pk7 (vf d, vf b, int t) { return __builtin_xenon_vpkd3d (d, b, t, 1, 0); }	/* { dg-error "argument 3 must be a literal between 0 and 6, inclusive" } */

vf up1 (vui b) { return __builtin_xenon_vupkd3d (b, 7); }	/* { dg-error "argument 2 must be a literal between 0 and 6, inclusive" } */
vf up2 (vui b) { return __builtin_xenon_vupkd3d (b, -1); }	/* { dg-error "argument 2 must be a literal between 0 and 6, inclusive" } */
vf up3 (vf b, int t) { return __builtin_xenon_vupkd3d (b, t); }	/* { dg-error "argument 2 must be a literal between 0 and 6, inclusive" } */

/* The limits themselves are accepted.  */
vf ok1 (vf d, vf b) { return __builtin_xenon_vrlimi (d, b, 15, 3); }
vf ok2 (vf d, vf b) { return __builtin_xenon_vrlimi (d, b, 0, 0); }
vf ok3 (vf b) { return __builtin_xenon_vpermwi (b, 255); }
vf ok4 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, 6, 3, 3); }
vf ok5 (vf d, vf b) { return __builtin_xenon_vpkd3d (d, b, 0, 1, 0); }
vf ok6 (vui b) { return __builtin_xenon_vupkd3d (b, 6); }
vf ok7 (vui b) { return __builtin_xenon_vupkd3d (b, 0); }
