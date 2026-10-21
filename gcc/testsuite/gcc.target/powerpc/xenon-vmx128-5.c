/* Xenon VMX128 (plan 4.2, step 5): with -mno-vmx128 the compiler emits no
   VMX128 instruction and no v32-v127, the multiply is the vmaddfp
   sequence and misaligned accesses use no lvlx/lvrx (the old Xenon
   output).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -maltivec -mdejagnu-cpu=xenon -mno-vmx128" } */
/* { dg-final { scan-assembler-not {^\t[a-z]+128\.?\s} } } */
/* { dg-final { scan-assembler {\mvmaddfp\M} } } */
/* { dg-final { scan-assembler-not {\mlv[lr]x\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
v4sf mul (v4sf a, v4sf b) { return a * b; }
v4sf fma3 (v4sf a, v4sf b, v4sf c) { return a * b + c; }
void
acc (v4sf *restrict o, const v4sf *restrict a, int n)
{
  v4sf s[40];
  for (int k = 0; k < 40; k++)
    s[k] = o[k];
  for (int j = 0; j < n; j++)
    for (int k = 0; k < 40; k++)
      s[k] = a[j * 40 + k] * s[k] + a[k];
  for (int k = 0; k < 40; k++)
    o[k] = s[k];
}
void copy (float *restrict d, const float *restrict s, int n)
{
  for (int i = 0; i < n; i++)
    d[i] = s[i] * 2.0f;
}
