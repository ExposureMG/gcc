/* The integer vector multiply sequence is an ISA property of -mcpu=xenon,
   so it is costed as its 33 instructions with any tuning (here power7's
   generic costs, one unit per instruction).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -mdejagnu-tune=power7 -fdump-tree-vect-details" } */
/* { dg-final { scan-tree-dump {\* _2 1 times vector_stmt costs 33 in body} "vect" } } */
/* { dg-final { scan-tree-dump {\* _2 1 times scalar_stmt costs 1 in prologue} "vect" } } */

#define N 1024
int a[N], b[N], c[N];

void m4 (void) { for (int i = 0; i < N; i++) a[i] = b[i] * c[i]; }
