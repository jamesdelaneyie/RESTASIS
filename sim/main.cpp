/*********************************************************
 * STAEXE desktop sim — runs real C++ choreography, emits
 * NDJSON frames on stdout for the PIXI bridge.
 *
 * Frame schema:
 *   {"t":<ms>,"leds":[r,g,b,...70*3],"motors":[d0..d11]}
 *********************************************************/
#define SIMULATOR 1

#include "arduino_shim.h"
#include "staexe_choreography.h"
#include "staexe_display.h"
#include "sim_export.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

static void emit_frame(unsigned long t) {
  const uint8_t* rgb = sim_leds_rgb();
  std::fputs("{\"t\":", stdout);
  std::fprintf(stdout, "%lu,\"leds\":[", t);
  for (int i = 0; i < STAEXE_LEDS_PER_STRIP * 3; i++) {
    if (i) std::fputc(',', stdout);
    std::fprintf(stdout, "%u", (unsigned)rgb[i]);
  }
  std::fputs("],\"motors\":[", stdout);
  for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
    if (i) std::fputc(',', stdout);
    std::fprintf(stdout, "%.2f", staexe_motor_degrees[i]);
  }
  std::fputs("]}\n", stdout);
  std::fflush(stdout);
}

int main(int argc, char** argv) {
  /* Optional: compress wall time so a 10s pulse is watchable.
   * STAEXE_TIME_SCALE=10 means 10x faster (1s wall ≈ 10s sim).
   * Default 20 → LED/motor pulse every ~0.5s wall clock.
   */
  double time_scale = 20.0;
  if (argc > 1) {
    time_scale = atof(argv[1]);
    if (time_scale < 0.1) time_scale = 0.1;
  }

  srand((unsigned)time(nullptr));

  motors_setup();
  choreography_setup();
  emit_frame(0);

  unsigned long sim_ms = 0;
  const unsigned long step_ms = 16; /* ~60 fps sim ticks */

  using clock = std::chrono::steady_clock;
  auto next = clock::now();

  for (;;) {
    /* Drive choreography with scaled virtual time via a local clock.
     * arduino_shim millis() is wall time; we pass virtual time instead.
     */
    choreography_loop(sim_ms);
    emit_frame(sim_ms);

    sim_ms += (unsigned long)(step_ms * time_scale);

    next += std::chrono::milliseconds(step_ms);
    std::this_thread::sleep_until(next);
  }

  return 0;
}
