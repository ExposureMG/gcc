/* Xenon executes lwsync (32 cycles against 162 for sync), so acquire and
   release fences use it; sequentially consistent ones use sync.  */
/* { dg-do compile { target powerpc*-*-* } } */
/* { dg-options "-O2 -mdejagnu-cpu=xenon" } */
/* { dg-final { scan-assembler {\mlwsync\M} } } */
/* { dg-final { scan-assembler {\msync\M} } } */
/* { dg-final { scan-assembler {\mlwarx\M} } } */
/* { dg-final { scan-assembler {\mldarx\M} } } */
/* { dg-final { scan-assembler-not {\m(lbarx|lharx|lqarx|stbcx|sthcx|stqcx)\M} } } */
/* { dg-final { scan-assembler-not {\mmcrxr\M} } } */

int x;
long y;

void fence_acq (void) { __atomic_thread_fence (__ATOMIC_ACQUIRE); }
void fence_rel (void) { __atomic_thread_fence (__ATOMIC_RELEASE); }
void fence_acq_rel (void) { __atomic_thread_fence (__ATOMIC_ACQ_REL); }
void fence_sc (void) { __atomic_thread_fence (__ATOMIC_SEQ_CST); }
int load_acq (void) { return __atomic_load_n (&x, __ATOMIC_ACQUIRE); }
void store_rel (int v) { __atomic_store_n (&x, v, __ATOMIC_RELEASE); }
int fetch_add (int v) { return __atomic_fetch_add (&x, v, __ATOMIC_ACQ_REL); }
long fetch_add_l (long v) { return __atomic_fetch_add (&y, v, __ATOMIC_SEQ_CST); }
int cas (int o, int n)
{
  return __atomic_compare_exchange_n (&x, &o, n, 0, __ATOMIC_ACQUIRE,
				      __ATOMIC_RELAXED);
}
char cas_c (char *p, char o, char n)
{
  __atomic_compare_exchange_n (p, &o, n, 0, __ATOMIC_SEQ_CST,
			       __ATOMIC_SEQ_CST);
  return o;
}
short xchg_s (short *p, short v)
{
  return __atomic_exchange_n (p, v, __ATOMIC_RELEASE);
}
void sync_sync (void) { __sync_synchronize (); }
