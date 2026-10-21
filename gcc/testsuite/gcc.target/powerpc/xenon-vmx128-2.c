/* Xenon VMX128 (plan 4.2, step 5): the destructive VMX128 forms and
   vperm128.  vmaddfp128 vD,vA,vB,vD = vA*vB + vD and vmaddcfp128
   vD,vA,vD,vB = vA*vD + vB implement fma (the addend or a multiplicand
   tied to the destination), vnmsubfp128 vD,vA,vB,vD = -(vA*vB - vD) the
   negated fms, vsel128 vD,vA,vB,vD takes its mask from vD; vperm128's
   control register is v0-v7 (wU), the legacy vperm's anything in v0-v31.
   With every register in v0-v31 the legacy mnemonics are printed.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

/* Addend tied.  */
/* { dg-final { scan-assembler {\mvmaddfp128 42,4[01],4[01],42$} } } */
/* Multiplicand tied (operand order vD,vA,vD,vB).  */
/* { dg-final { scan-assembler {\mvmaddcfp128 41,40,41,42$} } } */
/* { dg-final { scan-assembler {\mvmaddcfp128 40,41,40,42$} } } */
/* { dg-final { scan-assembler {\mvnmsubfp128 42,4[01],4[01],42$} } } */
/* { dg-final { scan-assembler {\mvsel128 42,40,41,42$} } } */
/* Result in a fresh register while the addend stays live: a copy, then
   a tied form.  */
/* { dg-final { scan-assembler {\mvmadd(c)?fp128 43,[0-9]+,[0-9]+,[0-9]+$} } } */
/* vperm128 with the control in v0-v7, copied there from v20.  */
/* { dg-final { scan-assembler {\mvperm128 42,40,41,[0-7]$} } } */
/* { dg-final { scan-assembler-not {\mvperm128 [0-9]+,[0-9]+,[0-9]+,([89]|[1-9][0-9]+)$} } } */
/* The same operations with every register in v0-v31: legacy forms; the
   legacy vperm takes its control from v20 directly.  */
/* { dg-final { scan-assembler {\mvmaddfp 6,4,5,6$} } } */
/* { dg-final { scan-assembler {\mvnmsubfp 6,4,5,6$} } } */
/* { dg-final { scan-assembler {\mvsel 6,4,5,6$} } } */
/* { dg-final { scan-assembler {\mvperm 6,4,5,20$} } } */
/* No legacy mnemonic with a v32+ operand.  */
/* { dg-final { scan-assembler-not {^\t(v[a-z]+|lv[a-z]+|stv[a-z]+)\.? ([0-9]+,)*(3[2-9]|[4-9][0-9]|1[0-2][0-9])(,|$)} } } */

#include <altivec.h>

typedef __vector float vf;
typedef __vector unsigned int vui;
typedef __vector unsigned char vuc;

#define IN3(T, A, B, C, RA, RB, RC)					\
  register T A __asm__ (RA) = pa[0];					\
  register T B __asm__ (RB) = pa[1];					\
  register T C __asm__ (RC) = pa[2];					\
  __asm__ ("" : "+wV" (A), "+wV" (B), "+wV" (C))

void
madd_addend (vf *pr, const vf *pa)
{
  IN3 (vf, a, b, c, "v40", "v41", "v42");
  c = vec_madd (a, b, c);
  __asm__ ("" : "+wV" (c));
  *pr = c;
}

void
madd_mult (vf *pr, const vf *pa)
{
  IN3 (vf, a, b, c, "v40", "v41", "v42");
  b = vec_madd (a, b, c);
  __asm__ ("" : "+wV" (b));
  *pr = b;
}

void
madd_mult1 (vf *pr, const vf *pa)
{
  IN3 (vf, a, b, c, "v40", "v41", "v42");
  a = vec_madd (a, b, c);
  __asm__ ("" : "+wV" (a));
  *pr = a;
}

void
madd_copy (vf *pr, const vf *pa)
{
  IN3 (vf, a, b, c, "v40", "v41", "v42");
  register vf r __asm__ ("v43") = vec_madd (a, b, c);
  __asm__ ("" : "+wV" (r), "+wV" (a), "+wV" (b), "+wV" (c));
  pr[0] = r;
  pr[1] = c;
}

void
nmsub_tied (vf *pr, const vf *pa)
{
  IN3 (vf, a, b, c, "v40", "v41", "v42");
  c = vec_nmsub (a, b, c);
  __asm__ ("" : "+wV" (c));
  *pr = c;
}

void
sel_tied (vui *pr, const vui *pa)
{
  IN3 (vui, a, b, m, "v40", "v41", "v42");
  m = vec_sel (a, b, m);
  __asm__ ("" : "+wV" (m));
  *pr = m;
}

void
perm_hi (vuc *pr, const vuc *pa)
{
  IN3 (vuc, a, b, c, "v40", "v41", "v20");
  register vuc r __asm__ ("v42") = vec_perm (a, b, c);
  __asm__ ("" : "+wV" (r));
  *pr = r;
}

/* Every register in v0-v31.  */
void
madd_lo (vf *pr, const vf *pa)
{
  IN3 (vf, a, b, c, "v4", "v5", "v6");
  c = vec_madd (a, b, c);
  __asm__ ("" : "+v" (c));
  *pr = c;
}

void
nmsub_lo (vf *pr, const vf *pa)
{
  IN3 (vf, a, b, c, "v4", "v5", "v6");
  c = vec_nmsub (a, b, c);
  __asm__ ("" : "+v" (c));
  *pr = c;
}

void
sel_lo (vui *pr, const vui *pa)
{
  IN3 (vui, a, b, m, "v4", "v5", "v6");
  m = vec_sel (a, b, m);
  __asm__ ("" : "+v" (m));
  *pr = m;
}

void
perm_lo (vuc *pr, const vuc *pa)
{
  IN3 (vuc, a, b, c, "v4", "v5", "v20");
  register vuc r __asm__ ("v6") = vec_perm (a, b, c);
  __asm__ ("" : "+v" (r));
  *pr = r;
}
