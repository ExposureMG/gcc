/* Xenon VMX128 ABI (plan 4.3): a frame larger than 32 KiB whose v64-v127
   save area reaches below the 288-byte red zone (20 registers, kept inline
   with --param=xenon-vmx128-ool-threshold=0): the prologue saves through
   the r12 pointer to the top of the vector area; the epilogue loads the
   back chain into r11 and restores through it before popping the frame.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon --param=xenon-vmx128-ool-threshold=0" } */
/* { dg-final { scan-assembler-times {\mstvx128 (10[89]|11[0-9]|12[0-7]),12,0\M} 20 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (10[89]|11[0-9]|12[0-7]),11,0\M} 20 } } */
/* { dg-final { scan-assembler {\mld 11,0\(1\)} } } */
/* { dg-final { scan-assembler-not {\m_savevr128_} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (char *);

void
keep20_bigframe (v4sf *restrict o, const v4sf *restrict a)
{
  char buf[40000];
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5], x6
    = a[6], x7 = a[7], x8 = a[8], x9 = a[9], x10 = a[10], x11 = a[11], x12
    = a[12], x13 = a[13], x14 = a[14], x15 = a[15], x16 = a[16], x17 =
    a[17], x18 = a[18], x19 = a[19];
  g (buf);
  o[0] = x19; o[1] = x18; o[2] = x17; o[3] = x16; o[4] = x15; o[5] = x14;
    o[6] = x13; o[7] = x12; o[8] = x11; o[9] = x10; o[10] = x9; o[11] = x8;
    o[12] = x7; o[13] = x6; o[14] = x5; o[15] = x4; o[16] = x3; o[17] = x2;
    o[18] = x1; o[19] = x0;
}
