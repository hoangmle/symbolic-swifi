#include "error_lib.h"

int npo2(int x) {
  start_error_injection();
  CHECK(x > 0);
  CHECK(x < (1 << 30));

  int result = x;
  for (int i = 1; i < sizeof(int) * 8; i *= 2) {
    CHECK(i > 0);
    result = result | (result >> i);
  }
  result = result + 1;

  stop_error_injection();
  CHECK(result <= (x << 1));
  CHECK((result & (result - 1)) == 0);
  CHECK(result > x);
  return result;
}

int main() {
  error_lib_initialize();
  int num = klee_int("num");

  injection_active = true;
  int ans1 = npo2(num);

  injection_active = false;
  int ans2 = npo2(num);

  assert(ans1 == ans2);

  return 0;
}
