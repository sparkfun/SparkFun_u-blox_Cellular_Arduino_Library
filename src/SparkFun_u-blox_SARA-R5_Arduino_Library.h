// This file is purely for backwards compatibility with the original SARA-R5 library

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

#ifndef SPARKFUN_SARA_R5_ARDUINO_LIBRARY_H
#define SPARKFUN_SARA_R5_ARDUINO_LIBRARY_H

#include "sfe_sara_r5.h"

#define SARA_R5_POWER_PIN -1 // Default to no pin
#define SARA_R5_RESET_PIN -1

// Timing
#define SARA_R5_STANDARD_RESPONSE_TIMEOUT 1000
#define SARA_R5_10_SEC_TIMEOUT 10000
#define SARA_R5_55_SECS_TIMEOUT 55000
#define SARA_R5_2_MIN_TIMEOUT 120000
#define SARA_R5_3_MIN_TIMEOUT 180000
#define SARA_R5_SET_BAUD_TIMEOUT 500
#define SARA_R5_POWER_OFF_PULSE_PERIOD 3200 // Hold PWR_ON low for this long to power the module off
#define SARA_R5_POWER_ON_PULSE_PERIOD 100 // Hold PWR_ON low for this long to power the module on (SARA-R510M8S)
#define SARA_R5_RESET_PULSE_PERIOD 23000 // Used to perform an abrupt emergency hardware shutdown. 23 seconds... (Yes, really!)
#define SARA_R5_POWER_OFF_TIMEOUT 40000 // Datasheet says 40 seconds...
#define SARA_R5_IP_CONNECT_TIMEOUT 130000
#define SARA_R5_POLL_DELAY 1
#define SARA_R5_SOCKET_WRITE_TIMEOUT 10000

// ## Suported AT Commands
// ### General
const char SARA_R5_COMMAND_AT[] = "AT";           // AT "Test"
const char SARA_R5_COMMAND_ECHO[] = "E";          // Local Echo
const char SARA_R5_COMMAND_MANU_ID[] = "+CGMI";   // Manufacturer identification
const char SARA_R5_COMMAND_MODEL_ID[] = "+CGMM";  // Model identification
const char SARA_R5_COMMAND_FW_VER_ID[] = "+CGMR"; // Firmware version identification
const char SARA_R5_COMMAND_SERIAL_NO[] = "+CGSN"; // Product serial number
const char SARA_R5_COMMAND_IMEI[] = "+GSN";       // IMEI identification
const char SARA_R5_COMMAND_IMSI[] = "+CIMI";      // IMSI identification
const char SARA_R5_COMMAND_CCID[] = "+CCID";      // SIM CCID
const char SARA_R5_COMMAND_REQ_CAP[] = "+GCAP";   // Request capabilities list
// ### Control and status
const char SARA_R5_COMMAND_POWER_OFF[] = "+CPWROFF"; // Module switch off
const char SARA_R5_COMMAND_FUNC[] = "+CFUN";         // Functionality (reset, etc.)
const char SARA_R5_COMMAND_CLOCK[] = "+CCLK";        // Real-time clock
const char SARA_R5_COMMAND_AUTO_TZ[] = "+CTZU";      // Automatic time zone update
const char SARA_R5_COMMAND_TZ_REPORT[] = "+CTZR";    // Time zone reporting
// ### Network service
const char SARA_R5_COMMAND_CNUM[] = "+CNUM"; // Subscriber number
const char SARA_R5_SIGNAL_QUALITY[] = "+CSQ";
const char SARA_R5_EXT_SIGNAL_QUALITY[] = "+CESQ";
const char SARA_R5_OPERATOR_SELECTION[] = "+COPS";
const char SARA_R5_REGISTRATION_STATUS[] = "+CREG";
const char SARA_R5_EPSREGISTRATION_STATUS[] = "+CEREG";
const char SARA_R5_READ_OPERATOR_NAMES[] = "+COPN";
const char SARA_R5_COMMAND_MNO[] = "+UMNOPROF"; // MNO (mobile network operator) Profile
// ### SIM
const char SARA_R5_SIM_STATE[] = "+USIMSTAT";
const char SARA_R5_COMMAND_SIMPIN[] = "+CPIN";    // SIM PIN
// ### SMS
const char SARA_R5_MESSAGE_FORMAT[] = "+CMGF";     // Set SMS message format
const char SARA_R5_SEND_TEXT[] = "+CMGS";          // Send SMS message
const char SARA_R5_NEW_MESSAGE_IND[] = "+CNMI";    // New [SMS] message indication
const char SARA_R5_PREF_MESSAGE_STORE[] = "+CPMS"; // Preferred message storage
const char SARA_R5_READ_TEXT_MESSAGE[] = "+CMGR";  // Read message
const char SARA_R5_DELETE_MESSAGE[] = "+CMGD";     // Delete message
// V24 control and V25ter (UART interface)
const char SARA_R5_FLOW_CONTROL[] = "&K";   // Flow control
const char SARA_R5_COMMAND_BAUD[] = "+IPR"; // Baud rate
// ### Packet switched data services
const char SARA_R5_MESSAGE_PDP_DEF[] = "+CGDCONT";            // Packet switched Data Profile context definition
const char SARA_R5_MESSAGE_PDP_CONFIG[] = "+UPSD";            // Packet switched Data Profile configuration
const char SARA_R5_MESSAGE_PDP_ACTION[] = "+UPSDA";           // Perform the action for the specified PSD profile
const char SARA_R5_MESSAGE_PDP_CONTEXT_ACTIVATE[] = "+CGACT"; // Activates or deactivates the specified PDP context
const char SARA_R5_MESSAGE_ENTER_PPP[] = "D";
const char SARA_R5_NETWORK_ASSIGNED_DATA[] = "+UPSND";        // Packet switched network-assigned data
// ### GPIO
const char SARA_R5_COMMAND_GPIO[] = "+UGPIOC"; // GPIO Configuration
// ### IP
const char SARA_R5_CREATE_SOCKET[] = "+USOCR";      // Create a new socket
const char SARA_R5_CLOSE_SOCKET[] = "+USOCL";       // Close a socket
const char SARA_R5_CONNECT_SOCKET[] = "+USOCO";     // Connect to server on socket
const char SARA_R5_WRITE_SOCKET[] = "+USOWR";       // Write data to a socket
const char SARA_R5_WRITE_UDP_SOCKET[] = "+USOST";   // Write data to a UDP socket
const char SARA_R5_READ_SOCKET[] = "+USORD";        // Read from a socket
const char SARA_R5_READ_UDP_SOCKET[] = "+USORF";    // Read UDP data from a socket
const char SARA_R5_LISTEN_SOCKET[] = "+USOLI";      // Listen for connection on socket
const char SARA_R5_GET_ERROR[] = "+USOER";          // Get last socket error.
const char SARA_R5_SOCKET_DIRECT_LINK[] = "+USODL"; // Set socket in Direct Link mode
const char SARA_R5_SOCKET_CONTROL[] = "+USOCTL";    // Query the socket parameters
const char SARA_R5_UD_CONFIGURATION[] = "+UDCONF";  // User Datagram Configuration
// ### Ping
const char SARA_R5_PING_COMMAND[] = "+UPING"; // Ping
// ### HTTP
const char SARA_R5_HTTP_PROFILE[] = "+UHTTP";          // Configure the HTTP profile. Up to 4 different profiles can be defined
const char SARA_R5_HTTP_COMMAND[] = "+UHTTPC";         // Trigger the specified HTTP command
const char SARA_R5_HTTP_PROTOCOL_ERROR[] = "+UHTTPER"; // Retrieves the error class and code of the latest HTTP operation on the specified HTTP profile.

const char SARA_R5_MQTT_NVM[] = "+UMQTTNV";
const char SARA_R5_MQTT_PROFILE[] = "+UMQTT";
const char SARA_R5_MQTT_COMMAND[] = "+UMQTTC";
const char SARA_R5_MQTT_PROTOCOL_ERROR[] = "+UMQTTER";
// ### FTP
const char SARA_R5_FTP_PROFILE[] = "+UFTP";
const char SARA_R5_FTP_COMMAND[] = "+UFTPC";
const char SARA_R5_FTP_PROTOCOL_ERROR[] = "+UFTPER";
// ### GNSS
const char SARA_R5_GNSS_POWER[] = "+UGPS";                   // GNSS power management configuration
const char SARA_R5_GNSS_ASSISTED_IND[] = "+UGIND";           // Assisted GNSS unsolicited indication
const char SARA_R5_GNSS_REQUEST_LOCATION[] = "+ULOC";        // Ask for localization information
const char SARA_R5_GNSS_GPRMC[] = "+UGRMC";                  // Ask for localization information
const char SARA_R5_GNSS_REQUEST_TIME[] = "+UTIME";           // Ask for time information from cellular modem (CellTime)
const char SARA_R5_GNSS_TIME_INDICATION[] = "+UTIMEIND";     // Time information request status unsolicited indication
const char SARA_R5_GNSS_TIME_CONFIGURATION[] = "+UTIMECFG";  // Sets time configuration
const char SARA_R5_GNSS_CONFIGURE_SENSOR[] = "+ULOCGNSS";    // Configure GNSS sensor
const char SARA_R5_GNSS_CONFIGURE_LOCATION[] = "+ULOCCELL";  // Configure cellular location sensor (CellLocate®)
const char SARA_R5_AIDING_SERVER_CONFIGURATION[] = "+UGSRV"; // Configure aiding server (CellLocate®)
// ### File System
// TO DO: Add support for file tags. Default tag to USER
const char SARA_R5_FILE_SYSTEM_READ_FILE[] = "+URDFILE";      // Read a file
const char SARA_R5_FILE_SYSTEM_READ_BLOCK[] = "+URDBLOCK";      // Read a block from a file
const char SARA_R5_FILE_SYSTEM_DOWNLOAD_FILE[] = "+UDWNFILE";    // Download a file into the module
const char SARA_R5_FILE_SYSTEM_LIST_FILES[] = "+ULSTFILE";    // List of files, size of file, etc.
const char SARA_R5_FILE_SYSTEM_DELETE_FILE[] = "+UDELFILE";   // Delete a file
// ### File System
// TO DO: Add support for file tags. Default tag to USER
const char SARA_R5_SEC_PROFILE[] = "+USECPRF";
const char SARA_R5_SEC_MANAGER[] = "+USECMNG";


// ### URC strings
const char SARA_R5_READ_SOCKET_URC[] = "+UUSORD:";
const char SARA_R5_READ_UDP_SOCKET_URC[] = "+UUSORF:";
const char SARA_R5_LISTEN_SOCKET_URC[] = "+UUSOLI:";
const char SARA_R5_CLOSE_SOCKET_URC[] = "+UUSOCL:";
const char SARA_R5_GNSS_REQUEST_LOCATION_URC[] = "+UULOC:";
const char SARA_R5_SIM_STATE_URC[] = "+UUSIMSTAT:";
const char SARA_R5_MESSAGE_PDP_ACTION_URC[] = "+UUPSDA:";
const char SARA_R5_HTTP_COMMAND_URC[] = "+UUHTTPCR:";
const char SARA_R5_MQTT_COMMAND_URC[] = "+UUMQTTC:";
const char SARA_R5_PING_COMMAND_URC[] = "+UUPING:";
const char SARA_R5_REGISTRATION_STATUS_URC[] = "+CREG:";
const char SARA_R5_EPSREGISTRATION_STATUS_URC[] = "+CEREG:";
const char SARA_R5_FTP_COMMAND_URC[] = "+UUFTPCR:";

// ### Response
const char SARA_R5_RESPONSE_MORE[] = "\n>";
const char SARA_R5_RESPONSE_OK[] = "\nOK\r\n";
const char SARA_R5_RESPONSE_ERROR[] = "\nERROR\r\n";
const char SARA_R5_RESPONSE_CONNECT[] = "\r\nCONNECT\r\n";
#define SARA_R5_RESPONSE_OK_OR_ERROR nullptr

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

#define SARA_R5_NUM_SOCKETS 6

#define NUM_SUPPORTED_BAUD 6
const unsigned long SARA_R5_SUPPORTED_BAUD[NUM_SUPPORTED_BAUD] =
    {
        115200,
        9600,
        19200,
        38400,
        57600,
        230400};
#define SARA_R5_DEFAULT_BAUD_RATE 115200

// Flow control definitions for AT&K
// Note: SW (XON/XOFF) flow control is not supported on the SARA_R5
typedef enum
{
  SARA_R5_DISABLE_FLOW_CONTROL = 0,
  SARA_R5_ENABLE_FLOW_CONTROL = 3
} SARA_R5_flow_control_t;

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
  SARA_R5_ERROR_INVALID = -1,         // -1
  SARA_R5_ERROR_SUCCESS = 0,          // 0
  SARA_R5_ERROR_OUT_OF_MEMORY,        // 1
  SARA_R5_ERROR_TIMEOUT,              // 2
  SARA_R5_ERROR_UNEXPECTED_PARAM,     // 3
  SARA_R5_ERROR_UNEXPECTED_RESPONSE,  // 4
  SARA_R5_ERROR_NO_RESPONSE,          // 5
  SARA_R5_ERROR_DEREGISTERED,         // 6
  SARA_R5_ERROR_ZERO_READ_LENGTH,     // 7
  SARA_R5_ERROR_ERROR                 // 8
} SARA_R5_error_t;
#define SARA_R5_SUCCESS SARA_R5_ERROR_SUCCESS

typedef enum
{
  SARA_R5_REGISTRATION_INVALID = -1,
  SARA_R5_REGISTRATION_NOT_REGISTERED = 0,
  SARA_R5_REGISTRATION_HOME = 1,
  SARA_R5_REGISTRATION_SEARCHING = 2,
  SARA_R5_REGISTRATION_DENIED = 3,
  SARA_R5_REGISTRATION_UNKNOWN = 4,
  SARA_R5_REGISTRATION_ROAMING = 5,
  SARA_R5_REGISTRATION_HOME_SMS_ONLY = 6,
  SARA_R5_REGISTRATION_ROAMING_SMS_ONLY = 7,
  SARA_R5_REGISTRATION_EMERGENCY_SERV_ONLY = 8,
  SARA_R5_REGISTRATION_HOME_CSFB_NOT_PREFERRED = 9,
  SARA_R5_REGISTRATION_ROAMING_CSFB_NOT_PREFERRED = 10
} SARA_R5_registration_status_t;

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
  SARA_R5_TCP = 6,
  SARA_R5_UDP = 17
} SARA_R5_socket_protocol_t;

typedef enum
{
  SARA_R5_TCP_SOCKET_STATUS_INACTIVE,
  SARA_R5_TCP_SOCKET_STATUS_LISTEN,
  SARA_R5_TCP_SOCKET_STATUS_SYN_SENT,
  SARA_R5_TCP_SOCKET_STATUS_SYN_RCVD,
  SARA_R5_TCP_SOCKET_STATUS_ESTABLISHED,
  SARA_R5_TCP_SOCKET_STATUS_FIN_WAIT_1,
  SARA_R5_TCP_SOCKET_STATUS_FIN_WAIT_2,
  SARA_R5_TCP_SOCKET_STATUS_CLOSE_WAIT,
  SARA_R5_TCP_SOCKET_STATUS_CLOSING,
  SARA_R5_TCP_SOCKET_STATUS_LAST_ACK,
  SARA_R5_TCP_SOCKET_STATUS_TIME_WAIT
} SARA_R5_tcp_socket_status_t;

typedef enum
{
  SARA_R5_MESSAGE_FORMAT_PDU = 0,
  SARA_R5_MESSAGE_FORMAT_TEXT = 1
} SARA_R5_message_format_t;

typedef enum
{
  SARA_R5_UTIME_MODE_STOP = 0,
  SARA_R5_UTIME_MODE_PPS,
  SARA_R5_UTIME_MODE_ONE_SHOT,
  SARA_R5_UTIME_MODE_EXT_INT
} SARA_R5_utime_mode_t;

typedef enum
{
  SARA_R5_UTIME_SENSOR_NONE = 0,
  SARA_R5_UTIME_SENSOR_GNSS_LTE = 1,
  SARA_R5_UTIME_SENSOR_LTE
} SARA_R5_utime_sensor_t;

typedef enum
{
  SARA_R5_UTIME_URC_CONFIGURATION_DISABLED = 0,
  SARA_R5_UTIME_URC_CONFIGURATION_ENABLED
} SARA_R5_utime_urc_configuration_t;

typedef enum
{
  SARA_R5_SIM_NOT_PRESENT = 0,
  SARA_R5_SIM_PIN_NEEDED,
  SARA_R5_SIM_PIN_BLOCKED,
  SARA_R5_SIM_PUK_BLOCKED,
  SARA_R5_SIM_NOT_OPERATIONAL,
  SARA_R5_SIM_RESTRICTED,
  SARA_R5_SIM_OPERATIONAL
  //SARA_R5_SIM_PHONEBOOK_READY, // Not reported by SARA-R5
  //SARA_R5_SIM_USIM_PHONEBOOK_READY, // Not reported by SARA-R5
  //SARA_R5_SIM_TOOLKIT_REFRESH_SUCCESSFUL, // Not reported by SARA-R5
  //SARA_R5_SIM_TOOLKIT_REFRESH_UNSUCCESSFUL, // Not reported by SARA-R5
  //SARA_R5_SIM_PPP_CONNECTION_ACTIVE, // Not reported by SARA-R5
  //SARA_R5_SIM_VOICE_CALL_ACTIVE, // Not reported by SARA-R5
  //SARA_R5_SIM_CSD_CALL_ACTIVE // Not reported by SARA-R5
} SARA_R5_sim_states_t;

#define SARA_R5_NUM_PSD_PROFILES 6             // Number of supported PSD profiles
#define SARA_R5_NUM_PDP_CONTEXT_IDENTIFIERS 11 // Number of supported PDP context identifiers
#define SARA_R5_NUM_HTTP_PROFILES 4            // Number of supported HTTP profiles

typedef enum
{
  SARA_R5_HTTP_OP_CODE_SERVER_IP = 0,
  SARA_R5_HTTP_OP_CODE_SERVER_NAME,
  SARA_R5_HTTP_OP_CODE_USERNAME,
  SARA_R5_HTTP_OP_CODE_PASSWORD,
  SARA_R5_HTTP_OP_CODE_AUTHENTICATION,
  SARA_R5_HTTP_OP_CODE_SERVER_PORT,
  SARA_R5_HTTP_OP_CODE_SECURE,
  SARA_R5_HTTP_OP_CODE_REQUEST_TIMEOUT,
  SARA_R5_HTTP_OP_CODE_ADD_CUSTOM_HEADERS = 9
} SARA_R5_http_op_codes_t;

typedef enum
{
  SARA_R5_HTTP_COMMAND_HEAD = 0,
  SARA_R5_HTTP_COMMAND_GET,
  SARA_R5_HTTP_COMMAND_DELETE,
  SARA_R5_HTTP_COMMAND_PUT,
  SARA_R5_HTTP_COMMAND_POST_FILE,
  SARA_R5_HTTP_COMMAND_POST_DATA,
  SARA_R5_HTTP_COMMAND_GET_FOTA = 100
} SARA_R5_http_commands_t;

typedef enum
{
  SARA_R5_HTTP_CONTENT_APPLICATION_X_WWW = 0,
  SARA_R5_HTTP_CONTENT_TEXT_PLAIN,
  SARA_R5_HTTP_CONTENT_APPLICATION_OCTET,
  SARA_R5_HTTP_CONTENT_MULTIPART_FORM,
  SARA_R5_HTTP_CONTENT_APPLICATION_JSON,
  SARA_R5_HTTP_CONTENT_APPLICATION_XML,
  SARA_R5_HTTP_CONTENT_USER_DEFINED
} SARA_R5_http_content_types_t;

typedef enum
{
    SARA_R5_MQTT_NV_RESTORE = 0,
    SARA_R5_MQTT_NV_SET,
    SARA_R5_MQTT_NV_STORE,
} SARA_R5_mqtt_nv_parameter_t;

typedef enum
{
    SARA_R5_MQTT_PROFILE_CLIENT_ID = 0,
    SARA_R5_MQTT_PROFILE_SERVERNAME = 2,
    SARA_R5_MQTT_PROFILE_IPADDRESS,
    SARA_R5_MQTT_PROFILE_USERNAMEPWD,
    SARA_R5_MQTT_PROFILE_QOS = 6,
    SARA_R5_MQTT_PROFILE_RETAIN,
    SARA_R5_MQTT_PROFILE_TOPIC,
    SARA_R5_MQTT_PROFILE_MESSAGE,
    SARA_R5_MQTT_PROFILE_INACTIVITYTIMEOUT,
    SARA_R5_MQTT_PROFILE_SECURE,
} SARA_R5_mqtt_profile_opcode_t;

typedef enum
{
  SARA_R5_MQTT_COMMAND_INVALID = -1,
  SARA_R5_MQTT_COMMAND_LOGOUT = 0,
  SARA_R5_MQTT_COMMAND_LOGIN,
  SARA_R5_MQTT_COMMAND_PUBLISH,
  SARA_R5_MQTT_COMMAND_PUBLISHFILE,
  SARA_R5_MQTT_COMMAND_SUBSCRIBE,
  SARA_R5_MQTT_COMMAND_UNSUBSCRIBE,
  SARA_R5_MQTT_COMMAND_READ,
  SARA_R5_MQTT_COMMAND_RCVMSGFORMAT,
  SARA_R5_MQTT_COMMAND_PING,
  SARA_R5_MQTT_COMMAND_PUBLISHBINARY,
} SARA_R5_mqtt_command_opcode_t;

constexpr uint16_t MAX_MQTT_HEX_MSG_LEN = 512;
constexpr uint16_t MAX_MQTT_DIRECT_MSG_LEN = 1024;

typedef enum
{
    SARA_R5_FTP_PROFILE_IPADDRESS = 0,
    SARA_R5_FTP_PROFILE_SERVERNAME,
    SARA_R5_FTP_PROFILE_USERNAME,
    SARA_R5_FTP_PROFILE_PWD,
    SARA_R5_FTP_PROFILE_ACCOUNT,
    SARA_R5_FTP_PROFILE_TIMEOUT,
    SARA_R5_FTP_PROFILE_MODE
} SARA_R5_ftp_profile_opcode_t;

typedef enum
{
  SARA_R5_FTP_COMMAND_INVALID = -1,
  SARA_R5_FTP_COMMAND_LOGOUT = 0,
  SARA_R5_FTP_COMMAND_LOGIN,
  SARA_R5_FTP_COMMAND_DELETE_FILE,
  SARA_R5_FTP_COMMAND_RENAME_FILE,
  SARA_R5_FTP_COMMAND_GET_FILE,
  SARA_R5_FTP_COMMAND_PUT_FILE,
  SARA_R5_FTP_COMMAND_GET_FILE_DIRECT,
  SARA_R5_FTP_COMMAND_PUT_FILE_DIRECT,
  SARA_R5_FTP_COMMAND_CHANGE_DIR,
  SARA_R5_FTP_COMMAND_MKDIR = 10,
  SARA_R5_FTP_COMMAND_RMDIR,
  SARA_R5_FTP_COMMAND_DIR_INFO = 13,
  SARA_R5_FTP_COMMAND_LS,
  SARA_R5_FTP_COMMAND_GET_FOTA_FILE = 100
} SARA_R5_ftp_command_opcode_t;

typedef enum
{
  SARA_R5_PSD_CONFIG_PARAM_PROTOCOL = 0,
  SARA_R5_PSD_CONFIG_PARAM_APN,
  //SARA_R5_PSD_CONFIG_PARAM_USERNAME, // Not allowed on SARA-R5
  //SARA_R5_PSD_CONFIG_PARAM_PASSWORD, // Not allowed on SARA-R5
  SARA_R5_PSD_CONFIG_PARAM_DNS1 = 4,
  SARA_R5_PSD_CONFIG_PARAM_DNS2,
  //SARA_R5_PSD_CONFIG_PARAM_AUTHENTICATION, // Not allowed on SARA-R5
  //SARA_R5_PSD_CONFIG_PARAM_IP_ADDRESS, // Not allowed on SARA-R5
  //SARA_R5_PSD_CONFIG_PARAM_DATA_COMPRESSION, // Not allowed on SARA-R5
  //SARA_R5_PSD_CONFIG_PARAM_HEADER_COMPRESSION, // Not allowed on SARA-R5
  SARA_R5_PSD_CONFIG_PARAM_MAP_TO_CID = 100
} SARA_R5_pdp_configuration_parameter_t;

typedef enum
{
  SARA_R5_PSD_PROTOCOL_IPV4 = 0,
  SARA_R5_PSD_PROTOCOL_IPV6,
  SARA_R5_PSD_PROTOCOL_IPV4V6_V4_PREF,
  SARA_R5_PSD_PROTOCOL_IPV4V6_V6_PREF
} SARA_R5_pdp_protocol_type_t;

typedef enum
{
  SARA_R5_PSD_ACTION_RESET = 0,
  SARA_R5_PSD_ACTION_STORE,
  SARA_R5_PSD_ACTION_LOAD,
  SARA_R5_PSD_ACTION_ACTIVATE,
  SARA_R5_PSD_ACTION_DEACTIVATE
} SARA_R5_pdp_actions_t;

typedef enum
{
  SARA_R5_SEC_PROFILE_PARAM_CERT_VAL_LEVEL = 0,
  SARA_R5_SEC_PROFILE_PARAM_TLS_VER,
  SARA_R5_SEC_PROFILE_PARAM_CYPHER_SUITE,
  SARA_R5_SEC_PROFILE_PARAM_ROOT_CA,
  SARA_R5_SEC_PROFILE_PARAM_HOSTNAME,
  SARA_R5_SEC_PROFILE_PARAM_CLIENT_CERT,
  SARA_R5_SEC_PROFILE_PARAM_CLIENT_KEY,
  SARA_R5_SEC_PROFILE_PARAM_CLIENT_KEY_PWD,
  SARA_R5_SEC_PROFILE_PARAM_PSK,
  SARA_R5_SEC_PROFILE_PARAM_PSK_IDENT,
  SARA_R5_SEC_PROFILE_PARAM_SNI,
} SARA_R5_sec_profile_parameter_t;

typedef enum
{
  SARA_R5_SEC_PROFILE_CERTVAL_OPCODE_NO = 0,
  SARA_R5_SEC_PROFILE_CERTVAL_OPCODE_YESNOURL,
  SARA_R5_SEC_PROFILE_CERVTAL_OPCODE_YESURL,
  SARA_R5_SEC_PROFILE_CERTVAL_OPCODE_YESURLDATE,
} SARA_R5_sec_profile_certval_op_code_t;

typedef enum
{
  SARA_R5_SEC_PROFILE_TLS_OPCODE_ANYVER = 0,
  SARA_R5_SEC_PROFILE_TLS_OPCODE_VER1_0,
  SARA_R5_SEC_PROFILE_TLS_OPCODE_VER1_1,
  SARA_R5_SEC_PROFILE_TLS_OPCODE_VER1_2,
  SARA_R5_SEC_PROFILE_TLS_OPCODE_VER1_3,
} SARA_R5_sec_profile_tls_op_code_t;

typedef enum
{
  SARA_R5_SEC_PROFILE_SUITE_OPCODE_PROPOSEDDEFAULT = 0,
} SARA_R5_sec_profile_suite_op_code_t;

typedef enum
{
    SARA_R5_SEC_MANAGER_OPCODE_IMPORT = 0,
} SARA_R5_sec_manager_opcode_t;

typedef enum
{
    SARA_R5_SEC_MANAGER_ROOTCA = 0,
    SARA_R5_SEC_MANAGER_CLIENT_CERT,
    SARA_R5_SEC_MANAGER_CLIENT_KEY,
    SARA_R5_SEC_MANAGER_SERVER_CERT
} SARA_R5_sec_manager_parameter_t;

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
} SARA_R5_functionality_t;

#endif //SPARKFUN_SARA_R5_ARDUINO_LIBRARY_H
