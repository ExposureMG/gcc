/* The vector __int128 forms of vperm, vsel and vsldoi need V1TImode, which
   is a vector mode only with VSX.  Without VSX (-mcpu=xenon) they are
   refused with the usual "requires -mvsx" error instead of an ICE in
   copy_to_mode_reg.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */

#include <altivec.h>

typedef __vector signed __int128 vsq;
typedef __vector unsigned __int128 vuq;
extern vsq a, b;
extern vuq ua, ub, uc;
extern __vector unsigned char pc;

vsq f0 (void) { return __builtin_altivec_vperm_1ti (a, b, pc); }	/* { dg-error "requires the .-mvsx. option" } */
vuq f1 (void) { return __builtin_altivec_vperm_1ti_uns (ua, ub, pc); }	/* { dg-error "requires the .-mvsx. option" } */
vsq f2 (void) { return __builtin_altivec_vsel_1ti (a, b, uc); }	/* { dg-error "requires the .-mvsx. option" } */
vuq f3 (void) { return __builtin_altivec_vsel_1ti_uns (ua, ub, uc); }	/* { dg-error "requires the .-mvsx. option" } */
vsq f4 (void) { return __builtin_altivec_vsldoi_v1ti (a, b, 3); }	/* { dg-error "requires the .-mvsx. option" } */
