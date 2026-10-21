/* _ARCH_XENON and __VMX128__ follow "#pragma GCC target ("cpu=xenon")" in
   a unit compiled for another cpu: defined inside, undefined after
   pop_options.  -mcpu=power7 changes ISA flags; see xenon-macros-3.c for
   cell, where the ISA flags are the same as xenon's.  */
/* { dg-do preprocess { target powerpc*-*-* } } */
/* { dg-options "-mdejagnu-cpu=power7" } */

#ifdef _ARCH_XENON
#error _ARCH_XENON for -mcpu=power7
#endif

#pragma GCC push_options
#pragma GCC target ("cpu=xenon")
#ifndef _ARCH_XENON
#error _ARCH_XENON missing inside cpu=xenon
#endif
#ifndef __VMX128__
#error __VMX128__ missing inside cpu=xenon
#endif
#pragma GCC pop_options

#ifdef _ARCH_XENON
#error _ARCH_XENON leaks after pop_options
#endif
#ifdef __VMX128__
#error __VMX128__ leaks after pop_options
#endif

int x;
