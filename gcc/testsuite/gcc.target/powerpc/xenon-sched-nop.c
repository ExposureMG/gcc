/* Xenon: "or 1,1,1" / "or 2,2,2" / "or 3,3,3" set the thread priority and
   are only ever emitted as volatile asm, never as a register self-move.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mor 1,1,1\M} 1 } } */
/* { dg-final { scan-assembler-not {\mmr ([0-9]+),\1\M} } } */

void low_priority (void) { __asm__ __volatile__ ("or 1,1,1"); }
long id (long x) { return x; }
