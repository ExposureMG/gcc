/* Xenon VMX128 (plan 4.1): the VMX128 registers follow the target
   attribute and pragma: no-vmx128 and cpu=cell functions in a -mcpu=xenon
   unit use only v0-v31, the default ones v0-v127 (the per-function check
   is in tools/run_gcc_xenon_regs_tests.sh).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mlvx128\M} } } */

typedef __attribute__ ((vector_size (16))) int v4si;

void
with_vmx128 (v4si *restrict o, const v4si *restrict a)
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
  __asm__ __volatile__ ("" ::: "memory");
  o[39] = x0;
  o[38] = x1;
  o[37] = x2;
  o[36] = x3;
  o[35] = x4;
  o[34] = x5;
  o[33] = x6;
  o[32] = x7;
  o[31] = x8;
  o[30] = x9;
  o[29] = x10;
  o[28] = x11;
  o[27] = x12;
  o[26] = x13;
  o[25] = x14;
  o[24] = x15;
  o[23] = x16;
  o[22] = x17;
  o[21] = x18;
  o[20] = x19;
  o[19] = x20;
  o[18] = x21;
  o[17] = x22;
  o[16] = x23;
  o[15] = x24;
  o[14] = x25;
  o[13] = x26;
  o[12] = x27;
  o[11] = x28;
  o[10] = x29;
  o[9] = x30;
  o[8] = x31;
  o[7] = x32;
  o[6] = x33;
  o[5] = x34;
  o[4] = x35;
  o[3] = x36;
  o[2] = x37;
  o[1] = x38;
  o[0] = x39;
}

__attribute__ ((target ("no-vmx128"))) void
no_vmx128 (v4si *restrict o, const v4si *restrict a)
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
  __asm__ __volatile__ ("" ::: "memory");
  o[39] = x0;
  o[38] = x1;
  o[37] = x2;
  o[36] = x3;
  o[35] = x4;
  o[34] = x5;
  o[33] = x6;
  o[32] = x7;
  o[31] = x8;
  o[30] = x9;
  o[29] = x10;
  o[28] = x11;
  o[27] = x12;
  o[26] = x13;
  o[25] = x14;
  o[24] = x15;
  o[23] = x16;
  o[22] = x17;
  o[21] = x18;
  o[20] = x19;
  o[19] = x20;
  o[18] = x21;
  o[17] = x22;
  o[16] = x23;
  o[15] = x24;
  o[14] = x25;
  o[13] = x26;
  o[12] = x27;
  o[11] = x28;
  o[10] = x29;
  o[9] = x30;
  o[8] = x31;
  o[7] = x32;
  o[6] = x33;
  o[5] = x34;
  o[4] = x35;
  o[3] = x36;
  o[2] = x37;
  o[1] = x38;
  o[0] = x39;
}

#pragma GCC push_options
#pragma GCC target ("cpu=cell")
#ifdef __VMX128__
#error __VMX128__ under cpu=cell
#endif
void
cell (v4si *restrict o, const v4si *restrict a)
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
  __asm__ __volatile__ ("" ::: "memory");
  o[39] = x0;
  o[38] = x1;
  o[37] = x2;
  o[36] = x3;
  o[35] = x4;
  o[34] = x5;
  o[33] = x6;
  o[32] = x7;
  o[31] = x8;
  o[30] = x9;
  o[29] = x10;
  o[28] = x11;
  o[27] = x12;
  o[26] = x13;
  o[25] = x14;
  o[24] = x15;
  o[23] = x16;
  o[22] = x17;
  o[21] = x18;
  o[20] = x19;
  o[19] = x20;
  o[18] = x21;
  o[17] = x22;
  o[16] = x23;
  o[15] = x24;
  o[14] = x25;
  o[13] = x26;
  o[12] = x27;
  o[11] = x28;
  o[10] = x29;
  o[9] = x30;
  o[8] = x31;
  o[7] = x32;
  o[6] = x33;
  o[5] = x34;
  o[4] = x35;
  o[3] = x36;
  o[2] = x37;
  o[1] = x38;
  o[0] = x39;
}
#pragma GCC pop_options

#ifndef __VMX128__
#error __VMX128__ not restored
#endif
