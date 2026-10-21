/* -mcpu=xenon never turns -mrecip-precision on: -mrecip=all keeps the
   standard Newton-Raphson step counts for the 8-bit fres and 9-bit frsqrte
   estimates (isa/FINDINGS.md 1.2).  With -mrecip-precision the single
   division would take one step instead of two and the double rsqrt two
   instead of three; the step counts are checked through the number of
   fused multiply-adds.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-Ofast -mrecip=all -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mfres\M} } } */
/* { dg-final { scan-assembler {\mfrsqrte\M} } } */
/* { dg-final { scan-assembler-times {\mf(n?m(add|sub))s\M} 5 } } */
/* { dg-final { scan-assembler-times {\mf(n?m(add|sub))\M} 11 } } */

float rf (float a, float b) { return a / b; }
double rs (double x) { return 1.0 / __builtin_sqrt (x); }
