/* Xenon VMX128 (plan 4.1): the wV (v0-v127) and wU (v0-v7) asm operand
   constraints, and register asm variables in v32-v127.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mvperm128 [0-9]+,[0-9]+,[0-9]+,[0-7]$} } } */
/* { dg-final { scan-assembler {\mvor128 2,120,77$} } } */
/* { dg-final { scan-assembler {\mlvx128 120,} } } */
/* { dg-final { scan-assembler {\mlvx128 77,} } } */
/* { dg-final { scan-assembler {\mvmaddfp128 100,101,102,100$} } } */
/* { dg-final { scan-assembler {\mvaddfp128 [0-9]+,[0-9]+,[0-9]+$} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
typedef __attribute__ ((vector_size (16))) int v4si;
typedef __attribute__ ((vector_size (16))) unsigned char v16qi;

#ifndef __VMX128__
#error __VMX128__ not defined
#endif

v16qi
perm (v16qi x, v16qi y, v16qi c)
{
  v16qi r;
  __asm__ ("vperm128 %0,%1,%2,%3" : "=wV" (r) : "wV" (x), "wV" (y), "wU" (c));
  return r;
}

void
hi_regs (v4si *o, const v4si *a)
{
  register v4si x __asm__ ("v120") = a[0];
  register v4si y __asm__ ("v77") = a[1];
  register v4si z __asm__ ("v2");
  __asm__ ("vor128 %0,%1,%2" : "=wV" (z) : "wV" (x), "wV" (y));
  o[0] = z;
}

/* A tied operand on a fixed high register (vmaddfp128 reads vD).  */
v4sf
madd (v4sf a, v4sf b, v4sf c)
{
  register v4sf d __asm__ ("v100") = c;
  register v4sf x __asm__ ("v101") = a;
  register v4sf y __asm__ ("v102") = b;
  __asm__ ("vmaddfp128 %0,%1,%2,%0" : "+wV" (d) : "wV" (x), "wV" (y));
  return d;
}

v4sf
add (v4sf a, v4sf b)
{
  v4sf r;
  __asm__ ("vaddfp128 %0,%1,%2" : "=wV" (r) : "wV" (a), "wV" (b));
  return r;
}
