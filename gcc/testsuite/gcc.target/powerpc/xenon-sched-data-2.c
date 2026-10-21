/* Xenon FP loads and stores take an alignment interrupt unless EA % 4 == 0
   (FINDINGS 1.4).  Also without 64-bit GPRs (-m32 -mno-powerpc64) an
   under-aligned SF/DF/SI/DI access never uses lfd/stfd/lfs/stfs/stfiwx at
   the unaligned address: 8-byte values are moved as two words.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -m32 -mno-powerpc64 -mdejagnu-cpu=xenon" } */
/* FP accesses only to the (aligned) stack.  */
/* { dg-final { scan-assembler-not {\m(lf|stf)[a-z]*u? [0-9]+,-?[0-9]+\((?!1\))[0-9]+\)} } } */
/* { dg-final { scan-assembler-not {\m(lf|stf)[a-z]*x [0-9]+,(?!0,)} } } */
/* { dg-final { scan-assembler-not {\mstfiwx [0-9]+,(?!0,)} } } */

struct __attribute__ ((packed)) s { char c; double d; long long l; int i; float f; };
double get_d (struct s *p) { return p->d; }
void set_d (struct s *p, double v) { p->d = v; }
double l2d (struct s *p) { return p->l; }
void d2l (struct s *p, double v) { p->l = v; }
void d2i (struct s *p, double v) { p->i = v; }
float get_f (struct s *p) { return p->f; }
void set_f (struct s *p, float v) { p->f = v; }
void cpy (struct s *p, char *b) { __builtin_memcpy (&p->d, b + 1, 8); }
