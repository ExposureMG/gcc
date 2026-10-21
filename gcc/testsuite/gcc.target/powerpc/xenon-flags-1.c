/* Explicit ISA options for instructions the Xenon PPE lacks are errors
   under -mcpu=xenon.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mpopcntb -misel -mvsx -mcmpb -mfprnd" } */
/* { dg-error "'-mpopcntb' is not supported by '-mcpu=xenon'" "" { target *-*-* } 0 } */
/* { dg-error "'-misel' is not supported by '-mcpu=xenon'" "" { target *-*-* } 0 } */
/* { dg-error "'-mvsx' is not supported by '-mcpu=xenon'" "" { target *-*-* } 0 } */
/* { dg-error "'-mcmpb' is not supported by '-mcpu=xenon'" "" { target *-*-* } 0 } */
/* { dg-error "'-mfprnd' is not supported by '-mcpu=xenon'" "" { target *-*-* } 0 } */

int f (unsigned int x) { return __builtin_popcount (x); }
