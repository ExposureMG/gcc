/* Xenon VMX128 ABI (plan 4.3): -fstack-protector-all and v64-v127 saves.
   The guard lives with the locals, apart from the register save areas;
   tools/check_xenon_abi_frames.py checks that no store goes into a save
   slot.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -fstack-protector-all" } */
/* { dg-final { scan-assembler-times {\mstvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (12[2-7]),1,0\M} 6 } } */
/* { dg-final { scan-assembler {\m__stack_chk_(guard|fail)\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (char *);

void
keep6_ssp (v4sf *restrict o, const v4sf *restrict a)
{
  char buf[64];
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5];
  g (buf);
  o[0] = x5; o[1] = x4; o[2] = x3; o[3] = x2; o[4] = x1; o[5] = x0;
}
