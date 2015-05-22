double no_divide_by_zero_1(double m, double n, double p) {
  if (m == 0) {
    m = m + 1;
    return n / m;
  }
  return p;
}

double no_divide_by_zero_2(double m, double n, double p) {
  p = m;
  int x;
  if (p > 4) {
    x = 1 / m;
  }
  return x;
}

double no_divide_by_zero_3(double m, double n, double p) {
  p = m;
  n = p;
  if (p <= -12) {
    return 1 / m;
  }
  return 2.0;
}
