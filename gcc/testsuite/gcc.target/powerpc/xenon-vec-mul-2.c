/* Xenon: dot products, widening sums and reductions (sdot_prod, udot_prod,
   widen_[su]sum, reduc_plus_scal) must not use vmsum* or vsum*.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -ftree-vectorize -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-not {\mvmul[eo][su][bh]\M} } } */
/* { dg-final { scan-assembler-not {\mvmsum[a-z]+\M} } } */
/* { dg-final { scan-assembler-not {\mvmhr?addshs\M} } } */
/* { dg-final { scan-assembler-not {\mvmladduhm\M} } } */
/* { dg-final { scan-assembler-not {\mvsum(4ubs|4sbs|4shs|2sws|sws)\M} } } */

#define N 1024

#define ARR(T, NAME) T NAME[N] __attribute__ ((aligned (16)))

ARR (int, si1); ARR (int, si2);
ARR (unsigned int, ui1);
ARR (short, ss1); ARR (short, ss2);
ARR (unsigned short, us1); ARR (unsigned short, us2);
ARR (signed char, sc1); ARR (signed char, sc2);
ARR (unsigned char, uc1); ARR (unsigned char, uc2);

int dot_ss (void)
{
  int s = 0;
  for (int i = 0; i < N; i++)
    s += ss1[i] * ss2[i];
  return s;
}

unsigned int dot_us (void)
{
  unsigned int s = 0;
  for (int i = 0; i < N; i++)
    s += (unsigned int) us1[i] * us2[i];
  return s;
}

int dot_sc (void)
{
  int s = 0;
  for (int i = 0; i < N; i++)
    s += sc1[i] * sc2[i];
  return s;
}

unsigned int dot_uc (void)
{
  unsigned int s = 0;
  for (int i = 0; i < N; i++)
    s += uc1[i] * uc2[i];
  return s;
}

int dot_si (void)
{
  int s = 0;
  for (int i = 0; i < N; i++)
    s += si1[i] * si2[i];
  return s;
}

/* Widening sums (widen_ssum / widen_usum).  */
int wsum_ss (void)
{
  int s = 0;
  for (int i = 0; i < N; i++)
    s += ss1[i];
  return s;
}

unsigned int wsum_uc (void)
{
  unsigned int s = 0;
  for (int i = 0; i < N; i++)
    s += uc1[i];
  return s;
}

int wsum_sc (void)
{
  int s = 0;
  for (int i = 0; i < N; i++)
    s += sc1[i];
  return s;
}

/* Same-width reductions (reduc_plus_scal_<mode>).  */
short sum_ss (void)
{
  short s = 0;
  for (int i = 0; i < N; i++)
    s += ss1[i];
  return s;
}

signed char sum_sc (void)
{
  signed char s = 0;
  for (int i = 0; i < N; i++)
    s += sc1[i];
  return s;
}

int sum_si (void)
{
  int s = 0;
  for (int i = 0; i < N; i++)
    s += si1[i];
  return s;
}

unsigned int sum_ui (void)
{
  unsigned int s = 0;
  for (int i = 0; i < N; i++)
    s += ui1[i];
  return s;
}

/* Sum of absolute differences.  */
int sad_uc (void)
{
  int s = 0;
  for (int i = 0; i < N; i++)
    s += __builtin_abs (uc1[i] - uc2[i]);
  return s;
}

/* Division by constants (multiply high part).  */
void div_ss (void)
{
  for (int i = 0; i < N; i++)
    ss1[i] = ss2[i] / 7;
}

void div_si (void)
{
  for (int i = 0; i < N; i++)
    si1[i] = si2[i] / 7;
}

void div_uc (void)
{
  for (int i = 0; i < N; i++)
    uc1[i] = uc2[i] / 5;
}
