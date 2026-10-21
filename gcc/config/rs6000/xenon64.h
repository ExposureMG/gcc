/* Definitions for 64-bit PowerPC bare-metal ELF targets using newlib:
   powerpc64-xenon-elf (Xbox 360 "Xenon") and the generic powerpc64-*-elf.
   Copyright (C) 2012-2026 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* This file follows rs6000/sysv4.h and rs6000/default64.h in tm_file and
   is derived from rs6000/freebsd64.h with the operating-system parts
   removed.  The ABI is the 64-bit ELF ABI version 1 (ELFv1): function
   descriptors in .opd, the TOC pointer in r2, LP64, big-endian.  There is
   no dynamic linker and no shared library support; executables are linked
   statically.  The C library is newlib; the board support library
   (libxenon) supplies crt0.o and the system calls.

   -m64 is the default.  Nothing here prevents a later -m32 multilib: the
   32-bit side keeps the SysV (ABI_V4) conventions of rs6000/sysv4.h.  */

/* gnu-user.h is only included because rs6000/sysv4.h refers to its spec
   macros.  Undo the parts that describe a GNU/Linux userland.  */
#undef CPLUSPLUS_CPP_SPEC
#undef LINK_EH_SPEC
#undef USE_LD_AS_NEEDED
#undef TARGET_POSIX_IO

/* newlib is not glibc: only claim the C99 functions.  */
#undef TARGET_LIBC_HAS_FUNCTION
#define TARGET_LIBC_HAS_FUNCTION default_libc_has_function

/* Override the defaults, which exist to force the proper definition.  */
#ifdef IN_LIBGCC2
#undef TARGET_64BIT
#ifdef __powerpc64__
#define TARGET_64BIT 1
#else
#define TARGET_64BIT 0
#endif
#endif

/* The 64-bit ABI is the AIX-style (descriptor) ABI.  */
#undef	TARGET_AIX
#define	TARGET_AIX TARGET_64BIT

#ifdef HAVE_LD_NO_DOT_SYMS
/* New ABI uses a local sym for the function entry point.  */
extern int dot_symbols;
#undef DOT_SYMBOLS
#define DOT_SYMBOLS dot_symbols
#endif

/* Use the -mcmodel= / -mprofile-kernel options and the 64-bit option
   checks of rs6000/linux64.opt (rs6000_linux64_override_options).  The
   linker is GNU ld from binutils 2.47 or later, which supports the
   medium and large models, so -mcmodel= is always honoured.  */
#define TARGET_USES_LINUX64_OPT 1
#undef TARGET_CMODEL
#define TARGET_CMODEL rs6000_current_cmodel
#define SET_CMODEL(opt) rs6000_current_cmodel = opt

/* Processor used only when no cpu is known at all (no -mcpu, no
   --with-cpu, e.g. --with-cpu=default64): plain 64-bit PowerPC, matching
   TARGET_DEFAULT's ISA flags.  config.gcc defaults --with-cpu to xenon for
   powerpc64-xenon-elf (and to powerpc64 for other powerpc64-*-elf), so the
   driver and cc1 normally use -mcpu=xenon.  */
#undef  PROCESSOR_DEFAULT
#define PROCESSOR_DEFAULT PROCESSOR_POWERPC64
#undef  PROCESSOR_DEFAULT64
#define PROCESSOR_DEFAULT64 PROCESSOR_POWERPC64

/* We don't need to generate entries in .fixup, except when
   -mrelocatable or -mrelocatable-lib is given.  */
#undef RELOCATABLE_NEEDS_FIXUP
#define RELOCATABLE_NEEDS_FIXUP \
  (rs6000_isa_flags & rs6000_isa_flags_explicit & OPTION_MASK_RELOCATABLE)

/* The -mcall-ABI name.  "linux" selects ABI_AIX (ELFv1) for -m64 and
   ABI_V4 for -m32 in sysv4.h's SUBTARGET_OVERRIDE_OPTIONS; it does not
   imply any Linux system support.  */
#undef  RS6000_ABI_NAME
#define RS6000_ABI_NAME "linux"

#define INVALID_64BIT "%<-m%s%> not supported in this configuration"
#define INVALID_32BIT INVALID_64BIT

/* ELFv1 is the default and the tested ABI; -mabi=elfv2 selects ELFv2.  */
#define ELFv2_ABI_CHECK (rs6000_elf_abi == 2)

/* The 64-bit option checks are shared with linux64.h and freebsd64.h.
   They default to -mcmodel=medium; this target defaults to
   -mcmodel=small (TOC-indirect addressing, 16-bit TOC offsets; GNU ld
   splits an oversized TOC into multiple TOC groups), so put the small
   model back unless the user asked for a model explicitly.  */
#undef  SUBSUBTARGET_OVERRIDE_OPTIONS
#define SUBSUBTARGET_OVERRIDE_OPTIONS					\
  do									\
    {									\
      rs6000_linux64_override_options ();				\
      if (TARGET_64BIT							\
	  && !OPTION_SET_P (rs6000_current_cmodel)			\
	  && rs6000_current_cmodel != CMODEL_SMALL)			\
	{								\
	  SET_CMODEL (CMODEL_SMALL);					\
	  if (!OPTION_SET_P (TARGET_NO_FP_IN_TOC))			\
	    TARGET_NO_FP_IN_TOC = 0;					\
	  if (!OPTION_SET_P (TARGET_NO_SUM_IN_TOC))			\
	    TARGET_NO_SUM_IN_TOC = 0;					\
	}								\
    }									\
  while (0)

/* Assembler: -a64 by default, -a32 for a future -m32 multilib.  */
#undef	ASM_SPEC
#define	ASM_SPEC "%{m32:%(asm_spec32)}%{!m32:%(asm_spec64)} %(asm_spec_common)"

#define ASM_SPEC32 "-a32 \
%{mrelocatable} %{mrelocatable-lib} %{" FPIE_OR_FPIC_SPEC ":-K PIC} \
%{memb|msdata=eabi: -memb}"

#define ASM_SPEC64 "-a64"

#define ASM_SPEC_COMMON "%(asm_cpu) \
%{,assembler|,assembler-with-cpp: %{mregnames} %{mno-regnames}}" \
  ENDIAN_SELECT(" -mbig", " -mlittle", " -mbig")

/* Linker: pick the emulation explicitly so -m32 links work later.  */
#define LINK_OS_SPEC32 "-m elf32ppc"
#define LINK_OS_SPEC64 "-m elf64ppc"

#undef	SUBSUBTARGET_EXTRA_SPECS
#define SUBSUBTARGET_EXTRA_SPECS					\
  { "asm_spec_common",		ASM_SPEC_COMMON },			\
  { "asm_spec32",		ASM_SPEC32 },				\
  { "asm_spec64",		ASM_SPEC64 },				\
  { "link_os_spec32",		LINK_OS_SPEC32 },			\
  { "link_os_spec64",		LINK_OS_SPEC64 },

#undef  LINK_OS_DEFAULT_SPEC
#define LINK_OS_DEFAULT_SPEC \
  "%{m32:%(link_os_spec32)}%{!m32:%(link_os_spec64)}"

/* Static linking only: no shared libraries, no dynamic linker.  */
#undef  LINK_SHLIB_SPEC
#define LINK_SHLIB_SPEC \
  "%{shared|symbolic:%eshared libraries are not supported on this target} \
   %{!r:-static}"

/* No default load address: the image is placed by the linker script
   (-T) of the board support package; ld's elf64ppc default script
   already links at 0x10000000, below 4 GiB.  */
#undef	LINK_START_DEFAULT_SPEC
#define	LINK_START_DEFAULT_SPEC ""

/* newlib start files.  The board support library (libxenon) supplies
   crt0.o (the entry point); libgcc supplies crti.o/crtn.o (the _init and
   _fini frames newlib's __libc_init_array calls) and crtbegin.o/crtend.o.  */
#undef	STARTFILE_DEFAULT_SPEC
#define	STARTFILE_DEFAULT_SPEC "%:if-exists(crt0%O%s) crti%O%s crtbegin%O%s"

#undef	ENDFILE_DEFAULT_SPEC
#define	ENDFILE_DEFAULT_SPEC "crtend%O%s crtn%O%s"

#undef	LIB_DEFAULT_SPEC
#define	LIB_DEFAULT_SPEC "-lc"

/* libgcc, libc and the board support library refer to each other.  */
#undef	LINK_GCC_C_SEQUENCE_SPEC
#define	LINK_GCC_C_SEQUENCE_SPEC \
  "%{nolibc:%G;:--start-group %G %L --end-group}"

#undef	CPP_OS_DEFAULT_SPEC
#define	CPP_OS_DEFAULT_SPEC ""

#undef	MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "mbig", "m64" }

#undef  TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()			\
  do							\
    {							\
      if (TARGET_XENON_OS)				\
	{						\
	  builtin_define ("__XENON__");			\
	  builtin_assert ("system=xenon");		\
	}						\
      if (TARGET_64BIT)					\
	{						\
	  builtin_define ("__PPC__");			\
	  builtin_define ("__PPC64__");			\
	  builtin_define ("__powerpc__");		\
	  builtin_define ("__powerpc64__");		\
	  builtin_assert ("cpu=powerpc64");		\
	  builtin_assert ("machine=powerpc64");		\
	}						\
      else						\
	{						\
	  builtin_define_std ("PPC");			\
	  builtin_define_std ("powerpc");		\
	  builtin_assert ("cpu=powerpc");		\
	  builtin_assert ("machine=powerpc");		\
	  TARGET_OS_SYSV_CPP_BUILTINS ();		\
	}						\
    }							\
  while (0)

#ifndef TARGET_XENON_OS
#define TARGET_XENON_OS 0
#endif

/* PowerPC-64 increases natural record alignment to doubleword if
   the first field is an FP double, only if in power alignment mode.  */
#undef  ROUND_TYPE_ALIGN
#define ROUND_TYPE_ALIGN(STRUCT, COMPUTED, SPECIFIED)			\
  ((TARGET_64BIT							\
    && RECORD_OR_UNION_TYPE_P (STRUCT)					\
    && TARGET_ALIGN_NATURAL == 0)					\
   ? rs6000_special_round_type_align (STRUCT, COMPUTED, SPECIFIED)	\
   : MAX ((COMPUTED), (SPECIFIED)))

/* Use the default for compiling target libs.  */
#ifdef IN_TARGET_LIBS
#undef TARGET_ALIGN_NATURAL
#define TARGET_ALIGN_NATURAL 1
#endif

/* Indicate that jump tables go in the text section.  */
#undef  JUMP_TABLES_IN_TEXT_SECTION
#define JUMP_TABLES_IN_TEXT_SECTION TARGET_64BIT

/* Aggregates in the parameter area are padded upward, as in the 64-bit
   ELF ABI (same settings as linux64.h and freebsd64.h).  */
#define AGGREGATE_PADDING_FIXED TARGET_64BIT
#define AGGREGATES_PAD_UPWARD_ALWAYS 0

/* Specify padding for the last element of a block move between
   registers and memory.  FIRST is nonzero if this is the only
   element.  */
#define BLOCK_REG_PADDING(MODE, TYPE, FIRST) \
  (!(FIRST) ? PAD_UPWARD : targetm.calls.function_arg_padding (MODE, TYPE))

/* Types (LP64 for -m64).  */
#undef  SIZE_TYPE
#define SIZE_TYPE (TARGET_64BIT ? "long unsigned int" : "unsigned int")

#undef  PTRDIFF_TYPE
#define PTRDIFF_TYPE (TARGET_64BIT ? "long int" : "int")

#undef  WCHAR_TYPE
#define WCHAR_TYPE (TARGET_64BIT ? "int" : "long int")
#undef  WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 32

/* Function profiling bits.  */
#undef  RS6000_MCOUNT
#define RS6000_MCOUNT "_mcount"

#define PROFILE_HOOK(LABEL) \
  do { if (TARGET_64BIT) output_profile_hook (LABEL); } while (0)

/* _init and _fini functions are built from bits spread across many
   object files, each potentially with a different TOC pointer.  For
   that reason, place a nop after the call so that the linker can
   restore the TOC pointer if a TOC adjusting call stub is needed.  */
#ifdef __powerpc64__
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)	\
  asm (SECTION_OP "\n"					\
"	bl " #FUNC "\n"					\
"	nop\n"						\
"	.previous");
#endif

/* Out-of-line FP save and restore routines (the 64-bit ones are
   generated by GNU ld).  POWERPC_LINUX selects the matching names in
   rs6000-logue.cc and IBM (not IEEE) 128-bit long double as the
   -mlong-double-128 format, as on rtems.h; it does not imply Linux.  */
#define POWERPC_LINUX

#undef  SAVE_FP_PREFIX
#define SAVE_FP_PREFIX (TARGET_64BIT ? "._savef" : "_savefpr_")
#undef  SAVE_FP_SUFFIX
#define SAVE_FP_SUFFIX ""
#undef  RESTORE_FP_PREFIX
#define RESTORE_FP_PREFIX (TARGET_64BIT ? "._restf" : "_restfpr_")
#undef  RESTORE_FP_SUFFIX
#define RESTORE_FP_SUFFIX ""

/* Select a format to encode pointers in exception handling data.  CODE
   is 0 for data, 1 for code labels, 2 for function pointers.  GLOBAL is
   true if the symbol may be affected by dynamic relocations.  */
#undef	ASM_PREFERRED_EH_DATA_FORMAT
#define	ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL) \
  (TARGET_64BIT || flag_pic						\
   ? (((GLOBAL) ? DW_EH_PE_indirect : 0) | DW_EH_PE_pcrel		\
      | (TARGET_64BIT ? DW_EH_PE_udata8 : DW_EH_PE_sdata4))		\
   : DW_EH_PE_absptr)

/* Static stack checking is supported by means of probes.  */
#define STACK_CHECK_STATIC_BUILTIN 1

/* The default value isn't sufficient in 64-bit mode.  */
#define STACK_CHECK_PROTECT (TARGET_64BIT ? 16 * 1024 : 12 * 1024)

/* Use standard DWARF numbering for DWARF debugging information.  */
#define RS6000_USE_DWARF_NUMBERING

/* The unwinder covers the Xenon VMX128 registers v32-v127 (plan 4.3):
   their .eh_frame numbers are 120-215 (rs6000_debugger_regno), after the
   DMRs (112-119), so 216 columns.  v64-v127 are callee-saved and the
   prologue describes their saves in the CFI, so the unwinder must restore
   them when it installs a landing pad.  _Unwind_Context and
   _Unwind_FrameState in libgcc grow accordingly (they are private to
   libgcc's unwinder), and rs6000_init_dwarf_reg_sizes_extra gives the
   new columns their 16-byte size.  .debug_frame (rarely used: GCC emits
   .cfi directives, which put the .eh_frame numbers in both) has 1156-1251
   for v32-v127.  */
#undef  DWARF_FRAME_REGISTERS
#define DWARF_FRAME_REGISTERS 216

/* PowerPC64 word-aligns FP doubles when -malign-power is given.  */
#undef  ADJUST_FIELD_ALIGN
#define ADJUST_FIELD_ALIGN(FIELD, TYPE, COMPUTED) \
  ((TARGET_64BIT							\
    && TARGET_ALIGN_NATURAL == 0					\
    && TYPE_MODE (strip_array_types (TYPE)) == DFmode)			\
   ? MIN ((COMPUTED), 32)						\
   : (COMPUTED))

#undef  TOC_SECTION_ASM_OP
#define TOC_SECTION_ASM_OP \
  (TARGET_64BIT                                         \
   ? "\t.section\t\".toc\",\"aw\""                      \
   : "\t.section\t\".got\",\"aw\"")

#undef  MINIMAL_TOC_SECTION_ASM_OP
#define MINIMAL_TOC_SECTION_ASM_OP \
  (TARGET_64BIT                                         \
   ? "\t.section\t\".toc1\",\"aw\""                     \
   : (flag_pic						\
      ? "\t.section\t\".got2\",\"aw\""                  \
      : "\t.section\t\".got1\",\"aw\""))

/* This is how to declare the size of a function.  */
#undef  ASM_DECLARE_FUNCTION_SIZE
#define ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)                    \
  do                                                                    \
    {                                                                   \
      if (!flag_inhibit_size_directive)                                 \
        {                                                               \
          fputs ("\t.size\t", (FILE));                                  \
          if (TARGET_64BIT && DOT_SYMBOLS)                              \
            putc ('.', (FILE));                                         \
          assemble_name ((FILE), (FNAME));                              \
          fputs (",.-", (FILE));                                        \
          rs6000_output_function_entry (FILE, FNAME);                   \
          putc ('\n', (FILE));                                          \
        }                                                               \
    }                                                                   \
  while (0)

/* Return nonzero if this entry is to be written into the constant
   pool in a special way.  We do so if this is a SYMBOL_REF, LABEL_REF
   or a CONST containing one of them.  If -mfp-in-toc (the default),
   we also do this for floating-point constants.  We actually can only
   do this if the FP formats of the target and host machines are the
   same, but we can't check that since not every file that uses these
   target macros includes real.h.  We also do this when we can write the
   entry into the TOC and the entry is not larger than a TOC entry.
   With the medium model, integer constants are materialised outside the
   TOC (as in linux64.h).  */
#undef  ASM_OUTPUT_SPECIAL_POOL_ENTRY_P
#define ASM_OUTPUT_SPECIAL_POOL_ENTRY_P(X, MODE)			\
  (TARGET_TOC								\
   && (SYMBOL_REF_P (X)							\
       || (GET_CODE (X) == CONST && GET_CODE (XEXP (X, 0)) == PLUS	\
	   && SYMBOL_REF_P (XEXP (XEXP (X, 0), 0)))			\
       || GET_CODE (X) == LABEL_REF					\
       || (CONST_INT_P (X)						\
	   && TARGET_CMODEL != CMODEL_MEDIUM				\
	   && GET_MODE_BITSIZE (MODE) <= GET_MODE_BITSIZE (Pmode))	\
       || (CONST_DOUBLE_P (X)						\
	   && ((TARGET_64BIT						\
		&& (TARGET_MINIMAL_TOC					\
		    || (SCALAR_FLOAT_MODE_P (GET_MODE (X))		\
			&& ! TARGET_NO_FP_IN_TOC)))			\
	       || (!TARGET_64BIT					\
		   && !TARGET_NO_FP_IN_TOC				\
		   && SCALAR_FLOAT_MODE_P (GET_MODE (X))		\
		   && BITS_PER_WORD == HOST_BITS_PER_INT)))))
