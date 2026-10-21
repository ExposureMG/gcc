/* A target attribute for another cpu in a -mcpu=xenon unit gets that cpu's
   ISA (xenon-flags-4.c: asking for an extension Xenon lacks in a -mcpu=xenon
   function is an error; that test is separate because a compile that fails
   writes no assembly to scan).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mpopcntd\M} } } */

__attribute__ ((target ("cpu=power7")))
int f2 (unsigned long x) { return __builtin_popcountl (x); }
