/* Xenon has the Cell vector load/store left/right instructions; the Cell
   intrinsics are available under -mcpu=xenon (TARGET_CELL_VEC_LDST).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mlvlx\M} 1 } } */
/* { dg-final { scan-assembler-times {\mlvlxl\M} 1 } } */
/* { dg-final { scan-assembler-times {\mlvrx\M} 1 } } */
/* { dg-final { scan-assembler-times {\mlvrxl\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstvlx\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstvlxl\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstvrx\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstvrxl\M} 1 } } */

#include <altivec.h>

typedef __vector unsigned char vuc;
typedef __vector float vf;

vuc l1 (long a, unsigned char *p) { return vec_lvlx (a, p); }
vuc l2 (long a, unsigned char *p) { return vec_lvlxl (a, p); }
vf l3 (long a, float *p) { return vec_lvrx (a, p); }
vf l4 (long a, float *p) { return vec_lvrxl (a, p); }
void s1 (vuc v, long a, unsigned char *p) { vec_stvlx (v, a, p); }
void s2 (vuc v, long a, unsigned char *p) { vec_stvlxl (v, a, p); }
void s3 (vf v, long a, float *p) { vec_stvrx (v, a, p); }
void s4 (vf v, long a, float *p) { vec_stvrxl (v, a, p); }
