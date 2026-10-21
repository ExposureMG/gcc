/* Xenon: the algebraic loads lha lhau lhax lhaux lwa lwax lwaux are
   microcoded (20-24 cycles); sign-extending loads use lhz/lwz + extsh/extsw
   (5 + 2 cycles, plan 5.3).  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\m(lwa|lwax|lwaux|lha|lhax|lhau|lhaux)\M} } } */
/* { dg-final { scan-assembler {\mextsw\M} } } */
/* { dg-final { scan-assembler {\mextsh\M} } } */
/* { dg-final { scan-assembler {\mlwzx?\M} } } */
/* { dg-final { scan-assembler {\mlhzx?\M} } } */

long ld_int (int *p) { return *p; }
long ld_int_idx (int *p, long i) { return p[i]; }
int ld_short (short *p) { return *p; }
long ld_short_long (short *p, long i) { return p[i]; }
long sum_int (int *p, int n)
{
  long s = 0;
  for (int i = 0; i < n; i++)
    s += p[i];
  return s;
}
int sum_short (short *p, int n)
{
  int s = 0;
  for (int i = 0; i < n; i++)
    s += p[i] * 3;
  return s;
}
struct s { short h; int w; };
long fld (struct s *p) { return p->h + (long) p->w; }
