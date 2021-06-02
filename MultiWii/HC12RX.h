#ifndef HC12RX_H_
#define HC12RX_H_

#include "config.h"

#if defined(HC12RX)

/*
struct HC12Data {
  int throttle;
  int yaw;
  int pitch;
  int roll;
  int aux1;
  int aux2;
  int switches;
};

struct HC12Payload {
  float lat;
  float lon;
  int16_t heading;
  int16_t pitch;
  int16_t roll;  
  int32_t alt;
  byte flags;
};

extern HC12Data HC12_Data;
extern HC12Payload HC12_Payload;
*/
extern int16_t HC12_rcData[RC_CHANS];

void HC12_Init();
void HC12_Read_RC();

#endif

#endif
