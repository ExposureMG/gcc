/* The same intrinsics as xenon-builtin-1.c still work on other CPUs: the
   Xenon gating (TARGET_ALTIVEC_INT_MUL / TARGET_ALTIVEC_VSUM) is specific to
   -mcpu=xenon.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=cell" } */
/* { dg-final { scan-assembler {\mvmhaddshs\M} } } */
/* { dg-final { scan-assembler {\mvmhraddshs\M} } } */
/* { dg-final { scan-assembler {\mvmladduhm\M} } } */
/* { dg-final { scan-assembler {\mvmsummbm\M} } } */
/* { dg-final { scan-assembler {\mvmsumshm\M} } } */
/* { dg-final { scan-assembler {\mvmsumshs\M} } } */
/* { dg-final { scan-assembler {\mvmsumubm\M} } } */
/* { dg-final { scan-assembler {\mvmsumuhm\M} } } */
/* { dg-final { scan-assembler {\mvmsumuhs\M} } } */
/* { dg-final { scan-assembler {\mvmulesb\M} } } */
/* { dg-final { scan-assembler {\mvmulesh\M} } } */
/* { dg-final { scan-assembler {\mvmuleub\M} } } */
/* { dg-final { scan-assembler {\mvmuleuh\M} } } */
/* { dg-final { scan-assembler {\mvmulosb\M} } } */
/* { dg-final { scan-assembler {\mvmulosh\M} } } */
/* { dg-final { scan-assembler {\mvmuloub\M} } } */
/* { dg-final { scan-assembler {\mvmulouh\M} } } */
/* { dg-final { scan-assembler {\mvsum2sws\M} } } */
/* { dg-final { scan-assembler {\mvsum4sbs\M} } } */
/* { dg-final { scan-assembler {\mvsum4shs\M} } } */
/* { dg-final { scan-assembler {\mvsum4ubs\M} } } */
/* { dg-final { scan-assembler {\mvsumsws\M} } } */
#include <altivec.h>

typedef __vector signed char vsc;
typedef __vector unsigned char vuc;
typedef __vector signed short vss;
typedef __vector unsigned short vus;
typedef __vector signed int vsi;
typedef __vector unsigned int vui;

extern vsc sc1, sc2;
extern vuc uc1, uc2;
extern vss ss1, ss2, ss3;
extern vus us1, us2, us3;
extern vsi si1, si2;
extern vui ui1;

vus f00 (void) { return vec_mule (uc1, uc2); }
vss f01 (void) { return vec_mule (sc1, sc2); }
vui f02 (void) { return vec_mule (us1, us2); }
vsi f03 (void) { return vec_mule (ss1, ss2); }
vus f04 (void) { return vec_mulo (uc1, uc2); }
vss f05 (void) { return vec_mulo (sc1, sc2); }
vui f06 (void) { return vec_mulo (us1, us2); }
vsi f07 (void) { return vec_mulo (ss1, ss2); }
vui f08 (void) { return vec_msum (uc1, uc2, ui1); }
vsi f09 (void) { return vec_msum (sc1, uc2, si1); }
vui f10 (void) { return vec_msum (us1, us2, ui1); }
vsi f11 (void) { return vec_msum (ss1, ss2, si1); }
vui f12 (void) { return vec_msums (us1, us2, ui1); }
vsi f13 (void) { return vec_msums (ss1, ss2, si1); }
vss f14 (void) { return vec_madds (ss1, ss2, ss3); }
vss f15 (void) { return vec_mradds (ss1, ss2, ss3); }
vss f16 (void) { return vec_mladd (ss1, ss2, ss3); }
vus f17 (void) { return vec_mladd (us1, us2, us3); }
vss f18 (void) { return vec_madd (ss1, ss2, ss3); }
vui f19 (void) { return vec_sum4s (uc1, ui1); }
vsi f20 (void) { return vec_sum4s (sc1, si1); }
vsi f21 (void) { return vec_sum4s (ss1, si1); }
vsi f22 (void) { return vec_sum2s (si1, si2); }
vsi f23 (void) { return vec_sums (si1, si2); }
vus f24 (void) { return vec_vmuleub (uc1, uc2); }
vsi f25 (void) { return vec_vmsumshm (ss1, ss2, si1); }
vui f26 (void) { return vec_vsum4ubs (uc1, ui1); }
vsi f27 (void) { return vec_vsumsws (si1, si2); }
vss f28 (void) { return vec_vmhaddshs (ss1, ss2, ss3); }
vss f29 (void) { return vec_vmladduhm (ss1, ss2, ss3); }
vui f30 (void) { return __builtin_altivec_vmulouh (us1, us2); }
vui f31 (void) { return __builtin_altivec_vmsumuhm (us1, us2, ui1); }
vss f32 (void) { return __builtin_altivec_vmladduhm (ss1, ss2, ss3); }
vsi f33 (void) { return __builtin_altivec_vsum4shs (ss1, si1); }
vsi f34 (void) { return __builtin_altivec_vsumsws_be (si1, si2); }
