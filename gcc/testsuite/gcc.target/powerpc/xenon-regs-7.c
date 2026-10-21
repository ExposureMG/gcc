/* Xenon VMX128 (plan 4.2): -mno-altivec turns off -mvmx128 too, and the
   v32-v127 register names are then not usable for vector variables.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mno-altivec -mvmx128" } */
/* { dg-error "'-mno-altivec' turns off '-mvmx128'" "" { target *-*-* } 0 } */

int f (int x) { return x + 1; }
