/* Xenon (-mcpu=xenon): the __builtin_altivec_* functions behind the
   integer multiply, multiply-sum and sum-across intrinsics are rejected when
   they are expanded, with an error that names the instruction.  (They are
   separate from xenon-builtin-1.c because expansion-time errors are not
   reported once the front end has given errors.)  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */

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

vui f30 (void) { return __builtin_altivec_vmulouh (us1, us2); }	/* { dg-error "is not supported on Xenon: the .vmulouh. instruction is not implemented" } */
vui f31 (void) { return __builtin_altivec_vmsumuhm (us1, us2, ui1); }	/* { dg-error "is not supported on Xenon: the .vmsumuhm. instruction is not implemented" } */
vss f32 (void) { return __builtin_altivec_vmladduhm (ss1, ss2, ss3); }	/* { dg-error "is not supported on Xenon: the .vmladduhm. instruction is not implemented" } */
vsi f33 (void) { return __builtin_altivec_vsum4shs (ss1, si1); }	/* { dg-error "is not supported on Xenon: the .vsum4shs. instruction is not implemented" } */
vsi f34 (void) { return __builtin_altivec_vsumsws_be (si1, si2); }	/* { dg-error "is not supported on Xenon: the .vsumsws. instruction is not implemented" } */
v2di f35 (void) { return (v2di) __builtin_altivec_vmulesw (si1, si2); }	/* { dg-error "is not supported on Xenon: the .vmulesw. instruction is not implemented" } */
v2di f36 (void) { return (v2di) __builtin_altivec_vmulouw (ui1, ui2); }	/* { dg-error "is not supported on Xenon: the .vmulouw. instruction is not implemented" } */
