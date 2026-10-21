/* Xenon VMX128-only builtins (step 7): a no-vmx128 function of a
   -mcpu=xenon unit cannot use them (front-end error for the overloaded
   name).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

typedef __vector float vf;

__attribute__ ((target ("no-vmx128"))) vf
f1 (vf b)
{
  return __builtin_xenon_vpermwi (b, 7);	/* { dg-error "'__builtin_xenon_vpermwi_v4sf' requires the '-mvmx128' option" } */
}

vf
f2 (vf b)
{
  return __builtin_xenon_vpermwi (b, 7);
}
