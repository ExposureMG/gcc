/* Xenon VMX128 (plan 4.2, step 5): misaligned vector accesses use
   movmisalign (lvlx + lvrx + vor, stvlx + stvrx), not the lvsl + vperm
   realignment, and the vectorizer uses them for loads and stores of
   unknown alignment.  lvsl128/lvsr128 (illegal) never appear.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -maltivec -mdejagnu-cpu=xenon -fno-vect-cost-model -fno-tree-loop-distribute-patterns" } */
/* { dg-final { scan-assembler {\mlvlx(128)?\M} } } */
/* { dg-final { scan-assembler {\mlvrx(128)?\M} } } */
/* { dg-final { scan-assembler {\mstvlx(128)?\M} } } */
/* { dg-final { scan-assembler {\mstvrx(128)?\M} } } */
/* { dg-final { scan-assembler-not {\mlvs[lr](128)?\M} } } */
/* { dg-final { scan-assembler-not {\mvperm(128)?\M} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
typedef __attribute__ ((vector_size (16), aligned (1))) float v4sf_u;
typedef __attribute__ ((vector_size (16), aligned (1))) int v4si_u;

/* Unknown alignment of both pointers.  */
void
copy (float *restrict d, const float *restrict s, int n)
{
  for (int i = 0; i < n; i++)
    d[i] = s[i];
}

void
scale (float *restrict d, const float *restrict s, float k, int n)
{
  for (int i = 0; i < n; i++)
    d[i] = s[i] * k;
}

/* Known misalignment.  */
float a[1028] __attribute__ ((aligned (16)));
float b[1028] __attribute__ ((aligned (16)));
void shift1 (void) { for (int i = 0; i < 1024; i++) b[i + 3] = a[i + 1] + 1.0f; }

/* Explicitly under-aligned vector types.  */
v4sf ld_u (const v4sf_u *p) { return *p; }
void st_u (v4sf_u *p, v4sf v) { *p = v; }
v4sf ld_u_idx (const float *p, long i) { return *(const v4sf_u *) (p + i); }
void cp_u_i (v4si_u *d, const v4si_u *s) { *d = *s; }
