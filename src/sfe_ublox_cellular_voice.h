#ifndef SPARKFUN_UBX_CELL_VOICE_ARDUINO_LIBRARY_H
#define SPARKFUN_UBX_CELL_VOICE_ARDUINO_LIBRARY_H

#include "sfe_ublox_cellular.h"

const char UBX_CELL_COMMAND_DIAL[] = "D";              // Dial command
const char UBX_CELL_COMMAND_ANSWER[] = "A";            // Answer call
const char UBX_CELL_COMMAND_HANG_UP[] = "+CHUP";       // Hang up call
const char UBX_CELL_COMMAND_PLAY_AUDIO[] = "+UPAR";    // Play audio resource
const char UBX_CELL_COMMAND_STOP_AUDIO[] = "+USAR";    // Stop audio resource
const char UBX_CELL_COMMAND_GENERATE_TONE[] = "+UTGN"; // Tone generator

// Base class for any modules supporting voice calls
class UBX_CELL_VOICE: virtual public UBX_CELL
{
public:
  UBX_CELL_error_t dial(String number);
  UBX_CELL_error_t answer(void);
  UBX_CELL_error_t hangUp(void);
  UBX_CELL_error_t playAudioResource(uint8_t audio_resource, uint8_t tone_id, uint8_t nof_repeat);
  UBX_CELL_error_t stopAudioResource(uint8_t audio_resource);
  UBX_CELL_error_t generateToneFreq(uint16_t frequency, uint16_t duration, uint8_t volume);
  UBX_CELL_error_t generateToneDTMF(char dtmf_character, uint16_t duration, uint8_t volume);
};

#endif