/*********************************************************
 * Minimal Arduino API surface for desktop simulation
 *
 * Uses C / POSIX headers only so macOS builds work even when
 * the C++ standard library include path is misconfigured.
 *********************************************************/
#ifndef ARDUINO_SHIM_H
#define ARDUINO_SHIM_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#ifndef constrain
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#endif

inline void staexe_sleep_ms(unsigned long ms) {
  struct timespec req;
  req.tv_sec = (time_t)(ms / 1000UL);
  req.tv_nsec = (long)((ms % 1000UL) * 1000000UL);
  while (nanosleep(&req, &req) == -1 && errno == EINTR) {
    /* retry remaining time */
  }
}

inline unsigned long millis() {
  static struct timeval start;
  static int primed = 0;
  struct timeval now;
  if (!primed) {
    gettimeofday(&start, 0);
    primed = 1;
  }
  gettimeofday(&now, 0);
  const long sec = (long)(now.tv_sec - start.tv_sec);
  const long usec = (long)(now.tv_usec - start.tv_usec);
  return (unsigned long)(sec * 1000L + usec / 1000L);
}

inline long random(long max) {
  if (max <= 0) return 0;
  return (long)(rand() % max);
}

/* Arduino random(min, max) — exclusive of max. */
inline long random(long min, long max) {
  if (max <= min) return min;
  return min + (long)(rand() % (max - min));
}

inline void delay(unsigned long ms) {
  staexe_sleep_ms(ms);
}

#endif /* ARDUINO_SHIM_H */
