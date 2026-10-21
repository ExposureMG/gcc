/* _ARCH_XENON and __VMX128__ follow the cpu of "#pragma GCC target", also
   when the ISA flags do not change (cell <-> xenon) and when the cpu
   changes back with pop_options.  Unit compiled for -mcpu=xenon.  */
/* { dg-do preprocess { target powerpc*-*-* } } */
/* { dg-options "-mdejagnu-cpu=xenon" } */

#if !defined (_ARCH_XENON) || !defined (__VMX128__)
#error xenon macros missing at the start
#endif

#pragma GCC push_options
#pragma GCC target ("cpu=power7")
#ifdef _ARCH_XENON
#error _ARCH_XENON inside cpu=power7
#endif
#ifdef __VMX128__
#error __VMX128__ inside cpu=power7
#endif
#pragma GCC pop_options

#if !defined (_ARCH_XENON) || !defined (__VMX128__)
#error xenon macros lost after pop_options
#endif

#pragma GCC push_options
#pragma GCC target ("cpu=cell")
#ifdef _ARCH_XENON
#error _ARCH_XENON inside cpu=cell
#endif
#ifndef __PPU__
#error __PPU__ missing inside cpu=cell
#endif
#pragma GCC pop_options

#ifdef __PPU__
#error __PPU__ leaks after pop_options
#endif
#if !defined (_ARCH_XENON) || !defined (__VMX128__)
#error xenon macros lost after the second pop_options
#endif

#pragma GCC push_options
#pragma GCC target ("no-altivec")
#ifdef __VMX128__
#error __VMX128__ without AltiVec
#endif
#ifndef _ARCH_XENON
#error _ARCH_XENON lost with no-altivec
#endif
#pragma GCC pop_options

#ifndef __VMX128__
#error __VMX128__ lost after no-altivec pop
#endif

int x;
