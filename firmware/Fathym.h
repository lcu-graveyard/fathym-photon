#ifndef _FATHYM
#define _FATHYM

// Load build configuration settings
//#include "FathymBuild.h" // uncomment for local Particle Dev build
#include "../FathymBuild.h"

// Standard Photon library
#include "application.h"

// MQTT library used for underlying message broker communication
#include "MQTT.h"

// If this is a local Particle Dev build, reference dependencies/libraries differently
#ifdef LOCAL_BUILD
// Used for JSON data communications
#include "SparkJson.h"
#else
#include "SparkJson/SparkJson.h"
#endif

// Used for access to device flash storage
// #ifdef LOCAL_BUILD
// #include "flashee-eeprom.h"
// #else
// #include "flashee-eeprom/flashee-eeprom.h"
// #endif
// using namespace Flashee;

// The name of the device ID property to use
#ifndef FATHYM_ID_PROPERTY
#define FATHYM_ID_PROPERTY "id"
#endif

// Whether or not to include the device's name
#ifndef FATHYM_ADD_DEVICE_NAME
#define FATHYM_ADD_DEVICE_NAME false
#endif

// The name of the device name property to use
#ifndef FATHYM_DEVICE_NAME_PROPERTY
#define FATHYM_DEVICE_NAME_PROPERTY "name"
#endif

// The default number of decimal places to include from numbers with decimal values
#ifndef FATHYM_DEFAULT_DECIMAL_PLACES
#define FATHYM_DEFAULT_DECIMAL_PLACES 3
#endif

// The interval in minutes that the device will resynchronize device time to Particle cloud time
#ifndef FATHYM_RESYNC_TIME_MINS
#define FATHYM_RESYNC_TIME_MINS 1440 // resync every 24 hours
#endif

// Whether or not to include the current uptime of the device
#ifndef FATHYM_ADD_UPTIME
#define FATHYM_ADD_UPTIME true
#endif

// The name of the device uptime property to use
#ifndef FATHYM_UPTIME_PROPERTY
#define FATHYM_UPTIME_PROPERTY "ut"
#endif

// Whether or not to include a time stamp on each published message
#ifndef FATHYM_ADD_TIMESTAMP
#define FATHYM_ADD_TIMESTAMP true
#endif

// The time zone offset to use when adding a time stamp to the published message
#ifndef FATHYM_TIMEZONE_OFFSET
#define FATHYM_TIMEZONE_OFFSET -7 // Colorado/Mountain time!
#endif

// The name of the time stamp property to use
#ifndef FATHYM_TIMESTAMP_PROPERTY
#define FATHYM_TIMESTAMP_PROPERTY "ts"
#endif

// Whether or not to include the device's free memory in the message for detecting memory leaks
#ifndef FATHYM_ADD_FREE_MEMORY
#define FATHYM_ADD_FREE_MEMORY false
#endif

// The name of the free memory property to use
#ifndef FATHYM_FREE_MEMORY_PROPERTY
#define FATHYM_FREE_MEMORY_PROPERTY "mem"
#endif

// Whether or not the Fathym library will automatically handle publishing data or not
#ifndef FATHYM_AUTO_PUBLISH
#define FATHYM_AUTO_PUBLISH true
#endif

// The publish rate (in seconds) for message data (applies when FATHYM_AUTO_PUBLISH is set to true)
#ifndef FATHYM_PUBLISH_RATE
#define FATHYM_PUBLISH_RATE 10
#endif

// Default to standard MQTT port
#ifndef FATHYM_DEFAULT_PORT
#define FATHYM_DEFAULT_PORT 1883
#endif

// The number of seconds to use for the MQTT connection keep alive
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 20
#endif

// The maximum size of the MQTT header in bytes
#ifndef MQTT_MAX_HEADER_SIZE
#define MQTT_MAX_HEADER_SIZE 160
#endif

// The reserved buffer size in bytes for MQTT data packet buffer.
// The maximum buffer size available to serialize JSON messages to string
// is determined by the MQTT_MAX_PACKET_SIZE - MQTT_MAX_HEADER_SIZE.
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 640
#endif

// Default topic on the message broker to publish messages to
#ifndef FATHYM_DEFAULT_TOPIC
#define FATHYM_DEFAULT_TOPIC ""
#endif

// Whether or not to use the defined debug pin for Fathym visual status debugging
#ifndef FATHYM_USE_DEBUG_LED
#define FATHYM_USE_DEBUG_LED true
#endif

// Default LED pin to use when debugging with LED status
#ifndef FATHYM_DEBUG_LED_PIN
#define FATHYM_DEBUG_LED_PIN D7
#endif

// Whether or not to use the debug LED (if it is enabled) to indicate a successful publish
#ifndef FATHYM_DEBUG_SHOW_PUBLISH
#define FATHYM_DEBUG_SHOW_PUBLISH true
#endif

// The number of milliseconds to flash the debug LED to indicate a successful publish
#ifndef FATHYM_DEBUG_PUBLISH_DELAY
#define FATHYM_DEBUG_PUBLISH_DELAY 20
#endif

// Main Fathym class that provides singleton-like access to the API
class Fathym {
public:
  // Constructors
  Fathym(char * server, char * username, char * password);
  Fathym(char * server, uint16_t port, char * username, char * password);

  // Event Handlers
  void nameHandler(const char * topic, const char * data); // used to retrieve device name

  // Connection
  void beginUpdate(void);
  void endUpdate(void);
  bool connect(void);
  bool isConnected(void);
  void setKeepAlive(uint16_t seconds);

  // Message
  void setPublishRate(uint16_t seconds);
  bool publishRaw(const char * topic, const char * payload);
  bool publish(void);
  bool publish(const char * topic);
  void removeValue(const char * name);
  void setValue(const char * name, bool value);
  void setValue(const char * name, const char * value);
  void setValue(const char * name, float value);
  void setValue(const char * name, float value, uint8_t decimals);
  void setValue(const char * name, double value);
  void setValue(const char * name, double value, uint8_t decimals);
  void setValue(const char * name, int value);
  void setValue(const char * name, long value);
  void setValue(const char * name, float value, const char * units);
  void setValue(const char * name, float value, const char * units, uint8_t decimals);
  void setValue(const char * name, double value, const char * units);
  void setValue(const char * name, double value, const char * units, uint8_t decimals);
  void setValue(const char * name, int value, const char * units);
  void setValue(const char * name, long value, const char * units);
  void printJson(void);

private:
  // Initialize
  void init(char * server, uint16_t port, char * username, char * password);

  // Device
  String _id; // stores the device's ID
  String _idProp = FATHYM_ID_PROPERTY; // the property name to use for the device ID
  String _name; // stores the device's name
  String _timeStamp; // stores the current timestamp string for the last publish
  String _error; // current error to report to the server, if any
  uint16_t _publishRate; // rate (in seconds) at which auto-publishing occurs if it is enabled
  unsigned long _lastTimeSync; // used to resync to cloud network time to avoid local time drift
  unsigned long _lastBeginUpdate; // used to adjust delay compensation to attempt to regulate a more stable update/publish rate

  // Connection
  char * _server;
  uint16_t _port;
  char * _username;
  char * _password;
  MQTT * _mqtt;
  uint16_t _keepAlive;
  bool reconnect(void);

  // Storage
  //FlashDevice * _flash;

  // JSON
  DynamicJsonBuffer _jsonBuffer;
  JsonObject * _json;

  // Utility
  void flash(uint8_t numFlashes, uint8_t delayMs);
};

#endif