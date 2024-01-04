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

bool callInProgress = false;
bool incomingCall = false;

void ringCallback()
{
    Serial.println(F("Incoming call! Enter \"A\" to answer, or anything else to reject"));
    incomingCall = true;
}

void setup()
{
    String currentOperator = "";

    Serial.begin(115200); // Start the serial console

    // Wait for user to press key to begin
    Serial.println(F("u-blox Cellular Audio Example 3 - Call Control"));

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

    // First check to see if we're connected to an operator:
    if (myModule.getOperator(&currentOperator) == UBX_CELL_SUCCESS)
    {
        Serial.print(F("Connected to: "));
        Serial.println(currentOperator);
    }
    else
    {
        Serial.print(F("The module is not yet connected to an operator. Please use the previous examples to connect. "
                       "Or wait and retry. Freezing..."));
        while (1)
            ; // Do nothing more
    }

    // Set callback function for when a new call is received
    myModule.setRingCallback(&ringCallback);

    Serial.println(F("Enter a number to dial"));

    // Clear any input
    while (Serial.available())
    {
        Serial.read();
    }
}

void loop()
{
    String inputString;

    myModule.bufferedPoll();

    if (Serial.available())
    {
        inputString = Serial.readStringUntil('\n');
        while (Serial.available())
        {
            Serial.read();
        }

        if (incomingCall)
        {
            if (inputString == "A" || inputString == "a")
            {
                Serial.println(F("Answering call, enter any key to hang up"));
                myModule.answer();
                callInProgress = true;
            }
            else
            {
                Serial.println(F("Rejecting call"));
                myModule.hangUp();
            }
            incomingCall = false;
        }
        else if (callInProgress == false)
        {
            Serial.println("Dialing " + inputString + ", enter any key to hang up");
            myModule.dial(inputString);
            callInProgress = true;
        }
        else
        {
            Serial.println(F("Hanging up, enter a new number to dial"));
            myModule.hangUp();
            callInProgress = false;
        }
    }
}