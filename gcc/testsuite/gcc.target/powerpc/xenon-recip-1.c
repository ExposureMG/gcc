/* Xenon has fres and frsqrte but no fre and no frsqrtes (both raise the
   illegal-instruction interrupt), so TARGET_FRE and TARGET_FRSQRTES are
   false and -mrecip may use only the estimates that exist: fres for SFmode
   division and frsqrte for DFmode reciprocal square roots.  DFmode division
   and SFmode reciprocal square roots keep fdiv / fsqrts + fres.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -ffast-math -mrecip=all -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mfre\M} } } */
/* { dg-final { scan-assembler-not {\mfre\.} } } */
/* { dg-final { scan-assembler-not {\mfrsqrtes\M} } } */
/* { dg-final { scan-assembler-not {\mx[sv][a-z]+} } } */
/* { dg-final { scan-assembler {\mfres\M} } } */
/* { dg-final { scan-assembler {\mfrsqrte\M} } } */
/* { dg-final { scan-assembler {\mfdiv\M} } } */
/* { dg-final { scan-assembler {\mvrefp\M} } } */
/* { dg-final { scan-assembler {\mvrsqrtefp\M} } } */

#include <altivec.h>

float divf (float a, float b) { return a / b; }
double divd (double a, double b) { return a / b; }
float rsqrtf (float a) { return 1.0f / __builtin_sqrtf (a); }
double rsqrtd (double a) { return 1.0 / __builtin_sqrt (a); }
float sqrtf_ (float a) { return __builtin_sqrtf (a); }
double sqrtd_ (double a) { return __builtin_sqrt (a); }

vector float vre (vector float a) { return vec_re (a); }
vector float vrsqrte (vector float a) { return vec_rsqrte (a); }
