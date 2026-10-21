/* Xenon VMX128 (plan 4.2; s345-fix finding 9): the estimate instructions
   are kept in their legacy forms.  Whether vrefp128, vrsqrtefp128,
   vexptefp128 and vlogefp128 return the same estimates as vrefp,
   vrsqrtefp, vexptefp and vlogefp is not established by the hardware
   sources (the estimate tables are unknown, FINDINGS 1.2), so GCC never
   emits them: with both forms allowed, the estimate computed for one
   source operation would depend on register allocation.  An operand in
   v40 is copied into v0-v31 first.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mv(refp|rsqrtefp|exptefp|logefp)128\M} } } */
/* { dg-final { scan-assembler-times {\mvrefp ([0-9]|[12][0-9]|3[01]),([0-9]|[12][0-9]|3[01])$} 1 } } */
/* { dg-final { scan-assembler-times {\mvrsqrtefp ([0-9]|[12][0-9]|3[01]),([0-9]|[12][0-9]|3[01])$} 1 } } */
/* { dg-final { scan-assembler-times {\mvexptefp ([0-9]|[12][0-9]|3[01]),([0-9]|[12][0-9]|3[01])$} 1 } } */
/* { dg-final { scan-assembler-times {\mvlogefp ([0-9]|[12][0-9]|3[01]),([0-9]|[12][0-9]|3[01])$} 1 } } */
/* The v40 input reaches the legacy instruction through a copy.  */
/* { dg-final { scan-assembler-times {\mvor128 ([0-9]|[12][0-9]|3[01]),40,40$} 4 } } */

#include <altivec.h>

typedef __vector float vf;

#define OP1(NAME, EXPR)							\
  void NAME (vf *pr, const vf *pa)					\
  {									\
    register vf a __asm__ ("v40") = *pa;				\
    register vf r __asm__ ("v42");					\
    __asm__ ("" : "+wV" (a));						\
    r = (EXPR);								\
    __asm__ ("" : "+wV" (r));						\
    *pr = r;								\
  }

OP1 (re_f, vec_re (a))
OP1 (rsqrte_f, vec_rsqrte (a))
OP1 (expte_f, vec_expte (a))
OP1 (loge_f, vec_loge (a))
