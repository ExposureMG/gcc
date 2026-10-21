/* -mxenon-smt takes idle, busy or same.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mxenon-smt=parked" } */
/* { dg-error "unrecognized argument in option '-mxenon-smt=parked'" "" { target *-*-* } 0 } */
/* { dg-message "valid arguments to '-mxenon-smt=' are: busy idle same" "" { target *-*-* } 0 } */

int f (int x) { return x; }
