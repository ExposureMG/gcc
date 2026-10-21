/* Xenon VMX128 (plan 4.1/4.3): an asm that clobbers v32-v127 makes the
   compiler keep its values out of them; a clobbered v64-v127 is a
   callee-saved register the function uses (its save is plan step 4), a
   clobbered v32-v63 needs no save.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\m(lvx|stvx|vor)128 (3[2-9]|4[0-9]|50),} } } */
/* { dg-final { scan-assembler {\mlvx128 51,} } } */

typedef __attribute__ ((vector_size (16))) int v4si;

void
f (v4si *restrict o, const v4si *restrict a)
{
  v4si x = a[0];
  __asm__ __volatile__ ("" ::: "v32", "v33", "v34", "v35", "v36", "v37",
			"v38", "v39", "v40", "v41", "v42", "v43", "v44",
			"v45", "v46", "v47", "v48", "v49", "v50", "memory");
  o[0] = x;
}
