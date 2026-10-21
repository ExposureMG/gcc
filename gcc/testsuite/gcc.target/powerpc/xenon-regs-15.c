/* Xenon VMX128 registers (s345-fix finding 3): v32-v63 are volatile and
   no prologue saves them, so -fcall-saved-vN for one of them would let a
   callee clobber a value its caller keeps there.  It is rejected.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -fcall-saved-v40 -fcall-saved-v63" } */
/* { dg-error "-fcall-saved-v40.* is not supported" "" { target *-*-* } 0 } */
/* { dg-error "-fcall-saved-v63.* is not supported" "" { target *-*-* } 0 } */

typedef __attribute__ ((vector_size (16))) float v4sf;
void g (void);

void
f40 (v4sf *o, v4sf *a)
{
  register v4sf x __asm__ ("v40") = a[0];
  __asm__ ("" : "+wV" (x));
  g ();
  __asm__ ("" : "+wV" (x));
  o[0] = x;
}
