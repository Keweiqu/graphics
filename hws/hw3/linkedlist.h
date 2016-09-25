#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdlib.h>

enum LL_Type { HEAD_TAIL = -1, VAL = 1};
typedef struct _node {
  enum LL_Type type;
  void *data;
  struct _node *prev;
struct _node *next;
} Node;

#endif
