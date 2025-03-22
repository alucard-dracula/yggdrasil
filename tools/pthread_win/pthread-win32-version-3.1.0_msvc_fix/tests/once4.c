/*
 * once4.c
 *
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads4w - POSIX Threads for Windows
 *      Copyright 1998 John E. Bossom
 *      Copyright 1999-2018, Pthreads4w contributors
 *
 *      Homepage: https://sourceforge.net/projects/pthreads4w/
 *
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *
 *      https://sourceforge.net/p/pthreads4w/wiki/Contributors/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * --------------------------------------------------------------------------
 *
 * Create several pthread_once objects and channel several threads
 * through each. Make the init_routine cancelable and cancel them
 * waiters waiting. Vary the priorities.
 *
 * Depends on API functions:
 *	pthread_once()
 *	pthread_create()
 *      pthread_testcancel()
 *      pthread_cancel()
 *      pthread_once()
 */

#include "test.h"

#define NUM_THREADS 100 /* Targeting each once control */
#define NUM_ONCE    10

pthread_once_t o = PTHREAD_ONCE_INIT;
pthread_once_t once[NUM_ONCE];

typedef struct {
  int i;
  CRITICAL_SECTION cs;
} sharedInt_t;

static sharedInt_t numOnce;
static sharedInt_t numThreads;

typedef struct {
  int threadnum;
  int oncenum;
  int myPrio;
  HANDLE w32Thread;
} bag_t;

static bag_t threadbag[NUM_THREADS][NUM_ONCE];

CRITICAL_SECTION print_lock;

void
mycleanupfunc(void * arg)
{
  bag_t * bag = (bag_t *) arg;
  EnterCriticalSection(&print_lock);
  /*      once thrd  prio error */
  printf("%4d %4d %4d %4d\n",
	 bag->oncenum,
	 bag->threadnum,
	 bag->myPrio,
	 bag->myPrio - GetThreadPriority(bag->w32Thread));
  LeaveCriticalSection(&print_lock);
}

void
myinitfunc(void)
{
  EnterCriticalSection(&numOnce.cs);
  numOnce.i++;
  LeaveCriticalSection(&numOnce.cs);
  /* Simulate slow once routine so that following threads pile up behind it */
  Sleep(10);
  /* test for cancellation late so we're sure to have waiters. */
  pthread_testcancel();
}

void *
mythread(void * arg)
{
  bag_t * bag = (bag_t *) arg;
  struct sched_param param;

  /*
   * Cancel every thread. These threads are deferred cancelable only, so
   * only the thread performing the init_routine will see it (there are
   * no other cancellation points here). The result will be that every thread
   * eventually cancels only when it becomes the new initter.
   */
  pthread_t self = pthread_self();
  bag->w32Thread = pthread_getw32threadhandle_np(self);
  /*
   * Set priority between -2 and 2 inclusive.
   */
  bag->myPrio = (bag->threadnum % 5) - 2;
  param.sched_priority = bag->myPrio;
  pthread_setschedparam(self, SCHED_OTHER, &param);

  /* Trigger a cancellation at the next cancellation point in this thread */
  pthread_cancel(self);
#if 0
  pthread_cleanup_push(mycleanupfunc, arg);
  assert(pthread_once(&once[bag->oncenum], myinitfunc) == 0);
  pthread_cleanup_pop(1);
#else
  assert(pthread_once(&once[bag->oncenum], myinitfunc) == 0);
#endif
  EnterCriticalSection(&numThreads.cs);
  numThreads.i++;
  LeaveCriticalSection(&numThreads.cs);
  return 0;
}

int
main()
{
  pthread_t t[NUM_THREADS][NUM_ONCE];
  int i, j;

#if defined (__PTW32_CONFIG_MSVC6) && defined(__PTW32_CLEANUP_CXX)
  puts("If this test fails or hangs, rebuild the library with /EHa instead of /EHs.");
  puts("(This is a known issue with Microsoft VC++6.0.)");
  fflush(stdout);
#endif

  memset(&numOnce, 0, sizeof(sharedInt_t));
  memset(&numThreads, 0, sizeof(sharedInt_t));

  InitializeCriticalSection(&print_lock);
  InitializeCriticalSection(&numThreads.cs);
  InitializeCriticalSection(&numOnce.cs);

#if 0
  /*       once thrd  prio change */
  printf("once thrd  prio  error\n");
#endif

  /*
   * Set the priority class to realtime - otherwise normal
   * Windows random priority boosting will obscure any problems.
   */
  SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
  /* Set main thread to lower prio than threads */
  SetThreadPriority(GetCurrentThread(), -2);

  for (j = 0; j < NUM_ONCE; j++)
    {
      once[j] = o;

      for (i = 0; i < NUM_THREADS; i++)
        {
	  bag_t * bag = &threadbag[i][j];
	  bag->threadnum = i;
	  bag->oncenum = j;
          /* GCC build: create was failing with EAGAIN after 790 threads */
          while (0 != pthread_create(&t[i][j], NULL, mythread, (void *)bag))
            sched_yield();
        }
    }

  for (j = 0; j < NUM_ONCE; j++)
    for (i = 0; i < NUM_THREADS; i++)
      if (pthread_join(t[i][j], NULL) != 0)
        printf("Join failed for [thread,once] = [%d,%d]\n", i, j);

  /*
   * All threads will cancel, none will return normally from
   * pthread_once and so numThreads should never be incremented. However,
   * numOnce should be incremented by every thread (NUM_THREADS*NUM_ONCE).
   */
  assert(numOnce.i == NUM_ONCE * NUM_THREADS);
  assert(numThreads.i == 0);

  DeleteCriticalSection(&numOnce.cs);
  DeleteCriticalSection(&numThreads.cs);
  DeleteCriticalSection(&print_lock);

  return 0;
}
