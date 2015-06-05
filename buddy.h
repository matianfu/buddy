/**
 * Copyright [2015] Tianfu Ma (matianfu@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * File: buddy.h
 *
 * Created on: Jun 5, 2015
 * Author: Tianfu Ma (matianfu@gmail.com)
 */

#ifndef BUDDY_H_
#define BUDDY_H_

#include <stdint.h>

/******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAX_ORDER       10
#define MIN_ORDER       4   // 2 ** 4 == 16 bytes

/* the order ranges 0..MAX_ORDER, the largest memblock is 2**(MAX_ORDER) */
#define POOLSIZE        (1 << MAX_ORDER)

/* blocks are of size 2**i. */
#define BLOCKSIZE(i)    (1 << (i))

/* the address of the buddy of a block from freelists[i]. */
#define _MEMBASE        ((uintptr_t)BUDDY->pool)
#define _OFFSET(b)      ((uintptr_t)b - _MEMBASE)
#define _BUDDYOF(b, i)  (_OFFSET(b) ^ (1 << (i)))
#define BUDDYOF(b, i)   ((pointer)( _BUDDYOF(b, i) + _MEMBASE))

// not used yet, for higher order memory alignment
#define ROUND4(x)       ((x % 4) ? (x / 4 + 1) * 4 : x)

/******************************************************************************
 *
 * Types & Globals
 *
 ******************************************************************************/

typedef void * pointer; /* used for untyped pointers */

typedef struct buddy {
  pointer freelist[MAX_ORDER + 2];  // one more slot for first block in pool
  uint8_t pool[POOLSIZE];
} buddy_t;

pointer bmalloc(int size);
void bfree(pointer block);

void buddy_init();
void buddy_deinit();

void print_buddy();

#endif /* BUDDY_H_ */
