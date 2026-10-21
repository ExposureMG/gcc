;; Xbox 360 Xenon VMX128-only operations (XENON_TARGET_PLAN 4.2 last
;; bullets, D7, 4.4 phase 3).
;; Copyright (C) 2026 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.
;;
;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; The VMX128 instructions without an AltiVec equivalent: vmsum3fp128,
;; vmsum4fp128, vrlimi128, vpermwi128, vpkd3d128, vupkd3d128, plus the
;; builtin forms of vmaddcfp128 and vmulfp128 (which the compiler also
;; reaches through fma and V4SF multiplication, altivec.md).  They are only
;; reachable through the __builtin_xenon_* functions (rs6000-builtins.def
;; stanza [xenon-vmx128], rs6000-overload.def): every pattern is an UNSPEC,
;; so no optimizer and in particular not the vectorizer creates, folds or
;; contracts them, and each builtin call emits exactly one instruction.
;; vmsum3fp128/vmsum4fp128 must stay that way: they are not IEEE dot products
;; (FINDINGS 1.3: truncated accumulator, one's-complement sum).
;;
;; Every register operand is wV (v0-v127, VMX128_REGS) and the 128 mnemonic
;; is always printed (there is no legacy form).  The destructive forms
;; vmaddcfp128, vrlimi128 and vpkd3d128 read their destination field (JSON
;; tied_operands [VD128]): the tied input uses the matching constraint "0",
;; so the register allocator inserts the copy when the old value stays live.
;; The immediates are const_int operands checked against their instruction
;; field (isa/xenon_isa.json "fields"); the builtins check the narrower
;; ranges with a defined meaning before expansion.
;;
;; Operand order and semantics (isa/xenon_isa.json, FINDINGS 1.3):
;;   vmsum3fp128 VD,VA,VB      D.xyzw = A.x*B.x + A.y*B.y + A.z*B.z
;;   vmsum4fp128 VD,VA,VB      D.xyzw = A.x*B.x + ... + A.w*B.w
;;   vmaddcfp128 VD,VA,VD,VB   D = A*D + B (fused, one RNE rounding)
;;   vmulfp128   VD,VA,VB      D = A*B (rounded product)
;;   vrlimi128   VD,VB,UIMM,VD3D2
;;                             D.i = rotl_words (B, VD3D2)[i] if UIMM bit
;;                             (3-i) is set, else D.i
;;   vpermwi128  VD,VB,VPERM128
;;                             D.i = B[(VPERM128 >> (6-2i)) & 3]
;;   vpkd3d128   VD,VB,VD3D0,VD3D1,VD3D2
;;                             pack B as D3D type VD3D0, pack size VD3D1,
;;                             into word slot VD3D2 of D; D's other words
;;                             are kept; sets VSCR[SAT] when it clamps
;;   vupkd3d128  VD,VB,UIMM    D = unpack B as D3D type UIMM >> 2
;; Scheduling classes (xenon.md): vmsum3/4fp128 veccomplex (14 cycles),
;; vmaddcfp128/vmulfp128 vecfloat (12), the others vecperm (4); FINDINGS
;; 2.x tables.

(define_c_enum "unspec"
  [UNSPEC_XENON_VMSUM3FP
   UNSPEC_XENON_VMSUM4FP
   UNSPEC_XENON_VMADDCFP
   UNSPEC_XENON_VMULFP
   UNSPEC_XENON_VRLIMI
   UNSPEC_XENON_VPERMWI
   UNSPEC_XENON_VPKD3D
   UNSPEC_XENON_VUPKD3D])

;; The word-granular operations exist for vector float and vector int.
(define_mode_iterator VXENON_W [V4SF V4SI])

(define_insn "xenon_vmsum3fp128"
  [(set (match_operand:V4SF 0 "vmx128_register_operand" "=wV")
	(unspec:V4SF [(match_operand:V4SF 1 "vmx128_register_operand" "wV")
		      (match_operand:V4SF 2 "vmx128_register_operand" "wV")]
		     UNSPEC_XENON_VMSUM3FP))]
  "TARGET_VMX128"
  "vmsum3fp128 %0,%1,%2"
  [(set_attr "type" "veccomplex")])

(define_insn "xenon_vmsum4fp128"
  [(set (match_operand:V4SF 0 "vmx128_register_operand" "=wV")
	(unspec:V4SF [(match_operand:V4SF 1 "vmx128_register_operand" "wV")
		      (match_operand:V4SF 2 "vmx128_register_operand" "wV")]
		     UNSPEC_XENON_VMSUM4FP))]
  "TARGET_VMX128"
  "vmsum4fp128 %0,%1,%2"
  [(set_attr "type" "veccomplex")])

;; __builtin_xenon_vmaddcfp (a, d, b) = a*d + b with d in the destination
;; register: vmaddcfp128 vD,vA,vD,vB.  Operand 2 is the tied multiplicand.
(define_insn "xenon_vmaddcfp128"
  [(set (match_operand:V4SF 0 "vmx128_register_operand" "=wV")
	(unspec:V4SF [(match_operand:V4SF 1 "vmx128_register_operand" "wV")
		      (match_operand:V4SF 2 "vmx128_register_operand" "0")
		      (match_operand:V4SF 3 "vmx128_register_operand" "wV")]
		     UNSPEC_XENON_VMADDCFP))]
  "TARGET_VMX128"
  "vmaddcfp128 %0,%1,%2,%3"
  [(set_attr "type" "vecfloat")])

;; __builtin_xenon_vmulfp: the rounded product as an UNSPEC, unlike
;; xenon_vmulfp128 (altivec.md, the V4SF multiply), which the optimizers
;; may fold or contract into an fma.
(define_insn "xenon_vmulfp128_exact"
  [(set (match_operand:V4SF 0 "vmx128_register_operand" "=wV")
	(unspec:V4SF [(match_operand:V4SF 1 "vmx128_register_operand" "wV")
		      (match_operand:V4SF 2 "vmx128_register_operand" "wV")]
		     UNSPEC_XENON_VMULFP))]
  "TARGET_VMX128"
  "vmulfp128 %0,%1,%2"
  [(set_attr "type" "vecfloat")])

;; vrlimi128 vD,vB,UIMM,VD3D2 (vD tied): operand 1 is the old vD, operand 3
;; the lane mask (UIMM, a 5-bit field, bits 3:0 used), operand 4 the
;; rotate count in words (VD3D2, 2 bits).
(define_insn "xenon_vrlimi128_<mode>"
  [(set (match_operand:VXENON_W 0 "vmx128_register_operand" "=wV")
	(unspec:VXENON_W
	  [(match_operand:VXENON_W 1 "vmx128_register_operand" "0")
	   (match_operand:VXENON_W 2 "vmx128_register_operand" "wV")
	   (match_operand:SI 3 "u5bit_cint_operand" "n")
	   (match_operand:SI 4 "const_0_to_3_operand" "n")]
	  UNSPEC_XENON_VRLIMI))]
  "TARGET_VMX128"
  "vrlimi128 %0,%2,%3,%4"
  [(set_attr "type" "vecperm")])

;; vpermwi128 vD,vB,VPERM128 (8-bit immediate).
(define_insn "xenon_vpermwi128_<mode>"
  [(set (match_operand:VXENON_W 0 "vmx128_register_operand" "=wV")
	(unspec:VXENON_W
	  [(match_operand:VXENON_W 1 "vmx128_register_operand" "wV")
	   (match_operand:SI 2 "u8bit_cint_operand" "n")]
	  UNSPEC_XENON_VPERMWI))]
  "TARGET_VMX128"
  "vpermwi128 %0,%1,%2"
  [(set_attr "type" "vecperm")])

;; vpkd3d128 vD,vB,VD3D0,VD3D1,VD3D2 (vD tied): operand 1 is the old vD,
;; operand 2 the floats to pack, operands 3-5 the D3D type (3 bits), the
;; pack size (2 bits) and the word slot (2 bits).  A clamped value sets
;; VSCR[SAT] (FINDINGS 1.3), modelled as for the saturating AltiVec ops.
(define_insn "xenon_vpkd3d128_<mode>"
  [(set (match_operand:VXENON_W 0 "vmx128_register_operand" "=wV")
	(unspec:VXENON_W
	  [(match_operand:VXENON_W 1 "vmx128_register_operand" "0")
	   (match_operand:V4SF 2 "vmx128_register_operand" "wV")
	   (match_operand:SI 3 "const_0_to_7_operand" "n")
	   (match_operand:SI 4 "const_0_to_3_operand" "n")
	   (match_operand:SI 5 "const_0_to_3_operand" "n")]
	  UNSPEC_XENON_VPKD3D))
   (set (reg:SI VSCR_REGNO) (unspec:SI [(const_int 0)] UNSPEC_SET_VSCR))]
  "TARGET_VMX128"
  "vpkd3d128 %0,%2,%3,%4,%5"
  [(set_attr "type" "vecperm")])

;; vupkd3d128 vD,vB,UIMM: operand 2 is the D3D type (0-7); the instruction's
;; 5-bit UIMM is type << 2 (the semantics use UIMM >> 2 only).
(define_insn "xenon_vupkd3d128"
  [(set (match_operand:V4SF 0 "vmx128_register_operand" "=wV")
	(unspec:V4SF [(match_operand:V4SI 1 "vmx128_register_operand" "wV")
		      (match_operand:SI 2 "const_0_to_7_operand" "n")]
		     UNSPEC_XENON_VUPKD3D))]
  "TARGET_VMX128"
{
  rtx ops[3];
  ops[0] = operands[0];
  ops[1] = operands[1];
  ops[2] = GEN_INT (INTVAL (operands[2]) << 2);
  output_asm_insn ("vupkd3d128 %0,%1,%2", ops);
  return "";
}
  [(set_attr "type" "vecperm")])
