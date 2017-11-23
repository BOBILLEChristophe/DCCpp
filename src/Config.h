/**********************************************************************

Config.h
COPYRIGHT (c) 2013-2016 Gregg E. Berman
Adapted for DcDcc by Thierry PARIS

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/
#ifndef __config_h
#define __config_h

#include "arduino.h"

#define UNDEFINED_PIN	255

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE NUMBER OF MAIN TRACK REGISTER

#define MAX_MAIN_REGISTERS 12

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE PINS ACCORDING TO MOTOR SHIELD MODEL
//

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)      // Configuration for UNO

#define DCC_SIGNAL_PIN_MAIN 10          // Ardunio Uno  - uses OC1B
#define DCC_SIGNAL_PIN_PROG 5           // Arduino Uno  - uses OC0B

#elif defined(ARDUINO_AVR_MEGA2560)

#define DCC_SIGNAL_PIN_MAIN 12          // Arduino Mega - uses OC1B
#define DCC_SIGNAL_PIN_PROG 2           // Arduino Mega - uses OC3B

#endif

/////////////////////////////////////////////////////////////////////////////////////
// SELECT MOTOR SHIELD
/////////////////////////////////////////////////////////////////////////////////////

#define MOTOR_SHIELD_SIGNAL_ENABLE_PIN_MAIN 3
#define MOTOR_SHIELD_SIGNAL_ENABLE_PIN_PROG 11

#define MOTOR_SHIELD_CURRENT_MONITOR_PIN_MAIN A0
#define MOTOR_SHIELD_CURRENT_MONITOR_PIN_PROG A1

#define MOTOR_SHIELD_DIRECTION_MOTOR_CHANNEL_PIN_A 12
#define MOTOR_SHIELD_DIRECTION_MOTOR_CHANNEL_PIN_B 13

#define POLOLU_SIGNAL_ENABLE_PIN_MAIN 9
#define POLOLU_SIGNAL_ENABLE_PIN_PROG 11

#define POLOLU_CURRENT_MONITOR_PIN_MAIN A0
#define POLOLU_CURRENT_MONITOR_PIN_PROG A1

#define POLOLU_DIRECTION_MOTOR_CHANNEL_PIN_A 7
#define POLOLU_DIRECTION_MOTOR_CHANNEL_PIN_B 8

#ifdef USE_ETHERNET
enum EthernetProtocol
{
	None,
	HTTP,
	TCP
};
#endif

struct DCCppConfig
{
#ifdef USE_ETHERNET
	static uint8_t EthernetIp[4];
	static uint8_t EthernetMac[6];
	static int EthernetPort;
	static EthernetProtocol Protocol;
#endif

	static byte SignalEnablePinMain;// PWM : *_SIGNAL_ENABLE_PIN_MAIN
	static byte CurrentMonitorMain;	// Current sensor : *_CURRENT_MONITOR_PIN_MAIN

	static byte SignalEnablePinProg;
	static byte CurrentMonitorProg;

	// Only for shields : indirection of the signal from SignalPinMain to DirectionMotor of the shield
	static byte DirectionMotorA;	// *_DIRECTION_MOTOR_CHANNEL_PIN_A
	static byte DirectionMotorB;	// *_DIRECTION_MOTOR_CHANNEL_PIN_B
};

/////////////////////////////////////////////////////////////////////////////////////

#endif