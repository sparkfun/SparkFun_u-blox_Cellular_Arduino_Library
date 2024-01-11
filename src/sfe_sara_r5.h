#ifndef SFE_SARA_R5_LIBRARY_H
#define SFE_SARA_R5_LIBRARY_H

#include "sfe_ublox_cellular.h"

const char *const UBX_CELL_MESSAGE_PDP_CONFIG = "+UPSD";     // Packet switched Data Profile configuration
const char *const UBX_CELL_MESSAGE_PDP_ACTION = "+UPSDA";    // Perform the action for the specified PSD profile
const char *const UBX_CELL_NETWORK_ASSIGNED_DATA = "+UPSND"; // Packet switched network-assigned data
const char *const UBX_CELL_GNSS_REQUEST_TIME = "+UTIME";     // Ask for time information from cellular modem (CellTime)
const char *const UBX_CELL_GNSS_TIME_INDICATION = "+UTIMEIND"; // Time information request status unsolicited indication
const char *const UBX_CELL_GNSS_TIME_CONFIGURATION = "+UTIMECFG"; // Sets time configuration

const char *const UBX_CELL_MESSAGE_PDP_ACTION_URC = "+UUPSDA:";

// Base SARA-R5 class
class SparkFun_ublox_SARA_R5 : public SparkFun_ublox_Cellular
{
  public:
    SparkFun_ublox_SARA_R5();

    // Bring up parent constructors
    using SparkFun_ublox_Cellular::SparkFun_ublox_Cellular;

    UBX_CELL_error_t setUtimeMode(
        UBX_CELL_utime_mode_t mode = UBX_CELL_UTIME_MODE_PPS,
        UBX_CELL_utime_sensor_t sensor = UBX_CELL_UTIME_SENSOR_GNSS_LTE); // Time mode, source etc. (+UTIME)
    UBX_CELL_error_t getUtimeMode(UBX_CELL_utime_mode_t *mode, UBX_CELL_utime_sensor_t *sensor);
    UBX_CELL_error_t setUtimeIndication(
        UBX_CELL_utime_urc_configuration_t config = UBX_CELL_UTIME_URC_CONFIGURATION_ENABLED); // +UTIMEIND
    UBX_CELL_error_t getUtimeIndication(UBX_CELL_utime_urc_configuration_t *config);
    UBX_CELL_error_t setUtimeConfiguration(int32_t offsetNanoseconds = 0, int32_t offsetSeconds = 0); // +UTIMECFG
    UBX_CELL_error_t getUtimeConfiguration(int32_t *offsetNanoseconds, int32_t *offsetSeconds);

    // Packet Switched Data
    // Configure the PDP using +UPSD. See UBX_CELL_pdp_configuration_parameter_t for the list of parameters: protocol,
    // APN, username, DNS, etc.
    UBX_CELL_error_t setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter,
                                         int value); // Set parameters in the chosen PSD profile
    UBX_CELL_error_t setPDPconfiguration(
        int profile, UBX_CELL_pdp_configuration_parameter_t parameter,
        UBX_CELL_pdp_protocol_type_t value); // Set parameters in the chosen PSD profile
    UBX_CELL_error_t setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter,
                                         String value); // Set parameters in the chosen PSD profile
    UBX_CELL_error_t setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter,
                                         IPAddress value); // Set parameters in the chosen PSD profile
    UBX_CELL_error_t performPDPaction(
        int profile, UBX_CELL_pdp_actions_t action); // Performs the requested action for the specified PSD profile:
                                                     // reset, store, load, activate, deactivate
    UBX_CELL_error_t getNetworkAssignedIPAddress(
        int profile, IPAddress *address); // Get the dynamic IP address assigned during PDP context activation

  protected:
    bool urcHandlerPDPAction(const char *event);
};

class SparkFun_ublox_SARA_R500S : public SparkFun_ublox_SARA_R5
{
    // Bring up parent constructors
    using SparkFun_ublox_SARA_R5::SparkFun_ublox_SARA_R5;
};

class SparkFun_ublox_SARA_R500S_01B : public SparkFun_ublox_SARA_R5
{
    // Bring up parent constructors
    using SparkFun_ublox_SARA_R5::SparkFun_ublox_SARA_R5;
};

class SparkFun_ublox_SARA_R500S_61B : public SparkFun_ublox_SARA_R5
{
    // Bring up parent constructors
    using SparkFun_ublox_SARA_R5::SparkFun_ublox_SARA_R5;
};

class SparkFun_ublox_SARA_R510M8S_61B : public SparkFun_ublox_SARA_R5
{
    // Bring up parent constructors
    using SparkFun_ublox_SARA_R5::SparkFun_ublox_SARA_R5;
};

class SparkFun_ublox_SARA_R510S : public SparkFun_ublox_SARA_R5
{
    // Bring up parent constructors
    using SparkFun_ublox_SARA_R5::SparkFun_ublox_SARA_R5;
};

#endif