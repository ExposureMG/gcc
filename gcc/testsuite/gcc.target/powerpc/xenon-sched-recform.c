/* Xenon: record forms of logical/rotate/shift-imm/cntlz/exts ops and
   andi./andis. are microcoded (14-16 cycles, plan 5.2/5.3), so combine must
   keep the operation and a separate compare.  add-type record forms are not
   microcoded (2 cycles) and are still formed.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -fdump-rtl-combine-details" } */
/* { dg-final { scan-assembler-not {\m(and|andc|or|orc|xor|nor|nand|eqv|rlwinm|rlwnm|rlwimi|rldicl|rldicr|rldic|rldimi|rldcl|rldcr|srawi|sradi|srwi|slwi|srdi|sldi|clrlwi|clrldi|clrrwi|clrrdi|extlwi|extrwi|rotlwi|rotldi|extsb|extsh|extsw|cntlzw|cntlzd|slw|srw|sraw|sld|srd|srad)\.} } } */
/* { dg-final { scan-assembler-not {\mandis?\.} } } */
/* { dg-final { scan-assembler {\madd\.} } } */
/* { dg-final { scan-assembler {\msubf\.} } } */
/* { dg-final { scan-rtl-dump {rejecting combination} "combine" } } */

int and_t (int a, int b) { int c = a & b; return c ? c : 5; }
int orc_t (int a, int b) { int c = a | ~b; return c < 0 ? 1 : c; }
int xor_t (int a, int b) { int c = a ^ b; return c > 0 ? c : -3; }
unsigned srw_t (unsigned x) { unsigned y = x >> 3; return y ? y : 100; }
long sld_t (long x) { long y = x << 7; return y ? y : 3; }
int andi_t (int x) { return (x & 0x55) != 0 ? 2 : 3; }
int bit_t (unsigned x) { return (x & 0x8000) ? 2 : 3; }
long andis_t (long x, long *p) { long y = x & 0x50000; *p = y; return y ? 4 : 6; }
long exts_t (int x) { long y = x; return y ? y : -1; }
int clz_t (unsigned x) { int c = __builtin_clz (x | 1); return c ? c : 1; }
extern void g (long);
long add_t (long a, long b) { long c = a + b; if (c == 0) g (a); return c; }
long subf_t (long a, long b) { long c = a - b; if (c > 0) g (c); return c; }
