/* Xenon VMX128 (plan 4.2, step 5): fma and multiplication of vector float.
   A V4SF multiply is vmulfp128 (the rounded product, FINDINGS 1.3), not
   the vmaddfp with a -0.0 addend; a*b+c contracts to fma, which is
   vmaddfp (v0-v31), vmaddfp128 (addend tied) or vmaddcfp128 (a
   multiplicand tied); with -ffp-contract=off it stays vmulfp128 +
   vaddfp.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -maltivec -mdejagnu-cpu=xenon -ffp-contract=fast" } */
/* { dg-final { scan-assembler {\mvmulfp128\M} } } */
/* { dg-final { scan-assembler {\mvmadd(c)?fp(128)?\M} } } */
/* { dg-final { scan-assembler-not {\mvspltisw(128)? [0-9]+,-1\M} } } */
/* { dg-final { scan-assembler-not {\mvslw(128)?\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;

v4sf mul (v4sf a, v4sf b) { return a * b; }
v4sf fma3 (v4sf a, v4sf b, v4sf c) { return a * b + c; }

#define N 1024
float x[N] __attribute__ ((aligned (16)));
float y[N] __attribute__ ((aligned (16)));
float z[N] __attribute__ ((aligned (16)));

void saxpy (float a) { for (int i = 0; i < N; i++) y[i] = a * x[i] + y[i]; }
void vmul (void) { for (int i = 0; i < N; i++) z[i] = x[i] * y[i]; }

/* Many live accumulators: v32-v127 and the tied 128 forms.  */
void
acc64 (v4sf *restrict o, const v4sf *restrict a, const v4sf *restrict b, int n)
{
  v4sf s[64];
  for (int k = 0; k < 64; k++)
    s[k] = o[k];
  for (int j = 0; j < n; j++)
    for (int k = 0; k < 64; k++)
      s[k] = a[j * 64 + k] * b[k] + s[k];
  for (int k = 0; k < 64; k++)
    o[k] = s[k];
}
