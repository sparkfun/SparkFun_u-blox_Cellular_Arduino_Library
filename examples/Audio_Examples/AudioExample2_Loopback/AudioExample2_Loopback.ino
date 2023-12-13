#include "SparkFun_u-blox_Cellular_Arduino_Library.h"

// Uncomment the line below that you need for Serial on your platform
#define mySerial Serial1
// SoftwareSerial mySerial(16, 17);

// Uncomment the module you're using. If your module is not listed below, then
// it's not supported for this example
UBX_CELL_VOICE_BASE myModule; // This example works with all voice-enabled modules, so this base class can be used
// LARA_R6001 myModule;
// LARA_R6401 myModule;
// LARA_R6801_00B myModule;

void setup()
{
  Serial.begin(115200); // Start the serial console

  // Wait for user to press key to begin
  Serial.println(F("u-blox Cellular Audio Example 2 - Loopback"));

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
}

void loop()
{
  while(Serial.available() != 0){Serial.read();}
  Serial.println(F("Enter any key to begin loopback"));
  while(Serial.available() == 0){}
  
  myModule.playAudioResource(UBX_CELL_AUDIO_RESOURCE_LOOPBACK);
  
  while(Serial.available() != 0){Serial.read();}
  Serial.println(F("Enter any key to stop loopback"));
  while(Serial.available() == 0){}
  
  myModule.stopAudioResource(UBX_CELL_AUDIO_RESOURCE_LOOPBACK);
}