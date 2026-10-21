/* Xenon (-mcpu=xenon): the AltiVec integer multiply, multiply-sum and
   sum-across instructions do not exist, so integer multiply, widening
   multiply, dot-product and reduction loops must be vectorized with the
   replacement sequences (or not at all) and never use one of them.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -ftree-vectorize -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mvmul[eo][su][bh]\M} } } */
/* { dg-final { scan-assembler-not {\mvmsum[a-z]+\M} } } */
/* { dg-final { scan-assembler-not {\mvmhr?addshs\M} } } */
/* { dg-final { scan-assembler-not {\mvmladduhm\M} } } */
/* { dg-final { scan-assembler-not {\mvsum(4ubs|4sbs|4shs|2sws|sws)\M} } } */
/* { dg-final { scan-assembler-not {\mvmuluwm\M} } } */
/* The replacement sequences: exact single-precision products (vmulfp128
   and the VMX128 forms of the conversions with -mvmx128, plan step 5).  */
/* { dg-final { scan-assembler {\m(vcfux|vcuxwfp128)\M} } } */
/* { dg-final { scan-assembler {\m(vcfsx|vcsxwfp128)\M} } } */
/* { dg-final { scan-assembler {\m(vctuxs|vcfpuxws128)\M} } } */
/* { dg-final { scan-assembler {\m(vctsxs|vcfpsxws128)\M} } } */
/* { dg-final { scan-assembler {\mvmulfp128\M} } } */

#define N 1024

#define ARR(T, NAME) T NAME[N] __attribute__ ((aligned (16)))

ARR (int, si1); ARR (int, si2); ARR (int, si3);
ARR (unsigned int, ui1); ARR (unsigned int, ui2); ARR (unsigned int, ui3);
ARR (short, ss1); ARR (short, ss2); ARR (short, ss3);
ARR (unsigned short, us1); ARR (unsigned short, us2); ARR (unsigned short, us3);
ARR (signed char, sc1); ARR (signed char, sc2); ARR (signed char, sc3);
ARR (unsigned char, uc1); ARR (unsigned char, uc2); ARR (unsigned char, uc3);

/* Same-width multiply (mulv4si3, mulv8hi3, mulv16qi3).  */
void mul_si (void) { for (int i = 0; i < N; i++) si3[i] = si1[i] * si2[i]; }
void mul_ui (void) { for (int i = 0; i < N; i++) ui3[i] = ui1[i] * ui2[i]; }
void mul_ss (void) { for (int i = 0; i < N; i++) ss3[i] = ss1[i] * ss2[i]; }
void mul_us (void) { for (int i = 0; i < N; i++) us3[i] = us1[i] * us2[i]; }
void mul_sc (void) { for (int i = 0; i < N; i++) sc3[i] = sc1[i] * sc2[i]; }
void mul_uc (void) { for (int i = 0; i < N; i++) uc3[i] = uc1[i] * uc2[i]; }

/* Multiply by constants.  */
void mulc_si (void) { for (int i = 0; i < N; i++) si3[i] = si1[i] * 12345; }
void mulc_ss (void) { for (int i = 0; i < N; i++) ss3[i] = ss1[i] * -77; }
void mulc_uc (void) { for (int i = 0; i < N; i++) uc3[i] = uc1[i] * 13; }

/* Widening multiply (vec_widen_[su]mult_{hi,lo}).  */
void wmul_ss (void) { for (int i = 0; i < N; i++) si3[i] = ss1[i] * ss2[i]; }
void wmul_us (void)
{
  for (int i = 0; i < N; i++)
    ui3[i] = (unsigned int) us1[i] * us2[i];
}
void wmul_sc (void) { for (int i = 0; i < N; i++) ss3[i] = sc1[i] * sc2[i]; }
void wmul_uc (void) { for (int i = 0; i < N; i++) us3[i] = uc1[i] * uc2[i]; }
void wmul_sc_si (void) { for (int i = 0; i < N; i++) si3[i] = sc1[i] * sc2[i]; }

/* Multiply-add.  */
void madd_si (void)
{
  for (int i = 0; i < N; i++)
    si3[i] += si1[i] * si2[i];
}
void madd_ss (void)
{
  for (int i = 0; i < N; i++)
    ss3[i] += ss1[i] * ss2[i];
}
