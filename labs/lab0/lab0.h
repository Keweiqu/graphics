#define INT 0
#define DOUBLE 1

typedef struct _number {
  int type; 
  void *num;
  struct _number *prev;
  struct _number *next;
} Number;

typedef struct _integer {
  int data;
} Integer;

typedef struct _double {
  double data;
} Double;

Integer *make_int(int x);
Double *make_double(double x);
Number *make_number(int type, void *num);
