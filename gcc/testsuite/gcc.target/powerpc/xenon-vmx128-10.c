/* Xenon VMX128 (s345-fix finding 10): which spellings of an under-aligned
   vector type reach the misaligned-access code (lvlx/lvrx + vor,
   stvlx/stvrx; plan 4.2 movmisalign).  aligned (N < 16) is honoured on a
   vector_size typedef and on a typedef of an existing vector typedef (also
   one spelled with __vector), but has no effect on a typedef that uses the
   AltiVec keyword itself, as for every AltiVec CPU (documented under
   -mvmx128 in invoke.texi): that type keeps 16-byte alignment and its
   accesses are plain lvx/stvx, which ignore EA[28:31].  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* a1, a2: left/right loads and stores.  */
/* { dg-final { scan-assembler-times {\mlvlx(128)? } 2 } } */
/* { dg-final { scan-assembler-times {\mlvrx(128)? } 2 } } */
/* { dg-final { scan-assembler-times {\mstvlx(128)? } 2 } } */
/* { dg-final { scan-assembler-times {\mstvrx(128)? } 2 } } */
/* a3: the AltiVec-keyword typedef stays aligned: lvx/stvx.  */
/* { dg-final { scan-assembler-times {\mlvx(128)? } 1 } } */
/* { dg-final { scan-assembler-times {\mstvx(128)? } 1 } } */

typedef float vf4u __attribute__ ((vector_size (16), aligned (4)));
typedef __vector float vfa;
typedef vfa vfau __attribute__ ((aligned (4)));
typedef __vector float vfk __attribute__ ((aligned (4)));

_Static_assert (__alignof__ (vf4u) == 4, "vector_size + aligned (4)");
_Static_assert (__alignof__ (vfau) == 4, "typedef of a vector typedef");
_Static_assert (__alignof__ (vfk) == 16, "__vector keyword typedef");

void a1 (vf4u *d, const vf4u *s) { *d = *s; }
void a2 (vfau *d, const vfau *s) { *d = *s; }
void a3 (vfk *d, const vfk *s) { *d = *s; }
