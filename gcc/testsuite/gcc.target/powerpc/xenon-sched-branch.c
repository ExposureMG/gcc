/* Xenon: BRANCH_COST stays below the dojump.cc threshold (4), so a branch
   on && / || of comparisons stays a short-circuit branch; it is not turned
   into logic on setcc values, which for FP compares is a 26-cycle
   microcoded mfcr and for integers a serial carry chain.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mmfcr\M} } } */
/* { dg-final { scan-assembler-not {\mmfocrf\M} } } */
/* { dg-final { scan-assembler-not {\msubf(ic|e)\M} } } */
/* { dg-final { scan-assembler-times {\mfcmpu\M} 2 } } */

extern void abort (void);
void g (_Complex double x, _Complex double y) { if (x != y) abort (); }
int k (long a, long b, long c, long d) { if (a != b || c != d) return 5; return 9; }
