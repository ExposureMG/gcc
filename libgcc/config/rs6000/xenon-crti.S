/* crti.o for powerpc64-xenon-elf / powerpc64-*-elf (newlib, bare metal).

   Copyright (C) 2026 Free Software Foundation, Inc.

   This file is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* Heads of _init and _fini.  Linked before every other object; crtn.o
   supplies the matching tails.  Whatever objects put in .init/.fini in
   between (calls of the form "bl func; nop") runs inside these frames.
   newlib's __libc_init_array / __libc_fini_array call _init / _fini.

   64-bit ELFv1: _init/_fini are function descriptors in .opd whose entry
   points are the code at the start of .init/.fini.  The frame is the
   minimum ELFv1 frame (48-byte header + 64-byte parameter save area);
   the caller's LR save slot is 16 bytes above the incoming stack
   pointer.  The TOC save slot (40(r1)) belongs to this frame, so the
   linker can turn the nop after a call into "ld r2,40(r1)" when a
   TOC-switching stub is needed.

   64-bit ELFv2 (-mabi=elfv2, not a tested configuration): plain global
   symbols at the start of .init/.fini; both are only called from code
   sharing the caller's TOC in a static link, so there is no global
   entry point.

   32-bit (SysV ABI, for a later -m32 multilib): a 16-byte frame, LR
   saved at 4 bytes above the incoming stack pointer.  */

	.ident	"GNU C xenon-crti.S"

#ifdef __powerpc64__
# if defined (_CALL_ELF) && _CALL_ELF == 2
	.abiversion 2

	.section ".init","ax",@progbits
	.align	2
	.globl	_init
	.type	_init,@function
_init:
	mflr	0
	std	0,16(1)
	stdu	1,-32(1)

	.section ".fini","ax",@progbits
	.align	2
	.globl	_fini
	.type	_fini,@function
_fini:
	mflr	0
	std	0,16(1)
	stdu	1,-32(1)
# else
	.section ".opd","aw"
	.align	3
	.globl	_init
	.type	_init,@function
_init:
	.quad	.L_init,.TOC.@tocbase,0
	.size	_init,24
	.globl	_fini
	.type	_fini,@function
_fini:
	.quad	.L_fini,.TOC.@tocbase,0
	.size	_fini,24

	.section ".init","ax",@progbits
	.align	2
.L_init:
	mflr	0
	std	0,16(1)
	stdu	1,-112(1)

	.section ".fini","ax",@progbits
	.align	2
.L_fini:
	mflr	0
	std	0,16(1)
	stdu	1,-112(1)
# endif
#else
	.section ".init","ax",@progbits
	.align	2
	.globl	_init
	.type	_init,@function
_init:
	stwu	1,-16(1)
	mflr	0
	stw	0,20(1)

	.section ".fini","ax",@progbits
	.align	2
	.globl	_fini
	.type	_fini,@function
_fini:
	stwu	1,-16(1)
	mflr	0
	stw	0,20(1)
#endif
