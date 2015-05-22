int null_ptr_1(int x) {
  if (!x) {
    return x;
  }
  return 1;
}

int null_ptr_2(int x) {
  if (x == 0) {
    return x;
  }
  return 1;
}

int null_ptr_3(int x) {
  if (x != 0) {
    return 1;
  }
  return x;
}

int null_ptr_4(int x) {
  if (x) {
    return 1;
  }
  return 0;
}

int not_null_ptr5(int* x) {
  if (x[0] > 12) {
    return x[9];
  }
  return x[0];
}
