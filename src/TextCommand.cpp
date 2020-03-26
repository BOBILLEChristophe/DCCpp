/**********************************************************************

  TextCommand.cpp
  COPYRIGHT (c) 2013-2016 Gregg E. Berman

  Part of DCC++ BASE STATION for the Arduino

 **********************************************************************/

#include "Arduino.h"

// See TextCommand::parse() below for defined text commands.

#include "TextCommand.h"
#ifdef USE_TEXTCOMMAND

#ifdef VISUALSTUDIO
#include "string.h"
#include "iostream"
#else
extern unsigned int __heap_start;
extern void *__brkval;
#endif


char TextCommand::commandString[MAX_COMMAND_LENGTH + 1];


void TextCommand::init(volatile RegisterList *_mRegs, volatile RegisterList *_pRegs, CurrentMonitor *_mMonitor) {
  commandString[0] = 0;
} // TextCommand:TextCommand


void TextCommand::process() {
  char c;

#if defined(USE_ETHERNET) || defined(USE_WIFI)
#if defined(USE_ETHERNET)
  EthernetClient client = DCCPP_INTERFACE.available();
#elif defined(USE_WIFI)
  WiFiClient client = DCCPP_INTERFACE.available();
#endif

  if (client) {
    if (DCCppConfig::Protocol == EthernetProtocol::HTTP) {
      DCCPP_INTERFACE.println("HTTP/1.1 200 OK");
      DCCPP_INTERFACE.println("Content-Type: text/html");
      DCCPP_INTERFACE.println("Access-Control-Allow-Origin: *");
      DCCPP_INTERFACE.println("Connection: close");
      DCCPP_INTERFACE.println("");
    }
    Serial.print(client.available());

    while (client.connected ()) {
      if (client.available ()) {
        c = client.read();
        if (c == '<')                    // start of new command
          commandString[0] = 0;
        else if (c == '>')               // end of new command
        {
          if (parse(commandString) == false)
          {
#if defined(DCCPP_DEBUG_MODE)
            Serial.println("invalid command !");
#endif
          }
        }
        else if (strlen(commandString) < MAX_COMMAND_LENGTH)    // if comandString still has space, append character just read from network
          sprintf(commandString, "%s%c", commandString, c);     // otherwise, character is ignored (but continue to look for '<' or '>')
      }
    } // while

    //if (DCCppConfig::Protocol == EthernetProtocol::HTTP)
    client.stop();
  }

#else  // SERIAL case
  while (DCCPP_INTERFACE.available() > 0) {  // while there is data on the serial line
    c = DCCPP_INTERFACE.read();
    if (c == '<')                    // start of new command
      commandString[0] = 0;
    else if (c == '>')               // end of new command
      parse(commandString);
    else if (strlen(commandString) < MAX_COMMAND_LENGTH)  // if commandString still has space, append character just read from serial line
      sprintf(commandString, "%s%c", commandString, c); // otherwise, character is ignored (but continue to look for '<' or '>')
  } // while

#endif
} // TextCommand:process


bool TextCommand::parse(char *com) {

#ifdef DCCPP_DEBUG_MODE
  Serial.print(com[0]);
  Serial.println(F(" command"));
#ifdef VISUALSTUDIO
  std::cout << com <<  " command received" << std::endl;
#endif
#endif

  switch (com[0]) {

    case 't':
      DCCpp::mainRegs.setThrottle(com + 1);
      return true;

    case 'f':
      DCCpp::mainRegs.setFunction(com + 1);
      return true;

    case 'a':
      DCCpp::mainRegs.setAccessory(com + 1);
      return true;

#ifdef USE_TURNOUT
    case 'T':
      /*
      * *** SEE TURNOUT.CPP FOR COMPLETE INFO ON THE DIFFERENT VARIATIONS OF THE "T" COMMAND
        USED TO CREATE/EDIT/REMOVE/SHOW TURNOUT DEFINITIONS
      */

      return Turnout::parse(com + 1);
#endif

#ifdef USE_OUTPUT

    case 'Z':
      /**** SEE OUTPUT.CPP FOR COMPLETE INFO ON THE DIFFERENT VARIATIONS OF THE "Z" COMMAND
          USED TO CREATE / EDIT / REMOVE / SHOW OUTPUT DEFINITIONS
      */

      return Output::parse(com + 1);
#endif

#ifdef USE_SENSOR

    case 'S':
      /*
       *   *** SEE SENSOR.CPP FOR COMPLETE INFO ON THE DIFFERENT VARIATIONS OF THE "S" COMMAND
           USED TO CREATE/EDIT/REMOVE/SHOW SENSOR DEFINITIONS
      */
      return Sensor::parse(com + 1);

#ifdef DCCPP_PRINT_DCCPP
    case 'Q':
      Sensor::status();
      return true;
#endif
#endif

    case 'w':

      DCCpp::mainRegs.writeCVByteMain(com + 1);
      return true;


    case 'b':
      DCCpp::mainRegs.writeCVBitMain(com + 1);
      return true;

    case 'W':
      DCCpp::progRegs.writeCVByte(com + 1);
      return true;

    case 'B':
      DCCpp::progRegs.writeCVBit(com + 1);
      return true;

    case 'R':
      DCCpp::progRegs.readCV(com + 1);
      return true;

    case '1':
      DCCpp::powerOn();
      return true;

    case '0':
      DCCpp::powerOff();
      return true;

    case 'c':
      DCCPP_INTERFACE.print("<a");
      DCCPP_INTERFACE.print(int(DCCpp::getCurrentMain()));
      DCCPP_INTERFACE.print(">");
#if !defined(USE_ETHERNET)
      DCCPP_INTERFACE.println("");
#endif
      return true;

    case 's':
      if (DCCppConfig::SignalEnablePinMain == UNDEFINED_PIN || digitalRead(DCCppConfig::SignalEnablePinMain) == HIGH)
        DCCPP_INTERFACE.print("<p0>");
      if (DCCppConfig::SignalEnablePinProg == UNDEFINED_PIN || digitalRead(DCCppConfig::SignalEnablePinProg) == HIGH)
        DCCPP_INTERFACE.print("<p1>");
#if !defined(USE_ETHERNET)
      DCCPP_INTERFACE.println("");
#endif

      for (int i = 1; i <= MAX_MAIN_REGISTERS; i++) {
        if (DCCpp::mainRegs.speedTable[i] == 0)
          continue;
        DCCPP_INTERFACE.print("<T");
        DCCPP_INTERFACE.print(i); DCCPP_INTERFACE.print(" ");
        if (DCCpp::mainRegs.speedTable[i] > 0) {
          DCCPP_INTERFACE.print(DCCpp::mainRegs.speedTable[i]);
          DCCPP_INTERFACE.print(" 1>");
        } else {
          DCCPP_INTERFACE.print(- DCCpp::mainRegs.speedTable[i]);
          DCCPP_INTERFACE.print(" 0>");
        }
#if !defined(USE_ETHERNET)
        DCCPP_INTERFACE.println("");
#endif
      }
      DCCPP_INTERFACE.print("<iDCCpp LIBRARY BASE STATION FOR ARDUINO ");
      //DCCPP_INTERFACE.print(ARDUINO_TYPE);
      //DCCPP_INTERFACE.print(" / ");
      //DCCPP_INTERFACE.print(MOTOR_SHIELD_NAME);
      DCCPP_INTERFACE.print(": V-");
      DCCPP_INTERFACE.print(VERSION);
      DCCPP_INTERFACE.print(" / ");
      DCCPP_INTERFACE.print(__DATE__);
      DCCPP_INTERFACE.print(" ");
      DCCPP_INTERFACE.print(__TIME__);
      DCCPP_INTERFACE.print(">");
#if !defined(USE_ETHERNET)
      DCCPP_INTERFACE.println("");
#endif

      DCCPP_INTERFACE.print("<N ");
#if defined(USE_ETHERNET)
      DCCPP_INTERFACE.print("ETHERNET :");
      DCCPP_INTERFACE.print(Ethernet.localIP());
      DCCPP_INTERFACE.print(">");
#if !defined(USE_ETHERNET)
      DCCPP_INTERFACE.println("");
#endif
#else
      DCCPP_INTERFACE.println("SERIAL>");
#endif

#ifdef DCCPP_PRINT_DCCPP
#ifdef USE_TURNOUT
      Turnout::show();
#endif
#ifdef USE_OUTPUT
      Output::show();
#endif
#ifdef USE_SENSOR
      Sensor::show();
#endif
#endif
      return true;

#ifdef USE_EEPROM
    case 'E':
      EEStore::store();
      DCCPP_INTERFACE.print("<e ");
      DCCPP_INTERFACE.print(EEStore::data.nTurnouts);
      DCCPP_INTERFACE.print(" ");
      DCCPP_INTERFACE.print(EEStore::data.nSensors);
      DCCPP_INTERFACE.print(" ");
      DCCPP_INTERFACE.print(EEStore::data.nOutputs);
      DCCPP_INTERFACE.print(">");
#if !defined(USE_ETHERNET)
      DCCPP_INTERFACE.println("");
#endif
      return true;

    case 'e':
      EEStore::clear();
      DCCPP_INTERFACE.print("<O>");
#if !defined(USE_ETHERNET)
      DCCPP_INTERFACE.println("");
#endif
      return true;
#endif

    case ' ':
      DCCPP_INTERFACE.println("");
      return true;


    case 'D':
      Serial.println("\nEntering Diagnostic Mode...");
      delay(1000);

      DCCpp::setDebugDccMode();

      return true;

    case 'M':
      DCCpp::mainRegs.writeTextPacket(com + 1);
      return true;

    case 'P':
      DCCpp::progRegs.writeTextPacket(com + 1);
      return true;

#ifndef VISUALSTUDIO
    case 'F':
      int v;
      DCCPP_INTERFACE.print("<f");
#ifdef ARDUINO_ARCH_AVR
      DCCPP_INTERFACE.print((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
#endif
      DCCPP_INTERFACE.print(">");
#if !defined(USE_ETHERNET)
      DCCPP_INTERFACE.println("");
#endif
      return true;
#endif

    case 'L':
      DCCPP_INTERFACE.println("");
      for (Register *p = DCCpp::mainRegs.reg; p <= DCCpp::mainRegs.maxLoadedReg; p++) {
        DCCPP_INTERFACE.print("M"); DCCPP_INTERFACE.print((int)(p - DCCpp::mainRegs.reg)); DCCPP_INTERFACE.print(":\t");
        DCCPP_INTERFACE.print((int)p); DCCPP_INTERFACE.print("\t");
        DCCPP_INTERFACE.print((int)(p->activePacket)); DCCPP_INTERFACE.print("\t");
        DCCPP_INTERFACE.print(p->activePacket->nBits); DCCPP_INTERFACE.print("\t");
        for (int i = 0; i < 10; i++) {
          DCCPP_INTERFACE.print(p->activePacket->buf[i], HEX); DCCPP_INTERFACE.print("\t");
        }
        DCCPP_INTERFACE.println("");
      }
      for (Register *p = DCCpp::progRegs.reg; p <= DCCpp::progRegs.maxLoadedReg; p++) {
        DCCPP_INTERFACE.print("P"); DCCPP_INTERFACE.print((int)(p - DCCpp::progRegs.reg)); DCCPP_INTERFACE.print(":\t");
        DCCPP_INTERFACE.print((int)p); DCCPP_INTERFACE.print("\t");
        DCCPP_INTERFACE.print((int)p->activePacket); DCCPP_INTERFACE.print("\t");
        DCCPP_INTERFACE.print(p->activePacket->nBits); DCCPP_INTERFACE.print("\t");
        for (int i = 0; i < 10; i++) {
          DCCPP_INTERFACE.print(p->activePacket->buf[i], HEX); DCCPP_INTERFACE.print("\t");
        }
        DCCPP_INTERFACE.println("");
      }
      DCCPP_INTERFACE.println("");
      return true;
  } // switch

  return false;
}; // SerialCommand::parse


#endif
