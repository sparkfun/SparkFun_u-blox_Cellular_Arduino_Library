#include "sfe_ublox_cellular_voice.h"

UBX_CELL_error_t UBX_CELL_VOICE::dial(String number)
{
  char *command;
  char *numberCStr;
  UBX_CELL_error_t err;

  numberCStr = ubx_cell_calloc_char(number.length() + 1);
  if (numberCStr == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  number.toCharArray(numberCStr, number.length() + 1);

  command = ubx_cell_calloc_char(strlen(UBX_CELL_COMMAND_DIAL) + strlen(numberCStr) + 3);
  if (command != nullptr)
  {
    // Heads up! The dial command is one of the only commands that requires a
    // semicolon at the end of it!
    sprintf(command, "%s=%s;", UBX_CELL_COMMAND_DIAL, numberCStr);

    err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK,
                                  nullptr, UBX_CELL_10_SEC_TIMEOUT);
    
    free(command);
  }
  else
  {
    err = UBX_CELL_ERROR_OUT_OF_MEMORY;
  }

  free(numberCStr);

  return err;
}

UBX_CELL_error_t UBX_CELL_VOICE::answer(void)
{
  return sendCommandWithResponse(UBX_CELL_COMMAND_ANSWER, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
}

UBX_CELL_error_t UBX_CELL_VOICE::hangUp(void)
{
  return sendCommandWithResponse(UBX_CELL_COMMAND_HANG_UP, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
}

UBX_CELL_error_t UBX_CELL_VOICE::playAudioResource(uint8_t audio_resource, uint8_t tone_id, uint8_t nof_repeat)
{
  UBX_CELL_error_t err;
  char *command;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_COMMAND_PLAY_AUDIO) + 13);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d,%d,%d", UBX_CELL_COMMAND_PLAY_AUDIO, audio_resource, tone_id, nof_repeat);

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
  free(command);
  return err;
}

UBX_CELL_error_t UBX_CELL_VOICE::stopAudioResource(uint8_t audio_resource)
{
  UBX_CELL_error_t err;
  char *command;

  command = ubx_cell_calloc_char(strlen(UBX_CELL_COMMAND_STOP_AUDIO) + 5);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d", UBX_CELL_COMMAND_STOP_AUDIO, audio_resource);

  err = sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
  free(command);
  return err;
}

UBX_CELL_error_t UBX_CELL_VOICE::generateToneFreq(uint16_t frequency, uint16_t duration, uint8_t volume)
{
  UBX_CELL_error_t err;
  char *command;
  char response[] = "\r\nOK\r\n\r\n+UUTGN: 0\r\n";

  command = ubx_cell_calloc_char(strlen(UBX_CELL_COMMAND_GENERATE_TONE) + 15);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=%d,%d,%d", UBX_CELL_COMMAND_GENERATE_TONE, frequency, duration, volume);

  err = sendCommandWithResponse(command, response,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
  free(command);
  return err;
}

UBX_CELL_error_t UBX_CELL_VOICE::generateToneDTMF(char dtmf_character, uint16_t duration, uint8_t volume)
{
  UBX_CELL_error_t err;
  char *command;
  char response[] = "\r\nOK\r\n\r\n+UUTGN: 0\r\n";

  command = ubx_cell_calloc_char(strlen(UBX_CELL_COMMAND_GENERATE_TONE) + 14);
  if (command == nullptr)
    return UBX_CELL_ERROR_OUT_OF_MEMORY;
  sprintf(command, "%s=\"%c\",%d,%d", UBX_CELL_COMMAND_GENERATE_TONE, dtmf_character, duration, volume);

  err = sendCommandWithResponse(command, response,
                                nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
  free(command);
  return err;
}