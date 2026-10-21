/* Xenon implements lwarx/ldarx only with EH = 0: the "local" exclusive
   access hint of __builtin_ppc_atomic_cas_local is dropped.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mlwarx\M} } } */
/* { dg-final { scan-assembler {\mldarx\M} } } */
/* { dg-final { scan-assembler-not {\ml[wd]arx [0-9]+,[0-9]+,[0-9]+,1} } } */

int
cas_si (int *ptr, int *expected, int *desired)
{
  return __builtin_ppc_atomic_cas_local (ptr, expected, desired, 0,
					 __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

long
cas_di (long *ptr, long *expected, long *desired)
{
  return __builtin_ppc_atomic_cas_local (ptr, expected, desired, 1,
					 __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
}
