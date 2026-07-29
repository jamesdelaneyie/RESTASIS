/*********************************************************
 * Minimal Arduino API surface for desktop simulation
 *********************************************************/
#ifndef ARDUINO_SHIM_H
#define ARDUINO_SHIM_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>

#ifndef constrain
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#endif

inline unsigned long millis() {
  using clock = std::chrono::steady_clock;
  static const auto start = clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start);
  return (unsigned long)ms.count();
}

inline long random(long max) {
  if (max <= 0) return 0;
  return (long)(rand() % max);
}

inline void delay(unsigned long ms) {
  auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
  while (std::chrono::steady_clock::now() < until) {
    /* spin — short delays only in sim */
  }
}

#endif /* ARDUINO_SHIM_H */
