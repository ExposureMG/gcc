/* Xenon data layout (plan 5.3): a line-crossing access costs ~55 cycles, so
   global arrays of a line or more are line (128-byte) aligned and arrays of
   16 bytes or more quadword aligned; FP loads and stores take an alignment
   interrupt unless EA % 4 == 0, so a packed double is not loaded with lfd
   from its unaligned address.  */
/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon -fno-common -fno-section-anchors" } */
/* { dg-final { scan-assembler {\.lcomm\s+big,256,128|\.align 7\n(\s+\.[a-z]+[^\n]*\n)*big:} } } */
/* { dg-final { scan-assembler {\.lcomm\s+mid,20,16|\.align 4\n(\s+\.[a-z]+[^\n]*\n)*mid:} } } */
/* { dg-final { scan-assembler {\.align 7\n(\s+\.[a-z]+[^\n]*\n)*ibig:} } } */
/* { dg-final { scan-assembler {\.align 4\n(\s+\.[a-z]+[^\n]*\n)*imid:} } } */
/* { dg-final { scan-assembler {\.align 2\n(\s+\.[a-z]+[^\n]*\n)*ismall:} } } */
/* FP loads and stores only touch the (aligned) stack; the packed fields
   are accessed with integer loads and stores (no trap, free in a line).  */
/* { dg-final { scan-assembler-not {\m(lf|stf)[a-z]*u? [0-9]+,-?[0-9]+\((?!1\))[0-9]+\)} } } */
/* { dg-final { scan-assembler-not {\m(lf|stf)[a-z]*x [0-9]+,(?!0,)} } } */
/* { dg-final { scan-assembler {\mstw [0-9]+,17\(3\)} } } */
/* { dg-final { scan-assembler-not {\mlbz\M} } } */

int big[64];
char mid[20];
int ibig[64] = { 1 };
char imid[20] = { 1 };
int ismall[3] = { 1 };

struct __attribute__ ((packed)) s { char c; double d; long l; int i; };
double get_d (struct s *p) { return p->d; }
void set_d (struct s *p, double v) { p->d = v; }
double l2d (struct s *p) { return p->l; }
void d2l (struct s *p, double v) { p->l = v; }
void d2i (struct s *p, double v) { p->i = v; }
