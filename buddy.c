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
 * File: buddy.c
 *
 * Created on: Jun 5, 2015
 * Author: Tianfu Ma (matianfu@gmail.com)
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "buddy.h"

buddy_t * BUDDY = 0;

pointer bmalloc(int size) {

  int i, order;
  pointer block, buddy;

  // calculate minimal order for this size
  i = 0;
  while (BLOCKSIZE(i) < size + 1) // one more byte for storing order
    i++;

  order = i = (i < MIN_ORDER) ? MIN_ORDER : i;

  // level up until non-null list found
  for (;; i++) {
    if (i > MAX_ORDER)
      return NULL;
    if (BUDDY->freelist[i])
      break;
  }

  // remove the block out of list
  block = BUDDY->freelist[i];
  BUDDY->freelist[i] = *(pointer*) BUDDY->freelist[i];

  // split until i == order
  while (i-- > order) {
    buddy = BUDDYOF(block, i);
    BUDDY->freelist[i] = buddy;
  }

  // store order in previous byte
  *((uint8_t*) (block - 1)) = order;
  return block;
}

void bfree(pointer block) {

  int i;
  pointer buddy;
  pointer * p;

  // fetch order in previous byte
  i = *((uint8_t*) (block - 1));

  for (;; i++) {
    // calculate buddy
    buddy = BUDDYOF(block, i);
    p = &(BUDDY->freelist[i]);

    // find buddy in list
    while ((*p != NULL) && (*p != buddy))
      p = (pointer *) *p;

    // not found, insert into list
    if (*p != buddy) {
      *(pointer*) block = BUDDY->freelist[i];
      BUDDY->freelist[i] = block;
      return;
    }
    // found, merged block starts from the lower one
    block = (block < buddy) ? block : buddy;
    // remove buddy out of list
    *p = *(pointer*) *p;
  }
}

void buddy_init(buddy_t * buddy) {

  BUDDY = buddy;
  memset(BUDDY, 0, sizeof(buddy_t));
  BUDDY->freelist[MAX_ORDER] = BUDDY->pool;
}

void buddy_deinit() {

  BUDDY = 0;
}

/*
 * The following functions are for simple tests.
 */

static int count_blocks(int i) {

  int count = 0;
  pointer * p = &(BUDDY->freelist[i]);

  while (*p != NULL) {
    count++;
    p = (pointer*) *p;
  }
  return count;
}

static int total_free() {

  int i, bytecount = 0;

  for (i = 0; i <= MAX_ORDER; i++) {
    bytecount += count_blocks(i) * BLOCKSIZE(i);
  }
  return bytecount;
}

static void print_list(int i) {

  printf("freelist[%d]: \n", i);

  pointer *p = &BUDDY->freelist[i];
  while (*p != NULL) {
    printf("    0x%08lx, 0x%08lx\n", (uintptr_t) *p, (uintptr_t) *p - (uintptr_t) BUDDY->pool);
    p = (pointer*) *p;
  }
}

void print_buddy() {

  int i;

  printf("========================================\n");
  printf("MEMPOOL size: %d\n", POOLSIZE);
  printf("MEMPOOL start @ 0x%08x\n", (unsigned int) (uintptr_t) BUDDY->pool);
  printf("total free: %d\n", total_free());

  for (i = 0; i <= MAX_ORDER; i++) {
    print_list(i);
  }
}
