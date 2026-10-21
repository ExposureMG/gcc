/* As xenon-macros-2.c, from -mcpu=cell: cell and xenon have the same ISA
   flags, so only the cpu changes.  */
/* { dg-do preprocess { target powerpc*-*-* } } */
/* { dg-options "-maltivec -mdejagnu-cpu=cell" } */

#ifndef __PPU__
#error __PPU__ missing for -mcpu=cell
#endif

#pragma GCC push_options
#pragma GCC target ("cpu=xenon")
#ifndef _ARCH_XENON
#error _ARCH_XENON missing inside cpu=xenon
#endif
#ifndef __VMX128__
#error __VMX128__ missing inside cpu=xenon
#endif
#ifdef __PPU__
#error __PPU__ inside cpu=xenon
#endif
#pragma GCC pop_options

#ifdef _ARCH_XENON
#error _ARCH_XENON leaks after pop_options
#endif
#ifdef __VMX128__
#error __VMX128__ leaks after pop_options
#endif
#ifndef __PPU__
#error __PPU__ lost after pop_options
#endif

int x;
