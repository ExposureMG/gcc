/* Xenon VMX128 (plan 4.2): constants in v32-v127.  Zero is vxor128,
   -16..15 splats are vspltisw128, the sign-bit vector is vspltisw128 -1 +
   vslw128; other constants are built in v0-v31 (legacy splats) or loaded
   from memory, and copied.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mvxor128 40,40,40$} } } */
/* { dg-final { scan-assembler {\mvspltisw128 100,-1$} } } */
/* { dg-final { scan-assembler {\mvspltisw128 127,15$} } } */
/* { dg-final { scan-assembler {\mvspltisw128 64,-16$} } } */
/* { dg-final { scan-assembler {\mvspltisw128 33,-1\n\tvslw128 33,33,33$} } } */
/* { dg-final { scan-assembler {\mvspltish [0-9]+,7$} } } */
/* { dg-final { scan-assembler {\mvor128 90,([0-9]|[12][0-9]|3[01]),\1$} } } */
/* { dg-final { scan-assembler {\mvspltisw 5,9$} } } */
/* { dg-final { scan-assembler-not {\mvspltis[bhw] (3[2-9]|[4-9][0-9]|1[01][0-9]|12[0-7]),} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
typedef __attribute__ ((vector_size (16))) int v4si;
typedef __attribute__ ((vector_size (16))) short v8hi;
typedef __attribute__ ((vector_size (16))) unsigned char v16qi;

#define HI_CONST(name, reg, type, ...)				\
  {								\
    register type name __asm__ (reg) = (type) { __VA_ARGS__ };	\
    __asm__ ("" : "+wV" (name));				\
    *(type *) o++ = name;					\
  }

void
const_hi (v4si *restrict o)
{
  HI_CONST (c0, "v40", v4si, 0, 0, 0, 0)
  HI_CONST (c1, "v100", v4si, -1, -1, -1, -1)
  HI_CONST (c2, "v127", v4si, 15, 15, 15, 15)
  HI_CONST (c3, "v64", v4si, -16, -16, -16, -16)
  HI_CONST (c4, "v33", v4sf, -0.0f, -0.0f, -0.0f, -0.0f)
  HI_CONST (c5, "v90", v8hi, 7, 7, 7, 7, 7, 7, 7, 7)
  HI_CONST (c6, "v91", v4si, 1, 2, 3, 4)
  HI_CONST (c7, "v5", v4si, 9, 9, 9, 9)
}
