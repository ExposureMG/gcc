/* Xenon has no isel: conditional moves of integers use branches or
   arithmetic.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\misel\M} } } */
/* { dg-final { scan-assembler-not {\msetb\M} } } */
/* { dg-final { scan-assembler-not {\mmaddld\M} } } */

int sel (int a, int b, int c) { return a < b ? c : a; }
long sell (long a, long b, long c, long d) { return a == b ? c : d; }
unsigned selu (unsigned a, unsigned b) { return a > b ? a : b; }
int mn (int a, int b) { return a < b ? a : b; }
long cmp3 (long a, long b) { return (a > b) - (a < b); }
