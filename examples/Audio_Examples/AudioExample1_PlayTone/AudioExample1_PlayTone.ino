#include "SparkFun_u-blox_Cellular_Arduino_Library.h"

// Uncomment the line below that you need for Serial on your platform
#define mySerial Serial1
// SoftwareSerial mySerial(16, 17);

// Uncomment the module you're using. If your module is not listed below, then
// it's not supported for this example
SparkFun_ublox_Cellular_Voice myModule; // This example works with all voice-enabled modules, so this base class can be used
// SparkFun_ublox_LARA_R6001 myModule;
// SparkFun_ublox_LARA_R6401 myModule;
// SparkFun_ublox_LARA_R6801_00B myModule;

void setup()
{
    Serial.begin(115200); // Start the serial console

    // Wait for user to press key to begin
    Serial.println(F("u-blox Cellular Audio Example 1 - Play Tone"));

    Serial.println();
    Serial.println(F("! ! ! ! ! ATTENTION ! ! ! ! ! ATTENTION ! ! ! ! ! ATTENTION ! ! ! ! !"));
    Serial.println(F("This example requires an audio codec attached to the I2S interface"));
    Serial.println(F("of the cellular modem. Please add one and update this example as"));
    Serial.println(F("needed to configure your audio codec!"));
    Serial.println(F("! ! ! ! ! ATTENTION ! ! ! ! ! ATTENTION ! ! ! ! ! ATTENTION ! ! ! ! !"));
    Serial.println();

    Serial.println(F("Press any key to begin"));

    while (!Serial.available()) // Wait for the user to press a key (send any serial character)
        ;
    while (Serial.available()) // Empty the serial RX buffer
        Serial.read();

    Serial.println(F("Beginning..."));

    // myModule.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

    // For the MicroMod Asset Tracker, we need to invert the power pin so it pulls high instead of low
    // Uncomment the next line if required
    // myModule.invertPowerPin(true);

    // Initialize the module
    if (myModule.begin(mySerial, UBX_CELL_DEFAULT_BAUD_RATE))
    {
        Serial.println(F("Module connected!"));
    }
    else
    {
        Serial.println(F("Unable to communicate with the module."));
        Serial.println(F("Manually power-on (hold the module's On button for 3 seconds) and try again."));
        while (1)
            ; // Loop forever on fail
    }
    Serial.println();
}

void loop()
{
    String inputString;
    char dtmfChar = 0;
    uint16_t frequency = 0;
    uint16_t duration = 0;
    uint8_t volume = 0;

    while (true)
    {
        while (Serial.available() != 0)
        {
            Serial.read();
        }
        Serial.println(F("Enter a frequency in Hz (300-3400) or a DTMF character (0-9, *, #)"));
        while (Serial.available() == 0)
        {
        }

        inputString = Serial.readStringUntil('\n');

        if (inputString.length() == 1)
        {
            dtmfChar = inputString.charAt(0);
            if ((dtmfChar >= '0' && dtmfChar <= '9') || dtmfChar == '*' || dtmfChar == '#')
            {
                break;
            }
        }
        else
        {
            frequency = inputString.toInt();
            if (frequency >= 300 && frequency <= 3400)
            {
                dtmfChar == 0;
                break;
            }
        }
    }

    while (true)
    {
        while (Serial.available() != 0)
        {
            Serial.read();
        }
        Serial.println(F("Enter a duration in ms (50-1360)"));
        while (Serial.available() == 0)
        {
        }

        inputString = Serial.readStringUntil('\n');
        duration = inputString.toInt();
        if (duration >= 50 && duration <= 1360)
        {
            break;
        }
    }

    while (true)
    {
        while (Serial.available() != 0)
        {
            Serial.read();
        }
        Serial.println(F("Enter a volume (0-100)"));
        while (Serial.available() == 0)
        {
        }

        inputString = Serial.readStringUntil('\n');
        volume = inputString.toInt();
        if (volume <= 100)
        {
            break;
        }
    }

    if (dtmfChar == 0)
    {
        myModule.generateToneFreq(frequency, duration, volume);
    }
    else
    {
        myModule.generateToneDTMF(dtmfChar, duration, volume);
    }
}