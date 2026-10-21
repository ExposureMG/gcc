/* Xenon VMX128 (plan 4.1): -mvmx128 needs -mcpu=xenon.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell -mvmx128" } */
/* { dg-error "'-mvmx128' requires '-mcpu=xenon'" "" { target *-*-* } 0 } */

int f (int x) { return x + 1; }
