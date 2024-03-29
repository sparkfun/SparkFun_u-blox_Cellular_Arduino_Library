#ifndef SPARKFUN_UBX_CELL_VOICE_ARDUINO_LIBRARY_H
#define SPARKFUN_UBX_CELL_VOICE_ARDUINO_LIBRARY_H

#include "sfe_ublox_cellular.h"

const char *const UBX_CELL_COMMAND_DIAL = "D";              // Dial command
const char *const UBX_CELL_COMMAND_ANSWER = "A";            // Answer call
const char *const UBX_CELL_COMMAND_HANG_UP = "+CHUP";       // Hang up call
const char *const UBX_CELL_COMMAND_PLAY_AUDIO = "+UPAR";    // Play audio resource
const char *const UBX_CELL_COMMAND_STOP_AUDIO = "+USAR";    // Stop audio resource
const char *const UBX_CELL_COMMAND_GENERATE_TONE = "+UTGN"; // Tone generator

const char *const UBX_CELL_RING_URC = "RING";

typedef enum
{
    UBX_CELL_AUDIO_RESOURCE_TONE = 0,
    UBX_CELL_AUDIO_RESOURCE_MIDI = 1,
    UBX_CELL_AUDIO_RESOURCE_LOOPBACK = 2
} UBX_CELL_audio_resource_t;

// Base class for any modules supporting voice calls
template <typename T> class SparkFun_ublox_Cellular_Voice_Base
{
  public:
    SparkFun_ublox_Cellular_Voice_Base(void)
    {
        // Set ring URC callback to nullptr
        _ringCallback = nullptr;

        // Add handler for ring URC
        static_cast<T *>(this)->addURCHandler(UBX_CELL_RING_URC,
                                              [this](const char *event) { return this->urcCheckRing(event); });
    }

    UBX_CELL_error_t dial(String number)
    {
        char *command;
        char *numberCStr;
        UBX_CELL_error_t err;

        numberCStr = static_cast<T *>(this)->ubx_cell_calloc_char(number.length() + 1);
        if (numberCStr == nullptr)
            return UBX_CELL_ERROR_OUT_OF_MEMORY;
        number.toCharArray(numberCStr, number.length() + 1);

        size_t cmdLen = strlen(UBX_CELL_COMMAND_DIAL) + strlen(numberCStr) + 3;
        command = static_cast<T *>(this)->ubx_cell_calloc_char(cmdLen);
        if (command != nullptr)
        {
            // Heads up! The dial command is one of the only commands that requires a
            // semicolon at the end of it!
            snprintf(command, cmdLen, "%s=%s;", UBX_CELL_COMMAND_DIAL, numberCStr);

            err = static_cast<T *>(this)->sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK, nullptr,
                                                                  UBX_CELL_10_SEC_TIMEOUT);

            free(command);
        }
        else
        {
            err = UBX_CELL_ERROR_OUT_OF_MEMORY;
        }

        free(numberCStr);

        return err;
    }

    UBX_CELL_error_t answer(void)
    {
        return static_cast<T *>(this)->sendCommandWithResponse(UBX_CELL_COMMAND_ANSWER, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                                               nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
    }

    UBX_CELL_error_t hangUp(void)
    {
        return static_cast<T *>(this)->sendCommandWithResponse(UBX_CELL_COMMAND_HANG_UP, UBX_CELL_RESPONSE_OK_OR_ERROR,
                                                               nullptr, UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
    }

    UBX_CELL_error_t playAudioResource(uint8_t audio_resource, uint8_t tone_id = 0, uint8_t nof_repeat = 0)
    {
        UBX_CELL_error_t err;
        size_t cmdLen = strlen(UBX_CELL_COMMAND_PLAY_AUDIO) + 13;
        char command[cmdLen];

        snprintf(command, cmdLen, "%s=%d,%d,%d", UBX_CELL_COMMAND_PLAY_AUDIO, audio_resource, tone_id, nof_repeat);

        err = static_cast<T *>(this)->sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr,
                                                              UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
        return err;
    }

    UBX_CELL_error_t stopAudioResource(uint8_t audio_resource)
    {
        UBX_CELL_error_t err;
        size_t cmdLen = strlen(UBX_CELL_COMMAND_STOP_AUDIO) + 5;
        char command[cmdLen];

        snprintf(command, cmdLen, "%s=%d", UBX_CELL_COMMAND_STOP_AUDIO, audio_resource);

        err = static_cast<T *>(this)->sendCommandWithResponse(command, UBX_CELL_RESPONSE_OK_OR_ERROR, nullptr,
                                                              UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
        return err;
    }

    UBX_CELL_error_t generateToneFreq(uint16_t frequency, uint16_t duration, uint8_t volume)
    {
        UBX_CELL_error_t err;
        size_t cmdLen = strlen(UBX_CELL_COMMAND_GENERATE_TONE) + 15;
        char command[cmdLen];
        char response[] = "\r\nOK\r\n\r\n+UUTGN: 0\r\n";

        snprintf(command, cmdLen, "%s=%d,%d,%d", UBX_CELL_COMMAND_GENERATE_TONE, frequency, duration, volume);

        err = static_cast<T *>(this)->sendCommandWithResponse(command, response, nullptr,
                                                              UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
        return err;
    }

    UBX_CELL_error_t generateToneDTMF(char dtmf_character, uint16_t duration, uint8_t volume)
    {
        UBX_CELL_error_t err;
        size_t cmdLen = strlen(UBX_CELL_COMMAND_GENERATE_TONE) + 14;
        char command[cmdLen];
        char response[] = "\r\nOK\r\n\r\n+UUTGN: 0\r\n";

        snprintf(command, cmdLen, "%s=\"%c\",%d,%d", UBX_CELL_COMMAND_GENERATE_TONE, dtmf_character, duration, volume);

        err = static_cast<T *>(this)->sendCommandWithResponse(command, response, nullptr,
                                                              UBX_CELL_STANDARD_RESPONSE_TIMEOUT);
        return err;
    }

    void setRingCallback(void (*callback)(void))
    {
        _ringCallback = callback;
    }

  protected:
    // Callback for incoming calls
    void (*_ringCallback)(void);

    bool urcCheckRing(const char *event)
    {
        int socket, length;
        char *searchPtr = strnstr(event, UBX_CELL_RING_URC, _RXBuffSize);
        if (searchPtr != nullptr)
        {
            if (_ringCallback != nullptr)
            {
                _ringCallback();
            }
            return true;
        }

        return false;
    }
};

class SparkFun_ublox_Cellular_Voice : public SparkFun_ublox_Cellular, public SparkFun_ublox_Cellular_Voice_Base<SparkFun_ublox_Cellular_Voice>
{
    // Bring up parent constructors
    using SparkFun_ublox_Cellular::SparkFun_ublox_Cellular;
};

#endif