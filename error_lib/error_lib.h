#ifndef _ERROR_LIB_H_
#define _ERROR_LIB_H_

#include "klee.h"
#include "assert.h"
#include "stdbool.h"

extern bool injection_active;
extern unsigned injected_location;
extern int injected_line_number;

/* error injection interface */

void error_lib_initialize();

int maybe_bitflip(int y, unsigned loc, int line_number);


/* CHECK / error interface */

void start_error_injection();
void stop_error_injection();

// inc/dec injection_active counter
void interrupt_error_injection();
void restore_error_injection();

#define CHECK(cond)                                                                                                    \
  interrupt_error_injection();                                                                                         \
  klee_assume(cond);                                                                                                   \
  restore_error_injection();

#endif // _ERROR_LIB_H_
