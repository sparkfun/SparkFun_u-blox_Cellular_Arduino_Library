#include "SparkFun_u-blox_Cellular_Arduino_Library.h"

// Uncomment the line below that you need for Serial on your platform
#define mySerial Serial1
// SoftwareSerial mySerial(16, 17);

// Uncomment the module you're using. If your module is not listed below, then
// it's not supported for this example
UBX_CELL myModule; // This example works with all modules, so the base class can be used
// UBX_CELL myModule; // Base SARA-R5 class
// UBX_CELL00S myModule;
// UBX_CELL00S_01B myModule;
// UBX_CELL00S_61B myModule;
// UBX_CELL10M8S_61B myModule;
// UBX_CELL10S myModule;
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
  Serial.println(F("u-blox Cellular Example 6 - Send SMS"));
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

  // First check to see if we're connected to an operator:
  if (myModule.getOperator(&currentOperator) == UBX_CELL_SUCCESS)
  {
    Serial.print(F("Connected to: "));
    Serial.println(currentOperator);
  }
  else
  {
    Serial.print(F("The SARA is not yet connected to an operator. Please use the previous examples to connect. Or wait and retry. Freezing..."));
    while (1)
      ; // Do nothing more
  }

  Serial.println();
  Serial.println(F("*** Set the Serial Monitor line ending to Newline ***"));
}

void loop()
{
  String destinationNumber = "";
  String message = "";
  boolean keepGoing = true;
  
  Serial.println();
  Serial.println(F("Enter the destination number (followed by LF / Newline): "));
  
  while (keepGoing)
  {
    if (Serial.available())
    {
      char c = Serial.read();
      if (c == '\n')
      {
        keepGoing = false; // Stop if we receive a newline
      }
      else
      {
        destinationNumber += c; // Add serial characters to the destination number
      }
    }
  }
  
  keepGoing = true;
  Serial.println();
  Serial.println(F("Enter the message (followed by LF): "));
  
  while (keepGoing)
  {
    if (Serial.available())
    {
      char c = Serial.read();
      if (c == '\n')
      {
        keepGoing = false; // Stop if we receive a newline
      }
      else
      {
        message += c; // Add serial characters to the destination number
      }
    }
  }

  // Once we receive a newline, send the text.
  Serial.println("Sending: \"" + message + "\" to " + destinationNumber);
  // Call myModule.sendSMS(String number, String message) to send an SMS message.
  if (myModule.sendSMS(destinationNumber, message) == UBX_CELL_SUCCESS)
  {
    Serial.println(F("sendSMS was successful"));
  }
}