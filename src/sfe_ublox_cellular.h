/*
  Arduino library for the u-blox SARA-R5 LTE-M / NB-IoT modules with secure cloud, as used on the SparkFun MicroMod Asset Tracker
  By: Paul Clark
  October 19th 2020

  Based extensively on the:
  Arduino Library for the SparkFun LTE CAT M1/NB-IoT Shield - SARA-R4
  Written by Jim Lindblom @ SparkFun Electronics, September 5, 2018

  This Arduino library provides mechanisms to initialize and use
  the SARA-R5 module over either a SoftwareSerial or hardware serial port.

  Please see LICENSE.md for the license information

*/

#ifndef SPARKFUN_UBX_CELL_ARDUINO_LIBRARY_H
#define SPARKFUN_UBX_CELL_ARDUINO_LIBRARY_H

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef ARDUINO_ARCH_AVR                 // Arduino AVR boards (Uno, Pro Micro, etc.)
#define UBX_CELL_SOFTWARE_SERIAL_ENABLED // Enable software serial
#endif

#ifdef ARDUINO_ARCH_SAMD                 // Arduino SAMD boards (SAMD21, etc.)
#define UBX_CELL_SOFTWARE_SERIAL_ENABLEDx // Disable software serial
#endif

#ifdef ARDUINO_ARCH_APOLLO3              // Arduino Apollo boards (Artemis module, RedBoard Artemis, etc)
#define UBX_CELL_SOFTWARE_SERIAL_ENABLEDx // Disable software serial (no longer supported with v2 of Apollo3)
// Note: paulvha has provided software serial support for v2 of the Apollo3 / Artemis core.
//       Further details are available at:
//       https://github.com/paulvha/apollo3/tree/master/SoftwareSerial
#endif

#ifdef ARDUINO_ARCH_STM32               // STM32 based boards (Disco, Nucleo, etc)
#define UBX_CELL_SOFTWARE_SERIAL_ENABLED // Enable software serial
#endif

#ifdef ARDUINO_ARCH_ESP32 // ESP32 based boards
// Check to see if ESP Software Serial has been included
// Note: you need to #include <SoftwareSerial.h> at the very start of your script,
// _before_ the #include <SparkFun_u-blox_SARA-R5_Arduino_Library.h>, for this to work.
// See SARA-R5_Example2_Identification_ESPSoftwareSerial for more details.
#if __has_include(<SoftwareSerial.h> )
#define UBX_CELL_SOFTWARE_SERIAL_ENABLED // Enable software serial
#else
#define UBX_CELL_SOFTWARE_SERIAL_ENABLEDx // Disable software serial
#endif
#endif

#ifdef ARDUINO_ARCH_ESP8266 // ESP8266 based boards
// Check to see if ESP Software Serial has been included
// Note: you need to #include <SoftwareSerial.h> at the very start of your script,
// _before_ the #include <SparkFun_u-blox_SARA-R5_Arduino_Library.h>, for this to work.
// See SARA-R5_Example2_Identification_ESPSoftwareSerial for more details.
#if __has_include(<SoftwareSerial.h> )
#define UBX_CELL_SOFTWARE_SERIAL_ENABLED // Enable software serial
#else
#define UBX_CELL_SOFTWARE_SERIAL_ENABLEDx // Disable software serial
#endif
#endif

#ifdef UBX_CELL_SOFTWARE_SERIAL_ENABLED
#include <SoftwareSerial.h> // SoftwareSerial.h is guarded. It is OK to include it twice.
#endif

#include <IPAddress.h>

#define UBX_CELL_POWER_PIN -1 // Default to no pin
#define UBX_CELL_RESET_PIN -1

// Timing
#define UBX_CELL_STANDARD_RESPONSE_TIMEOUT 1000
#define UBX_CELL_10_SEC_TIMEOUT 10000
#define UBX_CELL_55_SECS_TIMEOUT 55000
#define UBX_CELL_2_MIN_TIMEOUT 120000
#define UBX_CELL_3_MIN_TIMEOUT 180000
#define UBX_CELL_SET_BAUD_TIMEOUT 500
#define UBX_CELL_POWER_OFF_PULSE_PERIOD 3200 // Hold PWR_ON low for this long to power the module off
#define UBX_CELL_POWER_ON_PULSE_PERIOD 100 // Hold PWR_ON low for this long to power the module on (SARA-R510M8S)
#define UBX_CELL_RESET_PULSE_PERIOD 23000 // Used to perform an abrupt emergency hardware shutdown. 23 seconds... (Yes, really!)
#define UBX_CELL_POWER_OFF_TIMEOUT 40000 // Datasheet says 40 seconds...
#define UBX_CELL_IP_CONNECT_TIMEOUT 130000
#define UBX_CELL_POLL_DELAY 1
#define UBX_CELL_SOCKET_WRITE_TIMEOUT 10000

// ## Suported AT Commands
// ### General
const char* const UBX_CELL_COMMAND_AT = "AT";           // AT "Test"
const char* const UBX_CELL_COMMAND_ECHO = "E";          // Local Echo
const char* const UBX_CELL_COMMAND_MANU_ID = "+CGMI";   // Manufacturer identification
const char* const UBX_CELL_COMMAND_MODEL_ID = "+CGMM";  // Model identification
const char* const UBX_CELL_COMMAND_FW_VER_ID = "+CGMR"; // Firmware version identification
const char* const UBX_CELL_COMMAND_SERIAL_NO = "+CGSN"; // Product serial number
const char* const UBX_CELL_COMMAND_IMEI = "+GSN";       // IMEI identification
const char* const UBX_CELL_COMMAND_IMSI = "+CIMI";      // IMSI identification
const char* const UBX_CELL_COMMAND_CCID = "+CCID";      // SIM CCID
const char* const UBX_CELL_COMMAND_REQ_CAP = "+GCAP";   // Request capabilities list
// ### Control and status
const char* const UBX_CELL_COMMAND_POWER_OFF = "+CPWROFF"; // Module switch off
const char* const UBX_CELL_COMMAND_FUNC = "+CFUN";         // Functionality (reset, etc.)
const char* const UBX_CELL_COMMAND_CLOCK = "+CCLK";        // Real-time clock
const char* const UBX_CELL_COMMAND_AUTO_TZ = "+CTZU";      // Automatic time zone update
const char* const UBX_CELL_COMMAND_TZ_REPORT = "+CTZR";    // Time zone reporting
// ### Network service
const char* const UBX_CELL_COMMAND_CNUM = "+CNUM"; // Subscriber number
const char* const UBX_CELL_SIGNAL_QUALITY = "+CSQ";
const char* const UBX_CELL_EXT_SIGNAL_QUALITY = "+CESQ";
const char* const UBX_CELL_OPERATOR_SELECTION = "+COPS";
const char* const UBX_CELL_REGISTRATION_STATUS = "+CREG";
const char* const UBX_CELL_EPSREGISTRATION_STATUS = "+CEREG";
const char* const UBX_CELL_READ_OPERATOR_NAMES = "+COPN";
const char* const UBX_CELL_COMMAND_MNO = "+UMNOPROF"; // MNO (mobile network operator) Profile
// ### SIM
const char* const UBX_CELL_SIM_STATE = "+USIMSTAT";
const char* const UBX_CELL_COMMAND_SIMPIN = "+CPIN";    // SIM PIN
// ### SMS
const char* const UBX_CELL_MESSAGE_FORMAT = "+CMGF";     // Set SMS message format
const char* const UBX_CELL_SEND_TEXT = "+CMGS";          // Send SMS message
const char* const UBX_CELL_NEW_MESSAGE_IND = "+CNMI";    // New [SMS] message indication
const char* const UBX_CELL_PREF_MESSAGE_STORE = "+CPMS"; // Preferred message storage
const char* const UBX_CELL_READ_TEXT_MESSAGE = "+CMGR";  // Read message
const char* const UBX_CELL_DELETE_MESSAGE = "+CMGD";     // Delete message
// V24 control and V25ter (UART interface)
const char* const UBX_CELL_FLOW_CONTROL = "&K";   // Flow control
const char* const UBX_CELL_COMMAND_BAUD = "+IPR"; // Baud rate
// ### Packet switched data services
const char* const UBX_CELL_MESSAGE_PDP_DEF = "+CGDCONT";            // Packet switched Data Profile context definition
const char* const UBX_CELL_MESSAGE_PDP_CONTEXT_ACTIVATE = "+CGACT"; // Activates or deactivates the specified PDP context
const char* const UBX_CELL_MESSAGE_ENTER_PPP = "D";
// ### GPIO
const char* const UBX_CELL_COMMAND_GPIO = "+UGPIOC"; // GPIO Configuration
// ### IP
const char* const UBX_CELL_CREATE_SOCKET = "+USOCR";      // Create a new socket
const char* const UBX_CELL_CLOSE_SOCKET = "+USOCL";       // Close a socket
const char* const UBX_CELL_CONNECT_SOCKET = "+USOCO";     // Connect to server on socket
const char* const UBX_CELL_WRITE_SOCKET = "+USOWR";       // Write data to a socket
const char* const UBX_CELL_WRITE_UDP_SOCKET = "+USOST";   // Write data to a UDP socket
const char* const UBX_CELL_READ_SOCKET = "+USORD";        // Read from a socket
const char* const UBX_CELL_READ_UDP_SOCKET = "+USORF";    // Read UDP data from a socket
const char* const UBX_CELL_LISTEN_SOCKET = "+USOLI";      // Listen for connection on socket
const char* const UBX_CELL_GET_ERROR = "+USOER";          // Get last socket error.
const char* const UBX_CELL_SOCKET_DIRECT_LINK = "+USODL"; // Set socket in Direct Link mode
const char* const UBX_CELL_SOCKET_CONTROL = "+USOCTL";    // Query the socket parameters
const char* const UBX_CELL_UD_CONFIGURATION = "+UDCONF";  // User Datagram Configuration
// ### Ping
const char* const UBX_CELL_PING_COMMAND = "+UPING"; // Ping
// ### HTTP
const char* const UBX_CELL_HTTP_PROFILE = "+UHTTP";          // Configure the HTTP profile. Up to 4 different profiles can be defined
const char* const UBX_CELL_HTTP_COMMAND = "+UHTTPC";         // Trigger the specified HTTP command
const char* const UBX_CELL_HTTP_PROTOCOL_ERROR = "+UHTTPER"; // Retrieves the error class and code of the latest HTTP operation on the specified HTTP profile.

const char* const UBX_CELL_MQTT_NVM = "+UMQTTNV";
const char* const UBX_CELL_MQTT_PROFILE = "+UMQTT";
const char* const UBX_CELL_MQTT_COMMAND = "+UMQTTC";
const char* const UBX_CELL_MQTT_PROTOCOL_ERROR = "+UMQTTER";
// ### FTP
const char* const UBX_CELL_FTP_PROFILE = "+UFTP";
const char* const UBX_CELL_FTP_COMMAND = "+UFTPC";
const char* const UBX_CELL_FTP_PROTOCOL_ERROR = "+UFTPER";
// ### GNSS
const char* const UBX_CELL_GNSS_POWER = "+UGPS";                   // GNSS power management configuration
const char* const UBX_CELL_GNSS_ASSISTED_IND = "+UGIND";           // Assisted GNSS unsolicited indication
const char* const UBX_CELL_GNSS_REQUEST_LOCATION = "+ULOC";        // Ask for localization information
const char* const UBX_CELL_GNSS_GPRMC = "+UGRMC";                  // Ask for localization information
const char* const UBX_CELL_GNSS_CONFIGURE_SENSOR = "+ULOCGNSS";    // Configure GNSS sensor
const char* const UBX_CELL_GNSS_CONFIGURE_LOCATION = "+ULOCCELL";  // Configure cellular location sensor (CellLocate®)
const char* const UBX_CELL_AIDING_SERVER_CONFIGURATION = "+UGSRV"; // Configure aiding server (CellLocate®)
// ### File System
// TO DO: Add support for file tags. Default tag to USER
const char* const UBX_CELL_FILE_SYSTEM_READ_FILE = "+URDFILE";      // Read a file
const char* const UBX_CELL_FILE_SYSTEM_READ_BLOCK = "+URDBLOCK";      // Read a block from a file
const char* const UBX_CELL_FILE_SYSTEM_DOWNLOAD_FILE = "+UDWNFILE";    // Download a file into the module
const char* const UBX_CELL_FILE_SYSTEM_LIST_FILES = "+ULSTFILE";    // List of files, size of file, etc.
const char* const UBX_CELL_FILE_SYSTEM_DELETE_FILE = "+UDELFILE";   // Delete a file
// ### File System
// TO DO: Add support for file tags. Default tag to USER
const char* const UBX_CELL_SEC_PROFILE = "+USECPRF";
const char* const UBX_CELL_SEC_MANAGER = "+USECMNG";


// ### URC strings
const char* const UBX_CELL_READ_SOCKET_URC = "+UUSORD:";
const char* const UBX_CELL_READ_UDP_SOCKET_URC = "+UUSORF:";
const char* const UBX_CELL_LISTEN_SOCKET_URC = "+UUSOLI:";
const char* const UBX_CELL_CLOSE_SOCKET_URC = "+UUSOCL:";
const char* const UBX_CELL_GNSS_REQUEST_LOCATION_URC = "+UULOC:";
const char* const UBX_CELL_SIM_STATE_URC = "+UUSIMSTAT:";
const char* const UBX_CELL_MESSAGE_PDP_ACTION_URC = "+UUPSDA:";
const char* const UBX_CELL_HTTP_COMMAND_URC = "+UUHTTPCR:";
const char* const UBX_CELL_MQTT_COMMAND_URC = "+UUMQTTC:";
const char* const UBX_CELL_PING_COMMAND_URC = "+UUPING:";
const char* const UBX_CELL_REGISTRATION_STATUS_URC = "+CREG:";
const char* const UBX_CELL_EPSREGISTRATION_STATUS_URC = "+CEREG:";
const char* const UBX_CELL_FTP_COMMAND_URC = "+UUFTPCR:";

// ### Response
const char* const UBX_CELL_RESPONSE_MORE = "\n>";
const char* const UBX_CELL_RESPONSE_OK = "\nOK\r\n";
const char* const UBX_CELL_RESPONSE_ERROR = "\nERROR\r\n";
const char* const UBX_CELL_RESPONSE_CONNECT = "\r\nCONNECT\r\n";
#define UBX_CELL_RESPONSE_OK_OR_ERROR nullptr

// CTRL+Z and ESC ASCII codes for SMS message sends
const char ASCII_CTRL_Z = 0x1A;
const char ASCII_ESC = 0x1B;

// NMEA data size - used by parseGPRMCString
#define TEMP_NMEA_DATA_SIZE 16

#define NOT_AT_COMMAND false
#define AT_COMMAND true

// The minimum memory allocation for responses from sendCommandWithResponse
// This needs to be large enough to hold the response you're expecting plus and URC's that may arrive during the timeout
#define minimumResponseAllocation 128

#define UBX_CELL_NUM_SOCKETS 6

#define NUM_SUPPORTED_BAUD 6
const unsigned long UBX_CELL_SUPPORTED_BAUD[NUM_SUPPORTED_BAUD] =
    {
        115200,
        9600,
        19200,
        38400,
        57600,
        230400};
#define UBX_CELL_DEFAULT_BAUD_RATE 115200

// Flow control definitions for AT&K
// Note: SW (XON/XOFF) flow control is not supported on the UBX_CELL
typedef enum
{
  UBX_CELL_DISABLE_FLOW_CONTROL = 0,
  UBX_CELL_ENABLE_FLOW_CONTROL = 3
} UBX_CELL_flow_control_t;

// The standard Europe profile should be used as the basis for all other MNOs in Europe outside of Vodafone
// and Deutsche Telekom. However, there may be changes that need to be applied to the module for proper
// operation with any given European MNO such as attach type, RAT preference, band selection, etc. Please
// consult with the preferred network provider.
typedef enum
{
  MNO_INVALID = -1,
  MNO_SW_DEFAULT = 0, // Undefined / regulatory
  MNO_SIM_ICCID = 1,
  MNO_ATT = 2, // AT&T
  MNO_VERIZON = 3,
  MNO_TELSTRA = 4,
  MNO_TMO = 5, // T-Mobile US
  MNO_CT = 6,  // China Telecom
  MNO_SPRINT = 8,
  MNO_VODAFONE = 19,
  MNO_NTT_DOCOMO = 20,
  MNO_TELUS = 21,
  MNO_SOFTBANK = 28,
  MNO_DT = 31, // Deutsche Telekom
  MNO_US_CELLULAR = 32,
  MNO_SKT = 39,
  MNO_GLOBAL = 90,
  MNO_STD_EUROPE = 100,
  MNO_STD_EU_NOEPCO = 101
} mobile_network_operator_t;

typedef enum
{
  UBX_CELL_ERROR_INVALID = -1,         // -1
  UBX_CELL_ERROR_SUCCESS = 0,          // 0
  UBX_CELL_ERROR_OUT_OF_MEMORY,        // 1
  UBX_CELL_ERROR_TIMEOUT,              // 2
  UBX_CELL_ERROR_UNEXPECTED_PARAM,     // 3
  UBX_CELL_ERROR_UNEXPECTED_RESPONSE,  // 4
  UBX_CELL_ERROR_NO_RESPONSE,          // 5
  UBX_CELL_ERROR_DEREGISTERED,         // 6
  UBX_CELL_ERROR_ZERO_READ_LENGTH,     // 7
  UBX_CELL_ERROR_ERROR                 // 8
} UBX_CELL_error_t;
#define UBX_CELL_SUCCESS UBX_CELL_ERROR_SUCCESS

typedef enum
{
  UBX_CELL_REGISTRATION_INVALID = -1,
  UBX_CELL_REGISTRATION_NOT_REGISTERED = 0,
  UBX_CELL_REGISTRATION_HOME = 1,
  UBX_CELL_REGISTRATION_SEARCHING = 2,
  UBX_CELL_REGISTRATION_DENIED = 3,
  UBX_CELL_REGISTRATION_UNKNOWN = 4,
  UBX_CELL_REGISTRATION_ROAMING = 5,
  UBX_CELL_REGISTRATION_HOME_SMS_ONLY = 6,
  UBX_CELL_REGISTRATION_ROAMING_SMS_ONLY = 7,
  UBX_CELL_REGISTRATION_EMERGENCY_SERV_ONLY = 8,
  UBX_CELL_REGISTRATION_HOME_CSFB_NOT_PREFERRED = 9,
  UBX_CELL_REGISTRATION_ROAMING_CSFB_NOT_PREFERRED = 10
} UBX_CELL_registration_status_t;

struct DateData
{
  uint8_t day;
  uint8_t month;
  unsigned int year;
};

struct TimeData
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  unsigned int ms;
  uint8_t tzh;
  uint8_t tzm;
};

struct ClockData
{
  struct DateData date;
  struct TimeData time;
};

struct PositionData
{
  float utc;
  float lat; // Degrees: +/- 90
  float lon; // Degrees: +/- 180
  float alt;
  char mode;
  char status;
};

struct SpeedData
{
  float speed;  // m/s
  float cog;    // Degrees
  float magVar; // Degrees
};

struct operator_stats
{
  uint8_t stat;
  String shortOp;
  String longOp;
  unsigned long numOp;
  uint8_t act;
};

typedef struct ext_signal_quality_ {
    unsigned int rxlev;
    unsigned int ber;
    unsigned int rscp;
    unsigned int enc0;
    unsigned int rsrq;
    unsigned int rsrp;
} signal_quality;

typedef enum
{
  UBX_CELL_TCP = 6,
  UBX_CELL_UDP = 17
} UBX_CELL_socket_protocol_t;

typedef enum
{
  UBX_CELL_TCP_SOCKET_STATUS_INACTIVE,
  UBX_CELL_TCP_SOCKET_STATUS_LISTEN,
  UBX_CELL_TCP_SOCKET_STATUS_SYN_SENT,
  UBX_CELL_TCP_SOCKET_STATUS_SYN_RCVD,
  UBX_CELL_TCP_SOCKET_STATUS_ESTABLISHED,
  UBX_CELL_TCP_SOCKET_STATUS_FIN_WAIT_1,
  UBX_CELL_TCP_SOCKET_STATUS_FIN_WAIT_2,
  UBX_CELL_TCP_SOCKET_STATUS_CLOSE_WAIT,
  UBX_CELL_TCP_SOCKET_STATUS_CLOSING,
  UBX_CELL_TCP_SOCKET_STATUS_LAST_ACK,
  UBX_CELL_TCP_SOCKET_STATUS_TIME_WAIT
} UBX_CELL_tcp_socket_status_t;

typedef enum
{
  UBX_CELL_MESSAGE_FORMAT_PDU = 0,
  UBX_CELL_MESSAGE_FORMAT_TEXT = 1
} UBX_CELL_message_format_t;

typedef enum
{
  UBX_CELL_UTIME_MODE_STOP = 0,
  UBX_CELL_UTIME_MODE_PPS,
  UBX_CELL_UTIME_MODE_ONE_SHOT,
  UBX_CELL_UTIME_MODE_EXT_INT
} UBX_CELL_utime_mode_t;

typedef enum
{
  UBX_CELL_UTIME_SENSOR_NONE = 0,
  UBX_CELL_UTIME_SENSOR_GNSS_LTE = 1,
  UBX_CELL_UTIME_SENSOR_LTE
} UBX_CELL_utime_sensor_t;

typedef enum
{
  UBX_CELL_UTIME_URC_CONFIGURATION_DISABLED = 0,
  UBX_CELL_UTIME_URC_CONFIGURATION_ENABLED
} UBX_CELL_utime_urc_configuration_t;

typedef enum
{
  UBX_CELL_SIM_NOT_PRESENT = 0,
  UBX_CELL_SIM_PIN_NEEDED,
  UBX_CELL_SIM_PIN_BLOCKED,
  UBX_CELL_SIM_PUK_BLOCKED,
  UBX_CELL_SIM_NOT_OPERATIONAL,
  UBX_CELL_SIM_RESTRICTED,
  UBX_CELL_SIM_OPERATIONAL
  //UBX_CELL_SIM_PHONEBOOK_READY, // Not reported by SARA-R5
  //UBX_CELL_SIM_USIM_PHONEBOOK_READY, // Not reported by SARA-R5
  //UBX_CELL_SIM_TOOLKIT_REFRESH_SUCCESSFUL, // Not reported by SARA-R5
  //UBX_CELL_SIM_TOOLKIT_REFRESH_UNSUCCESSFUL, // Not reported by SARA-R5
  //UBX_CELL_SIM_PPP_CONNECTION_ACTIVE, // Not reported by SARA-R5
  //UBX_CELL_SIM_VOICE_CALL_ACTIVE, // Not reported by SARA-R5
  //UBX_CELL_SIM_CSD_CALL_ACTIVE // Not reported by SARA-R5
} UBX_CELL_sim_states_t;

#define UBX_CELL_NUM_PSD_PROFILES 6             // Number of supported PSD profiles
#define UBX_CELL_NUM_PDP_CONTEXT_IDENTIFIERS 11 // Number of supported PDP context identifiers
#define UBX_CELL_NUM_HTTP_PROFILES 4            // Number of supported HTTP profiles

typedef enum
{
  UBX_CELL_HTTP_OP_CODE_SERVER_IP = 0,
  UBX_CELL_HTTP_OP_CODE_SERVER_NAME,
  UBX_CELL_HTTP_OP_CODE_USERNAME,
  UBX_CELL_HTTP_OP_CODE_PASSWORD,
  UBX_CELL_HTTP_OP_CODE_AUTHENTICATION,
  UBX_CELL_HTTP_OP_CODE_SERVER_PORT,
  UBX_CELL_HTTP_OP_CODE_SECURE,
  UBX_CELL_HTTP_OP_CODE_REQUEST_TIMEOUT,
  UBX_CELL_HTTP_OP_CODE_ADD_CUSTOM_HEADERS = 9
} UBX_CELL_http_op_codes_t;

typedef enum
{
  UBX_CELL_HTTP_COMMAND_HEAD = 0,
  UBX_CELL_HTTP_COMMAND_GET,
  UBX_CELL_HTTP_COMMAND_DELETE,
  UBX_CELL_HTTP_COMMAND_PUT,
  UBX_CELL_HTTP_COMMAND_POST_FILE,
  UBX_CELL_HTTP_COMMAND_POST_DATA,
  UBX_CELL_HTTP_COMMAND_GET_FOTA = 100
} UBX_CELL_http_commands_t;

typedef enum
{
  UBX_CELL_HTTP_CONTENT_APPLICATION_X_WWW = 0,
  UBX_CELL_HTTP_CONTENT_TEXT_PLAIN,
  UBX_CELL_HTTP_CONTENT_APPLICATION_OCTET,
  UBX_CELL_HTTP_CONTENT_MULTIPART_FORM,
  UBX_CELL_HTTP_CONTENT_APPLICATION_JSON,
  UBX_CELL_HTTP_CONTENT_APPLICATION_XML,
  UBX_CELL_HTTP_CONTENT_USER_DEFINED
} UBX_CELL_http_content_types_t;

typedef enum
{
    UBX_CELL_MQTT_NV_RESTORE = 0,
    UBX_CELL_MQTT_NV_SET,
    UBX_CELL_MQTT_NV_STORE,
} UBX_CELL_mqtt_nv_parameter_t;

typedef enum
{
    UBX_CELL_MQTT_PROFILE_CLIENT_ID = 0,
    UBX_CELL_MQTT_PROFILE_SERVERNAME = 2,
    UBX_CELL_MQTT_PROFILE_IPADDRESS,
    UBX_CELL_MQTT_PROFILE_USERNAMEPWD,
    UBX_CELL_MQTT_PROFILE_QOS = 6,
    UBX_CELL_MQTT_PROFILE_RETAIN,
    UBX_CELL_MQTT_PROFILE_TOPIC,
    UBX_CELL_MQTT_PROFILE_MESSAGE,
    UBX_CELL_MQTT_PROFILE_INACTIVITYTIMEOUT,
    UBX_CELL_MQTT_PROFILE_SECURE,
} UBX_CELL_mqtt_profile_opcode_t;

typedef enum
{
  UBX_CELL_MQTT_COMMAND_INVALID = -1,
  UBX_CELL_MQTT_COMMAND_LOGOUT = 0,
  UBX_CELL_MQTT_COMMAND_LOGIN,
  UBX_CELL_MQTT_COMMAND_PUBLISH,
  UBX_CELL_MQTT_COMMAND_PUBLISHFILE,
  UBX_CELL_MQTT_COMMAND_SUBSCRIBE,
  UBX_CELL_MQTT_COMMAND_UNSUBSCRIBE,
  UBX_CELL_MQTT_COMMAND_READ,
  UBX_CELL_MQTT_COMMAND_RCVMSGFORMAT,
  UBX_CELL_MQTT_COMMAND_PING,
  UBX_CELL_MQTT_COMMAND_PUBLISHBINARY,
} UBX_CELL_mqtt_command_opcode_t;

constexpr uint16_t MAX_MQTT_HEX_MSG_LEN = 512;
constexpr uint16_t MAX_MQTT_DIRECT_MSG_LEN = 1024;

typedef enum
{
    UBX_CELL_FTP_PROFILE_IPADDRESS = 0,
    UBX_CELL_FTP_PROFILE_SERVERNAME,
    UBX_CELL_FTP_PROFILE_USERNAME,
    UBX_CELL_FTP_PROFILE_PWD,
    UBX_CELL_FTP_PROFILE_ACCOUNT,
    UBX_CELL_FTP_PROFILE_TIMEOUT,
    UBX_CELL_FTP_PROFILE_MODE
} UBX_CELL_ftp_profile_opcode_t;

typedef enum
{
  UBX_CELL_FTP_COMMAND_INVALID = -1,
  UBX_CELL_FTP_COMMAND_LOGOUT = 0,
  UBX_CELL_FTP_COMMAND_LOGIN,
  UBX_CELL_FTP_COMMAND_DELETE_FILE,
  UBX_CELL_FTP_COMMAND_RENAME_FILE,
  UBX_CELL_FTP_COMMAND_GET_FILE,
  UBX_CELL_FTP_COMMAND_PUT_FILE,
  UBX_CELL_FTP_COMMAND_GET_FILE_DIRECT,
  UBX_CELL_FTP_COMMAND_PUT_FILE_DIRECT,
  UBX_CELL_FTP_COMMAND_CHANGE_DIR,
  UBX_CELL_FTP_COMMAND_MKDIR = 10,
  UBX_CELL_FTP_COMMAND_RMDIR,
  UBX_CELL_FTP_COMMAND_DIR_INFO = 13,
  UBX_CELL_FTP_COMMAND_LS,
  UBX_CELL_FTP_COMMAND_GET_FOTA_FILE = 100
} UBX_CELL_ftp_command_opcode_t;

typedef enum
{
  UBX_CELL_PSD_CONFIG_PARAM_PROTOCOL = 0,
  UBX_CELL_PSD_CONFIG_PARAM_APN,
  //UBX_CELL_PSD_CONFIG_PARAM_USERNAME, // Not allowed on SARA-R5
  //UBX_CELL_PSD_CONFIG_PARAM_PASSWORD, // Not allowed on SARA-R5
  UBX_CELL_PSD_CONFIG_PARAM_DNS1 = 4,
  UBX_CELL_PSD_CONFIG_PARAM_DNS2,
  //UBX_CELL_PSD_CONFIG_PARAM_AUTHENTICATION, // Not allowed on SARA-R5
  //UBX_CELL_PSD_CONFIG_PARAM_IP_ADDRESS, // Not allowed on SARA-R5
  //UBX_CELL_PSD_CONFIG_PARAM_DATA_COMPRESSION, // Not allowed on SARA-R5
  //UBX_CELL_PSD_CONFIG_PARAM_HEADER_COMPRESSION, // Not allowed on SARA-R5
  UBX_CELL_PSD_CONFIG_PARAM_MAP_TO_CID = 100
} UBX_CELL_pdp_configuration_parameter_t;

typedef enum
{
  UBX_CELL_PSD_PROTOCOL_IPV4 = 0,
  UBX_CELL_PSD_PROTOCOL_IPV6,
  UBX_CELL_PSD_PROTOCOL_IPV4V6_V4_PREF,
  UBX_CELL_PSD_PROTOCOL_IPV4V6_V6_PREF
} UBX_CELL_pdp_protocol_type_t;

typedef enum
{
  UBX_CELL_PSD_ACTION_RESET = 0,
  UBX_CELL_PSD_ACTION_STORE,
  UBX_CELL_PSD_ACTION_LOAD,
  UBX_CELL_PSD_ACTION_ACTIVATE,
  UBX_CELL_PSD_ACTION_DEACTIVATE
} UBX_CELL_pdp_actions_t;

typedef enum
{
  UBX_CELL_SEC_PROFILE_PARAM_CERT_VAL_LEVEL = 0,
  UBX_CELL_SEC_PROFILE_PARAM_TLS_VER,
  UBX_CELL_SEC_PROFILE_PARAM_CYPHER_SUITE,
  UBX_CELL_SEC_PROFILE_PARAM_ROOT_CA,
  UBX_CELL_SEC_PROFILE_PARAM_HOSTNAME,
  UBX_CELL_SEC_PROFILE_PARAM_CLIENT_CERT,
  UBX_CELL_SEC_PROFILE_PARAM_CLIENT_KEY,
  UBX_CELL_SEC_PROFILE_PARAM_CLIENT_KEY_PWD,
  UBX_CELL_SEC_PROFILE_PARAM_PSK,
  UBX_CELL_SEC_PROFILE_PARAM_PSK_IDENT,
  UBX_CELL_SEC_PROFILE_PARAM_SNI,
} UBX_CELL_sec_profile_parameter_t;

typedef enum
{
  UBX_CELL_SEC_PROFILE_CERTVAL_OPCODE_NO = 0,
  UBX_CELL_SEC_PROFILE_CERTVAL_OPCODE_YESNOURL,
  UBX_CELL_SEC_PROFILE_CERVTAL_OPCODE_YESURL,
  UBX_CELL_SEC_PROFILE_CERTVAL_OPCODE_YESURLDATE,
} UBX_CELL_sec_profile_certval_op_code_t;

typedef enum
{
  UBX_CELL_SEC_PROFILE_TLS_OPCODE_ANYVER = 0,
  UBX_CELL_SEC_PROFILE_TLS_OPCODE_VER1_0,
  UBX_CELL_SEC_PROFILE_TLS_OPCODE_VER1_1,
  UBX_CELL_SEC_PROFILE_TLS_OPCODE_VER1_2,
  UBX_CELL_SEC_PROFILE_TLS_OPCODE_VER1_3,
} UBX_CELL_sec_profile_tls_op_code_t;

typedef enum
{
  UBX_CELL_SEC_PROFILE_SUITE_OPCODE_PROPOSEDDEFAULT = 0,
} UBX_CELL_sec_profile_suite_op_code_t;

typedef enum
{
    UBX_CELL_SEC_MANAGER_OPCODE_IMPORT = 0,
} UBX_CELL_sec_manager_opcode_t;

typedef enum
{
    UBX_CELL_SEC_MANAGER_ROOTCA = 0,
    UBX_CELL_SEC_MANAGER_CLIENT_CERT,
    UBX_CELL_SEC_MANAGER_CLIENT_KEY,
    UBX_CELL_SEC_MANAGER_SERVER_CERT
} UBX_CELL_sec_manager_parameter_t;

typedef enum
{
  MINIMUM_FUNCTIONALITY = 0, // (disable both transmit and receive RF circuits by deactivating both CS and PS services)
  FULL_FUNCTIONALITY = 1,
  AIRPLANE_MODE = 4,
  SIM_TOOLKIT_ENABLE_DEDICATED = 6,
  SIM_TOOLKIT_DISABLE_DEDICATED = 7,
  SIM_TOOLKIT_ENABLE_RAW = 9,
  FAST_SAFE_POWER_OFF = 10,
  //SILENT_RESET_WITHOUT_SIM = 15, // Not supported on SARA-R5
  SILENT_RESET_WITH_SIM = 16
  //MINIMUM_FUNCTIONALITY = 19, // Not supported on SARA-R5
  //DEEP_LOW_POWER_STATE = 127 // Not supported on SARA-R5
} UBX_CELL_functionality_t;

class UBX_CELL : public Print
{
public:
  // Constructor
  // The library will use the powerPin and resetPin (if provided) to power the module off/on and perform an emergency reset
  // maxInitTries sets the maximum number of initialization attempts. .init is called by .begin.
  UBX_CELL(int powerPin = UBX_CELL_POWER_PIN, int resetPin = UBX_CELL_RESET_PIN, uint8_t maxInitTries = 9);

  ~UBX_CELL();
  // Begin -- initialize module and ensure it's connected
#ifdef UBX_CELL_SOFTWARE_SERIAL_ENABLED
  bool begin(SoftwareSerial &softSerial, unsigned long baud = UBX_CELL_DEFAULT_BAUD_RATE);
#endif
  bool begin(HardwareSerial &hardSerial, unsigned long baud = UBX_CELL_DEFAULT_BAUD_RATE);

  // Debug prints
  void enableDebugging(Print &debugPort = Serial); //Turn on debug printing. If user doesn't specify then Serial will be used.
  void enableAtDebugging(Print &debugPort = Serial); //Turn on AT debug printing. If user doesn't specify then Serial will be used.

  // Invert the polarity of the power pin - if required
  // Normally the SARA's power pin is pulled low and released to toggle the power
  // But the Asset Tracker needs this to be pulled high and released instead
  void invertPowerPin(bool invert = false);

  UBX_CELL_error_t modulePowerOff(void); // Graceful disconnect and shutdown using +CPWROFF.
  void modulePowerOn(void); // Requires access to the PWR_ON pin

  // Loop polling and polling setup - process URC's etc. from the module

  // This function was originally written by Matthew Menze for the LTE Shield (SARA-R4) library
  // See: https://github.com/sparkfun/SparkFun_LTE_Shield_Arduino_Library/pull/8
  // It does the same job as ::poll but also processes any 'old' data stored in the backlog first
  // It also has a built-in timeout - which ::poll does not
  // Use this - it is way better than ::poll. Thank you Matthew!
  bool bufferedPoll(void);

  // This is the original poll function.
  // It is 'blocking' - it does not return when serial data is available until it receives a `\n`.
  // ::bufferedPoll is the new improved version. It processes any data in the backlog and includes a timeout.
  // Retained for backward-compatibility and just in case you do want to (temporarily) ignore any data in the backlog
  bool poll(void);

  // Callbacks (called during polling)
  void setSocketListenCallback(void (*socketListenCallback)(int, IPAddress, unsigned int, int, IPAddress, unsigned int)); // listen Socket, local IP Address, listen Port, socket, remote IP Address, port
  // This is the original read socket callback - called when a +UUSORD or +UUSORF URC is received
  // It works - and handles binary data correctly - but the remote IP Address and Port are lost for UDP connections
  // setSocketReadCallbackPlus is preferred!
  void setSocketReadCallback(void (*socketReadCallback)(int, String)); // socket, read data
  void setSocketReadCallbackPlus(void (*socketReadCallbackPlus)(int, const char *, int, IPAddress, int)); // socket, read data, length, remoteAddress, remotePort
  void setSocketCloseCallback(void (*socketCloseCallback)(int)); // socket
  void setGpsReadCallback(void (*gpsRequestCallback)(ClockData time,
                                                     PositionData gps, SpeedData spd, unsigned long uncertainty));
  void setSIMstateReportCallback(void (*simStateRequestCallback)(UBX_CELL_sim_states_t state));
  void setPSDActionCallback(void (*psdActionRequestCallback)(int result, IPAddress ip));
  void setPingCallback(void (*pingRequestCallback)(int retry, int p_size, String remote_hostname, IPAddress ip, int ttl, long rtt));
  void setHTTPCommandCallback(void (*httpCommandRequestCallback)(int profile, int command, int result));
  void setMQTTCommandCallback(void (*mqttCommandRequestCallback)(int command, int result));
  void setFTPCommandCallback(void (*ftpCommandRequestCallback)(int command, int result));

  UBX_CELL_error_t setRegistrationCallback(void (*registrationCallback)(UBX_CELL_registration_status_t status,
                                                                       unsigned int lac, unsigned int ci, int Act));
  UBX_CELL_error_t setEpsRegistrationCallback(void (*epsRegistrationCallback)(UBX_CELL_registration_status_t status,
                                                                            unsigned int tac, unsigned int ci, int Act));

  // Direct write/print to cell serial port
  virtual size_t write(uint8_t c);
  virtual size_t write(const char *str);
  virtual size_t write(const char *buffer, size_t size);

  // General AT Commands
  UBX_CELL_error_t at(void);
  UBX_CELL_error_t enableEcho(bool enable = true);
  String getManufacturerID(void);
  String getModelID(void);
  String getFirmwareVersion(void);
  String getSerialNo(void);
  String getIMEI(void);
  String getIMSI(void);
  String getCCID(void);
  String getSubscriberNo(void);
  String getCapabilities(void);

  // Control and status AT commands
  UBX_CELL_error_t reset(void);
  String clock(void);
  // TODO: Return a clock struct
  UBX_CELL_error_t clock(uint8_t *y, uint8_t *mo, uint8_t *d,
                        uint8_t *h, uint8_t *min, uint8_t *s, int8_t *tz); // TZ can be +/- and is in increments of 15 minutes. -28 == 7 hours behind UTC/GMT
  UBX_CELL_error_t setClock(String theTime);
  UBX_CELL_error_t setClock(uint8_t y, uint8_t mo, uint8_t d,
                            uint8_t h, uint8_t min, uint8_t s, int8_t tz); // TZ can be +/- and is in increments of 15 minutes. -28 == 7 hours behind UTC/GMT
  void autoTimeZoneForBegin(bool enable = true); // Call autoTimeZoneForBegin(false) _before_ .begin if you want to disable the automatic time zone
  UBX_CELL_error_t autoTimeZone(bool enable); // Enable/disable automatic time zone adjustment

  // Network service AT commands
  int8_t rssi(void); // Receive signal strength
  UBX_CELL_error_t getExtSignalQuality(signal_quality& signal_quality);

  UBX_CELL_registration_status_t registration(bool eps = true);
  bool setNetworkProfile(mobile_network_operator_t mno, bool autoReset = false, bool urcNotification = false);
  mobile_network_operator_t getNetworkProfile(void);
  typedef enum
  {
    PDP_TYPE_INVALID = -1,
    PDP_TYPE_IP = 0,
    PDP_TYPE_NONIP = 1,
    PDP_TYPE_IPV4V6 = 2,
    PDP_TYPE_IPV6 = 3
  } UBX_CELL_pdp_type;
  UBX_CELL_error_t setAPN(String apn, uint8_t cid = 1, UBX_CELL_pdp_type pdpType = PDP_TYPE_IP); // Set the Access Point Name
  UBX_CELL_error_t getAPN(int cid, String *apn, IPAddress *ip, UBX_CELL_pdp_type* pdpType = nullptr);                                 // Return the apn and IP address for the chosen context identifier

  UBX_CELL_error_t getSimStatus(String* code);
  UBX_CELL_error_t setSimPin(String pin);

  // SIM
  // Status report Mode:
  // Bit   States reported
  // 0     Reports the (U)SIM initialization status (<state>'s from 0 to 6 may be reported)
  // 1     Reports the (U)SIM phonebook initialization status (<state>'s from 7 to 8 may be reported)
  // 2     Reports the (U)SIM toolkit REFRESH proactive command execution result (<state>'s from 9 to 13 may be reported)
  // Note: For the SARA-R5: <state>=7, 8, 9, 10, 11, 12 and 13 are not reported.
  UBX_CELL_error_t setSIMstateReportingMode(int mode);
  UBX_CELL_error_t getSIMstateReportingMode(int *mode);

  typedef enum
  {
    L2P_DEFAULT,
    L2P_PPP,
    L2P_M_HEX,
    L2P_M_RAW_IP,
    L2P_M_OPT_PPP
  } UBX_CELL_l2p_t;
  UBX_CELL_error_t enterPPP(uint8_t cid = 1, char dialing_type_char = 0,
                           unsigned long dialNumber = 99, UBX_CELL_l2p_t l2p = L2P_DEFAULT);

  uint8_t getOperators(struct operator_stats *op, int maxOps = 3);
  UBX_CELL_error_t registerOperator(struct operator_stats oper);
  UBX_CELL_error_t automaticOperatorSelection();
  UBX_CELL_error_t getOperator(String *oper);
  UBX_CELL_error_t deregisterOperator(void);

  // SMS -- Short Messages Service
  UBX_CELL_error_t setSMSMessageFormat(UBX_CELL_message_format_t textMode = UBX_CELL_MESSAGE_FORMAT_TEXT);
  UBX_CELL_error_t sendSMS(String number, String message);
  UBX_CELL_error_t getPreferredMessageStorage(int *used, int *total, String memory = "ME");
  UBX_CELL_error_t readSMSmessage(int location, String *unread, String *from, String *dateTime, String *message);
  UBX_CELL_error_t deleteSMSmessage(int location, int deleteFlag = 0); // Default to deleting the single message at the specified location
  UBX_CELL_error_t deleteReadSMSmessages(void)           { return (deleteSMSmessage( 1, 1 )); }; // Delete all the read messages from preferred storage
  UBX_CELL_error_t deleteReadSentSMSmessages(void)       { return (deleteSMSmessage( 1, 2 )); }; // Delete the read and sent messages from preferred storage
  UBX_CELL_error_t deleteReadSentUnsentSMSmessages(void) { return (deleteSMSmessage( 1, 3 )); }; // Delete the read, sent and unsent messages from preferred storage
  UBX_CELL_error_t deleteAllSMSmessages(void)            { return (deleteSMSmessage( 1, 4 )); }; // Delete the read, sent, unsent and unread messages from preferred storage

  // V24 Control and V25ter (UART interface) AT commands
  UBX_CELL_error_t setBaud(unsigned long baud);
  UBX_CELL_error_t setFlowControl(UBX_CELL_flow_control_t value = UBX_CELL_ENABLE_FLOW_CONTROL);

  // GPIO
  // GPIO pin map
  typedef enum
  {
    GPIO1 = 16,
    GPIO2 = 23,
    GPIO3 = 24,
    GPIO4 = 25,
    GPIO5 = 42,
    GPIO6 = 19
  } UBX_CELL_gpio_t;
  // GPIO pin modes
  typedef enum
  {
    GPIO_MODE_INVALID = -1,
    GPIO_OUTPUT = 0,
    GPIO_INPUT,
    NETWORK_STATUS,
    GNSS_SUPPLY_ENABLE,
    GNSS_DATA_READY,
    GNSS_RTC_SHARING,
    JAMMING_DETECTION,
    SIM_CARD_DETECTION,
    HEADSET_DETECTION,
    GSM_TX_BURST_INDICATION,
    MODULE_STATUS_INDICATION,
    MODULE_OPERATING_MODE_INDICATION,
    I2S_DIGITAL_AUDIO_INTERFACE,
    SPI_SERIAL_INTERFACE,
    MASTER_CLOCK_GENRATION,
    UART_INTERFACE,
    WIFI_ENABLE,
    RING_INDICATION = 18,
    LAST_GASP_ENABLE,
    EXTERNAL_GNSS_ANTENNA,
    TIME_PULSE_GNSS,
    TIME_PULSE_OUTPUT,
    TIMESTAMP,
    FAST_POWER_OFF,
    LWM2M_PULSE,
    HARDWARE_FLOW_CONTROL,
    ANTENNA_TUNING,
    EXT_GNSS_TIME_PULSE,
    EXT_GNSS_TIMESTAMP,
    DTR_MODE,
    KHZ_32768_OUT = 32,
    PAD_DISABLED = 255
  } UBX_CELL_gpio_mode_t;
  UBX_CELL_error_t setGpioMode(UBX_CELL_gpio_t gpio, UBX_CELL_gpio_mode_t mode, int value = 0);
  UBX_CELL_gpio_mode_t getGpioMode(UBX_CELL_gpio_t gpio);

  // IP Transport Layer
  int socketOpen(UBX_CELL_socket_protocol_t protocol, unsigned int localPort = 0); // Open a socket. Returns the socket number.
  UBX_CELL_error_t socketClose(int socket, unsigned long timeout = UBX_CELL_2_MIN_TIMEOUT); // Close the socket
  UBX_CELL_error_t socketConnect(int socket, const char *address, unsigned int port); // TCP - connect to a remote IP Address using the specified port. Not required for UDP sockets.
  UBX_CELL_error_t socketConnect(int socket, IPAddress address, unsigned int port);
  // Write data to the specified socket. Works with binary data - but you must specify the data length when using the const char * version
  // Works with both TCP and UDP sockets - but socketWriteUDP is preferred for UDP and doesn't require socketOpen to be called first
  UBX_CELL_error_t socketWrite(int socket, const char *str, int len = -1);
  UBX_CELL_error_t socketWrite(int socket, String str); // OK for binary data
  // Write UDP data to the specified IP Address and port.
  // Works with binary data - but you must specify the data length when using the const char * versions
  // If you let len default to -1, strlen is used to calculate the data length - and will be incorrect for binary data
  UBX_CELL_error_t socketWriteUDP(int socket, const char *address, int port, const char *str, int len = -1);
  UBX_CELL_error_t socketWriteUDP(int socket, IPAddress address, int port, const char *str, int len = -1);
  UBX_CELL_error_t socketWriteUDP(int socket, String address, int port, String str);
  // Read data from the specified socket
  // Call socketReadAvailable first to determine how much data is available - or use the callbacks (triggered by URC's)
  // Works for both TCP and UDP - but socketReadUDP is preferred for UDP as it records the remote IP Address and port
  // bytesRead - if provided - will be updated with the number of bytes actually read. This could be less than length!
  UBX_CELL_error_t socketRead(int socket, int length, char *readDest, int *bytesRead = nullptr);
  // Return the number of bytes available (waiting to be read) on the chosen socket
  // Uses +USORD. Valid for both TCP and UDP sockets - but socketReadAvailableUDP is preferred for UDP
  UBX_CELL_error_t socketReadAvailable(int socket, int *length);
  // Read data from the specified UDP port
  // Call socketReadAvailableUDP first to determine how much data is available - or use the callbacks (triggered by URC's)
  // The remote IP Address and port are returned via *remoteIPAddress and *remotePort (if not nullptr)
  // bytesRead - if provided - will be updated with the number of bytes actually read. This could be less than length!
  UBX_CELL_error_t socketReadUDP(int socket, int length, char *readDest, IPAddress *remoteIPAddress = nullptr, int *remotePort = nullptr, int *bytesRead = nullptr);
  // Return the number of bytes available (waiting to be read) on the chosen UDP socket
  UBX_CELL_error_t socketReadAvailableUDP(int socket, int *length);
  // Start listening for a connection on the specified port. The connection is reported via the socket listen callback
  UBX_CELL_error_t socketListen(int socket, unsigned int port);
  // Place the socket into direct link mode - making it easy to transfer binary data. Wait two seconds and then send +++ to exit the link.
  UBX_CELL_error_t socketDirectLinkMode(int socket);
  // Configure when direct link data is sent
  UBX_CELL_error_t socketDirectLinkTimeTrigger(int socket, unsigned long timerTrigger);
  UBX_CELL_error_t socketDirectLinkDataLengthTrigger(int socket, int dataLengthTrigger);
  UBX_CELL_error_t socketDirectLinkCharacterTrigger(int socket, int characterTrigger);
  UBX_CELL_error_t socketDirectLinkCongestionTimer(int socket, unsigned long congestionTimer);
  // Use +USOCTL (Socket control) to query the socket parameters
  UBX_CELL_error_t querySocketType(int socket, UBX_CELL_socket_protocol_t *protocol);
  UBX_CELL_error_t querySocketLastError(int socket, int *error);
  UBX_CELL_error_t querySocketTotalBytesSent(int socket, uint32_t *total);
  UBX_CELL_error_t querySocketTotalBytesReceived(int socket, uint32_t *total);
  UBX_CELL_error_t querySocketRemoteIPAddress(int socket, IPAddress *address, int *port);
  UBX_CELL_error_t querySocketStatusTCP(int socket, UBX_CELL_tcp_socket_status_t *status);
  UBX_CELL_error_t querySocketOutUnackData(int socket, uint32_t *total);
  // Return the most recent socket error
  int socketGetLastError();
  // Return the remote IP Address from the most recent socket listen indication (socket connection)
  // Use the socket listen callback to get the full address and port information
  IPAddress lastRemoteIP(void);

  // Ping
  UBX_CELL_error_t ping(String remote_host, int retry = 4, int p_size = 32, unsigned long timeout = 5000, int ttl = 32);

  // HTTP
  UBX_CELL_error_t resetHTTPprofile(int profile);                          // Reset the HTTP profile. Note: The configured HTTP profile parameters are not saved in the non volatile memory.
  UBX_CELL_error_t setHTTPserverIPaddress(int profile, IPAddress address); // Default: empty string
  UBX_CELL_error_t setHTTPserverName(int profile, String server);          // Default: empty string
  UBX_CELL_error_t setHTTPusername(int profile, String username);          // Default: empty string
  UBX_CELL_error_t setHTTPpassword(int profile, String password);          // Default: empty string
  UBX_CELL_error_t setHTTPauthentication(int profile, bool authenticate);  // Default: no authentication
  UBX_CELL_error_t setHTTPserverPort(int profile, int port);               // Default: 80
  UBX_CELL_error_t setHTTPcustomHeader(int profile, String header);        // Default: format 0:Content-Type:application/json"
  UBX_CELL_error_t setHTTPsecure(int profile, bool secure, int secprofile = -1);  // Default: disabled (HTTP on port 80). Set to true for HTTPS on port 443
  // TO DO: Add custom request headers
  UBX_CELL_error_t getHTTPprotocolError(int profile, int *error_class, int *error_code); // Read the most recent HTTP protocol error for this profile
  UBX_CELL_error_t sendHTTPGET(int profile, String path, String responseFilename);
  UBX_CELL_error_t sendHTTPPOSTdata(int profile, String path, String responseFilename, String data, UBX_CELL_http_content_types_t httpContentType);
  UBX_CELL_error_t sendHTTPPOSTfile(int profile, String path, String responseFilename, String requestFile, UBX_CELL_http_content_types_t httpContentType);

  UBX_CELL_error_t nvMQTT(UBX_CELL_mqtt_nv_parameter_t parameter);
  UBX_CELL_error_t setMQTTclientId(const String& clientId);
  UBX_CELL_error_t setMQTTserver(const String& serverName, int port);
  UBX_CELL_error_t setMQTTcredentials(const String& userName, const String& pwd);
  UBX_CELL_error_t setMQTTsecure(bool secure, int secprofile = -1);
  UBX_CELL_error_t connectMQTT(void);
  UBX_CELL_error_t disconnectMQTT(void);
  UBX_CELL_error_t subscribeMQTTtopic(int max_Qos, const String& topic);
  UBX_CELL_error_t unsubscribeMQTTtopic(const String& topic);
  UBX_CELL_error_t readMQTT(int* pQos, String* pTopic, uint8_t *readDest, int readLength, int *bytesRead);
  UBX_CELL_error_t mqttPublishTextMsg(const String& topic, const char * const msg, uint8_t qos = 0, bool retain = false);
  UBX_CELL_error_t mqttPublishBinaryMsg(const String& topic, const char * const msg, size_t msg_len, uint8_t qos = 0, bool retain = false);
  UBX_CELL_error_t mqttPublishFromFile(const String& topic, const String& filename, uint8_t qos = 0, bool retain = false);
  UBX_CELL_error_t getMQTTprotocolError(int *error_code, int *error_code2);

  // FTP
  UBX_CELL_error_t setFTPserver(const String& serverName);
  UBX_CELL_error_t setFTPtimeouts(const unsigned int timeout, const unsigned int cmd_linger, const unsigned int data_linger);
  UBX_CELL_error_t setFTPcredentials(const String& userName, const String& pwd);
  UBX_CELL_error_t connectFTP(void);
  UBX_CELL_error_t disconnectFTP(void);
  UBX_CELL_error_t ftpGetFile(const String& filename);
  UBX_CELL_error_t getFTPprotocolError(int *error_code, int *error_code2);

  // Configure security profiles
  UBX_CELL_error_t resetSecurityProfile(int secprofile);
  UBX_CELL_error_t configSecurityProfileString(int secprofile, UBX_CELL_sec_profile_parameter_t parameter, String value);
  UBX_CELL_error_t configSecurityProfile(int secprofile, UBX_CELL_sec_profile_parameter_t parameter, int value);
  UBX_CELL_error_t setSecurityManager(UBX_CELL_sec_manager_opcode_t opcode, UBX_CELL_sec_manager_parameter_t parameter, String name, String data);

  UBX_CELL_error_t activatePDPcontext(bool status, int cid = -1);                // Activates or deactivates the specified PDP context. Default to all (cid = -1)

  // GPS
  typedef enum
  {
    GNSS_SYSTEM_GPS = 1,
    GNSS_SYSTEM_SBAS = 2,
    GNSS_SYSTEM_GALILEO = 4,
    GNSS_SYSTEM_BEIDOU = 8,
    GNSS_SYSTEM_IMES = 16,
    GNSS_SYSTEM_QZSS = 32,
    GNSS_SYSTEM_GLONASS = 64
  } gnss_system_t;
  typedef enum
  {
    GNSS_AIDING_MODE_NONE = 0,
    GNSS_AIDING_MODE_AUTOMATIC = 1,
    GNSS_AIDING_MODE_ASSISTNOW_OFFLINE = 2,
    GNSS_AIDING_MODE_ASSISTNOW_ONLINE = 4,
    GNSS_AIDING_MODE_ASSISTNOW_AUTONOMOUS = 8
  } gnss_aiding_mode_t;
  bool isGPSon(void);
  UBX_CELL_error_t gpsPower(bool enable = true,
                           gnss_system_t gnss_sys = GNSS_SYSTEM_GPS,
                           gnss_aiding_mode_t gnss_aiding = GNSS_AIDING_MODE_AUTOMATIC);
  //UBX_CELL_error_t gpsEnableClock(bool enable = true);
  //UBX_CELL_error_t gpsGetClock(struct ClockData *clock);
  //UBX_CELL_error_t gpsEnableFix(bool enable = true);
  //UBX_CELL_error_t gpsGetFix(float *lat, float *lon, unsigned int *alt, uint8_t *quality, uint8_t *sat);
  //UBX_CELL_error_t gpsGetFix(struct PositionData *pos);
  //UBX_CELL_error_t gpsEnablePos(bool enable = true);
  //UBX_CELL_error_t gpsGetPos(struct PositionData *pos);
  //UBX_CELL_error_t gpsEnableSat(bool enable = true);
  //UBX_CELL_error_t gpsGetSat(uint8_t *sats);
  UBX_CELL_error_t gpsEnableRmc(bool enable = true); // Enable GPRMC messages
  UBX_CELL_error_t gpsGetRmc(struct PositionData *pos, struct SpeedData *speed, struct ClockData *clk, bool *valid); //Parse a GPRMC message
  //UBX_CELL_error_t gpsEnableSpeed(bool enable = true);
  //UBX_CELL_error_t gpsGetSpeed(struct SpeedData *speed);

  UBX_CELL_error_t gpsRequest(unsigned int timeout, uint32_t accuracy, bool detailed = true, unsigned int sensor = 3);

  //CellLocate
  UBX_CELL_error_t gpsAidingServerConf(const char *primaryServer, const char *secondaryServer, const char *authToken,
                                      unsigned int days = 14, unsigned int period = 4, unsigned int resolution = 1,
                                      unsigned int gnssTypes = 65, unsigned int mode = 0, unsigned int dataType = 15);

  // File system
  // TO DO: add full support for file tags. Default tag to USER
  UBX_CELL_error_t getFileContents(String filename, String *contents); // OK for text files. But will fail with binary files (containing \0) on some platforms.
  UBX_CELL_error_t getFileContents(String filename, char *contents); // OK for binary files. Make sure contents can hold the entire file. Get the size first with getFileSize.
  UBX_CELL_error_t getFileBlock(const String& filename, char* buffer, size_t offset, size_t length, size_t& bytes_read); // OK for binary files. Make sure buffer can hold the requested block size.

  // Append data to a file, delete file first to not appends the data.
  UBX_CELL_error_t appendFileContents(String filename, String str);
  UBX_CELL_error_t appendFileContents(String filename, const char *str, int len);
  UBX_CELL_error_t getFileSize(String filename, int *size);
  UBX_CELL_error_t deleteFile(String filename);

  // Functionality
  UBX_CELL_error_t functionality(UBX_CELL_functionality_t function = FULL_FUNCTIONALITY);

  // Send a custom command with an expected (potentially partial) response, store entire response
  UBX_CELL_error_t sendCustomCommandWithResponse(const char *command, const char *expectedResponse,
                                                char *responseDest, unsigned long commandTimeout = UBX_CELL_STANDARD_RESPONSE_TIMEOUT, bool at = true);

  // Send command with an expected (potentially partial) response, store entire response
  UBX_CELL_error_t sendCommandWithResponse(const char *command, const char *expectedResponse,
                                          char *responseDest, unsigned long commandTimeout, int destSize = minimumResponseAllocation, bool at = true);

  char *ubx_cell_calloc_char(size_t num);

protected:
  HardwareSerial *_hardSerial;
#ifdef UBX_CELL_SOFTWARE_SERIAL_ENABLED
  SoftwareSerial *_softSerial;
#endif

  Print *_debugPort;       //The stream to send debug messages to if enabled. Usually Serial.
  bool _printDebug = false; //Flag to print debugging variables
  Print *_debugAtPort;      //The stream to send debug messages to if enabled. Usually Serial.
  bool _printAtDebug = false; //Flag to print debugging variables

  int _powerPin;
  int _resetPin;
  bool _invertPowerPin = false;

  unsigned long _baud;
  IPAddress _lastRemoteIP;
  IPAddress _lastLocalIP;
  uint8_t _maxInitTries;
  bool _autoTimeZoneForBegin = true;
  bool _bufferedPollReentrant = false; // Prevent reentry of bufferedPoll - just in case it gets called from a callback
  bool _pollReentrant = false; // Prevent reentry of poll - just in case it gets called from a callback

  #define _RXBuffSize 2056
  const unsigned long _rxWindowMillis = 2; // 1ms is not quite long enough for a single char at 9600 baud. millis roll over much less often than micros. See notes in .cpp re. ESP32!
  char *_saraRXBuffer; // Allocated in UBX_CELL::begin
  char *_pruneBuffer;
  char *_saraResponseBacklog;
  int _saraResponseBacklogLength = 0; // The backlog could contain binary data so we can't use strlen to find its length

  void (*_socketListenCallback)(int, IPAddress, unsigned int, int, IPAddress, unsigned int);
  void (*_socketReadCallback)(int, String);
  void (*_socketReadCallbackPlus)(int, const char *, int, IPAddress, int); // socket, data, length, remoteAddress, remotePort
  void (*_socketCloseCallback)(int);
  void (*_gpsRequestCallback)(ClockData, PositionData, SpeedData, unsigned long);
  void (*_simStateReportCallback)(UBX_CELL_sim_states_t);
  void (*_psdActionRequestCallback)(int, IPAddress);
  void (*_pingRequestCallback)(int, int, String, IPAddress, int, long);
  void (*_httpCommandRequestCallback)(int, int, int);
  void (*_mqttCommandRequestCallback)(int, int);
  void (*_ftpCommandRequestCallback)(int, int);
  void (*_registrationCallback)(UBX_CELL_registration_status_t status, unsigned int lac, unsigned int ci, int Act);
  void (*_epsRegistrationCallback)(UBX_CELL_registration_status_t status, unsigned int tac, unsigned int ci, int Act);


  int _lastSocketProtocol[UBX_CELL_NUM_SOCKETS]; // Record the protocol for each socket to avoid having to call querySocketType in parseSocketReadIndication

  typedef enum
  {
    UBX_CELL_INIT_STANDARD,
    UBX_CELL_INIT_AUTOBAUD,
    UBX_CELL_INIT_RESET
  } UBX_CELL_init_type_t;

  UBX_CELL_error_t init(unsigned long baud, UBX_CELL_init_type_t initType = UBX_CELL_INIT_STANDARD);

  void powerOn(void); // Brief pulse on PWR_ON to turn module back on
  void powerOff(void); // Long pulse on PWR_ON to do a graceful shutdown. Note modulePowerOff (+CPWROFF) is preferred.

  void hwReset(void);

  UBX_CELL_error_t setMNOprofile(mobile_network_operator_t mno, bool autoReset = false, bool urcNotification = false);
  UBX_CELL_error_t getMNOprofile(mobile_network_operator_t *mno);

  // Wait for an expected response (don't send a command)
  UBX_CELL_error_t waitForResponse(const char *expectedResponse, const char *expectedError, uint16_t timeout);

  // Send a command -- prepend AT if at is true
  void sendCommand(const char *command, bool at);

  const int _saraR5maxSocketRead = 1024; // The limit on bytes that can be read in a single read

  UBX_CELL_error_t parseSocketReadIndication(int socket, int length);
  UBX_CELL_error_t parseSocketReadIndicationUDP(int socket, int length);
  UBX_CELL_error_t parseSocketListenIndication(int listeningSocket, IPAddress localIP, unsigned int listeningPort, int socket, IPAddress remoteIP, unsigned int port);
  UBX_CELL_error_t parseSocketCloseIndication(String *closeIndication);

  // UART Functions
  size_t hwPrint(const char *s);
  size_t hwWriteData(const char *buff, int len);
  size_t hwWrite(const char c);
  int readAvailable(char *inString);
  char readChar(void);
  int hwAvailable(void);
  virtual void beginSerial(unsigned long baud);
  void setTimeout(unsigned long timeout);
  bool find(char *target);

  UBX_CELL_error_t autobaud(unsigned long desiredBaud);

  bool processURCEvent(const char *event);
  void pruneBacklog(void);

  // GPS Helper functions
  char *readDataUntil(char *destination, unsigned int destSize, char *source, char delimiter);
  bool parseGPRMCString(char *rmcString, PositionData *pos, ClockData *clk, SpeedData *spd);
};

#endif //SPARKFUN_UBX_CELL_ARDUINO_LIBRARY_H
