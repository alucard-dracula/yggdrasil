/*
 * affinity6.c
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
 * Test thread CPU affinity from thread attributes.
 *
 */

#if ! defined(WINCE)

#include "test.h"

typedef union
{
        /* Violates opacity */
        cpu_set_t cpuset;
        unsigned long int bits;  /* To stop GCC complaining about %lx args to printf */
} cpuset_to_ulint;

void *
mythread(void * arg)
{
  pthread_attr_t *attrPtr = (pthread_attr_t *) arg;
  cpu_set_t threadCpus, attrCpus;

  assert(pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &threadCpus) == 0);
  assert(pthread_attr_getaffinity_np(attrPtr, sizeof(cpu_set_t), &attrCpus) == 0);
  assert(CPU_EQUAL(&attrCpus, &threadCpus));

  return (void*) 0;
}

int
main()
{
  unsigned int cpu;
  pthread_t tid;
  pthread_attr_t attr1, attr2;
  cpu_set_t threadCpus;
  cpu_set_t keepCpus;
  pthread_t self = pthread_self();

  if (pthread_getaffinity_np(self, sizeof(cpu_set_t), &threadCpus) == ENOSYS)
    {
      printf("pthread_get/set_affinity_np API not supported for this platform: skipping test.");
      return 0;
    }

  assert(pthread_attr_init(&attr1) == 0);
  assert(pthread_attr_init(&attr2) == 0);

  CPU_ZERO(&keepCpus);
  for (cpu = 1; cpu < sizeof(cpu_set_t)*8; cpu += 2)
    {
          CPU_SET(cpu, &keepCpus);                                      /* 0b10101010101010101010101010101010 */
    }

  assert(pthread_getaffinity_np(self, sizeof(cpu_set_t), &threadCpus) == 0);

  if (CPU_COUNT(&threadCpus) > 1)
    {
          assert(pthread_attr_setaffinity_np(&attr1, sizeof(cpu_set_t), &threadCpus) == 0);
          CPU_AND(&threadCpus, &threadCpus, &keepCpus);
          assert(pthread_attr_setaffinity_np(&attr2, sizeof(cpu_set_t), &threadCpus) == 0);

          assert(pthread_create(&tid, &attr1, mythread, (void *) &attr1) == 0);
          assert(pthread_join(tid, NULL) == 0);
          assert(pthread_create(&tid, &attr2, mythread, (void *) &attr2) == 0);
          assert(pthread_join(tid, NULL) == 0);
    }
  assert(pthread_attr_destroy(&attr1) == 0);
  assert(pthread_attr_destroy(&attr2) == 0);
  return 0;
}

#else

#include <stdio.h>

int
main()
{
  fprintf(stderr, "Test N/A for this target environment.\n");
  return 0;
}

#endif
