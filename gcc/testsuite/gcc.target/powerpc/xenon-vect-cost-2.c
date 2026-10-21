/* Xenon vectorizer costs (plan 5.5): integer vector multiplies cost their
   instruction count, not 1.  The AltiVec multiply family is illegal on
   Xenon and altivec.md multiplies in single precision: V4SI 33, V8HI 25,
   V16QI 31 in-loop instructions, a widening V8HI/V16QI half 12/15
   (VMUL_*_INSNS / VWMUL_*_INSNS, checked against the compiled code by
   tools/xenon_vect_cost_check.py); a multiply by a constant costs its
   shift/add synthesis (x * 10: 3, x * 12345: 8).  The scalar side costs
   mullw's 11 cycles (unpipelined), mulli's 7 or the synthesis (x * 10: 3).
   Per element the vector forms are still cheaper (V4SI 8.3, V8HI 3.1,
   V16QI 1.9 against 11), so every loop here is vectorized; the sched2 model
   agrees (xenon_vect_cost_check.py "decide").  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump-times {optimized: loop vectorized} 9 "vect" } } */
/* { dg-final { scan-tree-dump {\* _2 1 times vector_stmt costs 33 in body} "vect" } } */
/* { dg-final { scan-tree-dump {\* _4 1 times vector_stmt costs 25 in body} "vect" } } */
/* { dg-final { scan-tree-dump {\* _4 1 times vector_stmt costs 31 in body} "vect" } } */
/* { dg-final { scan-tree-dump {w\* _3 2 times vector_stmt costs 24 in body} "vect" } } */
/* { dg-final { scan-tree-dump {w\* _3 2 times vector_stmt costs 30 in body} "vect" } } */
/* { dg-final { scan-tree-dump {\* 12345 1 times vector_stmt costs 8 in body} "vect" } } */
/* { dg-final { scan-tree-dump {\* 10 1 times vector_stmt costs 3 in body} "vect" } } */
/* { dg-final { scan-tree-dump {\* 12345 1 times scalar_stmt costs 7 in prologue} "vect" } } */
/* { dg-final { scan-tree-dump {\* 10 1 times scalar_stmt costs 3 in prologue} "vect" } } */
/* { dg-final { scan-tree-dump {\* _2 1 times scalar_stmt costs 11 in prologue} "vect" } } */
/* { dg-final { scan-assembler-not {\mvmul[eo][su][bh]\M|\mvmsum[a-z]+\M|\mvmladduhm\M|\mvmuluwm\M} } } */

#define N 1024
int a[N], b[N], c[N];
short sa[N], sb[N], sc[N];
signed char qa[N], qb[N], qc[N];

void m4 (void) { for (int i = 0; i < N; i++) a[i] = b[i] * c[i]; }
void m8 (void) { for (int i = 0; i < N; i++) sa[i] = sb[i] * sc[i]; }
void m16 (void) { for (int i = 0; i < N; i++) qa[i] = qb[i] * qc[i]; }
void w8 (void) { for (int i = 0; i < N; i++) a[i] = sb[i] * sc[i]; }
void w16 (void) { for (int i = 0; i < N; i++) sa[i] = qb[i] * qc[i]; }
void k4 (void) { for (int i = 0; i < N; i++) a[i] = b[i] * 12345; }
void k10 (void) { for (int i = 0; i < N; i++) a[i] = b[i] * 10; }
int dot (void) { int s = 0; for (int i = 0; i < N; i++) s += b[i] * c[i]; return s; }
int sdot (void) { int s = 0; for (int i = 0; i < N; i++) s += sb[i] * sc[i]; return s; }
