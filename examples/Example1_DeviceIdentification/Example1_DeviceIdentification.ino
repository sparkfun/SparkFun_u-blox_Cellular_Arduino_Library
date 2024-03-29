#include "SparkFun_u-blox_Cellular_Arduino_Library.h"

// Uncomment the line below that you need for Serial on your platform
#define mySerial Serial1
// SoftwareSerial mySerial(16, 17);

// Uncomment the module you're using. If your module is not listed below, then
// it's not supported for this example
SparkFun_ublox_Cellular myModule; // This example works with all modules, so the base class can be used
// SparkFun_ublox_SARA_R5 myModule; // Base SARA-R5 class
// SparkFun_ublox_SARA_R500S myModule;
// SparkFun_ublox_SARA_R500S_01B myModule;
// SparkFun_ublox_SARA_R500S_61B myModule;
// SparkFun_ublox_SARA_R510M8S_61B myModule;
// SparkFun_ublox_SARA_R510S myModule;
// SparkFun_ublox_LARA_R6 myModule; // Base LARA-R6 class
// SparkFun_ublox_LARA_R6001 myModule;
// SparkFun_ublox_LARA_R6001D myModule;
// SparkFun_ublox_LARA_R6401 myModule;
// SparkFun_ublox_LARA_R6401D myModule;
// SparkFun_ublox_LARA_R6801_00B myModule;
// SparkFun_ublox_LARA_R6801D myModule;

// Map SIM states to more readable strings
String simStateString[] = {
    "Not present",     // 0
    "PIN needed",      // 1
    "PIN blocked",     // 2
    "PUK blocked",     // 3
    "Not operational", // 4
    "Restricted",      // 5
    "Operational"      // 6
};

// processSIMstate is provided to the u-blox cellular library via a
// callback setter -- setSIMstateReadCallback. (See setup())
void processSIMstate(UBX_CELL_sim_states_t state)
{
    Serial.println();
    Serial.print(F("SIM state:           "));
    Serial.print(String(state));
    Serial.println();
}

void setup()
{
    Serial.begin(115200); // Start the serial console

    // Wait for user to press key to begin
    Serial.println(F("u-blox Cellular Example 1 - Device Identification"));
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

    Serial.println("Manufacturer ID:     " + String(myModule.getManufacturerID()));
    Serial.println("Model ID:            " + String(myModule.getModelID()));
    Serial.println("Firmware Version:    " + String(myModule.getFirmwareVersion()));
    Serial.println("Product Serial No.:  " + String(myModule.getSerialNo()));
    Serial.println("IMEI:                " + String(myModule.getIMEI()));
    Serial.println("IMSI:                " + String(myModule.getIMSI()));
    Serial.println("SIM CCID:            " + String(myModule.getCCID()));
    Serial.println("Subscriber No.:      " + String(myModule.getSubscriberNo()));
    Serial.println("Capabilities:        " + String(myModule.getCapabilities()));

    // Set a callback to return the SIM state once requested
    myModule.setSIMstateReportCallback(&processSIMstate);
    // Now enable SIM state reporting for states 0 to 6 (by setting the reporting mode LSb)
    if (myModule.setSIMstateReportingMode(1) == UBX_CELL_SUCCESS)
        Serial.println("SIM state reports requested...");
    // You can disable the SIM staus reports again by calling assetTracker.setSIMstateReportingMode(0)
}

void loop()
{
    myModule.poll(); // Keep processing data from the module so we can extract the SIM status
}