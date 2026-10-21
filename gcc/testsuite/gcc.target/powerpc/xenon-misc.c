/* Assorted scalar code under -mcpu=xenon: none of the hardware-illegal
   scalar instructions (popcntb mcrxr dcba fre frsqrtes) and none from
   later ISAs appear.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\m(popcntb|mcrxr|dcba|fre|frsqrtes)\M} } } */
/* { dg-final { scan-assembler-not {\m(isel|cmpb|fri[mnpz]|fcpsgn|lfiw[az]x|fcfids|fcfidu|fctiwu|fctidu|mfvsr|mtvsr|ldbrx\.)\M} } } */
/* { dg-final { scan-assembler-not {\mx[sv][a-z]+} } } */
/* { dg-final { scan-assembler {\mdcbt\M} } } */
/* { dg-final { scan-assembler {\mdcbtst\M} } } */

#include <stddef.h>

void pf (const char *p, char *q) { __builtin_prefetch (p); __builtin_prefetch (q, 1); }
double fmaxd (double a, double b) { return __builtin_fmax (a, b); }
double sel (double a, double b, double c)
{
  return a >= 0 ? b : c;
}
float selff (float a, float b, float c) { return __builtin_isless (a, 0.0f) ? b : c; }
double cps (double a, double b) { return __builtin_copysign (a, b); }
double rnd (double a) { return __builtin_floor (a) + __builtin_ceil (a) + __builtin_trunc (a); }
long ftoi (double a) { return (long) a; }
unsigned long ftou (double a) { return (unsigned long) a; }
double itof (long a) { return (double) a; }
double utof (unsigned long a) { return (double) a; }
float itofs (int a) { return (float) a; }
int ovf (int a, int b, int *r) { return __builtin_add_overflow (a, b, r); }
int ovfm (long a, long b, long *r) { return __builtin_mul_overflow (a, b, r); }
void clr (char *p) { __builtin_memset (p, 0, 4096); }
void cpy (char *p, const char *q, size_t n) { __builtin_memcpy (p, q, n); }
int cmpm (const char *p, const char *q) { return __builtin_memcmp (p, q, 64); }
int cmps (const char *p, const char *q) { return __builtin_strcmp (p, q); }
