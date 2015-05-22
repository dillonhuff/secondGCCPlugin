#include <stdio.h>

int null_ptr_1(int* x) {
  if (!x) {
    return x[0];
  }
  return 1;
}

int null_ptr_2(int* x) {
  if (x == 0) {
    return x[0];
  }
  return 1;
}

int null_ptr_3(int* x) {
  if (x != 0) {
    return 1;
  }
  return x[0];
}

int null_ptr_4(int* x) {
  if (x) {
    return 1;
  }
  return x[4];
}

int null_ptr_5(int* x) {
  if (x == NULL) {
    return 1;
  }
  return x[3];
}

int null_ptr_6(int* x) {
  if (0 == x) {
    return x[19];
  }
  return 1;
}

int null_ptr_7(int* x) {
  if (NULL == x) {
    return 1;
  }
  return x[23];
}

int null_ptr_8(int* x) {
  if (0 != x) {
    return 1;
  }
  int y = *(x + 2);
  return y;
}
