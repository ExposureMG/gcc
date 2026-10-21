/* Xenon VMX128-only builtins (step 7): the destructive forms vmaddcfp128,
   vrlimi128 and vpkd3d128 read their destination register (JSON
   tied_operands [VD128]).  When the tied input stays live, the register
   allocator copies it into the result register first (vor128 for v32+,
   the legacy vor when both are in v0-v31), then the instruction updates
   the copy.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

/* { dg-final { scan-assembler {\mvor128 43,41,41\M} } } */
/* { dg-final { scan-assembler {\mvrlimi128 43,40,3,1$} } } */
/* { dg-final { scan-assembler {\mvor128 44,41,41\M} } } */
/* { dg-final { scan-assembler {\mvpkd3d128 44,40,4,1,2$} } } */
/* { dg-final { scan-assembler {\mvor128 45,41,41\M} } } */
/* { dg-final { scan-assembler {\mvmaddcfp128 45,40,45,42$} } } */
/* Low registers: the legacy vor copies.  */
/* { dg-final { scan-assembler {\mvor 6,5,5\M} } } */
/* { dg-final { scan-assembler {\mvrlimi128 6,4,12,3$} } } */
/* The old value is still stored from its own register.  */
/* { dg-final { scan-assembler-times {\mstvx128 41,} 3 } } */
/* No legacy mnemonic with a v32+ operand.  */
/* { dg-final { scan-assembler-not {^\t(v[a-z]+|lv[a-z]+|stv[a-z]+)\.? ([0-9]+,)*(3[2-9]|[4-9][0-9]|1[0-2][0-9])(,|$)} } } */

typedef __vector float vf;

void
rl_copy (vf *pr, const vf *pa)
{
  register vf b __asm__ ("v40") = pa[0];
  register vf d __asm__ ("v41") = pa[1];
  __asm__ ("" : "+wV" (b), "+wV" (d));
  register vf r __asm__ ("v43") = __builtin_xenon_vrlimi (d, b, 3, 1);
  __asm__ ("" : "+wV" (r), "+wV" (d), "+wV" (b));
  pr[0] = r;
  pr[1] = d;
}

void
pk_copy (vf *pr, const vf *pa)
{
  register vf b __asm__ ("v40") = pa[0];
  register vf d __asm__ ("v41") = pa[1];
  __asm__ ("" : "+wV" (b), "+wV" (d));
  register vf r __asm__ ("v44") = __builtin_xenon_vpkd3d (d, b, 4, 1, 2);
  __asm__ ("" : "+wV" (r), "+wV" (d), "+wV" (b));
  pr[0] = r;
  pr[1] = d;
}

void
madd_copy (vf *pr, const vf *pa)
{
  register vf a __asm__ ("v40") = pa[0];
  register vf d __asm__ ("v41") = pa[1];
  register vf b __asm__ ("v42") = pa[2];
  __asm__ ("" : "+wV" (a), "+wV" (d), "+wV" (b));
  register vf r __asm__ ("v45") = __builtin_xenon_vmaddcfp (a, d, b);
  __asm__ ("" : "+wV" (r), "+wV" (d));
  pr[0] = r;
  pr[1] = d;
}

void
low_copy (vf *pr, const vf *pa)
{
  register vf a __asm__ ("v4") = pa[0];
  register vf d __asm__ ("v5") = pa[1];
  __asm__ ("" : "+v" (a), "+v" (d));
  register vf r __asm__ ("v6") = __builtin_xenon_vrlimi (d, a, 12, 3);
  __asm__ ("" : "+v" (r), "+v" (d));
  pr[0] = r;
  pr[1] = d;
}
