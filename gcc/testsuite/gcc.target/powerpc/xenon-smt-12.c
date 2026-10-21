/* A function tuned for Xenon by a target attribute in a unit compiled for
   another CPU uses the command-line -mxenon-smt= (the option is global):
   its insns get the busy-sibling reservations, the Cell function's do
   not, and the warning says that the option only affects code tuned for
   Xenon.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=cell -maltivec -mxenon-smt=busy -fsched-verbose=2 -fdump-rtl-sched2" } */
/* { dg-warning "'-mxenon-smt=' only affects code tuned for Xenon" "" { target *-*-* } 0 } */
/* { dg-final { scan-rtl-dump {:\(xenon_smt1_issue\+xe_fxu\)} "sched2" } } */
/* { dg-final { scan-rtl-dump {:slot01,fxu_cell} "sched2" } } */

__attribute__ ((target ("cpu=xenon")))
long xe (long x, long a, long b) { x += a; x ^= b; return x; }

long ce (long x, long a, long b) { x += a; x ^= b; return x; }
