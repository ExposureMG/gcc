/* Xenon executes ldbrx and stdbrx (TARGET_LDBRX).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mldbrx\M} } } */
/* { dg-final { scan-assembler {\mstdbrx\M} } } */
/* { dg-final { scan-assembler {\mlwbrx\M} } } */
/* { dg-final { scan-assembler {\mstwbrx\M} } } */
/* { dg-final { scan-assembler {\mlhbrx\M} } } */

unsigned long ld64 (unsigned long *p) { return __builtin_bswap64 (*p); }
void st64 (unsigned long *p, unsigned long v) { *p = __builtin_bswap64 (v); }
unsigned int ld32 (unsigned int *p) { return __builtin_bswap32 (*p); }
void st32 (unsigned int *p, unsigned int v) { *p = __builtin_bswap32 (v); }
unsigned short ld16 (unsigned short *p) { return __builtin_bswap16 (*p); }
