/* Xenon: "or N,N,N" sets the thread priority (or 1,1,1 = low priority, one
   insn per 32 cycles), so a GPR self-move or self-IOR must never reach the
   output.  That is a property of the hardware, so it holds for -mcpu=xenon
   whatever the tuning.  The RTL below hands the machine-reorg pass an
   (ior r3 r3) and a plain r1 self-move, which nothing else in GCC deletes
   (set_noop_p does not cover IOR).  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -mtune=cell" } */
/* { dg-final { scan-assembler-not {\mor ([0-9]+),\1,\1\M} } } */
/* { dg-final { scan-assembler-not {\mmr ([0-9]+),\1\M} } } */

long __RTL (startwith ("mach")) f (long x)
{
(function "f"
  (param "x"
    (DECL_RTL (reg/v:DI 3 [ x ]))
    (DECL_RTL_INCOMING (reg:DI 3 [ x ])))
  (insn-chain
    (block 2
      (edge-from entry (flags "FALLTHRU"))
      (cnote 1 [bb 2] NOTE_INSN_BASIC_BLOCK)
      (cinsn 2 (set (reg:DI 3) (ior:DI (reg:DI 3) (reg:DI 3))))
      (cinsn 4 (set (reg:DI 1) (reg:DI 1)))
      (cinsn 3 (use (reg/i:DI 3)))
      (edge-to exit (flags "FALLTHRU"))
    )
  )
  (crtl (return_rtx (reg/i:DI 3)))
)
}
