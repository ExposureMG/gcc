/* Xenon VMX128-only operations (step 7) are reached only through the
   __builtin_xenon_* functions: the vectorizer and the generic permute and
   fma expansion never emit them (vmsum3fp128/vmsum4fp128 are not IEEE dot
   products).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -ffast-math -maltivec -mdejagnu-cpu=xenon" } */

/* { dg-final { scan-assembler-not {\mvmsum[34]fp128\M} } } */
/* { dg-final { scan-assembler-not {\mvrlimi128\M} } } */
/* { dg-final { scan-assembler-not {\mvpermwi128\M} } } */
/* { dg-final { scan-assembler-not {\mv(up|pk)d3d128\M} } } */

typedef float v4sf __attribute__ ((vector_size (16)));
typedef int v4si __attribute__ ((vector_size (16)));

float
dot (const float *restrict a, const float *restrict b, int n)
{
  float s = 0.0f;
  for (int i = 0; i < n; i++)
    s += a[i] * b[i];
  return s;
}

void
dot4 (float *restrict o, const float *restrict a, const float *restrict b, int n)
{
  for (int i = 0; i < n; i++)
    o[i] = a[4 * i] * b[4 * i] + a[4 * i + 1] * b[4 * i + 1]
	   + a[4 * i + 2] * b[4 * i + 2] + a[4 * i + 3] * b[4 * i + 3];
}

void
dot3 (float *restrict o, const float *restrict a, const float *restrict b, int n)
{
  for (int i = 0; i < n; i++)
    o[i] = a[4 * i] * b[4 * i] + a[4 * i + 1] * b[4 * i + 1]
	   + a[4 * i + 2] * b[4 * i + 2];
}

v4sf
swizzle (v4sf a)
{
  return __builtin_shuffle (a, (v4si) { 3, 2, 1, 0 });
}

v4sf
blend (v4sf a, v4sf b)
{
  return __builtin_shuffle (a, b, (v4si) { 0, 5, 2, 7 });
}

v4sf
rot (v4sf a, v4sf b)
{
  return __builtin_shuffle (a, b, (v4si) { 5, 6, 7, 0 });
}
