/*************************************************************
  project: <Dc/Dcc Controller>
  author: <Thierry PARIS>
  description: <Dcc Wifi Controller sample>
*************************************************************/

/*
 * Example for wifi with : IP static, IP with DHCP and Access Point (WiThrottle for example)
 */

#include "DCCpp.h"

#if !defined(USE_TEXTCOMMAND) || !defined(USE_WIFI)
#error To be able to compile this sample,the lines #define USE_TEXTCOMMAND and #define USE_WIFI must be uncommented in DCCpp.h
#endif

#define PROTOCOL TCP  // <- Choice of protocol
//#define PROTOCOL HTTP  // <- Choice of protocol



// For Access Point
#define WIFI_ACCESSPOINT
#ifdef WIFI_ACCESSPOINT                         // If you want a Access Point
  uint32_t ip[] = { 10, 10, 10, 10 };
  const char* ssid     = "myAPname";
  const char* password = "myAPpsw";           // can be empty
#else
  const char* ssid     = "mySsid";
  const char* password = "myPswd";
  // For static IP
  #define STATIC_IP
  #ifdef STATIC_IP
    uint32_t ip[] = { 192, 168, 1, 200 };       // can be modified
    uint32_t gateway[] = { 192, 168, 1, 1 };
    uint32_t subnet[] = { 255, 255, 255, 0 };
  #endif
#endif

uint16_t port = 2560;                            // can be modified

WiFiServer DCCPP_INTERFACE(port);
void setup()
{
  Serial.begin(115200);
  Serial.println("DCCpp Wifi for ESP32 0.1");

#ifdef WIFI_ACCESSPOINT
  DCCpp::beginWifi(ssid, password, ip, port, EthernetProtocol::PROTOCOL);
#else
  #ifdef STATIC_IP
    // Begin with IP
    DCCpp::beginWifi(ssid, password, ip, gateway, subnet, port, EthernetProtocol::PROTOCOL);
  #else
    // Begin with DHCP
    DCCpp::beginWifi(ssid, password, port, EthernetProtocol::PROTOCOL);
  #endif
#endif


  DCCpp::begin();
  // Configuration for my LMD18200. See the page 'Configuration lines' in the documentation for other samples.
  /* My configuration for ESP32
    DIR -> GPIO_32
    PWM -> EN
    MAX471 -> GPIO_36 (A0)
  */
  DCCpp::beginMain(UNDEFINED_PIN, 32, UNDEFINED_PIN, A0);

}

void loop()
{
  DCCpp::loop();
}
