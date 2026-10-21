/* Without -mrecip, -ffast-math on Xenon divides and takes square roots with
   fdivs/fdiv/fsqrts/fsqrt and uses no estimate instruction Xenon lacks;
   the default -mrecip (low precision) leaves DFmode rsqrt alone as well.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -ffast-math -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mfre\M} } } */
/* { dg-final { scan-assembler-not {\mfrsqrtes\M} } } */
/* { dg-final { scan-assembler-not {\mfrsqrte\M} } } */
/* { dg-final { scan-assembler-not {\mfres\M} } } */
/* { dg-final { scan-assembler {\mfdivs\M} } } */
/* { dg-final { scan-assembler {\mfdiv\M} } } */
/* { dg-final { scan-assembler {\mfsqrt\M} } } */
/* { dg-final { scan-assembler {\mfsqrts\M} } } */

float divf (float a, float b) { return a / b; }
double divd (double a, double b) { return a / b; }
float rsqrtf (float a) { return 1.0f / __builtin_sqrtf (a); }
double sqrtd_ (double a) { return __builtin_sqrt (a); }
