/* Xenon VMX128 registers (s345-fix finding 2): -ffixed-vN for one of
   v32-v127 is honoured with -mvmx128.  FIXED_REGISTERS used to mark them
   fixed and rs6000_conditional_register_usage freed all of them again,
   undoing the user's option; now only that hook fixes them (without
   VMX128), so the option survives, also across target attribute and
   pragma switches.  No vector instruction may name v32, v40, v100 or v127
   (immediates never exceed 31, GPR operands neither).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -fno-ipa-icf -ffixed-v32 -ffixed-v40 -ffixed-v100 -ffixed-v127 -ffixed-v20" } */
/* { dg-final { scan-assembler-not {^\t(?:v[a-z0-9.]*|[ls]v[a-z]*128) (?:[0-9]+,)*(?:32|40|100|127|20)(?:,|$)} } } */
/* The other registers are used: v33 (the first free volatile), v126
   (the first free callee-saved one).  */
/* { dg-final { scan-assembler {^\t(?:v[a-z0-9.]*|lvx128) 33,} } } */
/* { dg-final { scan-assembler {\mstvx128 126,1,0\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
void g (void);

#define L4(b) v4sf x##b##0 = a[b*4+0], x##b##1 = a[b*4+1], \
  x##b##2 = a[b*4+2], x##b##3 = a[b*4+3];
#define S4(b) o[b*4+0] = x##b##3; o[b*4+1] = x##b##2; \
  o[b*4+2] = x##b##1; o[b*4+3] = x##b##0;
#define A4(b) x##b##0 += x##b##1; x##b##1 += x##b##2; \
  x##b##2 += x##b##3; x##b##3 += x##b##0;
#define L40 L4(0) L4(1) L4(2) L4(3) L4(4) L4(5) L4(6) L4(7) L4(8) L4(9)
#define S40 S4(0) S4(1) S4(2) S4(3) S4(4) S4(5) S4(6) S4(7) S4(8) S4(9)
#define A40 A4(0) A4(1) A4(2) A4(3) A4(4) A4(5) A4(6) A4(7) A4(8) A4(9)
#define BODY { L40 for (int i = 0; i < n; i++) { A40 } S40 }
#define BODYC { L4(0) L4(1) g (); S4(0) S4(1) }

void f1 (v4sf *restrict o, const v4sf *restrict a, int n) BODY
void f2 (v4sf *restrict o, const v4sf *restrict a, int n) BODYC
__attribute__ ((target ("cpu=cell")))
void f3 (v4sf *restrict o, const v4sf *restrict a, int n) BODY
void f4 (v4sf *restrict o, const v4sf *restrict a, int n) BODY
#pragma GCC push_options
#pragma GCC target ("no-vmx128")
void f5 (v4sf *restrict o, const v4sf *restrict a, int n) BODYC
#pragma GCC pop_options
void f6 (v4sf *restrict o, const v4sf *restrict a, int n) BODY
void f7 (v4sf *restrict o, const v4sf *restrict a, int n) BODYC
