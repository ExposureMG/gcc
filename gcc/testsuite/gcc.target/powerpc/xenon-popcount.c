/* Xenon has no popcntb (nor popcntw/popcntd): __builtin_popcount and
   __builtin_parity expand to generic code.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mpopcnt[bwd]\M} } } */
/* { dg-final { scan-assembler-not {\mprty[wd]\M} } } */
/* { dg-final { scan-assembler-not {\mcmpb\M} } } */

int pop32 (unsigned int x) { return __builtin_popcount (x); }
int pop64 (unsigned long x) { return __builtin_popcountl (x); }
int pop64ll (unsigned long long x) { return __builtin_popcountll (x); }
int par32 (unsigned int x) { return __builtin_parity (x); }
int par64 (unsigned long x) { return __builtin_parityl (x); }
int ctz32 (unsigned int x) { return __builtin_ctz (x); }
int ffs64 (long x) { return __builtin_ffsl (x); }
int clz64 (unsigned long x) { return __builtin_clzl (x); }
