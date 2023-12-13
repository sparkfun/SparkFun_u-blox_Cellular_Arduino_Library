#include "sfe_sara_r5.h"

UBX_CELL_error_t SARA_R5::setUtimeMode(UBX_CELL_utime_mode_t mode, UBX_CELL_utime_sensor_t sensor)
{
  UBX_CELL_error_t err;
  char *command;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_GNSS_REQUEST_TIME) + 16);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  if (mode == UBX_CELL_UTIME_MODE_STOP) // stop UTIME does not require a sensor
    sprintf(command, "%s=%d", UBX_CELL_GNSS_REQUEST_TIME, mode);
  else
    sprintf(command, "%s=%d,%d", UBX_CELL_GNSS_REQUEST_TIME, mode, sensor);

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                nullptr, UBX_CELL_10_SEC_TIMEOUT);
  free(command);
  return err;
}

UBX_CELL_error_t SARA_R5::getUtimeMode(UBX_CELL_utime_mode_t *mode, UBX_CELL_utime_sensor_t *sensor)
{
  UBX_CELL_error_t err;
  char *command;
  char *response;

  UBX_CELL_utime_mode_t m;
  UBX_CELL_utime_sensor_t s;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_GNSS_REQUEST_TIME) + 2);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s?", UBX_CELL_GNSS_REQUEST_TIME);

  response = ubx_cell_calloc_char(minimumResponseAllocation);
  if (response == nullptr)
  {
    free(command);
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  }

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                response, UBX_CELL_10_SEC_TIMEOUT);

  // Response format: \r\n+UTIME: <mode>[,<sensor>]\r\n\r\nOK\r\n
  if (err == UBX_CELL_ERROR_SUCCESS)
  {
    int mStore, sStore, scanned = 0;
    char *searchPtr = strstr(response, "+UTIME:");
    if (searchPtr != nullptr)
    {
      searchPtr += strlen("+UTIME:"); // Move searchPtr to first character - probably a space
      while (*searchPtr == ' ') searchPtr++; // skip spaces
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

  free(command);
  free(response);
  return err;
}

UBX_CELL_error_t SARA_R5::setUtimeIndication(UBX_CELL_utime_urc_configuration_t config)
{
  UBX_CELL_error_t err;
  char *command;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_GNSS_TIME_INDICATION) + 16);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d", UBX_CELL_GNSS_TIME_INDICATION, config);

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
  free(command);
  return err;
}

UBX_CELL_error_t SARA_R5::getUtimeIndication(UBX_CELL_utime_urc_configuration_t *config)
{
  UBX_CELL_error_t err;
  char *command;
  char *response;

  UBX_CELL_utime_urc_configuration_t c;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_GNSS_TIME_INDICATION) + 2);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s?", UBX_CELL_GNSS_TIME_INDICATION);

  response = ubx_cell_calloc_char(minimumResponseAllocation);
  if (response == nullptr)
  {
    free(command);
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  }

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                response, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

  // Response format: \r\n+UTIMEIND: <mode>\r\n\r\nOK\r\n
  if (err == UBX_CELL_ERROR_SUCCESS)
  {
    int cStore, scanned = 0;
    char *searchPtr = strstr(response, "+UTIMEIND:");
    if (searchPtr != nullptr)
    {
      searchPtr += strlen("+UTIMEIND:"); //  Move searchPtr to first char
      while (*searchPtr == ' ') searchPtr++; // skip spaces
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

  free(command);
  free(response);
  return err;
}

UBX_CELL_error_t SARA_R5::setUtimeConfiguration(int32_t offsetNanoseconds, int32_t offsetSeconds)
{
  UBX_CELL_error_t err;
  char *command;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_GNSS_TIME_CONFIGURATION) + 48);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  sprintf(command, "%s=%d,%d", UBX_CELL_GNSS_TIME_CONFIGURATION, offsetNanoseconds, offsetSeconds);
#else
  sprintf(command, "%s=%ld,%ld", UBX_CELL_GNSS_TIME_CONFIGURATION, offsetNanoseconds, offsetSeconds);
#endif

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
  free(command);
  return err;
}

UBX_CELL_error_t SARA_R5::getUtimeConfiguration(int32_t *offsetNanoseconds, int32_t *offsetSeconds)
{
  UBX_CELL_error_t err;
  char *command;
  char *response;

  int32_t ons;
  int32_t os;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_GNSS_TIME_CONFIGURATION) + 2);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s?", UBX_CELL_GNSS_TIME_CONFIGURATION);

  response = ubx_cell_calloc_char(minimumResponseAllocation);
  if (response == nullptr)
  {
    free(command);
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  }

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                response, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

  // Response format: \r\n+UTIMECFG: <offset_nano>,<offset_sec>\r\n\r\nOK\r\n
  if (err == UBX_CELL_ERROR_SUCCESS)
  {
    int scanned = 0;
    char *searchPtr = strstr(response, "+UTIMECFG:");
    if (searchPtr != nullptr)
    {
      searchPtr += strlen("+UTIMECFG:"); //  Move searchPtr to first char
      while (*searchPtr == ' ') searchPtr++; // skip spaces
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

  free(command);
  free(response);
  return err;
}

UBX_CELL_error_t SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter, int value)
{
  UBX_CELL_error_t err;
  char *command;

  if (profile >= UBX_CELL_NUM_PSD_PROFILES)
    return UBX_CELL_ERROR_ERROR;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_MESSAGE_PDP_CONFIG) + 24);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d,%d,%d", UBX_CELL_MESSAGE_PDP_CONFIG, profile, parameter,
          value);

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr,
                                UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

  free(command);
  return err;
}

UBX_CELL_error_t SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter, UBX_CELL_pdp_protocol_type_t value)
{
  return (setPDPconfiguration(profile, parameter, (int)value));
}

UBX_CELL_error_t SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter, String value)
{
  UBX_CELL_error_t err;
  char *command;

  if (profile >= UBX_CELL_NUM_PSD_PROFILES)
    return UBX_CELL_ERROR_ERROR;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_MESSAGE_PDP_CONFIG) + 64);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d,%d,\"%s\"", UBX_CELL_MESSAGE_PDP_CONFIG, profile, parameter,
          value.c_str());

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr,
                                UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

  free(command);
  return err;
}

UBX_CELL_error_t SARA_R5::setPDPconfiguration(int profile, UBX_CELL_pdp_configuration_parameter_t parameter, IPAddress value)
{
  UBX_CELL_error_t err;
  char *command;

  if (profile >= UBX_CELL_NUM_PSD_PROFILES)
    return UBX_CELL_ERROR_ERROR;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_MESSAGE_PDP_CONFIG) + 64);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d,%d,\"%d.%d.%d.%d\"", UBX_CELL_MESSAGE_PDP_CONFIG, profile, parameter,
          value[0], value[1], value[2], value[3]);

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr,
                                UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

  free(command);
  return err;
}

UBX_CELL_error_t SARA_R5::performPDPaction(int profile, UBX_CELL_pdp_actions_t action)
{
  UBX_CELL_error_t err;
  char *command;

  if (profile >= UBX_CELL_NUM_PSD_PROFILES)
    return UBX_CELL_ERROR_ERROR;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_MESSAGE_PDP_ACTION) + 32);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d,%d", UBX_CELL_MESSAGE_PDP_ACTION, profile, action);

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr,
                                UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

  free(command);
  return err;
}

UBX_CELL_error_t SARA_R5::getNetworkAssignedIPAddress(int profile, IPAddress *address)
{
  char *command;
  char *response;
  UBX_CELL_error_t err;
  int scanNum = 0;
  int profileStore = 0;
  int paramTag = 0; // 0: IP address: dynamic IP address assigned during PDP context activation
  int paramVals[4];

  command = ubx_cell_calloc_char(strlen(UBX_CELL_NETWORK_ASSIGNED_DATA) + 16);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d,%d", UBX_CELL_NETWORK_ASSIGNED_DATA, profile, paramTag);

  response = ubx_cell_calloc_char(minimumResponseAllocation);
  if (response == nullptr)
  {
    free(command);
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  }

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, response,
                                UBX_CELL_STANDARD_RESPONSE_TIMEOUT);

  if (err == UBX_CELL_ERROR_SUCCESS)
  {
    char *searchPtr = strstr(response, "+UPSND:");
    if (searchPtr != nullptr)
    {
      searchPtr += strlen("+UPSND:"); //  Move searchPtr to first char
      while (*searchPtr == ' ') searchPtr++; // skip spaces
      scanNum = sscanf(searchPtr, "%d,%d,\"%d.%d.%d.%d\"",
                        &profileStore, &paramTag,
                        &paramVals[0], &paramVals[1], &paramVals[2], &paramVals[3]);
    }
    if (scanNum != 6)
    {
      if (_printDebug == true)
      {
        _debugPort->print(F("getNetworkAssignedIPAddress: error: scanNum is "));
        _debugPort->println(scanNum);
      }
      free(command);
      free(response);
      return UBX_CELL_ERROR_UNEXPECTED_RESPONSE;
    }

    IPAddress tempAddress = { (uint8_t)paramVals[0], (uint8_t)paramVals[1],
                              (uint8_t)paramVals[2], (uint8_t)paramVals[3] };
    *address = tempAddress;
  }

  free(command);
  free(response);

  return err;
}
