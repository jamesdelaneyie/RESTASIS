/*********************************************************
 * STAEXE — LED + motor choreography (platform-agnostic)
 *********************************************************/
#ifndef STAEXE_CHOREOGRAPHY_H
#define STAEXE_CHOREOGRAPHY_H

#ifdef __cplusplus
extern "C" {
#endif

void choreography_setup(void);
void choreography_loop(unsigned long now_ms);

#ifdef __cplusplus
}
#endif

#endif /* STAEXE_CHOREOGRAPHY_H */
