#ifndef HC12RX_H_
#define HC12RX_H_

#include "config.h"

#if defined(HC12RX)

extern int16_t HC12_rcData[RC_CHANS];

void HC12_Init();
void HC12_Read_RC();

#endif

#endif
