/* Xenon: divw/divd cost 28 cycles of issue and mullw/mulld 11/13, mulli 7
   (plan 5.3).  Division by a constant uses the multiply-high sequence and
   multiplication by a small constant uses shifts and adds.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mdiv[wd]u?\M} } } */
/* { dg-final { scan-assembler-not {\mmulli\M} } } */
/* { dg-final { scan-assembler-not {\mmull[wd]\M} } } */
/* { dg-final { scan-assembler-not {\m(slw|srw|sraw|sld|srd|srad)\M} } } */
/* { dg-final { scan-assembler {\mmulhw\M} } } */

int div7 (int x) { return x / 7; }
unsigned udiv10 (unsigned x) { return x / 10; }
long ldiv3 (long x) { return x / 3; }
int mod16 (int x) { return x % 16; }
int div8 (int x) { return x / 8; }
int mul10 (int x) { return x * 10; }
long mul9 (long x) { return x * 9; }
int mul5 (int x) { return x * 5; }
int shl (int x) { int s = 5; return x << s; }
