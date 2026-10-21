// Xenon VMX128-only builtins (step 7): <xenon_vmx.h> in C++, clean with
// -Wall -Wextra -pedantic, and the overloaded builtins resolve in C++.
// { dg-do compile { target powerpc*-*-* } }
// { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon -std=c++17 -Wall -Wextra -pedantic" }

// { dg-final { scan-assembler {\mvmsum3fp128 } } }
// { dg-final { scan-assembler {\mvrlimi128 [0-9]+,[0-9]+,15,1$} } }
// { dg-final { scan-assembler {\mvpermwi128 [0-9]+,[0-9]+,27$} } }
// { dg-final { scan-assembler {\mvpkd3d128 [0-9]+,[0-9]+,1,1,2$} } }
// { dg-final { scan-assembler {\mvupkd3d128 [0-9]+,[0-9]+,4$} } }

#include <xenon_vmx.h>

typedef __vector float vf;
typedef __vector unsigned int vui;

template <typename T>
T
rot1 (T d, T b)
{
  return xenon_vrlimi (d, b, XENON_MASK_XYZW, 1);
}

vf
dot3 (vf a, vf b)
{
  return xenon_vmsum3fp (a, b);
}

vui
f (vui d, vui b)
{
  return rot1 (d, b);
}

vf
g (vf b)
{
  return xenon_vswizzle (b, 0, 1, 2, 3);
}

vui
h (vui d, vf b)
{
  return xenon_vpkd3d (d, b, XENON_D3D_NORMSHORT2, XENON_PACK_32, 2);
}

vf
k (vui b)
{
  return xenon_vupkd3d (b, XENON_D3D_NORMSHORT2);
}
