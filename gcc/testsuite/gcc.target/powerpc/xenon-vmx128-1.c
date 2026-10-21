/* Xenon VMX128 (plan 4.2, step 5): every legacy AltiVec/Cell instruction
   with a VMX128 form of the same operands (tools/gen_xenon_retarget.py,
   kind DIRECT) is emitted in its 128 form when its operands are in
   v32-v127.  The inputs are register asm variables in v40/v41 that an asm
   keeps there, so the instruction must read v40/v41 directly; the result
   goes through a register asm variable in v42.  The legacy mnemonic must
   not appear with a v32+ operand (the runner also assembles this under
   .machine xenon, which rejects that).  The estimates (vrefp, vrsqrtefp,
   vexptefp, vlogefp) are kept legacy-only (kind "unverified"): see
   xenon-vmx128-9.c.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

/* Float arithmetic.  */
/* { dg-final { scan-assembler {\mvaddfp128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvsubfp128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvmulfp128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvmaxfp128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvminfp128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvrfim128 [0-9]+,40$} } } */
/* { dg-final { scan-assembler {\mvrfin128 [0-9]+,40$} } } */
/* { dg-final { scan-assembler {\mvrfip128 [0-9]+,40$} } } */
/* { dg-final { scan-assembler {\mvrfiz128 [0-9]+,40$} } } */
/* Conversions (the 128 names differ; the scale immediate is the same).  */
/* { dg-final { scan-assembler {\mvcsxwfp128 [0-9]+,40,3$} } } */
/* { dg-final { scan-assembler {\mvcuxwfp128 [0-9]+,40,31$} } } */
/* { dg-final { scan-assembler {\mvcfpsxws128 [0-9]+,40,5$} } } */
/* { dg-final { scan-assembler {\mvcfpuxws128 [0-9]+,40,0$} } } */
/* Compares, and the record (predicate) forms.  */
/* { dg-final { scan-assembler {\mvcmpeqfp128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvcmpgtfp128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvcmpgefp128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvcmpbfp128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvcmpequw128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvcmpeqfp128\. [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvcmpgtfp128\. [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvcmpgefp128\. [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvcmpbfp128\. [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvcmpequw128\. [0-9]+,4[01],4[01]$} } } */
/* Logical.  */
/* { dg-final { scan-assembler {\mvand128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvandc128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvor128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvxor128 [0-9]+,4[01],4[01]$} } } */
/* { dg-final { scan-assembler {\mvnor128 [0-9]+,4[01],4[01]$} } } */
/* Word merges, splats, shifts, rotates, octet shifts.  */
/* { dg-final { scan-assembler {\mvmrghw128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvmrglw128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvspltw128 [0-9]+,40,2$} } } */
/* { dg-final { scan-assembler {\mvspltw128 [0-9]+,40,1$} } } */
/* { dg-final { scan-assembler {\mvspltisw128 42,-7$} } } */
/* { dg-final { scan-assembler {\mvslw128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvsrw128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvsraw128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvrlw128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvslo128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvsro128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvsldoi128 [0-9]+,40,41,15$} } } */
/* Packs and unpacks.  */
/* { dg-final { scan-assembler {\mvpkswss128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvpkswus128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvpkuwus128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvpkuwum128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvpkshss128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvpkshus128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvpkuhus128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvpkuhum128 [0-9]+,40,41$} } } */
/* { dg-final { scan-assembler {\mvupkhsb128 [0-9]+,40$} } } */
/* { dg-final { scan-assembler {\mvupklsb128 [0-9]+,40$} } } */
/* { dg-final { scan-assembler {\mvupkhsh128 [0-9]+,40$} } } */
/* { dg-final { scan-assembler {\mvupklsh128 [0-9]+,40$} } } */
/* Loads and stores, indexed or indirect only.  */
/* { dg-final { scan-assembler {\mlvx128 42,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mstvx128 40,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mlvxl128 42,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mstvxl128 40,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mlvewx128 42,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mstvewx128 40,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mlvlx128 42,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mlvlxl128 42,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mlvrx128 42,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mlvrxl128 42,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mstvlx128 40,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mstvlxl128 40,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mstvrx128 40,[0-9]+,[0-9]+$} } } */
/* { dg-final { scan-assembler {\mstvrxl128 40,[0-9]+,[0-9]+$} } } */
/* No legacy mnemonic names v32-v127, and the multiply is not the
   vmaddfp-with--0.0 sequence.  */
/* { dg-final { scan-assembler-not {^\t(v[a-z]+|lv[a-z]+|stv[a-z]+)\.? ([0-9]+,)*(3[2-9]|[4-9][0-9]|1[0-2][0-9])(,|$)} } } */
/* { dg-final { scan-assembler-not {\mvspltisw [0-9]+,-1$} } } */
/* { dg-final { scan-assembler-not {\mlvs[lr]128\M} } } */

#include <altivec.h>

typedef __vector float vf;
typedef __vector signed int vsi;
typedef __vector unsigned int vui;
typedef __vector signed short vss;
typedef __vector unsigned short vus;
typedef __vector signed char vsc;
typedef __vector unsigned char vuc;
typedef __vector __bool int vbi;

#ifndef __VMX128__
#error __VMX128__ not defined
#endif

/* r = EXPR with a in v40, b in v41, r in v42.  */
#define OP2(NAME, TR, TA, TB, EXPR)					\
  void NAME (TR *pr, const TA *pa, const TB *pb)			\
  {									\
    register TA a __asm__ ("v40") = *pa;				\
    register TB b __asm__ ("v41") = *pb;				\
    register TR r __asm__ ("v42");					\
    __asm__ ("" : "+wV" (a), "+wV" (b));				\
    r = (EXPR);								\
    __asm__ ("" : "+wV" (r));						\
    *pr = r;								\
  }

#define OP1(NAME, TR, TA, EXPR)						\
  void NAME (TR *pr, const TA *pa)					\
  {									\
    register TA a __asm__ ("v40") = *pa;				\
    register TR r __asm__ ("v42");					\
    __asm__ ("" : "+wV" (a));						\
    r = (EXPR);								\
    __asm__ ("" : "+wV" (r));						\
    *pr = r;								\
  }

/* Predicates: an int from the CR6 test.  */
#define PRED(NAME, TA, EXPR)						\
  int NAME (const TA *pa, const TA *pb)					\
  {									\
    register TA a __asm__ ("v40") = *pa;				\
    register TA b __asm__ ("v41") = *pb;				\
    __asm__ ("" : "+wV" (a), "+wV" (b));				\
    return (EXPR);							\
  }

OP2 (add_f, vf, vf, vf, vec_add (a, b))
OP2 (sub_f, vf, vf, vf, vec_sub (a, b))
OP2 (mul_f, vf, vf, vf, a * b)
OP2 (max_f, vf, vf, vf, vec_max (a, b))
OP2 (min_f, vf, vf, vf, vec_min (a, b))
OP1 (floor_f, vf, vf, vec_floor (a))
OP1 (round_f, vf, vf, vec_round (a))
OP1 (ceil_f, vf, vf, vec_ceil (a))
OP1 (trunc_f, vf, vf, vec_trunc (a))

OP1 (ctf_s, vf, vsi, vec_ctf (a, 3))
OP1 (ctf_u, vf, vui, vec_ctf (a, 31))
OP1 (cts_f, vsi, vf, vec_cts (a, 5))
OP1 (ctu_f, vui, vf, vec_ctu (a, 0))

OP2 (cmpeq_f, vbi, vf, vf, vec_cmpeq (a, b))
OP2 (cmpgt_f, vbi, vf, vf, vec_cmpgt (a, b))
OP2 (cmpge_f, vbi, vf, vf, vec_cmpge (a, b))
OP2 (cmpb_f, vsi, vf, vf, vec_cmpb (a, b))
OP2 (cmpeq_w, vbi, vsi, vsi, vec_cmpeq (a, b))
PRED (all_eq_f, vf, vec_all_eq (a, b))
PRED (all_gt_f, vf, vec_all_gt (a, b))
PRED (all_ge_f, vf, vec_all_ge (a, b))
PRED (all_in_f, vf, vec_all_in (a, b))
PRED (any_eq_w, vsi, vec_any_eq (a, b))

OP2 (and_w, vsi, vsi, vsi, vec_and (a, b))
OP2 (andc_w, vsi, vsi, vsi, vec_andc (a, b))
OP2 (or_w, vsi, vsi, vsi, vec_or (a, b))
OP2 (xor_w, vsi, vsi, vsi, vec_xor (a, b))
OP2 (nor_w, vsi, vsi, vsi, vec_nor (a, b))

OP2 (mergeh_f, vf, vf, vf, vec_mergeh (a, b))
OP2 (mergel_w, vsi, vsi, vsi, vec_mergel (a, b))
/* (vec_splat on vector float goes through memory for every non-VSX
   AltiVec CPU in GCC 16: the builtin is folded to an element extract and a
   broadcast.  A constant shuffle gives the vspltw pattern.)  */
OP1 (splat_f, vf, vf, __builtin_shuffle (a, (vsi) { 2, 2, 2, 2 }))
OP1 (splat_w, vsi, vsi, vec_splat (a, 1))
OP2 (sl_w, vui, vui, vui, vec_sl (a, b))
OP2 (sr_w, vui, vui, vui, vec_sr (a, b))
OP2 (sra_w, vsi, vsi, vui, vec_sra (a, b))
OP2 (rl_w, vui, vui, vui, vec_rl (a, b))
OP2 (slo_w, vsi, vsi, vsc, vec_slo (a, b))
OP2 (sro_w, vsi, vsi, vsc, vec_sro (a, b))
OP2 (sld_f, vf, vf, vf, vec_sld (a, b, 15))

OP2 (packs_w, vss, vsi, vsi, vec_packs (a, b))
OP2 (packsu_w, vus, vsi, vsi, vec_packsu (a, b))
OP2 (packsu_uw, vus, vui, vui, vec_packsu (a, b))
OP2 (pack_uw, vus, vui, vui, vec_pack (a, b))
OP2 (packs_h, vsc, vss, vss, vec_packs (a, b))
OP2 (packsu_h, vuc, vss, vss, vec_packsu (a, b))
OP2 (packsu_uh, vuc, vus, vus, vec_packsu (a, b))
OP2 (pack_uh, vuc, vus, vus, vec_pack (a, b))
OP1 (unpackh_b, vss, vsc, vec_unpackh (a))
OP1 (unpackl_b, vss, vsc, vec_unpackl (a))
OP1 (unpackh_h, vsi, vss, vec_unpackh (a))
OP1 (unpackl_h, vsi, vss, vec_unpackl (a))

void
splat_imm (vsi *pr)
{
  register vsi r __asm__ ("v42") = vec_splat_s32 (-7);
  __asm__ ("" : "+wV" (r));
  *pr = r;
}

/* Loads into v42, stores from v40.  */
#define LD(NAME, T, P, EXPR)						\
  void NAME (T *pr, P *p, long i)					\
  {									\
    register T r __asm__ ("v42") = (EXPR);				\
    __asm__ ("" : "+wV" (r));						\
    *pr = r;								\
  }
#define ST(NAME, T, P, STMT)						\
  void NAME (const T *pa, P *p, long i)					\
  {									\
    register T a __asm__ ("v40") = *pa;					\
    __asm__ ("" : "+wV" (a));						\
    STMT;								\
  }

LD (ld_f, vf, float, vec_ld (i, p))
LD (ldl_f, vf, float, vec_ldl (i, p))
LD (lde_i, vsi, int, vec_lde (i, p))
LD (lvlx_c, vuc, unsigned char, vec_lvlx (i, p))
LD (lvlxl_c, vuc, unsigned char, vec_lvlxl (i, p))
LD (lvrx_c, vuc, unsigned char, vec_lvrx (i, p))
LD (lvrxl_c, vuc, unsigned char, vec_lvrxl (i, p))
ST (st_f, vf, float, vec_st (a, i, p))
ST (stl_f, vf, float, vec_stl (a, i, p))
ST (ste_i, vsi, int, vec_ste (a, i, p))
ST (stvlx_c, vuc, unsigned char, vec_stvlx (a, i, p))
ST (stvlxl_c, vuc, unsigned char, vec_stvlxl (a, i, p))
ST (stvrx_c, vuc, unsigned char, vec_stvrx (a, i, p))
ST (stvrxl_c, vuc, unsigned char, vec_stvrxl (a, i, p))
