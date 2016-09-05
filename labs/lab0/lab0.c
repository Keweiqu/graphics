#include "lab0.h"
#include <stdio.h>
#include <stdlib.h>

Integer *make_int(int x) {
  Integer *i;
  if((i = (Integer *) malloc(sizeof(Integer))) != NULL) {
    i->data = x;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return i;
}

Double *make_double(double x) {
  Double *i;
  if((i = ( Double*) malloc(sizeof(Double))) != NULL) {
    i->data = x;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return i;

}

Number *make_number(int type, void *num) {
  Number *i;
  if((i = (Number*) malloc(sizeof(Number))) != NULL) {
    i->type = type;
    i->num = num;
  } else {
    printf("Out of memory\n");
    exit(0);
  }
  return i;
}

int main() {
  int init = 0;
  Number* prev;
  Number* head;
  unsigned int i;
  for(i = 0; i < 10; i++) {
    Number* temp;
    if(i % 2 == 0) {
      Integer* integer = make_int(i);
      temp = make_number(INT, integer);
    } else {
      Double* doub = make_double((double) i);
      temp = make_number(DOUBLE, doub);
    }
    if(!init) {
      head = temp;
      prev = temp;
      init = 1;
    } else {
      prev->next = temp;
      temp->prev = prev;
      prev = temp;
    }
  }
  Number* current = head;
  //  printf("%f", ((Double *)current->num)->data);

  for(i = 0; i < 10; i++) {
    if(i % 2 == 0){
      printf("Even number %d, %d\n", i, ((Integer *) current->num)->data);
    } else {
      printf("Odd number %d, %f\n", i, ((Double *) current->num)->data);
    }
    current = current->next;
  }


}
