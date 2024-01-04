#include "sfe_sara_r5.h"

SparkFun_ublox_SARA_R5::SparkFun_ublox_SARA_R5()
{
    addURCHandler(UBX_CELL_MESSAGE_PDP_ACTION_URC,
                  [this](const char *event) { return this->urcHandlerPDPAction(event); });
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::setUtimeMode(UBX_CELL_utime_mode_t mode, UBX_CELL_utime_sensor_t sensor)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_GNSS_REQUEST_TIME) + 16;
    char command[cmdLen];

    if (mode == UBX_CELL_UTIME_MODE_STOP) // stop UTIME does not require a sensor
        snprintf(command, cmdLen, "%s=%d", UBX_CELL_GNSS_REQUEST_TIME, mode);
    else
        snprintf(command, cmdLen, "%s=%d,%d", UBX_CELL_GNSS_REQUEST_TIME, mode, sensor);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr, UBX_CELL_10_SEC_TIMEOUT);
    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::getUtimeMode(UBX_CELL_utime_mode_t *mode, UBX_CELL_utime_sensor_t *sensor)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_GNSS_REQUEST_TIME) + 2;
    char command[cmdLen];
    char response[minimumResponseAllocation];

    UBX_CELL_utime_mode_t m;
    UBX_CELL_utime_sensor_t s;

    snprintf(command, cmdLen, "%s?", UBX_CELL_GNSS_REQUEST_TIME);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, response, UBX_CELL_10_SEC_TIMEOUT);

    // Response format: \r\n+UTIME: <mode>[,<sensor>]\r\n\r\nOK\r\n
    if (err == UBX_CELL_ERROR_SUCCESS)
    {
        int mStore, sStore, scanned = 0;
        char *searchPtr = strnstr(response, "+UTIME:", minimumResponseAllocation);
        if (searchPtr != nullptr)
        {
            searchPtr += strlen("+UTIME:"); // Move searchPtr to first character - probably a space
            while (*searchPtr == ' ')
                searchPtr++; // skip spaces
            scanned = sscanf(searchPtr, "%d,%d\r\n", &mStore, &sStore);
        }
        m = (UBX_CELL_utime_mode_t)mStore;
        s = (UBX_CELL_utime_sensor_t)sStore;
        if (scanned == 2)
        {
            *mode = m;
            *sensor = s;
        }
        else if (scanned == 1)
        {
            *mode = m;
            *sensor = UBX_CELL_UTIME_SENSOR_NONE;
        }
        else
            err = UBX_CELL_ERROR_UNEXPECTED_RESPONSE;
    }

    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::setUtimeIndication(UBX_CELL_utime_urc_configuration_t config)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_GNSS_TIME_INDICATION) + 16;
    char command[cmdLen];

    snprintf(command, cmdLen, "%s=%d", UBX_CELL_GNSS_TIME_INDICATION, config);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::getUtimeIndication(UBX_CELL_utime_urc_configuration_t *config)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_GNSS_TIME_INDICATION) + 2;
    char command[cmdLen];
    char response[minimumResponseAllocation];

    UBX_CELL_utime_urc_configuration_t c;

    snprintf(command, cmdLen, "%s?", UBX_CELL_GNSS_TIME_INDICATION);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, response, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

    // Response format: \r\n+UTIMEIND: <mode>\r\n\r\nOK\r\n
    if (err == UBX_CELL_ERROR_SUCCESS)
    {
        int cStore, scanned = 0;
        char *searchPtr = strnstr(response, "+UTIMEIND:", minimumResponseAllocation);
        if (searchPtr != nullptr)
        {
            searchPtr += strlen("+UTIMEIND:"); //  Move searchPtr to first char
            while (*searchPtr == ' ')
                searchPtr++; // skip spaces
            scanned = sscanf(searchPtr, "%d\r\n", &cStore);
        }
        c = (UBX_CELL_utime_urc_configuration_t)cStore;
        if (scanned == 1)
        {
            *config = c;
        }
        else
            err = UBX_CELL_ERROR_UNEXPECTED_RESPONSE;
    }

    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::setUtimeConfiguration(int32_t offsetNanoseconds, int32_t offsetSeconds)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_GNSS_TIME_CONFIGURATION) + 48;
    char command[cmdLen];

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
    snprintf(command, cmdLen, "%s=%d,%d", UBX_CELL_GNSS_TIME_CONFIGURATION, offsetNanoseconds, offsetSeconds);
#else
    snprintf(command, cmdLen, "%s=%ld,%ld", UBX_CELL_GNSS_TIME_CONFIGURATION, offsetNanoseconds, offsetSeconds);
#endif

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::getUtimeConfiguration(int32_t *offsetNanoseconds, int32_t *offsetSeconds)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_GNSS_TIME_CONFIGURATION) + 2;
    char command[cmdLen];
    char response[minimumResponseAllocation];

    int32_t ons;
    int32_t os;

    snprintf(command, cmdLen, "%s?", UBX_CELL_GNSS_TIME_CONFIGURATION);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, response, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

    // Response format: \r\n+UTIMECFG: <offset_nano>,<offset_sec>\r\n\r\nOK\r\n
    if (err == UBX_CELL_ERROR_SUCCESS)
    {
        int scanned = 0;
        char *searchPtr = strnstr(response, "+UTIMECFG:", minimumResponseAllocation);
        if (searchPtr != nullptr)
        {
            searchPtr += strlen("+UTIMECFG:"); //  Move searchPtr to first char
            while (*searchPtr == ' ')
                searchPtr++; // skip spaces
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
            scanned = sscanf(searchPtr, "%d,%d\r\n", &ons, &os);
#else
            scanned = sscanf(searchPtr, "%ld,%ld\r\n", &ons, &os);
#endif
        }
        if (scanned == 2)
        {
            *offsetNanoseconds = ons;
            *offsetSeconds = os;
        }
        else
            err = UBX_CELL_ERROR_UNEXPECTED_RESPONSE;
    }

    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter, int value)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_MESSAGE_PDP_CONFIG) + 24;
    char command[cmdLen];

    if (profile >= UBX_CELL_NUM_PSD_PROFILES)
        return UBX_CELL_ERROR_ERROR;

    snprintf(command, cmdLen, "%s=%d,%d,%d", UBX_CELL_MESSAGE_PDP_CONFIG, profile, parameter, value);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter,
                                              UBX_CELL_pdp_protocol_type_t value)
{
    return (setPDPconfiguration(profile, parameter, (int)value));
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter,
                                              String value)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_MESSAGE_PDP_CONFIG) + 64;
    char command[cmdLen];

    if (profile >= UBX_CELL_NUM_PSD_PROFILES)
        return UBX_CELL_ERROR_ERROR;

    snprintf(command, cmdLen, "%s=%d,%d,\"%s\"", UBX_CELL_MESSAGE_PDP_CONFIG, profile, parameter, value.c_str());

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter,
                                              IPAddress value)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_MESSAGE_PDP_CONFIG) + 64;
    char command[cmdLen];

    if (profile >= UBX_CELL_NUM_PSD_PROFILES)
        return UBX_CELL_ERROR_ERROR;

    snprintf(command, cmdLen, "%s=%d,%d,\"%d.%d.%d.%d\"", UBX_CELL_MESSAGE_PDP_CONFIG, profile, parameter, value[0],
             value[1], value[2], value[3]);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::performPDPaction(int profile, UBX_CELL_pdp_actions_t action)
{
    UBX_CELL_error_t err;
    size_t cmdLen = strlen(UBX_CELL_MESSAGE_PDP_ACTION) + 32;
    char command[cmdLen];

    if (profile >= UBX_CELL_NUM_PSD_PROFILES)
        return UBX_CELL_ERROR_ERROR;

    snprintf(command, cmdLen, "%s=%d,%d", UBX_CELL_MESSAGE_PDP_ACTION, profile, action);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

    return err;
}

UBX_CELL_error_t SparkFun_ublox_SARA_R5::getNetworkAssignedIPAddress(int profile, IPAddress *address)
{
    size_t cmdLen = strlen(UBX_CELL_NETWORK_ASSIGNED_DATA) + 16;
    char command[cmdLen];
    char response[minimumResponseAllocation];
    UBX_CELL_error_t err;
    int scanNum = 0;
    int profileStore = 0;
    int paramTag = 0; // 0: IP address: dynamic IP address assigned during PDP context activation
    int paramVals[4];

    snprintf(command, cmdLen, "%s=%d,%d", UBX_CELL_NETWORK_ASSIGNED_DATA, profile, paramTag);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, response, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

    if (err == UBX_CELL_ERROR_SUCCESS)
    {
        char *searchPtr = strnstr(response, "+UPSND:", minimumResponseAllocation);
        if (searchPtr != nullptr)
        {
            searchPtr += strlen("+UPSND:"); //  Move searchPtr to first char
            while (*searchPtr == ' ')
                searchPtr++; // skip spaces
            scanNum = sscanf(searchPtr, "%d,%d,\"%d.%d.%d.%d\"", &profileStore, &paramTag, &paramVals[0], &paramVals[1],
                             &paramVals[2], &paramVals[3]);
        }
        if (scanNum != 6)
        {
            if (_printDebug == true)
            {
                _debugPort->print(F("getNetworkAssignedIPAddress: error: scanNum is "));
                _debugPort->println(scanNum);
            }
            return UBX_CELL_ERROR_UNEXPECTED_RESPONSE;
        }

        IPAddress tempAddress = {(uint8_t)paramVals[0], (uint8_t)paramVals[1], (uint8_t)paramVals[2],
                                 (uint8_t)paramVals[3]};
        *address = tempAddress;
    }

    return err;
}

bool SparkFun_ublox_SARA_R5::urcHandlerPDPAction(const char *event)
{
    // URC: +UUPSDA (Packet Switched Data Action)
    int result;
    IPAddress remoteIP = {0, 0, 0, 0};
    int scanNum;
    int remoteIPstore[4];

    char *searchPtr = strnstr(event, UBX_CELL_MESSAGE_PDP_ACTION_URC, _RXBuffSize);
    if (searchPtr != nullptr)
    {
        searchPtr += strlen(UBX_CELL_MESSAGE_PDP_ACTION_URC); // Move searchPtr to first character - probably a space
        while (*searchPtr == ' ')
            searchPtr++; // skip spaces
        scanNum = sscanf(searchPtr, "%d,\"%d.%d.%d.%d\"", &result, &remoteIPstore[0], &remoteIPstore[1],
                         &remoteIPstore[2], &remoteIPstore[3]);

        if (scanNum == 5)
        {
            if (_printDebug == true)
                _debugPort->println(F("processReadEvent: packet switched data action"));

            for (int i = 0; i <= 3; i++)
            {
                remoteIP[i] = (uint8_t)remoteIPstore[i];
            }

            if (_psdActionRequestCallback != nullptr)
            {
                _psdActionRequestCallback(result, remoteIP);
            }

            return true;
        }
    }

    return false;
}
