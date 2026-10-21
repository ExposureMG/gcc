;; Scheduling description for the Xbox 360 Xenon PPE (-mtune=xenon).
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

;; Sources: isa/FINDINGS.md sections 2-4 and isa/findings_instructions.csv
;; (hardware measurements of one console, cycles at 3.2 GHz).  Every number
;; below the BEGIN GENERATED marker comes from tools/gen_xenon_sched.py, which
;; also writes xenon-costs.def (the numbers rs6000.cc uses) and
;; tools/generated/xenon_timing.md (each number with its source and evidence
;; class).  Change numbers there, not here.
;;
;; Issue model (FINDINGS 2.1)
;; --------------------------
;; One thread issues up to two insns per cycle, in order.  Two insns of the
;; same pipe group never pair (2.0 cycles per pair); two of different groups
;; cost ~1.35 cycles per pair, never 1.0.  Modelled with:
;;   - one unit per group: xe_fxu (int, cmp), xe_cr (cmp, CR logical, CR
;;     moves), xe_vsu1 (FP, VMX FP, VMX integer/logical/compare), xe_vsu2
;;     (VMX permute/splat/merge/pack), xe_lsu (load issue; FP/VMX stores too), the
;;     load paths xe_lfd and xe_lvx, the store path xe_st (shared by every
;;     store: stw+stfd 7.05, stvx+stfd 7.18) and xe_br;
;;   - two slots; slot 0 must be taken before slot 1 in a cycle (in-order
;;     pairs); an insn issued in slot 1 reserves xe_nodual in the next cycle,
;;     which excludes slot 1 there.  So a stream of pairable insns issues
;;     2, 1, 2, 1 ... = 3 insns per 2 cycles = 1.33 cycles per pair;
;;   - order rule: FP/VMX in slot 0 then a GPR load in slot 1 does not pair
;;     (FP then lwz 2.04, VMX then lwz 2.56), the reverse does (1.35);
;;   - a GPR store pairs in slot 1 behind a non-store (X then stw 2.04) but
;;     not in front of one (stw then X 3.0); FP/VMX stores, FP/VMX loads and
;;     microcode that never pairs issue alone (xe_solo excludes slot 1 in the
;;     same cycle).
;; The issue-blocking singletons of FINDINGS 2.2 (div, mul, varshift, recimm,
;; fdiv, fsqrt) are modelled as the cycles they hold the units they block
;; (pipes.busy): mul, varshift and record forms hold xe_fxu; div holds xe_fxu
;; and xe_lsu; fdiv and fsqrt hold xe_vsu1.  Bypass quirks (address
;; generation, store data, lvx -> vperm control, compare -> branch, mtctr ->
;; bctr, load-hit-store) are in rs6000_xenon_adjust_cost.
;;
;; SMT (plan 5.4, FINDINGS 7): -mxenon-smt=idle|busy|same says what the
;; sibling hardware thread does; the xenon_smt attribute selects the matching
;; reservation of each class.  idle is the measured default (sibling in
;; libxenon's low-priority wait loop).  With a busy sibling the threads
;; alternate issue cycles, so throughput-bound code runs at about half the
;; rate (x1.91) while latencies stay (x1.00) except the integer ALU (2.42,
;; modelled as 3), mullw (x1.06) and divw (x1.05); a sibling running the same
;; code costs throughput x2.28 and latency x1.27.  The busy/same reservations
;; reserve xe_gap0/xe_gap1 in the cycle(s) after each issue cycle (no insn of
;; this thread issues there; it still pairs in its own cycles) and scale unit
;; occupancy and latencies; all numbers come from the generated table.
;; The gap is rnd (factor) - 1 = 1 cycle for both busy (x1.91) and same
;; (x2.28): a DFA cannot alternate 1- and 2-cycle gaps, so issue-bound
;; integer and mixed code is modelled about x1.9 slower under same too, a
;; 16 % shortfall against FINDINGS 7.  same differs from busy only in the
;; unit occupancy (x2.28 instead of x1.89: store streams x2.26) and the
;; latencies (x1.27); tools/xenon_vect_cost_check.py --section smt pins
;; exactly that per mode.
(define_attr "xenon_smt" "idle,busy,same"
  (const (symbol_ref "(enum attr_xenon_smt) rs6000_xenon_smt")))

;; Every unit sits in its own automaton so the long holds (div 28, fdiv 74,
;; sync 162 ...) do not multiply the state count; the slot units and their
;; presence rules share the "xenon" automaton.

(define_automaton "xenon,xenon_fxu,xenon_cr,xenon_vsu1,xenon_vsu2,xenon_lsu,\
xenon_lfd,xenon_lvx,xenon_st,xenon_br")

(define_cpu_unit "xe_slot0,xe_slot1,xe_nodual,xe_solo,xe_s0_vec,xe_s1_ld" "xenon")
;; -mxenon-smt=busy/same: cycles the sibling thread owns (the generated
;; xenon_smt<N>_issue* templates reserve them after each issue cycle).
(define_cpu_unit "xe_gap0,xe_gap1" "xenon")
(define_cpu_unit "xe_fxu" "xenon_fxu")
(define_cpu_unit "xe_cr" "xenon_cr")
(define_cpu_unit "xe_vsu1" "xenon_vsu1")
(define_cpu_unit "xe_vsu2" "xenon_vsu2")
(define_cpu_unit "xe_lsu" "xenon_lsu")
(define_cpu_unit "xe_lfd" "xenon_lfd")
(define_cpu_unit "xe_lvx" "xenon_lvx")
(define_cpu_unit "xe_st" "xenon_st")
(define_cpu_unit "xe_br" "xenon_br")

;; In-order pairs: slot 0 only while slot 1 is still free.
(absence_set "xe_slot0" "xe_slot1")
;; After a pair, the next cycle issues one insn; a solo insn owns its cycle.
(exclusion_set "xe_slot1" "xe_nodual,xe_solo")
;; FP/VMX in slot 0 keeps a GPR load out of slot 1.
(absence_set "xe_s1_ld" "xe_s0_vec")
;; Nothing of this thread issues in a cycle the sibling owns.
(exclusion_set "xe_gap0,xe_gap1" "xe_slot0,xe_slot1")

(define_reservation "xenon_issue" "(xe_slot0|(xe_slot1,xe_nodual))")
(define_reservation "xenon_issue_vec" "((xe_slot0+xe_s0_vec)|(xe_slot1,xe_nodual))")
(define_reservation "xenon_issue_ld" "(xe_slot0|((xe_slot1+xe_s1_ld),xe_nodual))")
(define_reservation "xenon_issue_solo" "xe_slot0+xe_solo")
(define_reservation "xenon_issue_st" "((xe_slot0+xe_solo)|(xe_slot1,xe_nodual))")

;; Loop-top padding (plan 5.3, FINDINGS 4): rs6000_xenon_reorg puts an aligned
;; label (LABEL_ALIGN = 2^3) and this 4-byte nop in front of each hot loop
;; label, so the loop top sits at byte 4 or 12 of its 16-byte fetch group.
;; The nop is a real insn, so branch shortening counts it.
(define_c_enum "unspecv" [UNSPECV_XENON_LOOP_PAD])

(define_insn "xenon_loop_pad"
  [(unspec_volatile [(const_int 0)] UNSPECV_XENON_LOOP_PAD)]
  ""
  "nop"
  [(set_attr "type" "integer")
   (set_attr "length" "4")])

;; Data layout (plan 5.3, FINDINGS 1.4): Xenon FP loads and stores (all
;; forms, also stfiwx) take an alignment interrupt unless EA % 4 == 0, while
;; integer loads and stores never trap.  SImode/DImode values can live in FPRs
;; (fctiwz/fctidz/fcfid operands), and a packed-struct field would then be
;; loaded or stored with lfd/stfd/stfiwx at its unaligned address.
;; rs6000_emit_move therefore moves a register to or from a MEM known to be
;; less than 4-byte aligned with these patterns, which only take GPRs (an FPR
;; value goes through an aligned stack slot first).
(define_c_enum "unspec" [UNSPEC_XENON_MISALIGNED])

(define_insn "xenon_misaligned_movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=m,r")
	(unspec:SI [(match_operand:SI 1 "nonimmediate_operand" "r,m")]
		   UNSPEC_XENON_MISALIGNED))]
  "rs6000_cpu == PROCESSOR_XENON
   && (gpc_reg_operand (operands[0], SImode)
       || gpc_reg_operand (operands[1], SImode))"
  "@
   stw%U0%X0 %1,%0
   lwz%U1%X1 %0,%1"
  [(set_attr "type" "store,load")])

(define_insn "xenon_misaligned_movdi"
  [(set (match_operand:DI 0 "nonimmediate_operand" "=YZ,r")
	(unspec:DI [(match_operand:DI 1 "nonimmediate_operand" "r,YZ")]
		   UNSPEC_XENON_MISALIGNED))]
  "rs6000_cpu == PROCESSOR_XENON && TARGET_POWERPC64
   && (gpc_reg_operand (operands[0], DImode)
       || gpc_reg_operand (operands[1], DImode))"
  "@
   std%U0%X0 %1,%0
   ld%U1%X1 %0,%1"
  [(set_attr "type" "store,load")])

;; Algebraic loads (lha lhax lwa lwax, 20-24 cycles, microcoded; FINDINGS
;; 2.3) are not formed before register allocation (lha_operand, lwa_operand);
;; one LRA creates by substituting a stack slot for a spilled pseudo is split
;; here into the zero-extending load and an extsh/extsw of the same register
;; (5 + 2 cycles).
(define_split
  [(set (match_operand:DI 0 "int_reg_operand")
	(sign_extend:DI (match_operand:SI 1 "memory_operand")))]
  "TARGET_POWERPC64 && reload_completed
   && rs6000_xenon_avoid_algebraic_load_p ()"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (sign_extend:DI (match_dup 2)))]
  "operands[2] = gen_rtx_REG (SImode, REGNO (operands[0]));")

(define_split
  [(set (match_operand:DI 0 "int_reg_operand")
	(sign_extend:DI (match_operand:HI 1 "memory_operand")))]
  "TARGET_POWERPC64 && reload_completed
   && rs6000_xenon_avoid_algebraic_load_p ()"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (sign_extend:DI (match_dup 2)))]
  "operands[2] = gen_rtx_REG (HImode, REGNO (operands[0]));")

(define_split
  [(set (match_operand:SI 0 "int_reg_operand")
	(sign_extend:SI (match_operand:HI 1 "memory_operand")))]
  "reload_completed && rs6000_xenon_avoid_algebraic_load_p ()"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (sign_extend:SI (match_dup 2)))]
  "operands[2] = gen_rtx_REG (HImode, REGNO (operands[0]));")

;; BEGIN GENERATED by tools/gen_xenon_sched.py -- do not edit by hand
;; Regenerate with:  python3 tools/gen_xenon_sched.py
;; Latency = default dependence latency; the multiplier on a unit = cycles
;; the class holds it (issue / throughput occupancy).  Sources and
;; evidence for every number: xenon-costs.def and tools/generated/xenon_timing.md.
;;
;; Each class has a reservation per -mxenon-smt mode (attribute xenon_smt,
;; modes with the same result share one): idle = the measured tables; busy
;; and same = the issue templates below (the sibling owns the cycles after
;; each issue cycle), unit occupancy and latencies scaled by FINDINGS 7.

;; Issue templates with 1 sibling cycle after each issue cycle (SMT_*_ISSUE_GAP).
(define_reservation "xenon_smt1_issue" "(xe_slot0,xe_gap0)|(xe_slot1,xe_gap1,xe_nodual)")
(define_reservation "xenon_smt1_issue_vec" "((xe_slot0+xe_s0_vec),xe_gap0)|(xe_slot1,xe_gap1,xe_nodual)")
(define_reservation "xenon_smt1_issue_ld" "(xe_slot0,xe_gap0)|((xe_slot1+xe_s1_ld),xe_gap1,xe_nodual)")
(define_reservation "xenon_smt1_issue_solo" "(xe_slot0+xe_solo),xe_gap0")
(define_reservation "xenon_smt1_issue_st" "((xe_slot0+xe_solo),xe_gap0)|(xe_slot1,xe_gap1,xe_nodual)")

;; xenon-int: add/logical/rotate-imm/shift-imm/cntlz/exts; isel and popcnt never occur for -mcpu=xenon
;;   latency LAT_INT = 2, issue ISS_INT = 1
(define_insn_reservation "xenon-int" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (ior (and (eq_attr "type" "integer")
                  (not (match_test "rs6000_xenon_mffs_p (insn)"))
                  (not (match_test "rs6000_xenon_mftb_p (insn)")))
            (eq_attr "type" "isel,popcnt")
            (and (eq_attr "type" "add,logical,insert,exts,cntlz")
                  (eq_attr "dot" "no"))
            (and (eq_attr "type" "shift")
                  (eq_attr "dot" "no")
                  (eq_attr "var_shift" "no"))))
  "xenon_issue+xe_fxu")
;;   -mxenon-smt=busy/same: latency 3, issue 2
(define_insn_reservation "xenon-int-smt-busy-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy,same")
       (ior (and (eq_attr "type" "integer")
                  (not (match_test "rs6000_xenon_mffs_p (insn)"))
                  (not (match_test "rs6000_xenon_mftb_p (insn)")))
            (eq_attr "type" "isel,popcnt")
            (and (eq_attr "type" "add,logical,insert,exts,cntlz")
                  (eq_attr "dot" "no"))
            (and (eq_attr "type" "shift")
                  (eq_attr "dot" "no")
                  (eq_attr "var_shift" "no"))))
  "xenon_smt1_issue+xe_fxu")

;; xenon-add-rec: add./subf./neg./addic.: not microcoded
;;   latency LAT_ADD_REC = 2, issue ISS_INT = 1
(define_insn_reservation "xenon-add-rec" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "add")
             (eq_attr "dot" "yes")))
  "xenon_issue+xe_fxu+xe_cr")
;;   -mxenon-smt=busy/same: latency 3, issue 2
(define_insn_reservation "xenon-add-rec-smt-busy-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy,same")
       (and (eq_attr "type" "add")
             (eq_attr "dot" "yes")))
  "xenon_smt1_issue+xe_fxu+xe_cr")

;; xenon-two: two dependent simple insns; Power10 fused pairs never occur for Xenon
;;   latency 2*LAT_INT = 4, issue 2*ISS_INT = 2
(define_insn_reservation "xenon-two" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "two,fused_arith_logical,fused_cmp_isel,fused_carry,fused_load_cmpi,fused_load_load,fused_store_store,fused_addis_load,fused_mtbc,fused_vector"))
  "xenon_issue+xe_fxu*2")
;;   -mxenon-smt=busy: latency 6, issue 4
(define_insn_reservation "xenon-two-smt-busy" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "two,fused_arith_logical,fused_cmp_isel,fused_carry,fused_load_cmpi,fused_load_load,fused_store_store,fused_addis_load,fused_mtbc,fused_vector"))
  "xenon_smt1_issue+xe_fxu*4")
;;   -mxenon-smt=same: latency 6, issue 5
(define_insn_reservation "xenon-two-smt-same" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "two,fused_arith_logical,fused_cmp_isel,fused_carry,fused_load_cmpi,fused_load_load,fused_store_store,fused_addis_load,fused_mtbc,fused_vector"))
  "xenon_smt1_issue+xe_fxu*5")

;; xenon-three: three simple insns
;;   latency 3*LAT_INT = 6, issue 3*ISS_INT = 3
(define_insn_reservation "xenon-three" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "three"))
  "xenon_issue+xe_fxu*3")
;;   -mxenon-smt=busy: latency 9, issue 6
(define_insn_reservation "xenon-three-smt-busy" 9
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "three"))
  "xenon_smt1_issue+xe_fxu*6")
;;   -mxenon-smt=same: latency 9, issue 7
(define_insn_reservation "xenon-three-smt-same" 9
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "three"))
  "xenon_smt1_issue+xe_fxu*7")

;; xenon-cmp: cmp conflicts with int and crlogic (pair matrix)
;;   latency LAT_CMP = 1, issue ISS_CMP = 1
(define_insn_reservation "xenon-cmp" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "cmp"))
  "xenon_issue+xe_fxu+xe_cr")
;;   -mxenon-smt=busy/same: latency 1, issue 2
(define_insn_reservation "xenon-cmp-smt-busy-same" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy,same")
       (eq_attr "type" "cmp"))
  "xenon_smt1_issue+xe_fxu+xe_cr")

;; xenon-crlogical: crlogic pairs with int (1.35) but not with cmp (2.03)
;;   latency LAT_CRLOGIC = 1, issue ISS_INT = 1
(define_insn_reservation "xenon-crlogical" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "cr_logical"))
  "xenon_issue+xe_cr")
;;   -mxenon-smt=busy/same: latency 1, issue 2
(define_insn_reservation "xenon-crlogical-smt-busy-same" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy,same")
       (eq_attr "type" "cr_logical"))
  "xenon_smt1_issue+xe_cr")

;; xenon-rec: logical/rotate/shift-imm/cntlz Rc=1, andi., andis. (recimm: blocks int 15.7)
;;   latency LAT_REC = 14, issue ISS_REC = 14, microcoded
(define_insn_reservation "xenon-rec" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (ior (and (eq_attr "type" "logical,insert,cntlz")
                  (eq_attr "dot" "yes"))
            (and (eq_attr "type" "shift")
                  (eq_attr "dot" "yes")
                  (eq_attr "var_shift" "no"))))
  "xenon_issue+xe_fxu*14")
;;   -mxenon-smt=busy: latency 14, issue 26
(define_insn_reservation "xenon-rec-smt-busy" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (ior (and (eq_attr "type" "logical,insert,cntlz")
                  (eq_attr "dot" "yes"))
            (and (eq_attr "type" "shift")
                  (eq_attr "dot" "yes")
                  (eq_attr "var_shift" "no"))))
  "xenon_smt1_issue+xe_fxu*26")
;;   -mxenon-smt=same: latency 18, issue 32
(define_insn_reservation "xenon-rec-smt-same" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (ior (and (eq_attr "type" "logical,insert,cntlz")
                  (eq_attr "dot" "yes"))
            (and (eq_attr "type" "shift")
                  (eq_attr "dot" "yes")
                  (eq_attr "var_shift" "no"))))
  "xenon_smt1_issue+xe_fxu*32")

;; xenon-exts-rec: extsb./extsh./extsw.
;;   latency LAT_EXTS_REC = 16, issue ISS_EXTS_REC = 16, microcoded
(define_insn_reservation "xenon-exts-rec" 16
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "exts")
             (eq_attr "dot" "yes")))
  "xenon_issue+xe_fxu*16")
;;   -mxenon-smt=busy: latency 16, issue 30
(define_insn_reservation "xenon-exts-rec-smt-busy" 16
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "exts")
             (eq_attr "dot" "yes")))
  "xenon_smt1_issue+xe_fxu*30")
;;   -mxenon-smt=same: latency 20, issue 36
(define_insn_reservation "xenon-exts-rec-smt-same" 20
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "exts")
             (eq_attr "dot" "yes")))
  "xenon_smt1_issue+xe_fxu*36")

;; xenon-varshift: slw/srw/sraw/sld/srd/srad/rlwnm/rldcl/rldcr (blocks int 17.1, mul 15.9)
;;   latency LAT_VARSHIFT = 16, issue ISS_VARSHIFT = 16, microcoded
(define_insn_reservation "xenon-varshift" 16
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "shift")
             (eq_attr "dot" "no")
             (eq_attr "var_shift" "yes")))
  "xenon_issue+xe_fxu*16")
;;   -mxenon-smt=busy: latency 16, issue 30
(define_insn_reservation "xenon-varshift-smt-busy" 16
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "shift")
             (eq_attr "dot" "no")
             (eq_attr "var_shift" "yes")))
  "xenon_smt1_issue+xe_fxu*30")
;;   -mxenon-smt=same: latency 20, issue 36
(define_insn_reservation "xenon-varshift-smt-same" 20
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "shift")
             (eq_attr "dot" "no")
             (eq_attr "var_shift" "yes")))
  "xenon_smt1_issue+xe_fxu*36")

;; xenon-varshift-rec: variable shifts Rc=1
;;   latency LAT_VARSHIFT_REC = 18, issue ISS_VARSHIFT_REC = 18, microcoded
(define_insn_reservation "xenon-varshift-rec" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "shift")
             (eq_attr "dot" "yes")
             (eq_attr "var_shift" "yes")))
  "xenon_issue+xe_fxu*18")
;;   -mxenon-smt=busy: latency 18, issue 34
(define_insn_reservation "xenon-varshift-rec-smt-busy" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "shift")
             (eq_attr "dot" "yes")
             (eq_attr "var_shift" "yes")))
  "xenon_smt1_issue+xe_fxu*34")
;;   -mxenon-smt=same: latency 23, issue 41
(define_insn_reservation "xenon-varshift-rec-smt-same" 23
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "shift")
             (eq_attr "dot" "yes")
             (eq_attr "var_shift" "yes")))
  "xenon_smt1_issue+xe_fxu*41")

;; xenon-mulli: mulli (mul blocks int 10.8, div 10.0)
;;   latency LAT_MULLI = 8, issue ISS_MULLI = 7, microcoded
(define_insn_reservation "xenon-mulli" 8
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "mul")
             (eq_attr "size" "8,16")))
  "xenon_issue+xe_fxu*7")
;;   -mxenon-smt=busy: latency 9, issue 9
(define_insn_reservation "xenon-mulli-smt-busy" 9
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "mul")
             (eq_attr "size" "8,16")))
  "xenon_smt1_issue+xe_fxu*9")
;;   -mxenon-smt=same: latency 10, issue 16
(define_insn_reservation "xenon-mulli-smt-same" 10
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "mul")
             (eq_attr "size" "8,16")))
  "xenon_smt1_issue+xe_fxu*16")

;; xenon-mulw: mullw, mulhw[u] (unpipelined)
;;   latency LAT_MULW = 11, issue ISS_MULW = 11, microcoded
(define_insn_reservation "xenon-mulw" 11
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (ior (and (eq_attr "type" "mul")
                  (eq_attr "dot" "no")
                  (eq_attr "size" "32"))
            (eq_attr "type" "halfmul")))
  "xenon_issue+xe_fxu*11")
;;   -mxenon-smt=busy: latency 12, issue 12
(define_insn_reservation "xenon-mulw-smt-busy" 12
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (ior (and (eq_attr "type" "mul")
                  (eq_attr "dot" "no")
                  (eq_attr "size" "32"))
            (eq_attr "type" "halfmul")))
  "xenon_smt1_issue+xe_fxu*12")
;;   -mxenon-smt=same: latency 14, issue 25
(define_insn_reservation "xenon-mulw-smt-same" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (ior (and (eq_attr "type" "mul")
                  (eq_attr "dot" "no")
                  (eq_attr "size" "32"))
            (eq_attr "type" "halfmul")))
  "xenon_smt1_issue+xe_fxu*25")

;; xenon-muld: mulld, mulhd[u]
;;   latency LAT_MULD = 14, issue ISS_MULD = 13, microcoded
(define_insn_reservation "xenon-muld" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "no")
             (eq_attr "size" "64,128,256")))
  "xenon_issue+xe_fxu*13")
;;   -mxenon-smt=busy: latency 15, issue 15
(define_insn_reservation "xenon-muld-smt-busy" 15
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "no")
             (eq_attr "size" "64,128,256")))
  "xenon_smt1_issue+xe_fxu*15")
;;   -mxenon-smt=same: latency 18, issue 30
(define_insn_reservation "xenon-muld-smt-same" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "no")
             (eq_attr "size" "64,128,256")))
  "xenon_smt1_issue+xe_fxu*30")

;; xenon-mulw-rec: mullw., mulhw.
;;   latency LAT_MULW_REC = 24, issue ISS_MULW_REC = 24, microcoded
(define_insn_reservation "xenon-mulw-rec" 24
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "yes")
             (eq_attr "size" "32")))
  "xenon_issue+xe_fxu*24")
;;   -mxenon-smt=busy: latency 26, issue 26
(define_insn_reservation "xenon-mulw-rec-smt-busy" 26
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "yes")
             (eq_attr "size" "32")))
  "xenon_smt1_issue+xe_fxu*26")
;;   -mxenon-smt=same: latency 30, issue 55
(define_insn_reservation "xenon-mulw-rec-smt-same" 30
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "yes")
             (eq_attr "size" "32")))
  "xenon_smt1_issue+xe_fxu*55")

;; xenon-muld-rec: mulld., mulhd.
;;   latency LAT_MULD_REC = 26, issue ISS_MULD_REC = 26, microcoded
(define_insn_reservation "xenon-muld-rec" 26
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "yes")
             (eq_attr "size" "64,128,256")))
  "xenon_issue+xe_fxu*26")
;;   -mxenon-smt=busy: latency 28, issue 28
(define_insn_reservation "xenon-muld-rec-smt-busy" 28
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "yes")
             (eq_attr "size" "64,128,256")))
  "xenon_smt1_issue+xe_fxu*28")
;;   -mxenon-smt=same: latency 33, issue 59
(define_insn_reservation "xenon-muld-rec-smt-same" 33
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "mul")
             (eq_attr "dot" "yes")
             (eq_attr "size" "64,128,256")))
  "xenon_smt1_issue+xe_fxu*59")

;; xenon-divw: divw[u] (blocks int 31.5, lwz 30.7, mul 29.3)
;;   latency LAT_DIVW = 18, issue ISS_DIVW = 28, microcoded
(define_insn_reservation "xenon-divw" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "no")
             (eq_attr "size" "8,16,32")))
  "xenon_issue+xe_fxu*28+xe_lsu*28")
;;   -mxenon-smt=busy: latency 19, issue 28
(define_insn_reservation "xenon-divw-smt-busy" 19
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "no")
             (eq_attr "size" "8,16,32")))
  "xenon_smt1_issue+xe_fxu*28+xe_lsu*28")
;;   -mxenon-smt=same: latency 23, issue 64
(define_insn_reservation "xenon-divw-smt-same" 23
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "no")
             (eq_attr "size" "8,16,32")))
  "xenon_smt1_issue+xe_fxu*64+xe_lsu*64")

;; xenon-divd: divd[u]
;;   latency LAT_DIVD = 18, issue ISS_DIVD = 28, microcoded
(define_insn_reservation "xenon-divd" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "no")
             (eq_attr "size" "64,128,256")))
  "xenon_issue+xe_fxu*28+xe_lsu*28")
;;   -mxenon-smt=busy: latency 19, issue 28
(define_insn_reservation "xenon-divd-smt-busy" 19
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "no")
             (eq_attr "size" "64,128,256")))
  "xenon_smt1_issue+xe_fxu*28+xe_lsu*28")
;;   -mxenon-smt=same: latency 23, issue 64
(define_insn_reservation "xenon-divd-smt-same" 23
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "no")
             (eq_attr "size" "64,128,256")))
  "xenon_smt1_issue+xe_fxu*64+xe_lsu*64")

;; xenon-div-rec: divide Rc=1
;;   latency LAT_DIV_REC = 30, issue ISS_DIV_REC = 40, microcoded
(define_insn_reservation "xenon-div-rec" 30
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "yes")))
  "xenon_issue+xe_fxu*40+xe_lsu*40")
;;   -mxenon-smt=busy: latency 32, issue 40
(define_insn_reservation "xenon-div-rec-smt-busy" 32
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "yes")))
  "xenon_smt1_issue+xe_fxu*40+xe_lsu*40")
;;   -mxenon-smt=same: latency 38, issue 91
(define_insn_reservation "xenon-div-rec-smt-same" 38
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "div")
             (eq_attr "dot" "yes")))
  "xenon_smt1_issue+xe_fxu*91+xe_lsu*91")

;; xenon-load: lwz lbz lhz ld (+x, brx)
;;   latency LAT_LOAD = 5, issue ISS_LOAD = 1
(define_insn_reservation "xenon-load" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "no")
             (eq_attr "update" "no")))
  "xenon_issue_ld+xe_lsu")
;;   -mxenon-smt=busy: latency 5, issue 2
(define_insn_reservation "xenon-load-smt-busy" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "no")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_ld+xe_lsu")
;;   -mxenon-smt=same: latency 6, issue 2
(define_insn_reservation "xenon-load-smt-same" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "no")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_ld+xe_lsu")

;; xenon-load-update: lwzu lbzu lhzu ldu (+x)
;;   latency LAT_LOAD = 5, issue ISS_LOAD_UPDATE = 2
(define_insn_reservation "xenon-load-update" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "no")
             (eq_attr "update" "yes")))
  "xenon_issue_ld+xe_lsu*2+xe_fxu")
;;   -mxenon-smt=busy: latency 5, issue 4
(define_insn_reservation "xenon-load-update-smt-busy" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "no")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_ld+xe_lsu*4+xe_fxu")
;;   -mxenon-smt=same: latency 6, issue 5
(define_insn_reservation "xenon-load-update-smt-same" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "no")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_ld+xe_lsu*5+xe_fxu")

;; xenon-load-ext: lha lhau lhax lhaux lwa lwax lwaux (microcoded; avoided for Xenon)
;;   latency LAT_LOAD_EXT = 23, issue ISS_LOAD_EXT = 24, microcoded
(define_insn_reservation "xenon-load-ext" 23
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "yes")))
  "xenon_issue_solo+xe_lsu*24+xe_fxu*24")
;;   -mxenon-smt=busy: latency 23, issue 45
(define_insn_reservation "xenon-load-ext-smt-busy" 23
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "yes")))
  "xenon_smt1_issue_solo+xe_lsu*45+xe_fxu*45")
;;   -mxenon-smt=same: latency 29, issue 55
(define_insn_reservation "xenon-load-ext-smt-same" 29
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "load")
             (eq_attr "sign_extend" "yes")))
  "xenon_smt1_issue_solo+xe_lsu*55+xe_fxu*55")

;; xenon-store: stw stb sth std (+x, brx); pairs behind a non-store (X then stw 2.04, also lwz then stw), not in front of one (stw then X 3.0); all stores share xe_st (stw+stw 5.81, stw+stfd 7.05)
;;   latency LAT_INT = 2, issue ISS_STORE = 3
(define_insn_reservation "xenon-store" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "store")
             (eq_attr "update" "no")))
  "xenon_issue_st+xe_st*3")
;;   -mxenon-smt=busy: latency 3, issue 6
(define_insn_reservation "xenon-store-smt-busy" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "store")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_st+xe_st*6")
;;   -mxenon-smt=same: latency 3, issue 7
(define_insn_reservation "xenon-store-smt-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "store")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_st+xe_st*7")

;; xenon-store-update: stwu stbu sthu stdu (+x); latency = base register update
;;   latency LAT_INT = 2, issue ISS_STORE_UPDATE = 3
(define_insn_reservation "xenon-store-update" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "store")
             (eq_attr "update" "yes")))
  "xenon_issue_st+xe_st*3+xe_fxu")
;;   -mxenon-smt=busy: latency 3, issue 6
(define_insn_reservation "xenon-store-update-smt-busy" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "store")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_st+xe_st*6+xe_fxu")
;;   -mxenon-smt=same: latency 3, issue 7
(define_insn_reservation "xenon-store-update-smt-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "store")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_st+xe_st*7+xe_fxu")

;; xenon-fpload: lfd lfs (+x); lfd with int/FP 3.3, lfd+lfd 5.84
;;   latency LAT_FPLOAD = 5, issue ISS_FPLOAD = 3
(define_insn_reservation "xenon-fpload" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "fpload")
             (eq_attr "update" "no")))
  "xenon_issue_solo+xe_lsu+xe_lfd*3")
;;   -mxenon-smt=busy: latency 5, issue 6
(define_insn_reservation "xenon-fpload-smt-busy" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "fpload")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_solo+xe_lsu+xe_lfd*6")
;;   -mxenon-smt=same: latency 6, issue 7
(define_insn_reservation "xenon-fpload-smt-same" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "fpload")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_solo+xe_lsu+xe_lfd*7")

;; xenon-fpload-update: lfdu lfsu (+x): rotating-destination throughput 2.94 (same-register stream 10.7)
;;   latency LAT_FPLOAD = 5, issue ISS_FPLOAD_UPDATE = 3
(define_insn_reservation "xenon-fpload-update" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "fpload")
             (eq_attr "update" "yes")))
  "xenon_issue_solo+xe_lsu+xe_lfd*3+xe_fxu")
;;   -mxenon-smt=busy: latency 5, issue 6
(define_insn_reservation "xenon-fpload-update-smt-busy" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "fpload")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_solo+xe_lsu+xe_lfd*6+xe_fxu")
;;   -mxenon-smt=same: latency 6, issue 7
(define_insn_reservation "xenon-fpload-update-smt-same" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "fpload")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_solo+xe_lsu+xe_lfd*7+xe_fxu")

;; xenon-fpstore: stfd stfs stfiwx (+x); stfd with anything 3.8-4.0; shares xe_st with every store
;;   latency LAT_INT = 2, issue ISS_FPSTORE = 4
(define_insn_reservation "xenon-fpstore" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "fpstore")
             (eq_attr "update" "no")))
  "xenon_issue_solo+xe_lsu+xe_st*4")
;;   -mxenon-smt=busy: latency 3, issue 8
(define_insn_reservation "xenon-fpstore-smt-busy" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "fpstore")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_solo+xe_lsu+xe_st*8")
;;   -mxenon-smt=same: latency 3, issue 9
(define_insn_reservation "xenon-fpstore-smt-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "fpstore")
             (eq_attr "update" "no")))
  "xenon_smt1_issue_solo+xe_lsu+xe_st*9")

;; xenon-fpstore-update: stfdu stfsu (+x)
;;   latency LAT_INT = 2, issue ISS_FPSTORE_UPDATE = 4
(define_insn_reservation "xenon-fpstore-update" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "fpstore")
             (eq_attr "update" "yes")))
  "xenon_issue_solo+xe_lsu+xe_st*4+xe_fxu")
;;   -mxenon-smt=busy: latency 3, issue 8
(define_insn_reservation "xenon-fpstore-update-smt-busy" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "fpstore")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_solo+xe_lsu+xe_st*8+xe_fxu")
;;   -mxenon-smt=same: latency 3, issue 9
(define_insn_reservation "xenon-fpstore-update-smt-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "fpstore")
             (eq_attr "update" "yes")))
  "xenon_smt1_issue_solo+xe_lsu+xe_st*9+xe_fxu")

;; xenon-vecload: lvx lvxl lvebx lvehx lvewx lvlx lvrx (+128)
;;   latency LAT_VECLOAD = 5, issue ISS_VECLOAD = 3
(define_insn_reservation "xenon-vecload" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "vecload")
             (not (match_test "rs6000_xenon_lvsl_p (insn)"))))
  "xenon_issue_solo+xe_lsu+xe_lvx*3")
;;   -mxenon-smt=busy: latency 5, issue 6
(define_insn_reservation "xenon-vecload-smt-busy" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "vecload")
             (not (match_test "rs6000_xenon_lvsl_p (insn)"))))
  "xenon_smt1_issue_solo+xe_lsu+xe_lvx*6")
;;   -mxenon-smt=same: latency 6, issue 7
(define_insn_reservation "xenon-vecload-smt-same" 6
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "vecload")
             (not (match_test "rs6000_xenon_lvsl_p (insn)"))))
  "xenon_smt1_issue_solo+xe_lsu+xe_lvx*7")

;; xenon-lvsl: lvsl lvsr: no memory access (FINDINGS 1.4), VMX pipe (CSV unit vector); not an lvx for the lvx -> vperm control extra
;;   latency LAT_LVSL = 4, issue ISS_LVSL = 3
(define_insn_reservation "xenon-lvsl" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "vecload")
             (match_test "rs6000_xenon_lvsl_p (insn)")))
  "xenon_issue_vec+xe_vsu2*3")
;;   -mxenon-smt=busy: latency 4, issue 6
(define_insn_reservation "xenon-lvsl-smt-busy" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "vecload")
             (match_test "rs6000_xenon_lvsl_p (insn)")))
  "xenon_smt1_issue_vec+xe_vsu2*6")
;;   -mxenon-smt=same: latency 5, issue 7
(define_insn_reservation "xenon-lvsl-smt-same" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "vecload")
             (match_test "rs6000_xenon_lvsl_p (insn)")))
  "xenon_smt1_issue_vec+xe_vsu2*7")

;; xenon-vecstore: stvx stvxl stve*x stvlx stvrx (+128); shares xe_st (stvx then stfd 7.18)
;;   latency LAT_INT = 2, issue ISS_VECSTORE = 3
(define_insn_reservation "xenon-vecstore" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "vecstore"))
  "xenon_issue_solo+xe_lsu+xe_st*3")
;;   -mxenon-smt=busy: latency 3, issue 6
(define_insn_reservation "xenon-vecstore-smt-busy" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "vecstore"))
  "xenon_smt1_issue_solo+xe_lsu+xe_st*6")
;;   -mxenon-smt=same: latency 3, issue 7
(define_insn_reservation "xenon-vecstore-smt-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "vecstore"))
  "xenon_smt1_issue_solo+xe_lsu+xe_st*7")

;; xenon-branch: b bc bctr blr tw td
;;   latency ISS_BRANCH = 1, issue ISS_BRANCH = 1
(define_insn_reservation "xenon-branch" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "branch,jmpreg,trap"))
  "xenon_issue+xe_br")
;;   -mxenon-smt=busy/same: latency 1, issue 2
(define_insn_reservation "xenon-branch-smt-busy-same" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy,same")
       (eq_attr "type" "branch,jmpreg,trap"))
  "xenon_smt1_issue+xe_br")

;; xenon-mfjmpr: mflr mfctr (mfspr 3.01, microcoded)
;;   latency ISS_MFSPR = 3, issue ISS_MFSPR = 3, microcoded
(define_insn_reservation "xenon-mfjmpr" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "mfjmpr"))
  "xenon_issue+xe_fxu*3")
;;   -mxenon-smt=busy: latency 3, issue 6
(define_insn_reservation "xenon-mfjmpr-smt-busy" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "mfjmpr"))
  "xenon_smt1_issue+xe_fxu*6")
;;   -mxenon-smt=same: latency 4, issue 7
(define_insn_reservation "xenon-mfjmpr-smt-same" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "mfjmpr"))
  "xenon_smt1_issue+xe_fxu*7")

;; xenon-mtjmpr: mtlr mtctr (mtspr 1.02; behaves like int)
;;   latency ISS_MTSPR = 1, issue ISS_MTSPR = 1
(define_insn_reservation "xenon-mtjmpr" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "mtjmpr"))
  "xenon_issue+xe_fxu")
;;   -mxenon-smt=busy/same: latency 1, issue 2
(define_insn_reservation "xenon-mtjmpr-smt-busy-same" 1
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy,same")
       (eq_attr "type" "mtjmpr"))
  "xenon_smt1_issue+xe_fxu")

;; xenon-mfcr: mfcr, mfocrf (any FXM)
;;   latency ISS_MFCR = 26, issue ISS_MFCR = 26, microcoded
(define_insn_reservation "xenon-mfcr" 26
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "mfcr,mfcrf"))
  "xenon_issue_solo+xe_fxu*26+xe_cr")
;;   -mxenon-smt=busy: latency 26, issue 49
(define_insn_reservation "xenon-mfcr-smt-busy" 26
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "mfcr,mfcrf"))
  "xenon_smt1_issue_solo+xe_fxu*49+xe_cr")
;;   -mxenon-smt=same: latency 33, issue 59
(define_insn_reservation "xenon-mfcr-smt-same" 33
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "mfcr,mfcrf"))
  "xenon_smt1_issue_solo+xe_fxu*59+xe_cr")

;; xenon-mtcrf: mtcrf (several fields)
;;   latency LAT_MTCRF = 20, issue ISS_MTCRF = 20, microcoded
(define_insn_reservation "xenon-mtcrf" 20
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "mtcr")
             (match_test "rs6000_xenon_mtcrf_multi_p (insn)")))
  "xenon_issue+xe_fxu*20+xe_cr*20")
;;   -mxenon-smt=busy: latency 20, issue 38
(define_insn_reservation "xenon-mtcrf-smt-busy" 20
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "mtcr")
             (match_test "rs6000_xenon_mtcrf_multi_p (insn)")))
  "xenon_smt1_issue+xe_fxu*38+xe_cr*38")
;;   -mxenon-smt=same: latency 25, issue 46
(define_insn_reservation "xenon-mtcrf-smt-same" 25
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "mtcr")
             (match_test "rs6000_xenon_mtcrf_multi_p (insn)")))
  "xenon_smt1_issue+xe_fxu*46+xe_cr*46")

;; xenon-mtocrf: mtocrf (one field)
;;   latency LAT_MTOCRF = 2, issue ISS_MTOCRF = 1
(define_insn_reservation "xenon-mtocrf" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "mtcr")
             (not (match_test "rs6000_xenon_mtcrf_multi_p (insn)"))))
  "xenon_issue+xe_cr")
;;   -mxenon-smt=busy: latency 2, issue 2
(define_insn_reservation "xenon-mtocrf-smt-busy" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "mtcr")
             (not (match_test "rs6000_xenon_mtcrf_multi_p (insn)"))))
  "xenon_smt1_issue+xe_cr")
;;   -mxenon-smt=same: latency 3, issue 2
(define_insn_reservation "xenon-mtocrf-smt-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "mtcr")
             (not (match_test "rs6000_xenon_mtcrf_multi_p (insn)"))))
  "xenon_smt1_issue+xe_cr")

;; xenon-isync: isync
;;   latency ISS_ISYNC = 34, issue ISS_ISYNC = 34, microcoded
(define_insn_reservation "xenon-isync" 34
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "isync"))
  "xenon_issue_solo+xe_lsu*34+xe_fxu*34")
;;   -mxenon-smt=busy: latency 34, issue 64
(define_insn_reservation "xenon-isync-smt-busy" 34
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "isync"))
  "xenon_smt1_issue_solo+xe_lsu*64+xe_fxu*64")
;;   -mxenon-smt=same: latency 43, issue 78
(define_insn_reservation "xenon-isync-smt-same" 43
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "isync"))
  "xenon_smt1_issue_solo+xe_lsu*78+xe_fxu*78")

;; xenon-lwsync: lwsync, eieio-class barriers
;;   latency ISS_LWSYNC = 32, issue ISS_LWSYNC = 32, microcoded
(define_insn_reservation "xenon-lwsync" 32
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "sync")
             (not (match_test "rs6000_xenon_hwsync_p (insn)"))))
  "xenon_issue_solo+xe_lsu*32")
;;   -mxenon-smt=busy: latency 32, issue 60
(define_insn_reservation "xenon-lwsync-smt-busy" 32
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "sync")
             (not (match_test "rs6000_xenon_hwsync_p (insn)"))))
  "xenon_smt1_issue_solo+xe_lsu*60")
;;   -mxenon-smt=same: latency 41, issue 73
(define_insn_reservation "xenon-lwsync-smt-same" 41
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "sync")
             (not (match_test "rs6000_xenon_hwsync_p (insn)"))))
  "xenon_smt1_issue_solo+xe_lsu*73")

;; xenon-hwsync: sync (= ptesync)
;;   latency ISS_HWSYNC = 162, issue ISS_HWSYNC = 162, microcoded
(define_insn_reservation "xenon-hwsync" 162
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "sync")
             (match_test "rs6000_xenon_hwsync_p (insn)")))
  "xenon_issue_solo+xe_lsu*162")
;;   -mxenon-smt=busy: latency 162, issue 306
(define_insn_reservation "xenon-hwsync-smt-busy" 162
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "sync")
             (match_test "rs6000_xenon_hwsync_p (insn)")))
  "xenon_smt1_issue_solo+xe_lsu*306")
;;   -mxenon-smt=same: latency 206, issue 369
(define_insn_reservation "xenon-hwsync-smt-same" 206
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "sync")
             (match_test "rs6000_xenon_hwsync_p (insn)")))
  "xenon_smt1_issue_solo+xe_lsu*369")

;; xenon-larx: lwarx ldarx
;;   latency LAT_LARX = 74, issue ISS_LARX = 65, microcoded
(define_insn_reservation "xenon-larx" 74
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "load_l"))
  "xenon_issue_solo+xe_lsu*65")
;;   -mxenon-smt=busy: latency 74, issue 123
(define_insn_reservation "xenon-larx-smt-busy" 74
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "load_l"))
  "xenon_smt1_issue_solo+xe_lsu*123")
;;   -mxenon-smt=same: latency 94, issue 148
(define_insn_reservation "xenon-larx-smt-same" 94
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "load_l"))
  "xenon_smt1_issue_solo+xe_lsu*148")

;; xenon-stcx: stwcx. stdcx.
;;   latency ISS_STCX = 80, issue ISS_STCX = 80, microcoded
(define_insn_reservation "xenon-stcx" 80
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "store_c"))
  "xenon_issue_solo+xe_lsu*80")
;;   -mxenon-smt=busy: latency 80, issue 151
(define_insn_reservation "xenon-stcx-smt-busy" 80
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "store_c"))
  "xenon_smt1_issue_solo+xe_lsu*151")
;;   -mxenon-smt=same: latency 102, issue 182
(define_insn_reservation "xenon-stcx-smt-same" 102
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "store_c"))
  "xenon_smt1_issue_solo+xe_lsu*182")

;; xenon-fp: FP arith/FMA/fsel/fmr/frsp/fres/frsqrte/fcti*/fcfid; qmul/dfp/vecdouble never occur for Xenon
;;   latency LAT_FP = 10, issue ISS_FP = 1
(define_insn_reservation "xenon-fp" 10
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "fp,fpsimple,dmul,qmul,dfp,vecdouble"))
  "xenon_issue_vec+xe_vsu1")
;;   -mxenon-smt=busy: latency 10, issue 2
(define_insn_reservation "xenon-fp-smt-busy" 10
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "fp,fpsimple,dmul,qmul,dfp,vecdouble"))
  "xenon_smt1_issue_vec+xe_vsu1")
;;   -mxenon-smt=same: latency 13, issue 2
(define_insn_reservation "xenon-fp-smt-same" 13
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "fp,fpsimple,dmul,qmul,dfp,vecdouble"))
  "xenon_smt1_issue_vec+xe_vsu1")

;; xenon-fpcompare: fcmpu fcmpo
;;   latency LAT_FPCMP = 12, issue ISS_FPCMP = 16, microcoded
(define_insn_reservation "xenon-fpcompare" 12
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "fpcompare"))
  "xenon_issue_solo+xe_vsu1*16")
;;   -mxenon-smt=busy: latency 12, issue 30
(define_insn_reservation "xenon-fpcompare-smt-busy" 12
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "fpcompare"))
  "xenon_smt1_issue_solo+xe_vsu1*30")
;;   -mxenon-smt=same: latency 15, issue 36
(define_insn_reservation "xenon-fpcompare-smt-same" 15
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "fpcompare"))
  "xenon_smt1_issue_solo+xe_vsu1*36")

;; xenon-fdiv: fdiv fdivs (blocks fp/vfp 81.9); vecdiv/vecfdiv are VSX-only
;;   latency LAT_FDIV = 74, issue ISS_FDIV = 74
(define_insn_reservation "xenon-fdiv" 74
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "sdiv,ddiv,vecdiv,vecfdiv"))
  "xenon_issue_vec+xe_vsu1*74")
;;   -mxenon-smt=busy: latency 74, issue 140
(define_insn_reservation "xenon-fdiv-smt-busy" 74
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "sdiv,ddiv,vecdiv,vecfdiv"))
  "xenon_smt1_issue_vec+xe_vsu1*140")
;;   -mxenon-smt=same: latency 94, issue 169
(define_insn_reservation "xenon-fdiv-smt-same" 94
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "sdiv,ddiv,vecdiv,vecfdiv"))
  "xenon_smt1_issue_vec+xe_vsu1*169")

;; xenon-fsqrt: fsqrt fsqrts (blocks fp 91, fdiv 84)
;;   latency LAT_FSQRT = 84, issue ISS_FSQRT = 84
(define_insn_reservation "xenon-fsqrt" 84
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "ssqrt,dsqrt"))
  "xenon_issue_vec+xe_vsu1*84")
;;   -mxenon-smt=busy: latency 84, issue 159
(define_insn_reservation "xenon-fsqrt-smt-busy" 84
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "ssqrt,dsqrt"))
  "xenon_smt1_issue_vec+xe_vsu1*159")
;;   -mxenon-smt=same: latency 107, issue 192
(define_insn_reservation "xenon-fsqrt-smt-same" 107
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "ssqrt,dsqrt"))
  "xenon_smt1_issue_vec+xe_vsu1*192")

;; xenon-vecsimple: VMX integer/logical/compare, vmaxfp/vminfp (vint/vlogic share the FP pipe)
;;   latency LAT_VSIMPLE = 4, issue ISS_VSIMPLE = 1
(define_insn_reservation "xenon-vecsimple" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (ior (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
                  (not (match_test "rs6000_xenon_vscr_move_p (insn)")))
            (and (eq_attr "type" "veccmp,veccmpfx")
                  (not (match_test "rs6000_xenon_vec_predicate_p (insn)")))))
  "xenon_issue_vec+xe_vsu1")
;;   -mxenon-smt=busy: latency 4, issue 2
(define_insn_reservation "xenon-vecsimple-smt-busy" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (ior (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
                  (not (match_test "rs6000_xenon_vscr_move_p (insn)")))
            (and (eq_attr "type" "veccmp,veccmpfx")
                  (not (match_test "rs6000_xenon_vec_predicate_p (insn)")))))
  "xenon_smt1_issue_vec+xe_vsu1")
;;   -mxenon-smt=same: latency 5, issue 2
(define_insn_reservation "xenon-vecsimple-smt-same" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (ior (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
                  (not (match_test "rs6000_xenon_vscr_move_p (insn)")))
            (and (eq_attr "type" "veccmp,veccmpfx")
                  (not (match_test "rs6000_xenon_vec_predicate_p (insn)")))))
  "xenon_smt1_issue_vec+xe_vsu1")

;; xenon-vscr: mfvscr mtvscr
;;   latency ISS_VSCR = 40, issue ISS_VSCR = 40, microcoded
(define_insn_reservation "xenon-vscr" 40
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
             (match_test "rs6000_xenon_vscr_move_p (insn)")))
  "xenon_issue_solo+xe_vsu1*40")
;;   -mxenon-smt=busy: latency 40, issue 76
(define_insn_reservation "xenon-vscr-smt-busy" 40
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
             (match_test "rs6000_xenon_vscr_move_p (insn)")))
  "xenon_smt1_issue_solo+xe_vsu1*76")
;;   -mxenon-smt=same: latency 51, issue 91
(define_insn_reservation "xenon-vscr-smt-same" 51
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
             (match_test "rs6000_xenon_vscr_move_p (insn)")))
  "xenon_smt1_issue_solo+xe_vsu1*91")

;; xenon-veccmp-rec: vcmp*. (CR6)
;;   latency LAT_VCMP_REC = 32, issue ISS_VCMP_REC = 32, microcoded
(define_insn_reservation "xenon-veccmp-rec" 32
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "veccmp,veccmpfx")
             (match_test "rs6000_xenon_vec_predicate_p (insn)")))
  "xenon_issue_solo+xe_vsu1*32")
;;   -mxenon-smt=busy: latency 32, issue 60
(define_insn_reservation "xenon-veccmp-rec-smt-busy" 32
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "veccmp,veccmpfx")
             (match_test "rs6000_xenon_vec_predicate_p (insn)")))
  "xenon_smt1_issue_solo+xe_vsu1*60")
;;   -mxenon-smt=same: latency 41, issue 73
(define_insn_reservation "xenon-veccmp-rec-smt-same" 41
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "veccmp,veccmpfx")
             (match_test "rs6000_xenon_vec_predicate_p (insn)")))
  "xenon_smt1_issue_solo+xe_vsu1*73")

;; xenon-vecperm: vperm vsldoi vsplt* merge/pack/unpack
;;   latency LAT_VPERM = 4, issue ISS_VSIMPLE = 1
(define_insn_reservation "xenon-vecperm" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "vecperm"))
  "xenon_issue_vec+xe_vsu2")
;;   -mxenon-smt=busy: latency 4, issue 2
(define_insn_reservation "xenon-vecperm-smt-busy" 4
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "vecperm"))
  "xenon_smt1_issue_vec+xe_vsu2")
;;   -mxenon-smt=same: latency 5, issue 2
(define_insn_reservation "xenon-vecperm-smt-same" 5
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "vecperm"))
  "xenon_smt1_issue_vec+xe_vsu2")

;; xenon-vecfloat: VMX FP add/sub/mul/madd/nmsub, conversions, vrfi*, vexptefp, vlogefp
;;   latency LAT_VFLOAT = 12, issue ISS_VSIMPLE = 1
(define_insn_reservation "xenon-vecfloat" 12
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "vecfloat")
             (not (match_test "rs6000_xenon_vest_p (insn)"))))
  "xenon_issue_vec+xe_vsu1")
;;   -mxenon-smt=busy: latency 12, issue 2
(define_insn_reservation "xenon-vecfloat-smt-busy" 12
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "vecfloat")
             (not (match_test "rs6000_xenon_vest_p (insn)"))))
  "xenon_smt1_issue_vec+xe_vsu1")
;;   -mxenon-smt=same: latency 15, issue 2
(define_insn_reservation "xenon-vecfloat-smt-same" 15
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "vecfloat")
             (not (match_test "rs6000_xenon_vest_p (insn)"))))
  "xenon_smt1_issue_vec+xe_vsu1")

;; xenon-vest: vrefp vrsqrtefp (half pipelined)
;;   latency LAT_VEST = 14, issue ISS_VEST = 2
(define_insn_reservation "xenon-vest" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "vecfloat")
             (match_test "rs6000_xenon_vest_p (insn)")))
  "xenon_issue_vec+xe_vsu1*2")
;;   -mxenon-smt=busy: latency 14, issue 4
(define_insn_reservation "xenon-vest-smt-busy" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "vecfloat")
             (match_test "rs6000_xenon_vest_p (insn)")))
  "xenon_smt1_issue_vec+xe_vsu1*4")
;;   -mxenon-smt=same: latency 18, issue 5
(define_insn_reservation "xenon-vest-smt-same" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "vecfloat")
             (match_test "rs6000_xenon_vest_p (insn)")))
  "xenon_smt1_issue_vec+xe_vsu1*5")

;; xenon-veccomplex: vmsum3fp128/vmsum4fp128 class (the AltiVec integer multiply family is illegal on Xenon)
;;   latency LAT_VDOT = 14, issue ISS_VSIMPLE = 1
(define_insn_reservation "xenon-veccomplex" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "veccomplex"))
  "xenon_issue_vec+xe_vsu1")
;;   -mxenon-smt=busy: latency 14, issue 2
(define_insn_reservation "xenon-veccomplex-smt-busy" 14
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (eq_attr "type" "veccomplex"))
  "xenon_smt1_issue_vec+xe_vsu1")
;;   -mxenon-smt=same: latency 18, issue 2
(define_insn_reservation "xenon-veccomplex-smt-same" 18
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (eq_attr "type" "veccomplex"))
  "xenon_smt1_issue_vec+xe_vsu1")

;; xenon-mftb: mftb / mfspr 268
;;   latency ISS_MFTB = 44, issue ISS_MFTB = 44, microcoded
(define_insn_reservation "xenon-mftb" 44
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "integer")
             (not (match_test "rs6000_xenon_mffs_p (insn)"))
             (match_test "rs6000_xenon_mftb_p (insn)")))
  "xenon_issue_solo+xe_fxu*44")
;;   -mxenon-smt=busy: latency 44, issue 83
(define_insn_reservation "xenon-mftb-smt-busy" 44
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "integer")
             (not (match_test "rs6000_xenon_mffs_p (insn)"))
             (match_test "rs6000_xenon_mftb_p (insn)")))
  "xenon_smt1_issue_solo+xe_fxu*83")
;;   -mxenon-smt=same: latency 56, issue 100
(define_insn_reservation "xenon-mftb-smt-same" 56
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "integer")
             (not (match_test "rs6000_xenon_mffs_p (insn)"))
             (match_test "rs6000_xenon_mftb_p (insn)")))
  "xenon_smt1_issue_solo+xe_fxu*100")

;; xenon-mffs: mffs
;;   latency ISS_MFFS = 30, issue ISS_MFFS = 30, microcoded
(define_insn_reservation "xenon-mffs" 30
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (and (eq_attr "type" "integer")
             (match_test "rs6000_xenon_mffs_p (insn)")))
  "xenon_issue_solo+xe_vsu1*30")
;;   -mxenon-smt=busy: latency 30, issue 57
(define_insn_reservation "xenon-mffs-smt-busy" 30
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy")
       (and (eq_attr "type" "integer")
             (match_test "rs6000_xenon_mffs_p (insn)")))
  "xenon_smt1_issue_solo+xe_vsu1*57")
;;   -mxenon-smt=same: latency 38, issue 68
(define_insn_reservation "xenon-mffs-smt-same" 38
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "same")
       (and (eq_attr "type" "integer")
             (match_test "rs6000_xenon_mffs_p (insn)")))
  "xenon_smt1_issue_solo+xe_vsu1*68")

;; xenon-nonxenon: POWER8+/HTM/MMA instructions: never occur for -mcpu=xenon, kept classified for -mtune=xenon
;;   latency LAT_INT = 2, issue ISS_INT = 1
(define_insn_reservation "xenon-nonxenon" 2
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "idle")
       (eq_attr "type" "mtvsr,mfvsr,crypto,htm,htmsimple,mma,dmf"))
  "xenon_issue_solo+xe_fxu")
;;   -mxenon-smt=busy/same: latency 3, issue 2
(define_insn_reservation "xenon-nonxenon-smt-busy-same" 3
  (and (eq_attr "cpu" "xenon")
       (eq_attr "xenon_smt" "busy,same")
       (eq_attr "type" "mtvsr,mfvsr,crypto,htm,htmsimple,mma,dmf"))
  "xenon_smt1_issue_solo+xe_fxu")

;; Issue cycles per insn (the occupancy above), for TARGET_INSN_COST.
(define_attr "xenon_issue" ""
  (cond [
	 ;; xenon-int
	 (ior (and (eq_attr "type" "integer")
	            (not (match_test "rs6000_xenon_mffs_p (insn)"))
	            (not (match_test "rs6000_xenon_mftb_p (insn)")))
	      (eq_attr "type" "isel,popcnt")
	      (and (eq_attr "type" "add,logical,insert,exts,cntlz")
	            (eq_attr "dot" "no"))
	      (and (eq_attr "type" "shift")
	            (eq_attr "dot" "no")
	            (eq_attr "var_shift" "no")))
	 (const_int 1)
	 ;; xenon-add-rec
	 (and (eq_attr "type" "add")
	       (eq_attr "dot" "yes"))
	 (const_int 1)
	 ;; xenon-two
	 (eq_attr "type" "two,fused_arith_logical,fused_cmp_isel,fused_carry,fused_load_cmpi,fused_load_load,fused_store_store,fused_addis_load,fused_mtbc,fused_vector")
	 (const_int 2)
	 ;; xenon-three
	 (eq_attr "type" "three")
	 (const_int 3)
	 ;; xenon-cmp
	 (eq_attr "type" "cmp")
	 (const_int 1)
	 ;; xenon-crlogical
	 (eq_attr "type" "cr_logical")
	 (const_int 1)
	 ;; xenon-rec
	 (ior (and (eq_attr "type" "logical,insert,cntlz")
	            (eq_attr "dot" "yes"))
	      (and (eq_attr "type" "shift")
	            (eq_attr "dot" "yes")
	            (eq_attr "var_shift" "no")))
	 (const_int 14)
	 ;; xenon-exts-rec
	 (and (eq_attr "type" "exts")
	       (eq_attr "dot" "yes"))
	 (const_int 16)
	 ;; xenon-varshift
	 (and (eq_attr "type" "shift")
	       (eq_attr "dot" "no")
	       (eq_attr "var_shift" "yes"))
	 (const_int 16)
	 ;; xenon-varshift-rec
	 (and (eq_attr "type" "shift")
	       (eq_attr "dot" "yes")
	       (eq_attr "var_shift" "yes"))
	 (const_int 18)
	 ;; xenon-mulli
	 (and (eq_attr "type" "mul")
	       (eq_attr "size" "8,16"))
	 (const_int 7)
	 ;; xenon-mulw
	 (ior (and (eq_attr "type" "mul")
	            (eq_attr "dot" "no")
	            (eq_attr "size" "32"))
	      (eq_attr "type" "halfmul"))
	 (const_int 11)
	 ;; xenon-muld
	 (and (eq_attr "type" "mul")
	       (eq_attr "dot" "no")
	       (eq_attr "size" "64,128,256"))
	 (const_int 13)
	 ;; xenon-mulw-rec
	 (and (eq_attr "type" "mul")
	       (eq_attr "dot" "yes")
	       (eq_attr "size" "32"))
	 (const_int 24)
	 ;; xenon-muld-rec
	 (and (eq_attr "type" "mul")
	       (eq_attr "dot" "yes")
	       (eq_attr "size" "64,128,256"))
	 (const_int 26)
	 ;; xenon-divw
	 (and (eq_attr "type" "div")
	       (eq_attr "dot" "no")
	       (eq_attr "size" "8,16,32"))
	 (const_int 28)
	 ;; xenon-divd
	 (and (eq_attr "type" "div")
	       (eq_attr "dot" "no")
	       (eq_attr "size" "64,128,256"))
	 (const_int 28)
	 ;; xenon-div-rec
	 (and (eq_attr "type" "div")
	       (eq_attr "dot" "yes"))
	 (const_int 40)
	 ;; xenon-load
	 (and (eq_attr "type" "load")
	       (eq_attr "sign_extend" "no")
	       (eq_attr "update" "no"))
	 (const_int 1)
	 ;; xenon-load-update
	 (and (eq_attr "type" "load")
	       (eq_attr "sign_extend" "no")
	       (eq_attr "update" "yes"))
	 (const_int 2)
	 ;; xenon-load-ext
	 (and (eq_attr "type" "load")
	       (eq_attr "sign_extend" "yes"))
	 (const_int 24)
	 ;; xenon-store
	 (and (eq_attr "type" "store")
	       (eq_attr "update" "no"))
	 (const_int 3)
	 ;; xenon-store-update
	 (and (eq_attr "type" "store")
	       (eq_attr "update" "yes"))
	 (const_int 3)
	 ;; xenon-fpload
	 (and (eq_attr "type" "fpload")
	       (eq_attr "update" "no"))
	 (const_int 3)
	 ;; xenon-fpload-update
	 (and (eq_attr "type" "fpload")
	       (eq_attr "update" "yes"))
	 (const_int 3)
	 ;; xenon-fpstore
	 (and (eq_attr "type" "fpstore")
	       (eq_attr "update" "no"))
	 (const_int 4)
	 ;; xenon-fpstore-update
	 (and (eq_attr "type" "fpstore")
	       (eq_attr "update" "yes"))
	 (const_int 4)
	 ;; xenon-vecload
	 (and (eq_attr "type" "vecload")
	       (not (match_test "rs6000_xenon_lvsl_p (insn)")))
	 (const_int 3)
	 ;; xenon-lvsl
	 (and (eq_attr "type" "vecload")
	       (match_test "rs6000_xenon_lvsl_p (insn)"))
	 (const_int 3)
	 ;; xenon-vecstore
	 (eq_attr "type" "vecstore")
	 (const_int 3)
	 ;; xenon-branch
	 (eq_attr "type" "branch,jmpreg,trap")
	 (const_int 1)
	 ;; xenon-mfjmpr
	 (eq_attr "type" "mfjmpr")
	 (const_int 3)
	 ;; xenon-mtjmpr
	 (eq_attr "type" "mtjmpr")
	 (const_int 1)
	 ;; xenon-mfcr
	 (eq_attr "type" "mfcr,mfcrf")
	 (const_int 26)
	 ;; xenon-mtcrf
	 (and (eq_attr "type" "mtcr")
	       (match_test "rs6000_xenon_mtcrf_multi_p (insn)"))
	 (const_int 20)
	 ;; xenon-mtocrf
	 (and (eq_attr "type" "mtcr")
	       (not (match_test "rs6000_xenon_mtcrf_multi_p (insn)")))
	 (const_int 1)
	 ;; xenon-isync
	 (eq_attr "type" "isync")
	 (const_int 34)
	 ;; xenon-lwsync
	 (and (eq_attr "type" "sync")
	       (not (match_test "rs6000_xenon_hwsync_p (insn)")))
	 (const_int 32)
	 ;; xenon-hwsync
	 (and (eq_attr "type" "sync")
	       (match_test "rs6000_xenon_hwsync_p (insn)"))
	 (const_int 162)
	 ;; xenon-larx
	 (eq_attr "type" "load_l")
	 (const_int 65)
	 ;; xenon-stcx
	 (eq_attr "type" "store_c")
	 (const_int 80)
	 ;; xenon-fp
	 (eq_attr "type" "fp,fpsimple,dmul,qmul,dfp,vecdouble")
	 (const_int 1)
	 ;; xenon-fpcompare
	 (eq_attr "type" "fpcompare")
	 (const_int 16)
	 ;; xenon-fdiv
	 (eq_attr "type" "sdiv,ddiv,vecdiv,vecfdiv")
	 (const_int 74)
	 ;; xenon-fsqrt
	 (eq_attr "type" "ssqrt,dsqrt")
	 (const_int 84)
	 ;; xenon-vecsimple
	 (ior (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
	            (not (match_test "rs6000_xenon_vscr_move_p (insn)")))
	      (and (eq_attr "type" "veccmp,veccmpfx")
	            (not (match_test "rs6000_xenon_vec_predicate_p (insn)"))))
	 (const_int 1)
	 ;; xenon-vscr
	 (and (eq_attr "type" "vecsimple,veclogical,vecmove,veccmpsimple,vecexts")
	       (match_test "rs6000_xenon_vscr_move_p (insn)"))
	 (const_int 40)
	 ;; xenon-veccmp-rec
	 (and (eq_attr "type" "veccmp,veccmpfx")
	       (match_test "rs6000_xenon_vec_predicate_p (insn)"))
	 (const_int 32)
	 ;; xenon-vecperm
	 (eq_attr "type" "vecperm")
	 (const_int 1)
	 ;; xenon-vecfloat
	 (and (eq_attr "type" "vecfloat")
	       (not (match_test "rs6000_xenon_vest_p (insn)")))
	 (const_int 1)
	 ;; xenon-vest
	 (and (eq_attr "type" "vecfloat")
	       (match_test "rs6000_xenon_vest_p (insn)"))
	 (const_int 2)
	 ;; xenon-veccomplex
	 (eq_attr "type" "veccomplex")
	 (const_int 1)
	 ;; xenon-mftb
	 (and (eq_attr "type" "integer")
	       (not (match_test "rs6000_xenon_mffs_p (insn)"))
	       (match_test "rs6000_xenon_mftb_p (insn)"))
	 (const_int 44)
	 ;; xenon-mffs
	 (and (eq_attr "type" "integer")
	       (match_test "rs6000_xenon_mffs_p (insn)"))
	 (const_int 30)
	 ;; xenon-nonxenon
	 (eq_attr "type" "mtvsr,mfvsr,crypto,htm,htmsimple,mma,dmf")
	 (const_int 1)
	]
	(const_int 1)))

;; END GENERATED by tools/gen_xenon_sched.py
