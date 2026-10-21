/* Xenon VMX128 (plan 4.1/4.2): 50 vector values that are only loaded and
   stored fit in the volatile registers v32-v63 and v0-v19, with no stack
   slot and no callee-saved register (v20-v31, v64-v127).  Values that only
   move go to v32-v63 first (allocation order), leaving v0-v31 to values
   that legacy AltiVec instructions need.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mlvx128\M} 32 } } */
/* { dg-final { scan-assembler-times {\mstvx128\M} 32 } } */
/* { dg-final { scan-assembler-times {\mlvx\M} 18 } } */
/* { dg-final { scan-assembler-times {\mstvx\M} 18 } } */
/* { dg-final { scan-assembler-not {\m(lvx|stvx)(128)? [0-9]+,1,} } } */
/* { dg-final { scan-assembler-not {\m(lvx|stvx)(128)? (2[0-9]|3[01]|6[4-9]|[7-9][0-9]|1[0-2][0-9]),} } } */
/* { dg-final { scan-assembler-not {\mvor(128)?\M} } } */

typedef __attribute__ ((vector_size (16))) int v4si;

void
mov_50 (v4si *restrict o, const v4si *restrict a)
{
  v4si x0 = a[0];
  v4si x1 = a[1];
  v4si x2 = a[2];
  v4si x3 = a[3];
  v4si x4 = a[4];
  v4si x5 = a[5];
  v4si x6 = a[6];
  v4si x7 = a[7];
  v4si x8 = a[8];
  v4si x9 = a[9];
  v4si x10 = a[10];
  v4si x11 = a[11];
  v4si x12 = a[12];
  v4si x13 = a[13];
  v4si x14 = a[14];
  v4si x15 = a[15];
  v4si x16 = a[16];
  v4si x17 = a[17];
  v4si x18 = a[18];
  v4si x19 = a[19];
  v4si x20 = a[20];
  v4si x21 = a[21];
  v4si x22 = a[22];
  v4si x23 = a[23];
  v4si x24 = a[24];
  v4si x25 = a[25];
  v4si x26 = a[26];
  v4si x27 = a[27];
  v4si x28 = a[28];
  v4si x29 = a[29];
  v4si x30 = a[30];
  v4si x31 = a[31];
  v4si x32 = a[32];
  v4si x33 = a[33];
  v4si x34 = a[34];
  v4si x35 = a[35];
  v4si x36 = a[36];
  v4si x37 = a[37];
  v4si x38 = a[38];
  v4si x39 = a[39];
  v4si x40 = a[40];
  v4si x41 = a[41];
  v4si x42 = a[42];
  v4si x43 = a[43];
  v4si x44 = a[44];
  v4si x45 = a[45];
  v4si x46 = a[46];
  v4si x47 = a[47];
  v4si x48 = a[48];
  v4si x49 = a[49];
  __asm__ __volatile__ ("" ::: "memory");
  o[49] = x0;
  o[48] = x1;
  o[47] = x2;
  o[46] = x3;
  o[45] = x4;
  o[44] = x5;
  o[43] = x6;
  o[42] = x7;
  o[41] = x8;
  o[40] = x9;
  o[39] = x10;
  o[38] = x11;
  o[37] = x12;
  o[36] = x13;
  o[35] = x14;
  o[34] = x15;
  o[33] = x16;
  o[32] = x17;
  o[31] = x18;
  o[30] = x19;
  o[29] = x20;
  o[28] = x21;
  o[27] = x22;
  o[26] = x23;
  o[25] = x24;
  o[24] = x25;
  o[23] = x26;
  o[22] = x27;
  o[21] = x28;
  o[20] = x29;
  o[19] = x30;
  o[18] = x31;
  o[17] = x32;
  o[16] = x33;
  o[15] = x34;
  o[14] = x35;
  o[13] = x36;
  o[12] = x37;
  o[11] = x38;
  o[10] = x39;
  o[9] = x40;
  o[8] = x41;
  o[7] = x42;
  o[6] = x43;
  o[5] = x44;
  o[4] = x45;
  o[3] = x46;
  o[2] = x47;
  o[1] = x48;
  o[0] = x49;
}
