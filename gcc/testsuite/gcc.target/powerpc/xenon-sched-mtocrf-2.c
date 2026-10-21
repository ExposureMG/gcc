/* -mtune other than Xenon keeps mtcrf in the epilogue.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mdejagnu-tune=cell" } */
/* { dg-final { scan-assembler {\tmtcrf\M} } } */
/* { dg-final { scan-assembler-not {\tmtocrf\M} } } */

extern void g (void);

int
f (int a, int b)
{
  __asm__ __volatile__ ("" ::: "cr2", "cr3", "cr4");
  g ();
  return a + b;
}
