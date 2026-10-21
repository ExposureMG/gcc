/* Xenon: the lwz + extsw replacement of lwa is a speed choice; at -Os the
   shorter algebraic load stays.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-Os -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mlwax?\M} } } */
/* { dg-final { scan-assembler {\mlhax?\M} } } */

long ld_int (int *p) { return *p; }
int ld_short (short *p) { return *p; }
