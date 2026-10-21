/* crtn.o for powerpc64-xenon-elf / powerpc64-*-elf (newlib, bare metal).

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

/* Tails of _init and _fini; see xenon-crti.S for the frame layout.
   Linked after every other object.  */

	.ident	"GNU C xenon-crtn.S"

#ifdef __powerpc64__
# if defined (_CALL_ELF) && _CALL_ELF == 2
	.abiversion 2
#  define FRAME 32
# else
#  define FRAME 112
# endif
	.section ".init","ax",@progbits
	addi	1,1,FRAME
	ld	0,16(1)
	mtlr	0
	blr

	.section ".fini","ax",@progbits
	addi	1,1,FRAME
	ld	0,16(1)
	mtlr	0
	blr
#else
	.section ".init","ax",@progbits
	lwz	0,20(1)
	mtlr	0
	addi	1,1,16
	blr

	.section ".fini","ax",@progbits
	lwz	0,20(1)
	mtlr	0
	addi	1,1,16
	blr
#endif
