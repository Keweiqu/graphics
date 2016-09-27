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
  free_node(head);
}

int get_ll_size(Node *head) {
  int i = 0;
  Node * current = head;
  while(current->next->type != HEAD_TAIL) {
    i++;
    current->next = current->next->next;
  }
  return i;
}

void append(Node *n, Node *tail) {
  Node* temp = tail->prev;
  temp->next = n;
  n->prev = temp;
  n->next = tail;
  tail->prev = n;
}

void delete_last(Node *tail) {
  if (tail->prev->type != HEAD_TAIL) {   
    Node* temp = tail->prev->prev;
    free(temp->next);
    temp->next = tail;
    tail->prev = temp;
  }
}

void insert_before(Node *item, Node *pos) {
  Node* prev = pos->prev;
  item->prev = prev;
  item->next = pos;
  prev->next = item;
  pos->prev = item;
}

void insert_after(Node *item, Node *pos) {
  Node* next = pos->next;
  item->prev = pos;
  item->next = next;
  pos->next = item;
  next->prev = item;
}
