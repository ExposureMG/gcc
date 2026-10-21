/* Xenon: vec_mul on integer vectors (and plain * on vector types) uses the
   replacement sequences; the float intrinsics are unaffected.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mvmul[eo][su][bh]\M} } } */
/* { dg-final { scan-assembler-not {\mvmsum[a-z]+\M} } } */
/* { dg-final { scan-assembler-not {\mvmladduhm\M} } } */
/* { dg-final { scan-assembler-times {\m(vctuxs|vcfpuxws128)\M} 12 } } */
/* { dg-final { scan-assembler {\mvmaddfp\M} } } */
/* { dg-final { scan-assembler {\mvnmsubfp\M} } } */

#include <altivec.h>

vector signed int mul_si (vector signed int a, vector signed int b) { return vec_mul (a, b); }
vector unsigned int mul_ui (vector unsigned int a, vector unsigned int b) { return vec_mul (a, b); }
vector signed short mul_ss (vector signed short a, vector signed short b) { return vec_mul (a, b); }
vector unsigned short mul_us (vector unsigned short a, vector unsigned short b) { return vec_mul (a, b); }
vector signed char mul_sc (vector signed char a, vector signed char b) { return vec_mul (a, b); }
vector unsigned char mul_uc (vector unsigned char a, vector unsigned char b) { return vec_mul (a, b); }
vector float madd_f (vector float a, vector float b, vector float c) { return vec_madd (a, b, c); }
vector float nmsub_f (vector float a, vector float b, vector float c) { return vec_nmsub (a, b, c); }
vector float mul_f (vector float a, vector float b) { return a * b; }
