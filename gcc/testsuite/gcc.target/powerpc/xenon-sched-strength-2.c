/* Xenon costs are in units of a 2-cycle add (ppcxenon_cost via
   XENON_COSTS, as Cell's): divisions and remainders by constants use the
   multiply-high sequence, never a 28-cycle divide, and a large 64-bit
   multiplier uses mulld instead of a long serial shift/add chain.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mdiv[wd]u?\M} } } */
/* { dg-final { scan-assembler {\mmulhw\M} } } */
/* { dg-final { scan-assembler {\mmulhwu\M} } } */
/* { dg-final { scan-assembler {\mmulld\M} } } */

int rem19 (int x) { return x % 19; }
int div19 (int x) { return x / 19; }
unsigned udiv10 (unsigned x) { return x / 10; }
int rem10 (int x) { return x % 10; }
struct { unsigned int i : 6, j : 11, k : 15; } bf;
void bfrem (void) { bf.i %= 19; }
unsigned long lcg (unsigned long s) { return s * 0x41c64e6dUL + 12345; }
