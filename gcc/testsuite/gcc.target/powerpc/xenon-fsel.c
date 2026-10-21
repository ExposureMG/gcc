/* Xenon has fsel (it is in the allow-list, PPC_GFXOPT): with -ffast-math
   floating-point selects use it; integer selects never use isel.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -ffast-math -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mfsel\M} } } */
/* { dg-final { scan-assembler-not {\misel\M} } } */
/* { dg-final { scan-assembler-not {\mx[sv][a-z]+} } } */

double seld (double a, double b, double c) { return a >= 0 ? b : c; }
float self (float a, float b, float c) { return a >= 0 ? b : c; }
double maxd (double a, double b) { return a > b ? a : b; }
int seli (int a, int b, int c) { return a >= 0 ? b : c; }
