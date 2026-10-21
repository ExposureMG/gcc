/* -mrecip-precision tells GCC that fres/frsqrte give at least 14 bits, so
   the -mrecip Newton-Raphson sequences take fewer steps.  The Xenon PPE's
   fres keeps 8 bits and frsqrte 9 (isa/FINDINGS.md 1.2), so an explicit
   request is an error under -mcpu=xenon instead of silently inaccurate
   results.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Ofast -mrecip=all -mrecip-precision -mdejagnu-cpu=xenon" } */
/* { dg-error "'-mrecip-precision' is not supported by '-mcpu=xenon'" "" { target *-*-* } 0 } */

float rf (float a, float b) { return a / b; }
