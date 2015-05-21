int isquare(int i) {
  return i * i;
}

int obvious_div_by_zero(int x) {
  return x / 0;
}

int no_div_by_zero(int x) {
  return x / 2;
}

int div_by_var(int x, int y) {
  return x / y;
}

double cast_arg(double x, int y) {
  return x / y;
}

float float_div(double x, double y, double* m, int k, int p) {
  int i;
  for (i = 0; i < k; i++) {
    if (m[i] < p) {
      return x / y;
    }
  }
  return x;
}

double strange(double x, double y) {
  if (x > 12 && y <= 19) {
    return x + 1;
  }
  return y*y + x;
}
