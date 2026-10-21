/* Xenon VMX128 registers (s345-fix finding 2): with -ffixed-v127 at -Os,
   v127 is neither allocated nor saved, so the saved set cannot end at v127
   and the out-of-line _savevr128_N/_restvr128_N routines (which save
   vN..v127) are not used: the eight values kept across the call are saved
   inline from v126 down.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Os -maltivec -mdejagnu-cpu=xenon -ffixed-v127" } */
/* { dg-final { scan-assembler-not {^\t(?:v[a-z0-9.]*|[ls]v[a-z]*128) (?:[0-9]+,)*127(?:,|$)} } } */
/* { dg-final { scan-assembler-not {\m_(save|rest)vr128_} } } */
/* { dg-final { scan-assembler-times {\mstvx128 (119|12[0-6]),1,0\M} 8 } } */

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
