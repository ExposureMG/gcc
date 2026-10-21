/* Xenon (s345-fix finding 8), with -mno-vmx128 (the fix is keyed on
   -mcpu=xenon, not on VMX128): the Cell store builtins write only part of
   the 16 bytes at EA (FINDINGS 1.4): vec_stvlx the bytes from EA to the
   end of EA's quadword, vec_stvrx those from the start of EA's quadword
   up to EA - 1.  With -mcpu=xenon they are BLKmode stores of unknown size,
   so DSE no longer deletes earlier stores to bytes they leave alone (the
   patterns claimed a 16-byte store at each EA).  The unaligned store of
   16 bytes at buf + 1 (bytes 4-19) never touches buf[5] (bytes 20-23).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -mno-vmx128" } */
/* test: the store of 1.0f to buf[5] survives.  */
/* { dg-final { scan-assembler {\mst(w|fs) [0-9]+,20\(3\)} } } */
/* t4: p[4] (bytes 16-19) survives vec_stvrx (v, 16, p), which stores
   nothing when p is 16-byte aligned.  */
/* { dg-final { scan-assembler {\mst(w|fs) [0-9]+,16\(3\)} } } */
/* t2: p[2] survives vec_stvlx (v, 0, p).  */
/* { dg-final { scan-assembler {\mst(w|fs) [0-9]+,8\(3\)} } } */
/* tl: the LRU-hint forms; c[16] survives.  */
/* { dg-final { scan-assembler {\mstb [0-9]+,16\(3\)} } } */
/* { dg-final { scan-assembler-times {\mstvlx\M} 2 } } */
/* { dg-final { scan-assembler-times {\mstvrx\M} 2 } } */
/* { dg-final { scan-assembler-times {\mstvlxl\M} 1 } } */
/* { dg-final { scan-assembler-times {\mstvrxl\M} 1 } } */

#include <altivec.h>

static inline void
store_unaligned (float *p, vector float v)
{
  vec_stvlx (v, 0, p);
  vec_stvrx (v, 16, p);
}

void
test (float *buf, vector float v)
{
  buf[5] = 1.0f;
  store_unaligned (buf + 1, v);
}

void
t4 (float *p, vector float v)
{
  p[4] = 7.0f;
  vec_stvrx (v, 16, p);
}

void
t2 (float *p, vector float v)
{
  p[2] = 7.0f;
  vec_stvlx (v, 0, p);
}

void
tl (unsigned char *c, vector unsigned char v)
{
  c[16] = 9;
  vec_stvlxl (v, 0, c);
  vec_stvrxl (v, 16, c);
}
