/* -mcpu=xenon -mtune=cell: the tuning is not Xenon's, so -mxenon-smt=same
   is ignored with a warning and the Cell scheduler runs.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mdejagnu-tune=cell -mxenon-smt=same -fsched-verbose=2 -fdump-rtl-sched2" } */
/* { dg-warning "'-mxenon-smt=' only affects code tuned for Xenon" "" { target *-*-* } 0 } */
/* { dg-final { scan-rtl-dump-not {xenon_} "sched2" } } */

long f (long x, long a, long b) { x += a; x ^= b; return x; }
