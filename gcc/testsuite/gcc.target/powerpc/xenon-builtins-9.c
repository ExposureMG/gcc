/* Xenon VMX128-only builtins (step 7): <xenon_vmx.h> compiles cleanly
   with -Wall -Wextra -pedantic (C99) and each wrapper emits exactly its
   instruction.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -std=c99 -Wall -Wextra -pedantic" } */

/* { dg-final { scan-assembler-times {\mvmsum3fp128 } 1 } } */
/* { dg-final { scan-assembler-times {\mvmsum4fp128 } 1 } } */
/* { dg-final { scan-assembler-times {\mvmaddcfp128 } 1 } } */
/* { dg-final { scan-assembler-times {\mvmulfp128 } 1 } } */
/* { dg-final { scan-assembler {\mvrlimi128 [0-9]+,[0-9]+,9,2$} } } */
/* { dg-final { scan-assembler {\mvpermwi128 [0-9]+,[0-9]+,228$} } } */
/* { dg-final { scan-assembler {\mvpermwi128 [0-9]+,[0-9]+,83$} } } */
/* { dg-final { scan-assembler {\mvpkd3d128 [0-9]+,[0-9]+,5,2,0$} } } */
/* { dg-final { scan-assembler {\mvpkd3d128 [0-9]+,[0-9]+,0,1,3$} } } */
/* { dg-final { scan-assembler {\mvupkd3d128 [0-9]+,[0-9]+,24$} } } */
/* { dg-final { scan-assembler {\mvupkd3d128 [0-9]+,[0-9]+,12$} } } */

#include <xenon_vmx.h>

#if !defined (__VMX128__) || XENON_D3D_NORMPACKED64 != 6 || XENON_PERMWI (0, 1, 2, 3) != 0x1B
#error "xenon_vmx.h macros"
#endif

typedef __vector float vf;
typedef __vector unsigned int vui;

void
use_all (vf *o, const vf *a, const vui *u)
{
  o[0] = xenon_vmsum3fp (a[0], a[1]);
  o[1] = xenon_vmsum4fp (a[0], a[1]);
  o[2] = xenon_vmaddcfp (a[0], a[1], a[2]);
  o[3] = xenon_vmulfp (a[0], a[2]);
  o[4] = xenon_vrlimi (a[1], a[2], XENON_MASK_X | XENON_MASK_W, 2);
  o[5] = xenon_vpermwi (a[0], XENON_PERMWI (3, 2, 1, 0));
  o[6] = xenon_vswizzle (a[1], 1, 1, 0, 3);
  o[7] = xenon_vpkd3d (a[2], a[0], XENON_D3D_FLOAT16_4, XENON_PACK_64LO, 0);
  o[8] = xenon_vpkd3d (a[2], a[0], XENON_D3D_D3DCOLOR, XENON_PACK_32, 3);
  o[9] = xenon_vupkd3d (u[0], XENON_D3D_NORMPACKED64);
  o[10] = xenon_vupkd3d (u[1], XENON_D3D_FLOAT16_2);
}
