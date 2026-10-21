/* Xenon VMX128 registers (s345-fix findings 2/3): -fcall-used-vN for one
   of the callee-saved v64-v127 makes it volatile: it is never saved, and
   no value is kept in it across a call.  -fcall-saved-v64 (already
   callee-saved) is accepted.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -fcall-used-v127 -fcall-used-v126 -fcall-saved-v64" } */
/* { dg-final { scan-assembler-not {\mstvx128 12[67],} } } */
/* { dg-final { scan-assembler-not {\mlvx128 12[67],} } } */
/* { dg-final { scan-assembler-times {\mstvx128 (11[0-9]|12[0-5]),1,0\M} 8 } } */
/* { dg-final { scan-assembler-not {\m_(save|rest)vr128_} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
void g (void);

#define L4(b) v4sf x##b##0 = a[b*4+0], x##b##1 = a[b*4+1], \
  x##b##2 = a[b*4+2], x##b##3 = a[b*4+3];
#define S4(b) o[b*4+0] = x##b##3; o[b*4+1] = x##b##2; \
  o[b*4+2] = x##b##1; o[b*4+3] = x##b##0;

void
k8 (v4sf *restrict o, const v4sf *restrict a)
{
  L4 (0) L4 (1)
  g ();
  S4 (0) S4 (1)
}
