/* Xenon VMX128 ABI (plan 4.3): the default speed threshold is 16 saved
   registers.  15 are saved inline (v113-v127); 16 (v112-v127) and all 64
   (v64-v127) call _savevr128_N / _restvr128_N.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mstvx128 (11[3-9]|12[0-7]),1,0\M} 15 } } */
/* { dg-final { scan-assembler-times {\mlvx128 (11[3-9]|12[0-7]),1,0\M} 15 } } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_112\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_112\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_64\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_64\M} 1 } } */
/* { dg-final { scan-assembler-not {\mstvx\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

void g (void);

void
keep15 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5], x6
    = a[6], x7 = a[7], x8 = a[8], x9 = a[9], x10 = a[10], x11 = a[11], x12
    = a[12], x13 = a[13], x14 = a[14];
  g ();
  o[0] = x14; o[1] = x13; o[2] = x12; o[3] = x11; o[4] = x10; o[5] = x9;
    o[6] = x8; o[7] = x7; o[8] = x6; o[9] = x5; o[10] = x4; o[11] = x3;
    o[12] = x2; o[13] = x1; o[14] = x0;
}

void
keep16 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5], x6
    = a[6], x7 = a[7], x8 = a[8], x9 = a[9], x10 = a[10], x11 = a[11], x12
    = a[12], x13 = a[13], x14 = a[14], x15 = a[15];
  g ();
  o[0] = x15; o[1] = x14; o[2] = x13; o[3] = x12; o[4] = x11; o[5] = x10;
    o[6] = x9; o[7] = x8; o[8] = x7; o[9] = x6; o[10] = x5; o[11] = x4;
    o[12] = x3; o[13] = x2; o[14] = x1; o[15] = x0;
}

void
keep64 (v4sf *restrict o, const v4sf *restrict a)
{
  v4sf x0 = a[0], x1 = a[1], x2 = a[2], x3 = a[3], x4 = a[4], x5 = a[5], x6
    = a[6], x7 = a[7], x8 = a[8], x9 = a[9], x10 = a[10], x11 = a[11], x12
    = a[12], x13 = a[13], x14 = a[14], x15 = a[15], x16 = a[16], x17 =
    a[17], x18 = a[18], x19 = a[19], x20 = a[20], x21 = a[21], x22 = a[22],
    x23 = a[23], x24 = a[24], x25 = a[25], x26 = a[26], x27 = a[27], x28 =
    a[28], x29 = a[29], x30 = a[30], x31 = a[31], x32 = a[32], x33 = a[33],
    x34 = a[34], x35 = a[35], x36 = a[36], x37 = a[37], x38 = a[38], x39 =
    a[39], x40 = a[40], x41 = a[41], x42 = a[42], x43 = a[43], x44 = a[44],
    x45 = a[45], x46 = a[46], x47 = a[47], x48 = a[48], x49 = a[49], x50 =
    a[50], x51 = a[51], x52 = a[52], x53 = a[53], x54 = a[54], x55 = a[55],
    x56 = a[56], x57 = a[57], x58 = a[58], x59 = a[59], x60 = a[60], x61 =
    a[61], x62 = a[62], x63 = a[63];
  g ();
  o[0] = x63; o[1] = x62; o[2] = x61; o[3] = x60; o[4] = x59; o[5] = x58;
    o[6] = x57; o[7] = x56; o[8] = x55; o[9] = x54; o[10] = x53; o[11] =
    x52; o[12] = x51; o[13] = x50; o[14] = x49; o[15] = x48; o[16] = x47;
    o[17] = x46; o[18] = x45; o[19] = x44; o[20] = x43; o[21] = x42; o[22]
    = x41; o[23] = x40; o[24] = x39; o[25] = x38; o[26] = x37; o[27] = x36;
    o[28] = x35; o[29] = x34; o[30] = x33; o[31] = x32; o[32] = x31; o[33]
    = x30; o[34] = x29; o[35] = x28; o[36] = x27; o[37] = x26; o[38] = x25;
    o[39] = x24; o[40] = x23; o[41] = x22; o[42] = x21; o[43] = x20; o[44]
    = x19; o[45] = x18; o[46] = x17; o[47] = x16; o[48] = x15; o[49] = x14;
    o[50] = x13; o[51] = x12; o[52] = x11; o[53] = x10; o[54] = x9; o[55] =
    x8; o[56] = x7; o[57] = x6; o[58] = x5; o[59] = x4; o[60] = x3; o[61] =
    x2; o[62] = x1; o[63] = x0;
}
