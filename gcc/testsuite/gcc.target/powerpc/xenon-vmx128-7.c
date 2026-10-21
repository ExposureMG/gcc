/* Xenon VMX128: without contraction a*b+c is vmulfp128 + vaddfp.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -ffp-contract=off" } */
/* { dg-final { scan-assembler-times {\mvmulfp128\M} 2 } } */
/* { dg-final { scan-assembler-times {\mvaddfp(128)?\M} 1 } } */
/* { dg-final { scan-assembler-not {\mvmadd(c)?fp(128)?\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

v4sf mul (v4sf a, v4sf b) { return a * b; }
v4sf muladd (v4sf a, v4sf b, v4sf c) { return a * b + c; }
