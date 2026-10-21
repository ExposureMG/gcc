/* Out-of-line routines that save the Xbox 360 Xenon VMX128
   callee-saved vector registers v64-v127.

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

/* GCC calls these from prologues and epilogues (rs6000-logue.cc, patterns
   in gcc/config/rs6000/xenon-logue.md) when -mvmx128 code saves many of
   v64-v127.  The convention follows the 64-bit linker-generated _savevr_N
   and _restvr_N for v20-v31:

     _savevr128_N, N = 64..127, saves vN..v127 to the 16-byte slots at
     r0 - 16 * (128 - N) .. r0 - 16, i.e. vM at r0 - 16 * (128 - M).
     On entry r0 points just past the end of the (16-byte aligned) save
     area.  r12 is clobbered (it holds the offsets); nothing else
     changes.  The routine is entered with bl and returns
     with blr; it uses no stack and no TOC.

   Each entry point falls through to the next one, so the code is one
   sequence of 64 (li, stvx128) pairs and a blr.  The symbols are hidden
   code labels (no function descriptors), like the linker's own routines,
   so that bl reaches them directly in the ELFv1 ABI.  stvx128 and lvx128
   take [reg+reg] addresses only; RA must not be r0 (it would read as 0),
   hence r12 as RA and r0 as RB.  */

	.machine push
	.machine xenon
	.section ".text"
	.align 2
	.cfi_startproc
	.globl _savevr128_64
	.hidden _savevr128_64
	.type _savevr128_64,@function
_savevr128_64:
	li 12,-1024
	stvx128 64,12,0
	.globl _savevr128_65
	.hidden _savevr128_65
	.type _savevr128_65,@function
_savevr128_65:
	li 12,-1008
	stvx128 65,12,0
	.globl _savevr128_66
	.hidden _savevr128_66
	.type _savevr128_66,@function
_savevr128_66:
	li 12,-992
	stvx128 66,12,0
	.globl _savevr128_67
	.hidden _savevr128_67
	.type _savevr128_67,@function
_savevr128_67:
	li 12,-976
	stvx128 67,12,0
	.globl _savevr128_68
	.hidden _savevr128_68
	.type _savevr128_68,@function
_savevr128_68:
	li 12,-960
	stvx128 68,12,0
	.globl _savevr128_69
	.hidden _savevr128_69
	.type _savevr128_69,@function
_savevr128_69:
	li 12,-944
	stvx128 69,12,0
	.globl _savevr128_70
	.hidden _savevr128_70
	.type _savevr128_70,@function
_savevr128_70:
	li 12,-928
	stvx128 70,12,0
	.globl _savevr128_71
	.hidden _savevr128_71
	.type _savevr128_71,@function
_savevr128_71:
	li 12,-912
	stvx128 71,12,0
	.globl _savevr128_72
	.hidden _savevr128_72
	.type _savevr128_72,@function
_savevr128_72:
	li 12,-896
	stvx128 72,12,0
	.globl _savevr128_73
	.hidden _savevr128_73
	.type _savevr128_73,@function
_savevr128_73:
	li 12,-880
	stvx128 73,12,0
	.globl _savevr128_74
	.hidden _savevr128_74
	.type _savevr128_74,@function
_savevr128_74:
	li 12,-864
	stvx128 74,12,0
	.globl _savevr128_75
	.hidden _savevr128_75
	.type _savevr128_75,@function
_savevr128_75:
	li 12,-848
	stvx128 75,12,0
	.globl _savevr128_76
	.hidden _savevr128_76
	.type _savevr128_76,@function
_savevr128_76:
	li 12,-832
	stvx128 76,12,0
	.globl _savevr128_77
	.hidden _savevr128_77
	.type _savevr128_77,@function
_savevr128_77:
	li 12,-816
	stvx128 77,12,0
	.globl _savevr128_78
	.hidden _savevr128_78
	.type _savevr128_78,@function
_savevr128_78:
	li 12,-800
	stvx128 78,12,0
	.globl _savevr128_79
	.hidden _savevr128_79
	.type _savevr128_79,@function
_savevr128_79:
	li 12,-784
	stvx128 79,12,0
	.globl _savevr128_80
	.hidden _savevr128_80
	.type _savevr128_80,@function
_savevr128_80:
	li 12,-768
	stvx128 80,12,0
	.globl _savevr128_81
	.hidden _savevr128_81
	.type _savevr128_81,@function
_savevr128_81:
	li 12,-752
	stvx128 81,12,0
	.globl _savevr128_82
	.hidden _savevr128_82
	.type _savevr128_82,@function
_savevr128_82:
	li 12,-736
	stvx128 82,12,0
	.globl _savevr128_83
	.hidden _savevr128_83
	.type _savevr128_83,@function
_savevr128_83:
	li 12,-720
	stvx128 83,12,0
	.globl _savevr128_84
	.hidden _savevr128_84
	.type _savevr128_84,@function
_savevr128_84:
	li 12,-704
	stvx128 84,12,0
	.globl _savevr128_85
	.hidden _savevr128_85
	.type _savevr128_85,@function
_savevr128_85:
	li 12,-688
	stvx128 85,12,0
	.globl _savevr128_86
	.hidden _savevr128_86
	.type _savevr128_86,@function
_savevr128_86:
	li 12,-672
	stvx128 86,12,0
	.globl _savevr128_87
	.hidden _savevr128_87
	.type _savevr128_87,@function
_savevr128_87:
	li 12,-656
	stvx128 87,12,0
	.globl _savevr128_88
	.hidden _savevr128_88
	.type _savevr128_88,@function
_savevr128_88:
	li 12,-640
	stvx128 88,12,0
	.globl _savevr128_89
	.hidden _savevr128_89
	.type _savevr128_89,@function
_savevr128_89:
	li 12,-624
	stvx128 89,12,0
	.globl _savevr128_90
	.hidden _savevr128_90
	.type _savevr128_90,@function
_savevr128_90:
	li 12,-608
	stvx128 90,12,0
	.globl _savevr128_91
	.hidden _savevr128_91
	.type _savevr128_91,@function
_savevr128_91:
	li 12,-592
	stvx128 91,12,0
	.globl _savevr128_92
	.hidden _savevr128_92
	.type _savevr128_92,@function
_savevr128_92:
	li 12,-576
	stvx128 92,12,0
	.globl _savevr128_93
	.hidden _savevr128_93
	.type _savevr128_93,@function
_savevr128_93:
	li 12,-560
	stvx128 93,12,0
	.globl _savevr128_94
	.hidden _savevr128_94
	.type _savevr128_94,@function
_savevr128_94:
	li 12,-544
	stvx128 94,12,0
	.globl _savevr128_95
	.hidden _savevr128_95
	.type _savevr128_95,@function
_savevr128_95:
	li 12,-528
	stvx128 95,12,0
	.globl _savevr128_96
	.hidden _savevr128_96
	.type _savevr128_96,@function
_savevr128_96:
	li 12,-512
	stvx128 96,12,0
	.globl _savevr128_97
	.hidden _savevr128_97
	.type _savevr128_97,@function
_savevr128_97:
	li 12,-496
	stvx128 97,12,0
	.globl _savevr128_98
	.hidden _savevr128_98
	.type _savevr128_98,@function
_savevr128_98:
	li 12,-480
	stvx128 98,12,0
	.globl _savevr128_99
	.hidden _savevr128_99
	.type _savevr128_99,@function
_savevr128_99:
	li 12,-464
	stvx128 99,12,0
	.globl _savevr128_100
	.hidden _savevr128_100
	.type _savevr128_100,@function
_savevr128_100:
	li 12,-448
	stvx128 100,12,0
	.globl _savevr128_101
	.hidden _savevr128_101
	.type _savevr128_101,@function
_savevr128_101:
	li 12,-432
	stvx128 101,12,0
	.globl _savevr128_102
	.hidden _savevr128_102
	.type _savevr128_102,@function
_savevr128_102:
	li 12,-416
	stvx128 102,12,0
	.globl _savevr128_103
	.hidden _savevr128_103
	.type _savevr128_103,@function
_savevr128_103:
	li 12,-400
	stvx128 103,12,0
	.globl _savevr128_104
	.hidden _savevr128_104
	.type _savevr128_104,@function
_savevr128_104:
	li 12,-384
	stvx128 104,12,0
	.globl _savevr128_105
	.hidden _savevr128_105
	.type _savevr128_105,@function
_savevr128_105:
	li 12,-368
	stvx128 105,12,0
	.globl _savevr128_106
	.hidden _savevr128_106
	.type _savevr128_106,@function
_savevr128_106:
	li 12,-352
	stvx128 106,12,0
	.globl _savevr128_107
	.hidden _savevr128_107
	.type _savevr128_107,@function
_savevr128_107:
	li 12,-336
	stvx128 107,12,0
	.globl _savevr128_108
	.hidden _savevr128_108
	.type _savevr128_108,@function
_savevr128_108:
	li 12,-320
	stvx128 108,12,0
	.globl _savevr128_109
	.hidden _savevr128_109
	.type _savevr128_109,@function
_savevr128_109:
	li 12,-304
	stvx128 109,12,0
	.globl _savevr128_110
	.hidden _savevr128_110
	.type _savevr128_110,@function
_savevr128_110:
	li 12,-288
	stvx128 110,12,0
	.globl _savevr128_111
	.hidden _savevr128_111
	.type _savevr128_111,@function
_savevr128_111:
	li 12,-272
	stvx128 111,12,0
	.globl _savevr128_112
	.hidden _savevr128_112
	.type _savevr128_112,@function
_savevr128_112:
	li 12,-256
	stvx128 112,12,0
	.globl _savevr128_113
	.hidden _savevr128_113
	.type _savevr128_113,@function
_savevr128_113:
	li 12,-240
	stvx128 113,12,0
	.globl _savevr128_114
	.hidden _savevr128_114
	.type _savevr128_114,@function
_savevr128_114:
	li 12,-224
	stvx128 114,12,0
	.globl _savevr128_115
	.hidden _savevr128_115
	.type _savevr128_115,@function
_savevr128_115:
	li 12,-208
	stvx128 115,12,0
	.globl _savevr128_116
	.hidden _savevr128_116
	.type _savevr128_116,@function
_savevr128_116:
	li 12,-192
	stvx128 116,12,0
	.globl _savevr128_117
	.hidden _savevr128_117
	.type _savevr128_117,@function
_savevr128_117:
	li 12,-176
	stvx128 117,12,0
	.globl _savevr128_118
	.hidden _savevr128_118
	.type _savevr128_118,@function
_savevr128_118:
	li 12,-160
	stvx128 118,12,0
	.globl _savevr128_119
	.hidden _savevr128_119
	.type _savevr128_119,@function
_savevr128_119:
	li 12,-144
	stvx128 119,12,0
	.globl _savevr128_120
	.hidden _savevr128_120
	.type _savevr128_120,@function
_savevr128_120:
	li 12,-128
	stvx128 120,12,0
	.globl _savevr128_121
	.hidden _savevr128_121
	.type _savevr128_121,@function
_savevr128_121:
	li 12,-112
	stvx128 121,12,0
	.globl _savevr128_122
	.hidden _savevr128_122
	.type _savevr128_122,@function
_savevr128_122:
	li 12,-96
	stvx128 122,12,0
	.globl _savevr128_123
	.hidden _savevr128_123
	.type _savevr128_123,@function
_savevr128_123:
	li 12,-80
	stvx128 123,12,0
	.globl _savevr128_124
	.hidden _savevr128_124
	.type _savevr128_124,@function
_savevr128_124:
	li 12,-64
	stvx128 124,12,0
	.globl _savevr128_125
	.hidden _savevr128_125
	.type _savevr128_125,@function
_savevr128_125:
	li 12,-48
	stvx128 125,12,0
	.globl _savevr128_126
	.hidden _savevr128_126
	.type _savevr128_126,@function
_savevr128_126:
	li 12,-32
	stvx128 126,12,0
	.globl _savevr128_127
	.hidden _savevr128_127
	.type _savevr128_127,@function
_savevr128_127:
	li 12,-16
	stvx128 127,12,0
	blr
.Lsaveend:
	.size _savevr128_64,.Lsaveend-_savevr128_64
	.size _savevr128_65,.Lsaveend-_savevr128_65
	.size _savevr128_66,.Lsaveend-_savevr128_66
	.size _savevr128_67,.Lsaveend-_savevr128_67
	.size _savevr128_68,.Lsaveend-_savevr128_68
	.size _savevr128_69,.Lsaveend-_savevr128_69
	.size _savevr128_70,.Lsaveend-_savevr128_70
	.size _savevr128_71,.Lsaveend-_savevr128_71
	.size _savevr128_72,.Lsaveend-_savevr128_72
	.size _savevr128_73,.Lsaveend-_savevr128_73
	.size _savevr128_74,.Lsaveend-_savevr128_74
	.size _savevr128_75,.Lsaveend-_savevr128_75
	.size _savevr128_76,.Lsaveend-_savevr128_76
	.size _savevr128_77,.Lsaveend-_savevr128_77
	.size _savevr128_78,.Lsaveend-_savevr128_78
	.size _savevr128_79,.Lsaveend-_savevr128_79
	.size _savevr128_80,.Lsaveend-_savevr128_80
	.size _savevr128_81,.Lsaveend-_savevr128_81
	.size _savevr128_82,.Lsaveend-_savevr128_82
	.size _savevr128_83,.Lsaveend-_savevr128_83
	.size _savevr128_84,.Lsaveend-_savevr128_84
	.size _savevr128_85,.Lsaveend-_savevr128_85
	.size _savevr128_86,.Lsaveend-_savevr128_86
	.size _savevr128_87,.Lsaveend-_savevr128_87
	.size _savevr128_88,.Lsaveend-_savevr128_88
	.size _savevr128_89,.Lsaveend-_savevr128_89
	.size _savevr128_90,.Lsaveend-_savevr128_90
	.size _savevr128_91,.Lsaveend-_savevr128_91
	.size _savevr128_92,.Lsaveend-_savevr128_92
	.size _savevr128_93,.Lsaveend-_savevr128_93
	.size _savevr128_94,.Lsaveend-_savevr128_94
	.size _savevr128_95,.Lsaveend-_savevr128_95
	.size _savevr128_96,.Lsaveend-_savevr128_96
	.size _savevr128_97,.Lsaveend-_savevr128_97
	.size _savevr128_98,.Lsaveend-_savevr128_98
	.size _savevr128_99,.Lsaveend-_savevr128_99
	.size _savevr128_100,.Lsaveend-_savevr128_100
	.size _savevr128_101,.Lsaveend-_savevr128_101
	.size _savevr128_102,.Lsaveend-_savevr128_102
	.size _savevr128_103,.Lsaveend-_savevr128_103
	.size _savevr128_104,.Lsaveend-_savevr128_104
	.size _savevr128_105,.Lsaveend-_savevr128_105
	.size _savevr128_106,.Lsaveend-_savevr128_106
	.size _savevr128_107,.Lsaveend-_savevr128_107
	.size _savevr128_108,.Lsaveend-_savevr128_108
	.size _savevr128_109,.Lsaveend-_savevr128_109
	.size _savevr128_110,.Lsaveend-_savevr128_110
	.size _savevr128_111,.Lsaveend-_savevr128_111
	.size _savevr128_112,.Lsaveend-_savevr128_112
	.size _savevr128_113,.Lsaveend-_savevr128_113
	.size _savevr128_114,.Lsaveend-_savevr128_114
	.size _savevr128_115,.Lsaveend-_savevr128_115
	.size _savevr128_116,.Lsaveend-_savevr128_116
	.size _savevr128_117,.Lsaveend-_savevr128_117
	.size _savevr128_118,.Lsaveend-_savevr128_118
	.size _savevr128_119,.Lsaveend-_savevr128_119
	.size _savevr128_120,.Lsaveend-_savevr128_120
	.size _savevr128_121,.Lsaveend-_savevr128_121
	.size _savevr128_122,.Lsaveend-_savevr128_122
	.size _savevr128_123,.Lsaveend-_savevr128_123
	.size _savevr128_124,.Lsaveend-_savevr128_124
	.size _savevr128_125,.Lsaveend-_savevr128_125
	.size _savevr128_126,.Lsaveend-_savevr128_126
	.size _savevr128_127,.Lsaveend-_savevr128_127
	.cfi_endproc
	.machine pop

	.section .note.GNU-stack,"",@progbits
