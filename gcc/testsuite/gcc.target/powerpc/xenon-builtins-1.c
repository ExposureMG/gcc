/* Xenon VMX128-only builtins (plan 4.2, D7, step 7): each builtin emits
   exactly its instruction, with the operand order of isa/xenon_isa.json:
   vmsum3fp128/vmsum4fp128/vmulfp128 vD,vA,vB; vmaddcfp128 vD,vA,vD,vB
   (= vA*vD + vB, the multiplicand tied); vrlimi128 vD,vB,UIMM,VD3D2 (vD
   tied); vpermwi128 vD,vB,PERM; vpkd3d128 vD,vB,VD3D0,VD3D1,VD3D2 (vD
   tied); vupkd3d128 vD,vB,UIMM with UIMM = type << 2.  The operands are
   forced into v40-v43 (and v0-v31, v64+, v127) with register asm; the 128
   mnemonic is printed for every register.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

/* { dg-final { scan-assembler {\mvmsum3fp128 42,40,41$} } } */
/* { dg-final { scan-assembler {\mvmsum4fp128 42,40,41$} } } */
/* { dg-final { scan-assembler {\mvmulfp128 42,40,41$} } } */
/* { dg-final { scan-assembler {\mvmaddcfp128 41,40,41,42$} } } */
/* { dg-final { scan-assembler {\mvrlimi128 42,40,9,2$} } } */
/* { dg-final { scan-assembler {\mvpermwi128 42,40,27$} } } */
/* { dg-final { scan-assembler {\mvpkd3d128 42,40,5,2,3$} } } */
/* { dg-final { scan-assembler {\mvupkd3d128 42,40,24$} } } */
/* Low registers: still the 128 forms (no legacy equivalent).  */
/* { dg-final { scan-assembler {\mvmsum4fp128 6,4,5$} } } */
/* { dg-final { scan-assembler {\mvmaddcfp128 5,4,5,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mvrlimi128 6,4,15,0$} } } */
/* High registers.  */
/* { dg-final { scan-assembler {\mvmsum3fp128 127,64,100$} } } */
/* { dg-final { scan-assembler {\mvpermwi128 127,64,255$} } } */
/* { dg-final { scan-assembler {\mvpkd3d128 127,64,0,1,0$} } } */
/* { dg-final { scan-assembler {\mvupkd3d128 64,127,0$} } } */
/* Integer types use the same instructions.  */
/* { dg-final { scan-assembler {\mvrlimi128 42,40,1,3$} } } */
/* { dg-final { scan-assembler {\mvpermwi128 42,40,147$} } } */
/* { dg-final { scan-assembler {\mvpkd3d128 42,40,6,3,1$} } } */
/* Exactly one instruction per call.  */
/* { dg-final { scan-assembler-times {\mvmsum3fp128 } 2 } } */
/* { dg-final { scan-assembler-times {\mvmsum4fp128 } 2 } } */
/* { dg-final { scan-assembler-times {\mvmulfp128 } 1 } } */
/* { dg-final { scan-assembler-times {\mvmaddcfp128 } 2 } } */
/* { dg-final { scan-assembler-times {\mvrlimi128 } 3 } } */
/* { dg-final { scan-assembler-times {\mvpermwi128 } 3 } } */
/* { dg-final { scan-assembler-times {\mvpkd3d128 } 3 } } */
/* { dg-final { scan-assembler-times {\mvupkd3d128 } 2 } } */
/* No other vector arithmetic (only the asm-barrier-free loads/stores).  */
/* { dg-final { scan-assembler-not {\mv(madd|nmsub)fp|\mvperm |\mvsldoi|\mvsel} } } */

typedef __vector float vf;
typedef __vector unsigned int vui;
typedef __vector signed int vsi;

#define IN2(T, A, B, RA, RB)						\
  register T A __asm__ (RA) = pa[0];					\
  register T B __asm__ (RB) = pa[1];					\
  __asm__ ("" : "+wV" (A), "+wV" (B))

#define OUT(T, R, RR, E)						\
  register T R __asm__ (RR) = (E);					\
  __asm__ ("" : "+wV" (R));						\
  *pr = R

void
msum3 (vf *pr, const vf *pa)
{
  IN2 (vf, a, b, "v40", "v41");
  OUT (vf, r, "v42", __builtin_xenon_vmsum3fp (a, b));
}

void
msum4 (vf *pr, const vf *pa)
{
  IN2 (vf, a, b, "v40", "v41");
  OUT (vf, r, "v42", __builtin_xenon_vmsum4fp (a, b));
}

void
mulfp (vf *pr, const vf *pa)
{
  IN2 (vf, a, b, "v40", "v41");
  OUT (vf, r, "v42", __builtin_xenon_vmulfp (a, b));
}

void
maddc (vf *pr, const vf *pa)
{
  register vf a __asm__ ("v40") = pa[0];
  register vf d __asm__ ("v41") = pa[1];
  register vf b __asm__ ("v42") = pa[2];
  __asm__ ("" : "+wV" (a), "+wV" (d), "+wV" (b));
  d = __builtin_xenon_vmaddcfp (a, d, b);
  __asm__ ("" : "+wV" (d));
  *pr = d;
}

void
rlimi (vf *pr, const vf *pa)
{
  IN2 (vf, b, d, "v40", "v42");
  d = __builtin_xenon_vrlimi (d, b, 9, 2);
  __asm__ ("" : "+wV" (d));
  *pr = d;
}

void
permwi (vf *pr, const vf *pa)
{
  register vf b __asm__ ("v40") = pa[0];
  __asm__ ("" : "+wV" (b));
  OUT (vf, r, "v42", __builtin_xenon_vpermwi (b, 0x1B));
}

void
pkd3d (vf *pr, const vf *pa)
{
  IN2 (vf, b, d, "v40", "v42");
  d = __builtin_xenon_vpkd3d (d, b, 5, 2, 3);
  __asm__ ("" : "+wV" (d));
  *pr = d;
}

void
upkd3d (vf *pr, const vui *pa)
{
  register vui b __asm__ ("v40") = pa[0];
  __asm__ ("" : "+wV" (b));
  OUT (vf, r, "v42", __builtin_xenon_vupkd3d (b, 6));
}

/* v0-v31 */
void
low (vf *pr, const vf *pa)
{
  register vf a __asm__ ("v4") = pa[0];
  register vf b __asm__ ("v5") = pa[1];
  __asm__ ("" : "+v" (a), "+v" (b));
  register vf r __asm__ ("v6") = __builtin_xenon_vmsum4fp (a, b);
  __asm__ ("" : "+v" (r));
  pr[0] = r;
  b = __builtin_xenon_vmaddcfp (a, b, r);
  __asm__ ("" : "+v" (b));
  pr[1] = b;
  r = __builtin_xenon_vrlimi (r, a, 15, 0);
  __asm__ ("" : "+v" (r));
  pr[2] = r;
}

/* v64-v127 */
void
high (vf *pr, const vf *pa, const vui *pu)
{
  register vf a __asm__ ("v64") = pa[0];
  register vf b __asm__ ("v100") = pa[1];
  __asm__ ("" : "+wV" (a), "+wV" (b));
  register vf r __asm__ ("v127") = __builtin_xenon_vmsum3fp (a, b);
  __asm__ ("" : "+wV" (r));
  pr[0] = r;
  r = __builtin_xenon_vpermwi (a, 255);
  __asm__ ("" : "+wV" (r));
  pr[1] = r;
  r = __builtin_xenon_vpkd3d (r, a, 0, 1, 0);
  __asm__ ("" : "+wV" (r));
  pr[2] = r;
  register vui u __asm__ ("v127") = pu[0];
  __asm__ ("" : "+wV" (u));
  a = __builtin_xenon_vupkd3d (u, 0);
  __asm__ ("" : "+wV" (a));
  pr[3] = a;
}

/* vector unsigned int / signed int */
void
ints (vui *pr, const vui *pa, vsi *ps)
{
  IN2 (vui, b, d, "v40", "v42");
  d = __builtin_xenon_vrlimi (d, b, 1, 3);
  __asm__ ("" : "+wV" (d));
  pr[0] = d;
  register vsi s __asm__ ("v40") = ps[0];
  __asm__ ("" : "+wV" (s));
  register vsi t __asm__ ("v42") = __builtin_xenon_vpermwi (s, 0x93);
  __asm__ ("" : "+wV" (t));
  ps[1] = t;
  t = __builtin_xenon_vpkd3d (t, (vf) s, 6, 3, 1);
  __asm__ ("" : "+wV" (t));
  ps[2] = t;
}
