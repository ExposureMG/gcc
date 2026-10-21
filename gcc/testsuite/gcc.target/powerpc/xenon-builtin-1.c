/* Xenon (-mcpu=xenon): the AltiVec intrinsics for the integer multiply,
   multiply-sum, multiply-high-add and sum-across instructions are rejected
   with an error that names the instruction (the Xenon PPE raises an
   illegal-instruction interrupt for all of them).  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-prune-output "overloaded builtin" } */

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
extern vui ui1, ui2;
typedef long long v2di __attribute__ ((vector_size (16)));

vus f00 (void) { return vec_mule (uc1, uc2); }	/* { dg-error "is not supported on Xenon: the .vmuleub. instruction is not implemented" } */
vss f01 (void) { return vec_mule (sc1, sc2); }	/* { dg-error "is not supported on Xenon: the .vmulesb. instruction is not implemented" } */
vui f02 (void) { return vec_mule (us1, us2); }	/* { dg-error "is not supported on Xenon: the .vmuleuh. instruction is not implemented" } */
vsi f03 (void) { return vec_mule (ss1, ss2); }	/* { dg-error "is not supported on Xenon: the .vmulesh. instruction is not implemented" } */
vus f04 (void) { return vec_mulo (uc1, uc2); }	/* { dg-error "is not supported on Xenon: the .vmuloub. instruction is not implemented" } */
vss f05 (void) { return vec_mulo (sc1, sc2); }	/* { dg-error "is not supported on Xenon: the .vmulosb. instruction is not implemented" } */
vui f06 (void) { return vec_mulo (us1, us2); }	/* { dg-error "is not supported on Xenon: the .vmulouh. instruction is not implemented" } */
vsi f07 (void) { return vec_mulo (ss1, ss2); }	/* { dg-error "is not supported on Xenon: the .vmulosh. instruction is not implemented" } */
vui f08 (void) { return vec_msum (uc1, uc2, ui1); }	/* { dg-error "is not supported on Xenon: the .vmsumubm. instruction is not implemented" } */
vsi f09 (void) { return vec_msum (sc1, uc2, si1); }	/* { dg-error "is not supported on Xenon: the .vmsummbm. instruction is not implemented" } */
vui f10 (void) { return vec_msum (us1, us2, ui1); }	/* { dg-error "is not supported on Xenon: the .vmsumuhm. instruction is not implemented" } */
vsi f11 (void) { return vec_msum (ss1, ss2, si1); }	/* { dg-error "is not supported on Xenon: the .vmsumshm. instruction is not implemented" } */
vui f12 (void) { return vec_msums (us1, us2, ui1); }	/* { dg-error "is not supported on Xenon: the .vmsumuhs. instruction is not implemented" } */
vsi f13 (void) { return vec_msums (ss1, ss2, si1); }	/* { dg-error "is not supported on Xenon: the .vmsumshs. instruction is not implemented" } */
vss f14 (void) { return vec_madds (ss1, ss2, ss3); }	/* { dg-error "is not supported on Xenon: the .vmhaddshs. instruction is not implemented" } */
vss f15 (void) { return vec_mradds (ss1, ss2, ss3); }	/* { dg-error "is not supported on Xenon: the .vmhraddshs. instruction is not implemented" } */
vss f16 (void) { return vec_mladd (ss1, ss2, ss3); }	/* { dg-error "is not supported on Xenon: the .vmladduhm. instruction is not implemented" } */
vus f17 (void) { return vec_mladd (us1, us2, us3); }	/* { dg-error "is not supported on Xenon: the .vmladduhm. instruction is not implemented" } */
vss f18 (void) { return vec_madd (ss1, ss2, ss3); }	/* { dg-error "is not supported on Xenon: the .vmladduhm. instruction is not implemented" } */
vui f19 (void) { return vec_sum4s (uc1, ui1); }	/* { dg-error "is not supported on Xenon: the .vsum4ubs. instruction is not implemented" } */
vsi f20 (void) { return vec_sum4s (sc1, si1); }	/* { dg-error "is not supported on Xenon: the .vsum4sbs. instruction is not implemented" } */
vsi f21 (void) { return vec_sum4s (ss1, si1); }	/* { dg-error "is not supported on Xenon: the .vsum4shs. instruction is not implemented" } */
vsi f22 (void) { return vec_sum2s (si1, si2); }	/* { dg-error "is not supported on Xenon: the .vsum2sws. instruction is not implemented" } */
vsi f23 (void) { return vec_sums (si1, si2); }	/* { dg-error "is not supported on Xenon: the .vsumsws. instruction is not implemented" } */
vus f24 (void) { return vec_vmuleub (uc1, uc2); }	/* { dg-error "is not supported on Xenon: the .vmuleub. instruction is not implemented" } */
vsi f25 (void) { return vec_vmsumshm (ss1, ss2, si1); }	/* { dg-error "is not supported on Xenon: the .vmsumshm. instruction is not implemented" } */
vui f26 (void) { return vec_vsum4ubs (uc1, ui1); }	/* { dg-error "is not supported on Xenon: the .vsum4ubs. instruction is not implemented" } */
vsi f27 (void) { return vec_vsumsws (si1, si2); }	/* { dg-error "is not supported on Xenon: the .vsumsws. instruction is not implemented" } */
vss f28 (void) { return vec_vmhaddshs (ss1, ss2, ss3); }	/* { dg-error "is not supported on Xenon: the .vmhaddshs. instruction is not implemented" } */
vss f29 (void) { return vec_vmladduhm (ss1, ss2, ss3); }	/* { dg-error "is not supported on Xenon: the .vmladduhm. instruction is not implemented" } */
/* The vector int forms are the ISA 2.07 vmulesw/vmuleuw/vmulosw/vmulouw;
   they used to pass the builtin check and ICE at expansion.  */
v2di f30 (void) { return (v2di) vec_mule (si1, si2); }	/* { dg-error "is not supported on Xenon: the .vmulesw. instruction is not implemented" } */
v2di f31 (void) { return (v2di) vec_mule (ui1, ui2); }	/* { dg-error "is not supported on Xenon: the .vmuleuw. instruction is not implemented" } */
v2di f32 (void) { return (v2di) vec_mulo (si1, si2); }	/* { dg-error "is not supported on Xenon: the .vmulosw. instruction is not implemented" } */
v2di f33 (void) { return (v2di) vec_mulo (ui1, ui2); }	/* { dg-error "is not supported on Xenon: the .vmulouw. instruction is not implemented" } */
