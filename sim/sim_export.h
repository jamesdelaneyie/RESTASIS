#ifndef SIM_EXPORT_H
#define SIM_EXPORT_H

#include <stdint.h>
#include "staexe_config.h"

#ifdef __cplusplus
extern "C" {
#endif

const uint8_t* sim_leds_rgb(void);
int sim_take_frame_dirty(void);

#ifdef __cplusplus
}
#endif

#endif
