#include "error_lib.h"

int error_case = -1; // invalid value on initalization

int error_counter = 0;

bool injection_active = false;

bool check_active = false;

unsigned injected_location = 0;

int injected_line_number = -1;

void error_lib_initialize() {
  error_case = klee_int("error_case");
  klee_assume(error_case > 0);
}

void start_error_injection() { check_active = true; }

void stop_error_injection() {
  check_active = false;
  klee_assume(injected_location > 0);
}

static bool injection_status_backup = false;

void interrupt_error_injection() {
  injection_status_backup = check_active;
  check_active = false;
}

void restore_error_injection() { check_active = injection_status_backup; }

int bitflip(int y) {
  int res = klee_int("bitflip_result");
  int t = res ^ y;
  klee_assume(t != 0);
  klee_assume((t & (t - 1)) == 0);
  return res;
}

int maybe_bitflip(int y, unsigned loc, int line_number) {
  ++error_counter;
  if (injection_active && check_active) {
    if (error_counter == error_case) {
      injected_location = loc;
      injected_line_number = line_number;
      return bitflip(y);
    }
  }
  return y;
}


