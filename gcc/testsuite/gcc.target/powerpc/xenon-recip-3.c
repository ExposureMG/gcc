/* -mrecip (default precision) on Xenon: fres for SFmode division, no
   frsqrte for DFmode (low-precision estimates), never fre/frsqrtes.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -ffast-math -mrecip -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mfre\M} } } */
/* { dg-final { scan-assembler-not {\mfrsqrtes\M} } } */
/* { dg-final { scan-assembler {\mfres\M} } } */
/* { dg-final { scan-assembler {\mfdiv\M} } } */

float divf (float a, float b) { return a / b; }
double divd (double a, double b) { return a / b; }
float rsqrtf (float a) { return 1.0f / __builtin_sqrtf (a); }
double rsqrtd (double a) { return 1.0 / __builtin_sqrt (a); }
