/* Xenon VMX128 ABI (plan 4.3): a leaf that clobbers all of v64-v127 needs
   1 KiB of save area, more than the 288-byte red zone, so it allocates a
   16-byte aligned frame; optimizing for size it calls _savevr128_64 and
   _restvr128_64.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-Os -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler-times {\mbl _savevr128_64\M} 1 } } */
/* { dg-final { scan-assembler-times {\mbl _restvr128_64\M} 1 } } */
/* { dg-final { scan-assembler {\mstdu 1,-1(0[4-9][0-9]|1[0-9][0-9])\(1\)} } } */
/* { dg-final { scan-assembler {\.cfi_offset 152, } } } */
/* { dg-final { scan-assembler {\.cfi_offset 215, } } } */

#define C8(B) "v" #B "0", "v" #B "1", "v" #B "2", "v" #B "3", \
  "v" #B "4", "v" #B "5", "v" #B "6", "v" #B "7"

void
clobber_all (void)
{
  __asm__ volatile ("# clobbers v64-v127" ::: "v64", "v65", "v66", "v67",
		    "v68", "v69", C8 (7), "v78", "v79", C8 (8), "v88", "v89",
		    C8 (9), "v98", "v99", C8 (10), "v108", "v109", C8 (11),
		    "v118", "v119", "v120", "v121", "v122", "v123", "v124",
		    "v125", "v126", "v127");
}
