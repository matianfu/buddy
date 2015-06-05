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
 * File: main.c
 *
 * Created on: Jun 5, 2015
 * Author: Tianfu Ma (matianfu@gmail.com)
 */

#include <stdlib.h>
#include "buddy.h"

void test001() {
  void * p1, *p2;
  p1 = bmalloc(4);
  p2 = bmalloc(13);

  bfree(p2);
  bfree(p1);

  print_buddy();
}

void test002() {
  void * p1, *p2;
  p1 = bmalloc(4);
  p2 = bmalloc(13);

  bfree(p1);
  bfree(p2);

  print_buddy();
}

void test003() {
  void * p1, * p2, * p3;
  p1 = bmalloc(25);
  p2 = bmalloc(9);
  p3 = bmalloc(7);

  bfree(p2);
  bfree(p3);
  bfree(p1);

  print_buddy();
}

int main() {

  buddy_t * buddy = (buddy_t*) malloc(sizeof(buddy_t));
  buddy_init(buddy);
  test001();
  test002();
  test003();
  buddy_deinit();
  free(buddy);
  return 0;
}
