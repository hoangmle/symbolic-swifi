#include "error_lib.h"

int error_case = -1; // invalid value on initalization

int error_counter = 0;

bool injection_active = false;

bool check_active = false;

unsigned injected_location = 0;

int injected_line_number = -1;

void error_lib_initialize() {}

void start_error_injection() { check_active = true; }

void stop_error_injection() { check_active = false; }

static bool injection_status_backup = false;

void interrupt_error_injection() {
  injection_status_backup = check_active;
  check_active = false;
}

void restore_error_injection() { check_active = injection_status_backup; }

int bitflip(int y) {
  return y;
}

int maybe_bitflip(int y, unsigned loc, int line_number) {
  if (injection_active && check_active) ++error_counter;
  return y;
}

