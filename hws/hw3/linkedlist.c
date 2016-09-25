#include "linkedlist.h"

Node* create_node(void *data, enum LL_Type t) {
  Node *n;
  if((n = (Node *) malloc(sizeof(Node))) != NULL) {
    n->data = data;
    n->type = t;
  }
  return n;
}

void free_node(Node *n) {
  free(n);
}

Node* create_linkedlist() {
  Node* head = create_node(NULL, HEAD_TAIL);
  Node* tail = create_node(NULL, HEAD_TAIL);
  head->prev = NULL;
  head->next = tail;
  tail->prev = head;
  tail->next = NULL;
  return head;
}

void free_linkedlist(Node* head) {
  while(head->next != NULL) {
    Node* current = head->next;
    head->next = head->next->next;
    free_node(current);
  }
  free(head);
}
