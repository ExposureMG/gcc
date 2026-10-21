;; Xbox 360 Xenon VMX128: prologue and epilogue patterns for the
;; callee-saved vector registers v64-v127 (plan 4.3).
;; Copyright (C) 2026 Free Software Foundation, Inc.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; This file is included last from rs6000.md, so that the insn codes of
;; every other pattern stay as they are.  The patterns are only used by
;; rs6000-logue.cc, and only with TARGET_VMX128.

;; Inline saves and restores.  stvx128 and lvx128 take indexed or indirect
;; addresses only, like stvx and lvx; the prologue and epilogue put the
;; offset in r0 (li 0,OFFSET; stvx128 vN,FRAME,0).  The unspec keeps these
;; apart from ordinary vector moves, as altivec_stvx_<mode>_internal and
;; altivec_lvx_<mode>_internal do for the AltiVec registers v20-v31; its
;; operand is 128 rather than their 0, so that those legacy patterns (which
;; come first and accept any register_operand, but only v0-v31 in their
;; constraints) never match these.
(define_insn "xenon_stvx128_v4si_internal"
  [(parallel
    [(set (match_operand:V4SI 0 "memory_operand" "=Z")
	  (match_operand:V4SI 1 "vmx128_register_operand" "wV"))
     (unspec [(const_int 128)] UNSPEC_STVX)])]
  "TARGET_VMX128"
  "stvx128 %1,%y0"
  [(set_attr "type" "vecstore")])

(define_insn "xenon_lvx128_v4si_internal"
  [(parallel
    [(set (match_operand:V4SI 0 "vmx128_register_operand" "=wV")
	  (match_operand:V4SI 1 "memory_operand" "Z"))
     (unspec [(const_int 128)] UNSPEC_LVX)])]
  "TARGET_VMX128"
  "lvx128 %0,%y1"
  [(set_attr "type" "vecload")])

;; Calls to the out-of-line libgcc routines _savevr128_N and _restvr128_N,
;; N = 64..127 (libgcc/config/rs6000/crtsavevr128.S, crtrestvr128.S).
;; Convention: r0 points just past the end of the v64-v127 save area (16-byte
;; aligned); the routine stores or loads vN..v127 at r0 - 16 * (128 - N) ..
;; r0 - 16, using r12 for the offsets, changes nothing else (LR is set by the
;; bl), and returns with blr.  The RTL describes the addresses relative to
;; r0, the real base; (plus r0 const) is not a valid vector address, so like
;; the save_vregs patterns in altivec.md these do not use memory_operand.
;; Only the first register store or load is matched; the parallel holds one
;; per register saved or restored.
(define_insn "*xenon_save_vregs128_<mode>"
  [(match_parallel 0 "any_parallel_operand"
     [(clobber (reg:P LR_REGNO))
      (use (match_operand:P 1 "symbol_ref_operand" "s"))
      (clobber (reg:P 12))
      (use (reg:P 0))
      (set (mem:V4SI (plus:P (reg:P 0)
			     (match_operand:P 2 "short_cint_operand" "I")))
	   (match_operand:V4SI 3 "vmx128_register_operand" "wV"))])]
  "TARGET_VMX128"
  "bl %1"
  [(set_attr "type" "branch")])

(define_insn "*xenon_restore_vregs128_<mode>"
  [(match_parallel 0 "any_parallel_operand"
     [(clobber (reg:P LR_REGNO))
      (use (match_operand:P 1 "symbol_ref_operand" "s"))
      (clobber (reg:P 12))
      (use (reg:P 0))
      (set (match_operand:V4SI 2 "vmx128_register_operand" "=wV")
	   (mem:V4SI (plus:P (reg:P 0)
			     (match_operand:P 3 "short_cint_operand" "I"))))])]
  "TARGET_VMX128"
  "bl %1"
  [(set_attr "type" "branch")])
