int isquare(int i) {
  return i * i;
}

int obvious_div_by_zero(int x) {
  return x / 0;
}

int no_div_by_zero(int x) {
  return x / 2;
}

double cast_arg(double x, int y) {
  return x / y;
}

double strange(double x, double y) {
  if (x > 12 && y <= 19) {
    return x + 1;
  }
  return y*y + x;
}
