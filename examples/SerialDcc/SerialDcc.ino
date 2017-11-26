/*************************************************************
project: <Dc/Dcc Controller>
author: <Thierry PARIS>
description: <Dcc Serial Controller sample>
*************************************************************/

#include "DCCpp.h"

#ifndef USE_TEXTCOMMAND
#error To be able to compile this sample,the line #define USE_TEXTCOMMAND must be uncommented in DCCpp.h
#endif

void setup()
{
	Serial.begin(115200);

	DCCpp.begin();
	DCCpp.beginMain(UNDEFINED_PIN, DCC_SIGNAL_PIN_MAIN, 11, A6);
}

void loop()
{
	DCCpp.loop();
}

