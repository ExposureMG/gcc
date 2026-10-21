/* Xenon vectorizer costs (plan 5.5): a lane goes between a VR and a GPR/FPR
   only through memory (store, then a load that hits it: ~40 cycles,
   FINDINGS 3.5), so vec_to_scalar costs 40 and basic-block SLP that needs
   lane extracts is not done (h); the same block without scalar uses of the
   lanes is (k).  xenon-vect-cost-6.c: Cell tuning SLPs both.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O3 -mdejagnu-cpu=xenon -fdump-tree-slp2-details" } */
/* { dg-final { scan-tree-dump {vec_to_scalar costs 40 in epilogue} "slp2" } } */
/* { dg-final { scan-tree-dump-times {basic block part vectorized} 1 "slp2" } } */
/* { dg-final { scan-tree-dump {not vectorized: vectorization is not profitable} "slp2" } } */

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
