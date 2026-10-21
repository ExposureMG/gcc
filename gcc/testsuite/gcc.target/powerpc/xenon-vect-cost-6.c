/* Contrast for xenon-vect-cost-5.c: with Cell tuning (the generic rs6000
   costs: vec_to_scalar 1) both blocks are SLP vectorized, so the Xenon
   decision comes from the Xenon transfer cost.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -mdejagnu-tune=cell -fdump-tree-slp2-details" } */
/* { dg-final { scan-tree-dump {vec_to_scalar costs 1 in epilogue} "slp2" } } */
/* { dg-final { scan-tree-dump-times {basic block part vectorized} 2 "slp2" } } */

float A[4] __attribute__ ((aligned (16)));
float B[4] __attribute__ ((aligned (16)));
float OUT[4] __attribute__ ((aligned (16)));
float SIDE[16];

void h (void)
{
  float x0 = A[0] * B[0], x1 = A[1] * B[1], x2 = A[2] * B[2], x3 = A[3] * B[3];
  OUT[0] = x0; OUT[1] = x1; OUT[2] = x2; OUT[3] = x3;
  SIDE[0] = x0 - x3;
  SIDE[9] = x1 + x2;
}

void k (void)
{
  float x0 = A[0] * B[0], x1 = A[1] * B[1], x2 = A[2] * B[2], x3 = A[3] * B[3];
  OUT[0] = x0; OUT[1] = x1; OUT[2] = x2; OUT[3] = x3;
}
