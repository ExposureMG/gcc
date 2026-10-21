/* Xenon VMX128-only builtins (step 7) follow the target attribute and
   pragma: available in a cpu=xenon function of a -mcpu=cell unit.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */

/* { dg-final { scan-assembler {\mvmsum4fp128 } } } */
/* { dg-final { scan-assembler {\mvrlimi128 [0-9]+,[0-9]+,1,1$} } } */
/* { dg-final { scan-assembler {\mvpermwi128 [0-9]+,[0-9]+,7$} } } */

typedef __vector float vf;

__attribute__ ((target ("cpu=xenon"))) vf
f1 (vf a, vf b)
{
  return __builtin_xenon_vmsum4fp (a, b);
}

__attribute__ ((target ("cpu=xenon"))) vf
f2 (vf d, vf b)
{
  return __builtin_xenon_vrlimi (d, b, 1, 1);
}

#pragma GCC push_options
#pragma GCC target ("cpu=xenon")
vf
f3 (vf b)
{
  return __builtin_xenon_vpermwi (b, 7);
}
#pragma GCC pop_options
