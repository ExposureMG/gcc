/* Xenon: mtcrf costs 20 cycles even for one field, mtocrf 1 (FINDINGS 3.6);
   the epilogue restores each saved CR field with its own mtocrf (plan 5.2).
   mtocrf is never given several fields (Xenon then writes only one).  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mmtocrf (32|16|8),} 3 } } */
/* { dg-final { scan-assembler-not {\tmtcrf\M} } } */

extern void g (void);

int
f (int a, int b)
{
  __asm__ __volatile__ ("" ::: "cr2", "cr3", "cr4");
  g ();
  return a + b;
}
