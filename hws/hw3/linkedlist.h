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

Node* create_node(void *data, enum LL_Type t);
void free_node(Node *n);
Node* create_linkedlist();
void free_linkedlist(Node* head);
int get_ll_size(Node *head);
void append(Node *n, Node *tail);
void insert_before(Node *item, Node *pos);
void insert_after(Node *item, Node *pos);
void delete_last(Node* tail);

#endif
