/* Out-of-line routines that restore the Xbox 360 Xenon VMX128
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

     _restvr128_N, N = 64..127, restores vN..v127 from the 16-byte slots at
     r0 - 16 * (128 - N) .. r0 - 16, i.e. vM at r0 - 16 * (128 - M).
     On entry r0 points just past the end of the (16-byte aligned) save
     area.  r12 is clobbered (it holds the offsets); nothing else but
     the registers loaded changes.  The routine is entered with bl and returns
     with blr; it uses no stack and no TOC.

   Each entry point falls through to the next one, so the code is one
   sequence of 64 (li, lvx128) pairs and a blr.  The symbols are hidden
   code labels (no function descriptors), like the linker's own routines,
   so that bl reaches them directly in the ELFv1 ABI.  stvx128 and lvx128
   take [reg+reg] addresses only; RA must not be r0 (it would read as 0),
   hence r12 as RA and r0 as RB.  */

	.machine push
	.machine xenon
	.section ".text"
	.align 2
	.cfi_startproc
	.globl _restvr128_64
	.hidden _restvr128_64
	.type _restvr128_64,@function
_restvr128_64:
	li 12,-1024
	lvx128 64,12,0
	.globl _restvr128_65
	.hidden _restvr128_65
	.type _restvr128_65,@function
_restvr128_65:
	li 12,-1008
	lvx128 65,12,0
	.globl _restvr128_66
	.hidden _restvr128_66
	.type _restvr128_66,@function
_restvr128_66:
	li 12,-992
	lvx128 66,12,0
	.globl _restvr128_67
	.hidden _restvr128_67
	.type _restvr128_67,@function
_restvr128_67:
	li 12,-976
	lvx128 67,12,0
	.globl _restvr128_68
	.hidden _restvr128_68
	.type _restvr128_68,@function
_restvr128_68:
	li 12,-960
	lvx128 68,12,0
	.globl _restvr128_69
	.hidden _restvr128_69
	.type _restvr128_69,@function
_restvr128_69:
	li 12,-944
	lvx128 69,12,0
	.globl _restvr128_70
	.hidden _restvr128_70
	.type _restvr128_70,@function
_restvr128_70:
	li 12,-928
	lvx128 70,12,0
	.globl _restvr128_71
	.hidden _restvr128_71
	.type _restvr128_71,@function
_restvr128_71:
	li 12,-912
	lvx128 71,12,0
	.globl _restvr128_72
	.hidden _restvr128_72
	.type _restvr128_72,@function
_restvr128_72:
	li 12,-896
	lvx128 72,12,0
	.globl _restvr128_73
	.hidden _restvr128_73
	.type _restvr128_73,@function
_restvr128_73:
	li 12,-880
	lvx128 73,12,0
	.globl _restvr128_74
	.hidden _restvr128_74
	.type _restvr128_74,@function
_restvr128_74:
	li 12,-864
	lvx128 74,12,0
	.globl _restvr128_75
	.hidden _restvr128_75
	.type _restvr128_75,@function
_restvr128_75:
	li 12,-848
	lvx128 75,12,0
	.globl _restvr128_76
	.hidden _restvr128_76
	.type _restvr128_76,@function
_restvr128_76:
	li 12,-832
	lvx128 76,12,0
	.globl _restvr128_77
	.hidden _restvr128_77
	.type _restvr128_77,@function
_restvr128_77:
	li 12,-816
	lvx128 77,12,0
	.globl _restvr128_78
	.hidden _restvr128_78
	.type _restvr128_78,@function
_restvr128_78:
	li 12,-800
	lvx128 78,12,0
	.globl _restvr128_79
	.hidden _restvr128_79
	.type _restvr128_79,@function
_restvr128_79:
	li 12,-784
	lvx128 79,12,0
	.globl _restvr128_80
	.hidden _restvr128_80
	.type _restvr128_80,@function
_restvr128_80:
	li 12,-768
	lvx128 80,12,0
	.globl _restvr128_81
	.hidden _restvr128_81
	.type _restvr128_81,@function
_restvr128_81:
	li 12,-752
	lvx128 81,12,0
	.globl _restvr128_82
	.hidden _restvr128_82
	.type _restvr128_82,@function
_restvr128_82:
	li 12,-736
	lvx128 82,12,0
	.globl _restvr128_83
	.hidden _restvr128_83
	.type _restvr128_83,@function
_restvr128_83:
	li 12,-720
	lvx128 83,12,0
	.globl _restvr128_84
	.hidden _restvr128_84
	.type _restvr128_84,@function
_restvr128_84:
	li 12,-704
	lvx128 84,12,0
	.globl _restvr128_85
	.hidden _restvr128_85
	.type _restvr128_85,@function
_restvr128_85:
	li 12,-688
	lvx128 85,12,0
	.globl _restvr128_86
	.hidden _restvr128_86
	.type _restvr128_86,@function
_restvr128_86:
	li 12,-672
	lvx128 86,12,0
	.globl _restvr128_87
	.hidden _restvr128_87
	.type _restvr128_87,@function
_restvr128_87:
	li 12,-656
	lvx128 87,12,0
	.globl _restvr128_88
	.hidden _restvr128_88
	.type _restvr128_88,@function
_restvr128_88:
	li 12,-640
	lvx128 88,12,0
	.globl _restvr128_89
	.hidden _restvr128_89
	.type _restvr128_89,@function
_restvr128_89:
	li 12,-624
	lvx128 89,12,0
	.globl _restvr128_90
	.hidden _restvr128_90
	.type _restvr128_90,@function
_restvr128_90:
	li 12,-608
	lvx128 90,12,0
	.globl _restvr128_91
	.hidden _restvr128_91
	.type _restvr128_91,@function
_restvr128_91:
	li 12,-592
	lvx128 91,12,0
	.globl _restvr128_92
	.hidden _restvr128_92
	.type _restvr128_92,@function
_restvr128_92:
	li 12,-576
	lvx128 92,12,0
	.globl _restvr128_93
	.hidden _restvr128_93
	.type _restvr128_93,@function
_restvr128_93:
	li 12,-560
	lvx128 93,12,0
	.globl _restvr128_94
	.hidden _restvr128_94
	.type _restvr128_94,@function
_restvr128_94:
	li 12,-544
	lvx128 94,12,0
	.globl _restvr128_95
	.hidden _restvr128_95
	.type _restvr128_95,@function
_restvr128_95:
	li 12,-528
	lvx128 95,12,0
	.globl _restvr128_96
	.hidden _restvr128_96
	.type _restvr128_96,@function
_restvr128_96:
	li 12,-512
	lvx128 96,12,0
	.globl _restvr128_97
	.hidden _restvr128_97
	.type _restvr128_97,@function
_restvr128_97:
	li 12,-496
	lvx128 97,12,0
	.globl _restvr128_98
	.hidden _restvr128_98
	.type _restvr128_98,@function
_restvr128_98:
	li 12,-480
	lvx128 98,12,0
	.globl _restvr128_99
	.hidden _restvr128_99
	.type _restvr128_99,@function
_restvr128_99:
	li 12,-464
	lvx128 99,12,0
	.globl _restvr128_100
	.hidden _restvr128_100
	.type _restvr128_100,@function
_restvr128_100:
	li 12,-448
	lvx128 100,12,0
	.globl _restvr128_101
	.hidden _restvr128_101
	.type _restvr128_101,@function
_restvr128_101:
	li 12,-432
	lvx128 101,12,0
	.globl _restvr128_102
	.hidden _restvr128_102
	.type _restvr128_102,@function
_restvr128_102:
	li 12,-416
	lvx128 102,12,0
	.globl _restvr128_103
	.hidden _restvr128_103
	.type _restvr128_103,@function
_restvr128_103:
	li 12,-400
	lvx128 103,12,0
	.globl _restvr128_104
	.hidden _restvr128_104
	.type _restvr128_104,@function
_restvr128_104:
	li 12,-384
	lvx128 104,12,0
	.globl _restvr128_105
	.hidden _restvr128_105
	.type _restvr128_105,@function
_restvr128_105:
	li 12,-368
	lvx128 105,12,0
	.globl _restvr128_106
	.hidden _restvr128_106
	.type _restvr128_106,@function
_restvr128_106:
	li 12,-352
	lvx128 106,12,0
	.globl _restvr128_107
	.hidden _restvr128_107
	.type _restvr128_107,@function
_restvr128_107:
	li 12,-336
	lvx128 107,12,0
	.globl _restvr128_108
	.hidden _restvr128_108
	.type _restvr128_108,@function
_restvr128_108:
	li 12,-320
	lvx128 108,12,0
	.globl _restvr128_109
	.hidden _restvr128_109
	.type _restvr128_109,@function
_restvr128_109:
	li 12,-304
	lvx128 109,12,0
	.globl _restvr128_110
	.hidden _restvr128_110
	.type _restvr128_110,@function
_restvr128_110:
	li 12,-288
	lvx128 110,12,0
	.globl _restvr128_111
	.hidden _restvr128_111
	.type _restvr128_111,@function
_restvr128_111:
	li 12,-272
	lvx128 111,12,0
	.globl _restvr128_112
	.hidden _restvr128_112
	.type _restvr128_112,@function
_restvr128_112:
	li 12,-256
	lvx128 112,12,0
	.globl _restvr128_113
	.hidden _restvr128_113
	.type _restvr128_113,@function
_restvr128_113:
	li 12,-240
	lvx128 113,12,0
	.globl _restvr128_114
	.hidden _restvr128_114
	.type _restvr128_114,@function
_restvr128_114:
	li 12,-224
	lvx128 114,12,0
	.globl _restvr128_115
	.hidden _restvr128_115
	.type _restvr128_115,@function
_restvr128_115:
	li 12,-208
	lvx128 115,12,0
	.globl _restvr128_116
	.hidden _restvr128_116
	.type _restvr128_116,@function
_restvr128_116:
	li 12,-192
	lvx128 116,12,0
	.globl _restvr128_117
	.hidden _restvr128_117
	.type _restvr128_117,@function
_restvr128_117:
	li 12,-176
	lvx128 117,12,0
	.globl _restvr128_118
	.hidden _restvr128_118
	.type _restvr128_118,@function
_restvr128_118:
	li 12,-160
	lvx128 118,12,0
	.globl _restvr128_119
	.hidden _restvr128_119
	.type _restvr128_119,@function
_restvr128_119:
	li 12,-144
	lvx128 119,12,0
	.globl _restvr128_120
	.hidden _restvr128_120
	.type _restvr128_120,@function
_restvr128_120:
	li 12,-128
	lvx128 120,12,0
	.globl _restvr128_121
	.hidden _restvr128_121
	.type _restvr128_121,@function
_restvr128_121:
	li 12,-112
	lvx128 121,12,0
	.globl _restvr128_122
	.hidden _restvr128_122
	.type _restvr128_122,@function
_restvr128_122:
	li 12,-96
	lvx128 122,12,0
	.globl _restvr128_123
	.hidden _restvr128_123
	.type _restvr128_123,@function
_restvr128_123:
	li 12,-80
	lvx128 123,12,0
	.globl _restvr128_124
	.hidden _restvr128_124
	.type _restvr128_124,@function
_restvr128_124:
	li 12,-64
	lvx128 124,12,0
	.globl _restvr128_125
	.hidden _restvr128_125
	.type _restvr128_125,@function
_restvr128_125:
	li 12,-48
	lvx128 125,12,0
	.globl _restvr128_126
	.hidden _restvr128_126
	.type _restvr128_126,@function
_restvr128_126:
	li 12,-32
	lvx128 126,12,0
	.globl _restvr128_127
	.hidden _restvr128_127
	.type _restvr128_127,@function
_restvr128_127:
	li 12,-16
	lvx128 127,12,0
	blr
.Lrestend:
	.size _restvr128_64,.Lrestend-_restvr128_64
	.size _restvr128_65,.Lrestend-_restvr128_65
	.size _restvr128_66,.Lrestend-_restvr128_66
	.size _restvr128_67,.Lrestend-_restvr128_67
	.size _restvr128_68,.Lrestend-_restvr128_68
	.size _restvr128_69,.Lrestend-_restvr128_69
	.size _restvr128_70,.Lrestend-_restvr128_70
	.size _restvr128_71,.Lrestend-_restvr128_71
	.size _restvr128_72,.Lrestend-_restvr128_72
	.size _restvr128_73,.Lrestend-_restvr128_73
	.size _restvr128_74,.Lrestend-_restvr128_74
	.size _restvr128_75,.Lrestend-_restvr128_75
	.size _restvr128_76,.Lrestend-_restvr128_76
	.size _restvr128_77,.Lrestend-_restvr128_77
	.size _restvr128_78,.Lrestend-_restvr128_78
	.size _restvr128_79,.Lrestend-_restvr128_79
	.size _restvr128_80,.Lrestend-_restvr128_80
	.size _restvr128_81,.Lrestend-_restvr128_81
	.size _restvr128_82,.Lrestend-_restvr128_82
	.size _restvr128_83,.Lrestend-_restvr128_83
	.size _restvr128_84,.Lrestend-_restvr128_84
	.size _restvr128_85,.Lrestend-_restvr128_85
	.size _restvr128_86,.Lrestend-_restvr128_86
	.size _restvr128_87,.Lrestend-_restvr128_87
	.size _restvr128_88,.Lrestend-_restvr128_88
	.size _restvr128_89,.Lrestend-_restvr128_89
	.size _restvr128_90,.Lrestend-_restvr128_90
	.size _restvr128_91,.Lrestend-_restvr128_91
	.size _restvr128_92,.Lrestend-_restvr128_92
	.size _restvr128_93,.Lrestend-_restvr128_93
	.size _restvr128_94,.Lrestend-_restvr128_94
	.size _restvr128_95,.Lrestend-_restvr128_95
	.size _restvr128_96,.Lrestend-_restvr128_96
	.size _restvr128_97,.Lrestend-_restvr128_97
	.size _restvr128_98,.Lrestend-_restvr128_98
	.size _restvr128_99,.Lrestend-_restvr128_99
	.size _restvr128_100,.Lrestend-_restvr128_100
	.size _restvr128_101,.Lrestend-_restvr128_101
	.size _restvr128_102,.Lrestend-_restvr128_102
	.size _restvr128_103,.Lrestend-_restvr128_103
	.size _restvr128_104,.Lrestend-_restvr128_104
	.size _restvr128_105,.Lrestend-_restvr128_105
	.size _restvr128_106,.Lrestend-_restvr128_106
	.size _restvr128_107,.Lrestend-_restvr128_107
	.size _restvr128_108,.Lrestend-_restvr128_108
	.size _restvr128_109,.Lrestend-_restvr128_109
	.size _restvr128_110,.Lrestend-_restvr128_110
	.size _restvr128_111,.Lrestend-_restvr128_111
	.size _restvr128_112,.Lrestend-_restvr128_112
	.size _restvr128_113,.Lrestend-_restvr128_113
	.size _restvr128_114,.Lrestend-_restvr128_114
	.size _restvr128_115,.Lrestend-_restvr128_115
	.size _restvr128_116,.Lrestend-_restvr128_116
	.size _restvr128_117,.Lrestend-_restvr128_117
	.size _restvr128_118,.Lrestend-_restvr128_118
	.size _restvr128_119,.Lrestend-_restvr128_119
	.size _restvr128_120,.Lrestend-_restvr128_120
	.size _restvr128_121,.Lrestend-_restvr128_121
	.size _restvr128_122,.Lrestend-_restvr128_122
	.size _restvr128_123,.Lrestend-_restvr128_123
	.size _restvr128_124,.Lrestend-_restvr128_124
	.size _restvr128_125,.Lrestend-_restvr128_125
	.size _restvr128_126,.Lrestend-_restvr128_126
	.size _restvr128_127,.Lrestend-_restvr128_127
	.cfi_endproc
	.machine pop

	.section .note.GNU-stack,"",@progbits
