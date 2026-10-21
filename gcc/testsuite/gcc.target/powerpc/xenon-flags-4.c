/* A target attribute asking for an ISA extension Xenon lacks is an error
   in a -mcpu=xenon function (xenon-flags-2.c: another cpu's function in the
   same kind of unit gets that cpu's ISA).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */

__attribute__ ((target ("popcntd")))
int f1 (unsigned long x) { return __builtin_popcountl (x); }	/* { dg-error "'-mpopcntd' is not supported by '-mcpu=xenon'" } */

__attribute__ ((target ("cpu=power7")))
int f2 (unsigned long x) { return __builtin_popcountl (x); }
