/* Xbox 360 Xenon VMX128 intrinsics.
   Copyright (C) 2026 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* A thin layer over the __builtin_xenon_* functions, which expose the
   VMX128 instructions that have no AltiVec equivalent (see "Xbox 360 Xenon
   VMX128 Built-in Functions" in the GCC manual).  Each function or macro
   below emits exactly one instruction.  Word 0 (x) is the leftmost
   (lowest-addressed) element of a vector, as everywhere on this big-endian
   target.

     xenon_vmsum3fp (a, b)        vmsum3fp128: every word of the result is
                                  a.x*b.x + a.y*b.y + a.z*b.z
     xenon_vmsum4fp (a, b)        vmsum4fp128: every word is
                                  a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w
          These two are NOT IEEE operations: the products are exact but are
          truncated to a common accumulator, the sum is a one's-complement
          sum and the result is truncated to 24 bits, so its magnitude can
          be an ulp or so below the correctly rounded dot product (more
          when the products cancel); a NaN, inf*0 or inf-inf gives the
          default NaN 0x7FC00000 and an overflow a NaN, never an infinity.
     xenon_vmaddcfp (a, d, b)     vmaddcfp128: a*d + b, fused (one
                                  round-to-nearest-even rounding); d lives
                                  in the destination register
     xenon_vmulfp (a, b)          vmulfp128: a*b, rounded
     xenon_vrlimi (d, b, mask, rot)
                                  vrlimi128: word i of the result is word
                                  (i + rot) & 3 of b if bit (3 - i) of mask
                                  is set (XENON_MASK_X = word 0 ...
                                  XENON_MASK_W = word 3), else word i of d
     xenon_vpermwi (b, perm)      vpermwi128: word i of the result is word
                                  (perm >> (6 - 2*i)) & 3 of b
     xenon_vswizzle (b, x, y, z, w)
                                  vpermwi128 with XENON_PERMWI (x, y, z, w):
                                  the result is { b[x], b[y], b[z], b[w] }
     xenon_vpkd3d (d, b, type, pack, slot)
                                  vpkd3d128: packs the floats of b as D3D
                                  type TYPE (XENON_D3D_*) with pack size
                                  PACK (XENON_PACK_*) and merges the result
                                  into d (d is the destination register).
                                  SLOT is the instruction's VD3D2 field
                                  (0-3), which selects where in d the result
                                  goes.  Which word that is, which way the
                                  slots count, and where the two words of a
                                  64-bit result (PACK 2 and 3) go are NOT
                                  verified on hardware (the hardware
                                  findings leave several (type, pack) pairs
                                  open); the host models assume word SLOT
                                  counted from the left (x) for PACK 1.
                                  tools/xts-probes settles it on a console.
                                  Sets VSCR[SAT] when a value is clamped.
     xenon_vupkd3d (b, type)      vupkd3d128: unpacks b as D3D type TYPE
                                  into four floats.

   The integer D3D formats are "floats biased by 3.0": the integer code of
   a lane is its float bit pattern minus 0x40400000 (FINDINGS 1.3), both
   for the input of a pack and the output of an unpack.  FLOAT16 is the
   Xenos half format (it saturates instead of producing inf or NaN).

   MASK, ROT, PERM, TYPE, PACK and SLOT must be integer constant
   expressions (the instruction encodes them); the compiler checks their
   ranges: mask 0-15, rot 0-3, perm 0-255, type 0-6, pack 1-3, slot 0-3.
   d and b of xenon_vrlimi, b of xenon_vpermwi and d of xenon_vpkd3d may be
   vector float, vector signed int or vector unsigned int (xenon_vrlimi and
   xenon_vpermwi also vector bool int; the result has the type of d or b);
   b of xenon_vupkd3d may be any non-bool 128-bit vector type.

   The functions need code compiled for Xenon with AltiVec and VMX128
   (-mcpu=xenon, where -mvmx128 is the default; __VMX128__ is then
   defined).  A unit compiled for another CPU can include the header too,
   for functions marked __attribute__ ((target ("cpu=xenon"))) or compiled
   under #pragma GCC target ("cpu=xenon"), as with the x86 intrinsic
   headers: the definitions below are then compiled for Xenon, and using
   one in a function not compiled for Xenon is an error (inlining fails on
   the target mismatch).  */

#ifndef _XENON_VMX_H
#define _XENON_VMX_H 1

#if !defined (__VMX128__)
#if !defined (__has_builtin)
#error "xenon_vmx.h needs a compiler with the Xenon VMX128 built-in functions (GCC with -mcpu=xenon)"
#elif !__has_builtin (__builtin_xenon_vmsum4fp)
#error "xenon_vmx.h needs a compiler with the Xenon VMX128 built-in functions (GCC with -mcpu=xenon)"
#endif
#pragma GCC push_options
#pragma GCC target ("cpu=xenon,altivec,vmx128")
#define __XENON_VMX_H_TARGET_PUSHED 1
#endif

/* D3D data types (the TYPE operand of xenon_vpkd3d and xenon_vupkd3d,
   VD3D0 / UIMM >> 2 of the instructions).  */
#define XENON_D3D_D3DCOLOR	0	/* 4 x 8-bit unsigned normalized  */
#define XENON_D3D_NORMSHORT2	1	/* 2 x 16-bit signed normalized  */
#define XENON_D3D_NORMPACKED32	2	/* 2_10_10_10  */
#define XENON_D3D_FLOAT16_2	3	/* 2 x Xenos half  */
#define XENON_D3D_NORMSHORT4	4	/* 4 x 16-bit signed normalized  */
#define XENON_D3D_FLOAT16_4	5	/* 4 x Xenos half  */
#define XENON_D3D_NORMPACKED64	6	/* 4_20_20_20  */

/* Pack sizes (the PACK operand of xenon_vpkd3d, VD3D1): a 32-bit result,
   or one of the two halves of a 64-bit result.  */
#define XENON_PACK_32		1
#define XENON_PACK_64LO		2
#define XENON_PACK_64HI		3

/* Lane masks for xenon_vrlimi.  */
#define XENON_MASK_X		8
#define XENON_MASK_Y		4
#define XENON_MASK_Z		2
#define XENON_MASK_W		1
#define XENON_MASK_XYZW		15

/* The vpermwi128 immediate that selects words X, Y, Z, W (each 0-3) of the
   source for result words 0-3.  XENON_PERMWI (0, 1, 2, 3) is the
   identity.  */
#define XENON_PERMWI(x, y, z, w) \
  ((((x) & 3) << 6) | (((y) & 3) << 4) | (((z) & 3) << 2) | ((w) & 3))

static __inline__ __vector float
__attribute__ ((__always_inline__, __artificial__))
xenon_vmsum3fp (__vector float __a, __vector float __b)
{
  return __builtin_xenon_vmsum3fp (__a, __b);
}

static __inline__ __vector float
__attribute__ ((__always_inline__, __artificial__))
xenon_vmsum4fp (__vector float __a, __vector float __b)
{
  return __builtin_xenon_vmsum4fp (__a, __b);
}

static __inline__ __vector float
__attribute__ ((__always_inline__, __artificial__))
xenon_vmaddcfp (__vector float __a, __vector float __d, __vector float __b)
{
  return __builtin_xenon_vmaddcfp (__a, __d, __b);
}

static __inline__ __vector float
__attribute__ ((__always_inline__, __artificial__))
xenon_vmulfp (__vector float __a, __vector float __b)
{
  return __builtin_xenon_vmulfp (__a, __b);
}

/* The operations with immediates are macros, so that the immediates stay
   constant expressions at every optimization level.  */
#define xenon_vrlimi(d, b, mask, rot) \
  __builtin_xenon_vrlimi ((d), (b), (mask), (rot))
#define xenon_vpermwi(b, perm) \
  __builtin_xenon_vpermwi ((b), (perm))
#define xenon_vswizzle(b, x, y, z, w) \
  __builtin_xenon_vpermwi ((b), XENON_PERMWI (x, y, z, w))
#define xenon_vpkd3d(d, b, type, pack, slot) \
  __builtin_xenon_vpkd3d ((d), (b), (type), (pack), (slot))
#define xenon_vupkd3d(b, type) \
  __builtin_xenon_vupkd3d ((b), (type))

#ifdef __XENON_VMX_H_TARGET_PUSHED
#undef __XENON_VMX_H_TARGET_PUSHED
#pragma GCC pop_options
#endif

#endif /* _XENON_VMX_H */
