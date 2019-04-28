//! @file
//pat9125.h

#ifndef PAT9125_H
#define PAT9125_H
#define MCU_AT85  //ATTINY 85
//#define MCU_AUNO //MICRO/UNO

#include <inttypes.h>


extern uint8_t pat9125_PID1;
extern uint8_t pat9125_PID2;

extern int16_t pat9125_x;
extern int16_t pat9125_y;
extern uint8_t pat9125_b;
extern uint8_t pat9125_s;

extern uint8_t pat9125_init(void);
extern uint8_t pat9125_update(void);
extern uint8_t pat9125_update_y(void);
extern uint8_t pat9125_update_y2(void);

#endif
