/* Xenon VMX128 (plan 4.2, step 5): lvsl and lvsr stay legal and are
   emitted in their legacy form only (lvsl128 and lvsr128 are illegal on
   the hardware): a result wanted in v32-v127 is loaded into v0-v31 and
   copied.  Legacy-only instructions (integer add, byte merges) never name
   v32-v127 either.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mlvsl [0-9],} } } */
/* { dg-final { scan-assembler {\mlvsr [0-9]+,} } } */
/* { dg-final { scan-assembler-not {\mlvs[lr]128\M} } } */
/* { dg-final { scan-assembler {\mvor128 40,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mvadduwm [0-9]+,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mvmrghb [0-9]+,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler-not {^\t(v[a-z]+|lv[a-z]+|stv[a-z]+)\.? ([0-9]+,)*(3[2-9]|[4-9][0-9]|1[0-2][0-9])(,|$)} } } */

#include <altivec.h>

typedef __vector unsigned char vuc;
typedef __vector signed int vsi;

void
lvsl_hi (vuc *pr, const unsigned char *p)
{
  register vuc r __asm__ ("v40") = vec_lvsl (0, p);
  __asm__ ("" : "+wV" (r));
  *pr = r;
}

vuc lvsr_lo (const unsigned char *p) { return vec_lvsr (5, p); }

void
add_hi (vsi *pr, const vsi *pa)
{
  register vsi a __asm__ ("v40") = pa[0];
  register vsi b __asm__ ("v41") = pa[1];
  __asm__ ("" : "+wV" (a), "+wV" (b));
  register vsi r __asm__ ("v42") = vec_add (a, b);
  __asm__ ("" : "+wV" (r));
  *pr = r;
}

void
mrgb_hi (vuc *pr, const vuc *pa)
{
  register vuc a __asm__ ("v40") = pa[0];
  register vuc b __asm__ ("v41") = pa[1];
  __asm__ ("" : "+wV" (a), "+wV" (b));
  register vuc r __asm__ ("v42") = vec_mergeh (a, b);
  __asm__ ("" : "+wV" (r));
  *pr = r;
}
