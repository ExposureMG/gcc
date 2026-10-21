/* Xenon VMX128 ABI (s345-fix finding 1): in a frame over 32 KiB the
   epilogue restores v64-v127 through r11 (the back chain) before popping
   the frame, because their slots are below the 288-byte red zone.  The
   stack pointer restore (mr 1,11) must stay after the restores: a stack
   tie keeps sched2 from moving it above them (it did, both above the
   out-of-line bl _restvr128_N and above inline lvx128 restores), which
   read deallocated stack and put the REG_CFA_RESTORE notes too early.
   20 vectors live across the call: out of line by default (threshold 16).
   Each file holds one function, so a match spanning "mr 1,11" and a later
   restore can only be a restore after the pop.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -maltivec -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mbl _restvr128_108\M} } } */
/* { dg-final { scan-assembler {\mld 11,0\(1\)} } } */
/* { dg-final { scan-assembler {\mmr 1,11\M} } } */
/* { dg-final { scan-assembler-not {\mmr 1,11\n(?:[^\n]*\n)*?[^\n]*(?:_restvr128_|lvx128 [0-9]+,11,0)} } } */
/* The CFI restores of v108-v127 (columns 196-215) come after the restore.  */
/* { dg-final { scan-assembler-not {\.cfi_restore 215\n(?:[^\n]*\n)*?[^\n]*_restvr128_} } } */

typedef __attribute__ ((vector_size (16))) float v4sf;
void h (void *);
int loc (int);

#define L4(b) v4sf x##b##0 = a[b*4+0], x##b##1 = a[b*4+1], \
  x##b##2 = a[b*4+2], x##b##3 = a[b*4+3];
#define S4(b) o[b*4+0] = x##b##3; o[b*4+1] = x##b##2; \
  o[b*4+2] = x##b##1; o[b*4+3] = x##b##0;

int
big_ool (v4sf *restrict o, const v4sf *restrict a, int z)
{
  char buf[40000];
  L4 (0) L4 (1) L4 (2) L4 (3) L4 (4)
  h (buf);
  S4 (0) S4 (1) S4 (2) S4 (3) S4 (4)
  return loc (z);
}
