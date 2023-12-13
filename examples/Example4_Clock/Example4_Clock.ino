#include "SparkFun_u-blox_Cellular_Arduino_Library.h"

// Uncomment the line below that you need for Serial on your platform
#define mySerial Serial1
// SoftwareSerial mySerial(16, 17);

// Uncomment the module you're using. If your module is not listed below, then
// it's not supported for this example
UBX_CELL myModule; // This example works with all modules, so the base class can be used
// SARA_R5 myModule; // Base SARA-R5 class
// SARA_R500S myModule;
// SARA_R500S_01B myModule;
// SARA_R500S_61B myModule;
// SARA_R510M8S_61B myModule;
// SARA_R510S myModule;
// LARA_R6 myModule; // Base LARA-R6 class
// LARA_R6001 myModule;
// LARA_R6001D myModule;
// LARA_R6401 myModule;
// LARA_R6401D myModule;
// LARA_R6801_00B myModule;
// LARA_R6801D myModule;

void setup()
{
  String currentOperator = "";

  Serial.begin(115200); // Start the serial console

  // Wait for user to press key to begin
  Serial.println(F("u-blox Cellular Example 4 - Clock"));
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
  if (myModule.begin(mySerial, UBX_CELL_DEFAULT_BAUD_RATE) )
  {
    Serial.println(F("Module connected!"));
  }
  else
  {
    Serial.println(F("Unable to communicate with the module."));
    Serial.println(F("Manually power-on (hold the module's On button for 3 seconds) and try again."));
    while (1) ; // Loop forever on fail
  }
  Serial.println();

  // Make sure automatic time zone updates are enabled
  if (myModule.autoTimeZone(true) != UBX_CELL_SUCCESS)
    Serial.println(F("Enable autoTimeZone failed!"));

  // Read and print the clock as a String
  String theTime = myModule.clock();
  Serial.println(theTime);

  // Read and print the hour, minute, etc. separately
  uint8_t year, month, day, hour, minute, second;
  int8_t timeZone;
  if (myModule.clock( &year, &month, &day, &hour, &minute, &second, &timeZone ) == UBX_CELL_SUCCESS)
    // Note: not all Arduino boards implement printf correctly. The formatting may not be correct on some boards.
    // Note: the timeZone is defined in 15 minute increments, not hours. -28 indicates the time zone is 7 hours behind UTC/GMT.
    Serial.printf("%02d/%02d/%02d %02d:%02d:%02d %+d\r\n", year, month, day, hour, minute, second, timeZone);
}

void loop()
{
  // Nothing to do here
}